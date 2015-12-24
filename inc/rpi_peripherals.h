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
  int gpioFunctionReg[6];
  int reserved0;
  int gpioPinOutputSet[2];
  int reserved1;
  int gpioPinOutputClear[2];
  int reserved2;
  int gpioPinLevel[2];
  int reserved3;
  int gpioPinEventDetectStatus[2];
  int reserved4;
  int gpioPinRisingEdgeDetectEnable[2];
  int reserved5;
  int gpioPinFallingEdgeDetectEnable[2];
  int reserved6;
  int gpioPinHighDetectEnable[2];
  int reserved7;
  int gpioPinLowDetectEnable[2];
  int reserved8;
  int gpioPinAsyncRisingEdgeDetect[2];
  int reserved9;
  int gpioPinAsyncFallingEdgeDetect[2];
  int reserved10;
  int gpioPinPullUpDownEnable;
  int gpioPinPullUpDownEnableClock[2];
  int reserved11;
  int test;
};

//////////////////
// System timer /
////////////////

#define SYS_TIMER_BASE (PERIPHERAL_BASE + 0x00003000)

/////////////
// Mailbox /
///////////

#define MAILBOX_BASE (PERIPHERAL_BASE + 0x0000B880)

#endif
