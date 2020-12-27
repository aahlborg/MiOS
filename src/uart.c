#include <uart.h>
#include <gpio.h>
#include <rpi_peripherals.h>

#define BUF_SIZE (1024u)

// Circular buffer type
struct buffer {
  int start;
  int end;
  char buf[BUF_SIZE];
};

// UART Rx buffer
static struct buffer rx_buffer;

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
  rx_buffer.start = 0;
  rx_buffer.end = 0;

  // Enable Rx interrupt
  AUX_REGS->muIrqEnable = AUX_MU_IER_ENABLE_RX;
  AUX_REGS->muIrqId = AUX_MU_IIR_CLEAR_RX | AUX_MU_IIR_CLEAR_TX;
  IRQ_REGS->irq_en1 |= IRQ_AUX;

  // Enable UART Tx
  AUX_REGS->muExtraCtrl = AUX_MU_CNTL_TX_ENABLE;
}

void uart_write(char c)
{
  // Wait for previous write to finish
  while (0 == (AUX_REGS->muLineStatus & AUX_MU_LSR_TX_EMPTY)) {}

  // Send character
  AUX_REGS->muIO = c;
}

int uart_read(char * buffer)
{
  // Check length of Rx buffer and return all data
  int length = buf_len(&rx_buffer);
  for (int i = 0; i < length; i++)
  {
    buffer[i] = buf_get(&rx_buffer);
  }
  return length;
}

void uart_rx_isr(void)
{
  // Read char and reset interrupt
  buf_append(&rx_buffer, AUX_REGS->muIO & 0xff);
  AUX_REGS->muIrqId = AUX_MU_IIR_CLEAR_RX;
}
