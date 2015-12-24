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
#define GPIO_FUNC_ALT4 2
#define GPIO_FUNC_ALT5 3

#if defined(RPI2) || defined(RPI_PLUS)
#define BOARD_LED_GPIO_PIN 47
#else
#define BOARD_LED_GPIO_PIN 16
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

#endif
