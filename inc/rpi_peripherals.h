// Defines for accessing and configuring
// Raspberry Pi chip peripherals such as
// GPIO, UART, System timer, etc.

#ifndef RPI_PRERIPHERALS_H
#define RPI_PRERIPHERALS_H

/////////////
// General /
///////////

#ifdef RPI2
#define PERIPHERAL_BASE 0x3F000000
#else
#define PERIPHERAL_BASE 0x20000000
#endif

#define SYS_FREQ 250000000

typedef volatile unsigned int reg_t;

//////////
// GPIO /
////////

#define GPIO_BASE (PERIPHERAL_BASE + 0x00200000)

#define GPIO_PIN_MAX 53

#define GPIO_FUNC_MAX 7
#define GPIO_FUNC_IN 0
#define GPIO_FUNC_OUT 1
#define GPIO_FUNC_ALT0 4
#define GPIO_FUNC_ALT1 5
#define GPIO_FUNC_ALT2 6
#define GPIO_FUNC_ALT3 7
#define GPIO_FUNC_ALT4 3
#define GPIO_FUNC_ALT5 2

#define GPIO_FUNC_MASK 7

#define GPIO_PULL_DISABLE 0
#define GPIO_PULL_DOWN 1
#define GPIO_PULL_UP 2

#define GPIO_PIN_UART_TX 14
#define GPIO_PIN_UART_RX 15
#if defined(RPI2) || defined(RPI_PLUS)
#define GPIO_PIN_BOARD_LED 47
#else
#define GPIO_PIN_BOARD_LED 16
#endif

struct rpi_gpio_regs {
  reg_t gpioFunctionReg[6];
  reg_t reserved0;
  reg_t gpioPinOutputSet[2];
  reg_t reserved1;
  reg_t gpioPinOutputClear[2];
  reg_t reserved2;
  reg_t gpioPinLevel[2];
  reg_t reserved3;
  reg_t gpioPinEventDetectStatus[2];
  reg_t reserved4;
  reg_t gpioPinRisingEdgeDetectEnable[2];
  reg_t reserved5;
  reg_t gpioPinFallingEdgeDetectEnable[2];
  reg_t reserved6;
  reg_t gpioPinHighDetectEnable[2];
  reg_t reserved7;
  reg_t gpioPinLowDetectEnable[2];
  reg_t reserved8;
  reg_t gpioPinAsyncRisingEdgeDetect[2];
  reg_t reserved9;
  reg_t gpioPinAsyncFallingEdgeDetect[2];
  reg_t reserved10;
  reg_t gpioPinPullUpDownEnable;
  reg_t gpioPinPullUpDownEnableClock[2];
  reg_t reserved11;
  reg_t test;
};

#define GPIO_REGS ((struct rpi_gpio_regs *)GPIO_BASE)

//////////////////
// System timer /
////////////////

#define SYS_TIMER_BASE (PERIPHERAL_BASE + 0x00003000)
#define SYS_TIMER_FREQ 1000000
#define SYS_TIMER_M0 (1 << 0)
#define SYS_TIMER_M1 (1 << 1)
#define SYS_TIMER_M2 (1 << 2)
#define SYS_TIMER_M3 (1 << 3)

struct rpi_sys_timer_regs {
  reg_t ctrlStatus;
  reg_t cntLo;
  reg_t cntHi;
  reg_t compare[4];
};

#define SYS_TIMER_REGS ((struct rpi_sys_timer_regs *)SYS_TIMER_BASE)

/////////////
// Mailbox /
///////////

#define MAILBOX_BASE (PERIPHERAL_BASE + 0x0000B880)

#define MBOX_CHAN_FRAMEBUFFER 1
#define MBOX_CHAN_VC_IF 8
#define MBOX_CHAN_MAX 0xF
#define MBOX_PTR_MASK 0xFFFFFFF0
#define MBOX_FULL 0x80000000
#define MBOX_EMPTY 0x40000000

struct rpi_mbox_regs {
  reg_t read;
  reg_t reserved[3];
  reg_t peek;
  reg_t sender;
  reg_t status;
  reg_t config;
  reg_t write;
};

#define MBOX_REGS ((struct rpi_mbox_regs *)MAILBOX_BASE)

/////////////////////////////
// Auxiliaries: UART & SPI /
///////////////////////////

#define AUX_BASE (PERIPHERAL_BASE + 0x00215000)

#define AUX_IRQ_MU   (1 << 0)
#define AUX_IRQ_SPI1 (1 << 0)
#define AUX_IRQ_SPI2 (1 << 0)
#define AUX_EN_MU   (1 << 0)
#define AUX_EN_SPI1 (1 << 1)
#define AUX_EN_SPI2 (1 << 2)
#define AUX_MU_IER_ENABLE_TX (1 << 1)
#define AUX_MU_IER_ENABLE_RX (1 << 0)
#define AUX_MU_IIR_CLEAR_RX (1 << 1)
#define AUX_MU_IIR_CLEAR_TX (1 << 2)
#define AUX_MU_IIR_TX_PENDING (1 << 1)
#define AUX_MU_IIR_RX_PENDING (1 << 2)
#define AUX_MU_7BIT_MODE 0
#define AUX_MU_8BIT_MODE 3
#define AUX_MU_LSR_TX_EMPTY (1 << 5)
#define AUX_MU_LSR_DATA_READY (1 << 0)
#define AUX_MU_CNTL_RX_ENABLE (1 << 0)
#define AUX_MU_CNTL_TX_ENABLE (1 << 1)
#define AUX_MU_STAT_RX_FIFO (0xf << 16)
#define AUX_MU_STAT_TX_FIFO (0xf << 24)
#define AUX_MU_FIFO_SIZE (8u)

struct rpi_aux_regs {
  reg_t irqStatus;
  reg_t enables;
  reg_t reserved0[14];
  reg_t muIO;
  reg_t muIrqEnable;
  reg_t muIrqId;
  reg_t muLineCtrl;
  reg_t muModemCtrl;
  reg_t muLineStatus;
  reg_t muModemStatus;
  reg_t muScratch;
  reg_t muExtraCtrl;
  reg_t muExtraStatus;
  reg_t muBaudRate;
  reg_t reserved1[5];
  reg_t spi1reg0;
  reg_t spi1reg1;
  reg_t spi1reg2;
  reg_t spi1reg3;
  reg_t spi1reg4;
  reg_t reserved2[10];
  reg_t spi2reg0;
  reg_t spi2reg1;
  reg_t spi2reg2;
  reg_t spi2reg3;
  reg_t spi2reg4;
};

#define AUX_REGS ((struct rpi_aux_regs *)AUX_BASE)

////////////////
// Interrupts /
//////////////

#define IRQ_BASE (PERIPHERAL_BASE + 0x0000B200)

#define IRQ_SYS_TIMER_0 (1 << 0)
#define IRQ_SYS_TIMER_1 (1 << 1)
#define IRQ_SYS_TIMER_2 (1 << 2)
#define IRQ_SYS_TIMER_3 (1 << 3)
#define IRQ_SYS_TIMERS (IRQ_SYS_TIMER_0 | IRQ_SYS_TIMER_1 | IRQ_SYS_TIMER_2 | IRQ_SYS_TIMER_3)
#define IRQ_AUX (1 << 29)

struct rpi_irq_regs {
  reg_t irq_basic_pend;
  reg_t irq_pend1;
  reg_t irq_pend2;
  reg_t fiq_control;
  reg_t irq_en1;
  reg_t irq_en2;
  reg_t irq_basic_en;
  reg_t irq_disable1;
  reg_t irq_disable2;
  reg_t irq_basic_disable;
};

#define IRQ_REGS ((struct rpi_irq_regs *)IRQ_BASE)

#endif
