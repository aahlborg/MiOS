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
  volatile int gpioFunctionReg[6];
  volatile int reserved0;
  volatile int gpioPinOutputSet[2];
  volatile int reserved1;
  volatile int gpioPinOutputClear[2];
  volatile int reserved2;
  volatile int gpioPinLevel[2];
  volatile int reserved3;
  volatile int gpioPinEventDetectStatus[2];
  volatile int reserved4;
  volatile int gpioPinRisingEdgeDetectEnable[2];
  volatile int reserved5;
  volatile int gpioPinFallingEdgeDetectEnable[2];
  volatile int reserved6;
  volatile int gpioPinHighDetectEnable[2];
  volatile int reserved7;
  volatile int gpioPinLowDetectEnable[2];
  volatile int reserved8;
  volatile int gpioPinAsyncRisingEdgeDetect[2];
  volatile int reserved9;
  volatile int gpioPinAsyncFallingEdgeDetect[2];
  volatile int reserved10;
  volatile int gpioPinPullUpDownEnable;
  volatile int gpioPinPullUpDownEnableClock[2];
  volatile int reserved11;
  volatile int test;
};

//////////////////
// System timer /
////////////////

#define SYS_TIMER_BASE (PERIPHERAL_BASE + 0x00003000)
#define SYS_TIMER_FREQ 1000000

struct rpi_sys_timer_regs {
  volatile int ctrlStatus;
  volatile unsigned int cntLo;
  volatile unsigned int cntHi;
  volatile int compare[4];
};

/////////////
// Mailbox /
///////////

#define MAILBOX_BASE (PERIPHERAL_BASE + 0x0000B880)

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
#define AUX_MU_IIR_CLEAR_RX (1 << 1)
#define AUX_MU_IIR_CLEAR_TX (1 << 2)
#define AUX_MU_7BIT_MODE 0
#define AUX_MU_8BIT_MODE 3
#define AUX_MU_LSR_TX_EMPTY (1 << 5)
#define AUX_MU_CNTL_RX_ENABLE (1 << 0)
#define AUX_MU_CNTL_TX_ENABLE (1 << 1)

struct rpi_aux_regs {
  volatile int irqStatus;
  volatile int enables;
  volatile int reserved0[14];
  volatile int muIO;
  volatile int muIrqEnable;
  volatile int muIrqId;
  volatile int muLineCtrl;
  volatile int muModemCtrl;
  volatile int muLineStatus;
  volatile int muModemStatus;
  volatile int muScratch;
  volatile int muExtraCtrl;
  volatile int muExtraStatus;
  volatile int muBaudRate;
  volatile int reserved1[5];
  volatile int spi1reg0;
  volatile int spi1reg1;
  volatile int spi1reg2;
  volatile int spi1reg3;
  volatile int spi1reg4;
  volatile int reserved2[10];
  volatile int spi2reg0;
  volatile int spi2reg1;
  volatile int spi2reg2;
  volatile int spi2reg3;
  volatile int spi2reg4;
};

#endif
