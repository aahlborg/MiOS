#include <uart.h>
#include <gpio.h>
#include <rpi_peripherals.h>
#include <kernel.h>

#define BUF_SIZE (1024u)

// Circular buffer type
struct buffer {
  int start;
  int end;
  char buf[BUF_SIZE];
};

// UART interrupt handlers
static void uart_rx_isr(void);
static void uart_tx_isr(void);

// UART buffers
static struct buffer tx_buffer;
static struct buffer rx_buffer;

// Statistics data
static struct uart_stats stats;

static void buf_append(struct buffer * buf, char c)
{
  // Append a char to circular buffer
  buf->buf[buf->end] = c;
  buf->end = (buf->end + 1) % BUF_SIZE;
}

static int buf_len(struct buffer * buf)
{
  // Get length of circular buffer
  int start = buf->start;
  int end = buf->end;
  if (start > end)
  {
    end += BUF_SIZE;
  }
  return end - start;
}

static char buf_get(struct buffer * buf)
{
  // Read a char from circular buffer
  char c = buf->buf[buf->start];
  buf->start = (buf->start + 1) % BUF_SIZE;
  return c;
}

static void uart_tx_isr(void)
{
  int fifo_len = (AUX_REGS->muExtraStatus & AUX_MU_STAT_TX_FIFO) >> 24;
  int avail = AUX_MU_FIFO_SIZE - fifo_len - 1;
  for (int i = 0; i < avail; i++)
  {
    if (buf_len(&tx_buffer))
    {
      // Write next char
      AUX_REGS->muIO = buf_get(&tx_buffer);
    }
    else
    {
      // Disable Tx interrupt
      AUX_REGS->muIrqEnable = AUX_MU_IER_ENABLE_RX;
      break;
    }
  }
}

static void uart_rx_isr(void)
{
  // Read from Rx queue
  while (AUX_REGS->muExtraStatus & AUX_MU_STAT_RX_FIFO)
  {
    buf_append(&rx_buffer, AUX_REGS->muIO & 0xff);
  }
}

void uart_init(int baud, int bits)
{
  // Configure mini UART
  AUX_REGS->enables = AUX_EN_MU;
  AUX_REGS->muIrqEnable = 0;
  AUX_REGS->muExtraCtrl = 0;

  if (bits == 8)
    AUX_REGS->muLineCtrl = AUX_MU_8BIT_MODE;
  else
    AUX_REGS->muLineCtrl = AUX_MU_7BIT_MODE;

  AUX_REGS->muModemCtrl = 0;
  // Calculate baud multiplier from sys frequency
  AUX_REGS->muBaudRate = SYS_FREQ / (8 * baud) - 1;

  // Configure Tx/Rx pins
  gpio_pin_set_function(GPIO_PIN_UART_TX, GPIO_FUNC_ALT5);
  gpio_pin_set_function(GPIO_PIN_UART_RX, GPIO_FUNC_ALT5);
  gpio_pin_set_pull_up_down(GPIO_PIN_UART_TX, GPIO_PULL_DISABLE);

  // Init buffers
  tx_buffer.start = 0;
  tx_buffer.end = 0;
  rx_buffer.start = 0;
  rx_buffer.end = 0;

  // Enable Rx interrupt
  AUX_REGS->muIrqEnable = AUX_MU_IER_ENABLE_RX;
  AUX_REGS->muIrqId = AUX_MU_IIR_CLEAR_RX | AUX_MU_IIR_CLEAR_TX;
  IRQ_REGS->irq_en1 |= IRQ_AUX;

  // Enable UART
  AUX_REGS->muExtraCtrl = AUX_MU_CNTL_RX_ENABLE | AUX_MU_CNTL_TX_ENABLE;
}

void uart_write_ch(char c)
{
  disable();
  uart_write_ch_isr(c);
  enable();
}

void uart_write_ch_isr(char c)
{
  // Add to Tx buffer
  buf_append(&tx_buffer, c);

  // Enable Tx interrupt
  AUX_REGS->muIrqEnable = AUX_MU_IER_ENABLE_RX | AUX_MU_IER_ENABLE_TX;

  stats.tx_count++;
}

void uart_write(char * str, int length)
{
  disable();

  // Add to Tx buffer
  for (int i = 0; i < length; i++)
  {
    buf_append(&tx_buffer, str[i]);
  }

  // Enable Tx interrupt
  AUX_REGS->muIrqEnable = AUX_MU_IER_ENABLE_RX | AUX_MU_IER_ENABLE_TX;

  enable();

  stats.tx_count += length;
}

int uart_read(char * buffer)
{
  disable();
  // Check length of Rx buffer and return all data
  int length = buf_len(&rx_buffer);
  for (int i = 0; i < length; i++)
  {
    buffer[i] = buf_get(&rx_buffer);
  }
  enable();
  stats.rx_count += length;
  return length;
}

void uart_isr(void)
{
  unsigned int status = AUX_REGS->muIrqId;
  if (status & AUX_MU_IIR_RX_PENDING)
  {
    uart_rx_isr();
  }
  if (status & AUX_MU_IIR_TX_PENDING)
  {
    uart_tx_isr();
  }
  stats.isr_count++;
}

struct uart_stats uart_get_stats(void)
{
  return stats;
}
