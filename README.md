# at90swuart
Software UART for AVR like AltSoftSerial or NeoICSerial without Arduino layer

Currently only AT90USB128X(rpk2, Teensy++ 2.0), ATMEGA32U4(arduino leonardo) and ATMEGA328P(arduino uno) is supported (uncomment USE_ARDUINO_UNO define)

You can adapt this library for your AVR with a few changes (look below)

Official post to ask about this project, feedback is always welcome: 
- https://www.avrfreaks.net/s/topic/a5C3l000000UcjqEAC/t162702

This library is like AltSoftSerial: 
- Can simultaneously transmit and receive. Minimal interference with simultaneous use of HardwareSerial and other libraries
- Consumes a 16 bit timer (and will not work with any libraries which need that timer) and disables some PWM pins
- Its possible to switch between TIMER1 and TIMER3 with a simple #define
- Can be sensitive to interrupt usage by other libraries
- Capable of running up to 31250 baud on 16 MHz AVR. Slower baud rates are recommended when other code may delay library's interrupt response
- More info about Interrupt Latency Requirements,  Timer Usage, Usable Speed: https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

**WARNING**: this library is more simple and small than AltSoftSerial/NeoICSerial. RX bytes is not stored in a internal ring buffer. If you dont want loss RX bytes just keep calling SWU_RxByte quickly or use SWU_RegisterRxCb function to handle your our ring buffer (look the main.c example)

Compiled and tested in: 
- Microchip Atmel Studio 

Default values:
- TIMER1
- BAUD 9600
- F_CPU 16000000L

This class can only use one predefined Input Capture pin

Each MCU and board has a pre-determined pin:

<table><tr><td> <b>Board</b> </td><td align=center> <b>Transmit</b> </td><td align=center> <b>Receive</b> </td><td align=center> <b>PWM Unusable</b></td></tr>
<tr><td> Teensy 3.0 & 3.1 </td><td align=center> 21 </td><td align=center> 20 </td><td align=center> 22</td></tr>
<tr><td> Teensy 2.0 </td><td align=center> 9  </td><td align=center> 10 </td><td align=center> (none)</td></tr>
<tr><td> TIMER1: rpk2 / Teensy++ 2.0 / AT90USBKEY </td><td align=center> 25 (PB5) </td><td align=center> 4 (PD4) </td><td align=center> 26, 27 (PB6, PB7)</td></tr>
<tr><td> TIMER3: rpk2 / Teensy++ 2.0 / AT90USBKEY </td><td align=center> 16 (PC6) </td><td align=center> 17 (PC7) </td><td align=center> 14, 15 (PC4, PC5)</td></tr>
<tr><td> Arduino Uno </td><td align=center> 9  </td><td align=center> 8 </td><td align=center> 10</td></tr>
<tr><td> Arduino Leonardo </td><td align=center> 5 </td><td align=center> 13 </td><td align=center> (none)</td></tr>
<tr><td> Arduino Mega </td><td align=center> 46 </td><td align=center> 48 </td><td align=center> 44, 45</td></tr>
<tr><td> Wiring-S </td><td align=center> 5 </td><td align=center> 6 </td><td align=center> 4</td></tr>
<tr><td> Sanguino </td><td align=center> 13 </td><td align=center> 14 </td><td align=center> 12</td></tr>
</table>

The "Unusable PWM" pins can be used normally but their PWM function will not work properly, because this library uses the timer which controls that pin's PWM feature

![demo.png](https://github.com/therealdreg/at90swuart/blob/main/demo.PNG)

## Files ready to flash in 16mhz AT90USB128x (rpk2, Teensy++ 2.0 ..)

### TIMER1
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_at90usb1287_16mhz_timer1.hex
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_at90usb1287_16mhz_timer1.elf

### TIMER3
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_at90usb1287_16mhz_timer3.hex
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_at90usb1287_16mhz_timer3.elf

## Files ready to flash in 16mhz ATMEGA328P (Arduino UNO ..) 

- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_atmega328p_arduino_uno_timer1.hex
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_atmega328p_arduino_uno_timer1.elf

## Files ready to flash in 16mhz ATMEGA32U4 (Arduino LEONARDO ..) 

### TIMER1
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_atmega32u4_arduino_leonardo_16mhz_timer1.hex
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_atmega32u4_arduino_leonardo_16mhz_timer1.elf

### TIMER3
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_atmega32u4_arduino_leonardo_16mhz_timer3.hex
- https://github.com/therealdreg/at90swuart/blob/main/at90swuart_atmega32u4_arduino_leonardo_16mhz_timer3.elf

## Teensy++ 2.0 pinout

![teensy_pinout.png](https://github.com/therealdreg/at90swuart/blob/main/teensy_pinout.PNG)

## Adapt this library for your AVR

You can adapt this library for your AVR with a few changes:

### Change default TIMER1 to TIMER3

Edit at90swuart.h and comment #define SWUSE_TIMER1 1

### Change BAUD 

Edit at90swuart.h and modify #define BAUDRATE    9600

### Change CPU speed

Edit at90swuart.h and modify #define F_CPU   16000000L

### Adapt your own AVR PORTs

Edit at90swuart.h and modify:
- #define SWU_DDR     DDRB
- #define SWU_PORT    PORTB
- #define SWU_PIN     PIND
- #define SWU_RX      PD4
- #define SWU_TX      PB5

Example for **ARDUINO UNO** (Atmega328p):

- #define SWU_DDR		DDRB
- #define SWU_PORT	    PORTB
- #define SWU_PIN		PINB
- #define SWU_RX		PB0
- #define SWU_TX		PB1

And done! this is all the work necessary for ARDUINO UNO. Just change MCU device from AT90USB1287 to Atmega328p in Microchip Atmel Studio and compile the project.

### Adapt other stuff like AVR TIMERS etc

Just take a look to:

- https://github.com/PaulStoffregen/AltSoftSerial/blob/master/AltSoftSerial.cpp
- https://github.com/PaulStoffregen/AltSoftSerial/blob/master/AltSoftSerial.h
- https://github.com/PaulStoffregen/AltSoftSerial/blob/master/config/AltSoftSerial_Boards.h
- https://github.com/PaulStoffregen/AltSoftSerial/blob/master/config/AltSoftSerial_Timers.h

Now you know what TIMERS, registers etc are used in your AVR board.

Just make a the necessary changes to: at90swuart.c and at90swuart.h

### IDE and Make supported 

Currently only Microchip Atmel Studio is supported

![atmelstudio_at90swuart.PNG](https://github.com/therealdreg/at90swuart/blob/main/atmelstudio_at90swuart.PNG)

## Other UART Software resources

- https://www.avrfreaks.net/index.php?name=PNphpBB2&file=viewtopic&t=58886&highlight=

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=133&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=436&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=3344&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=2450&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=2875&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=1895&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=521&item_type=project

- https://www.avrfreaks.net/index.php?module=Freaks%20Academy&func=viewItem&item_id=1495&item_type=project

- http://www.atmel.com/Images/avr304.zip

- https://github.com/SlashDevin/NeoHWSerial

- https://github.com/SlashDevin/NeoSWSerial

## Credits

A second UART in software by P. Dannegger danni@specs.de
- https://community.atmel.com/projects/second-uart-software

AVR_SWUART by kiki (eziya)
- https://github.com/eziya/AVR_SWUART
- https://blog.naver.com/eziya76/221244587970

AltSoftSerial
- https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html

NeoICSerial
- https://github.com/SlashDevin/NeoICSerial

## CHANGELOG

v3 18-07-2023:
- Arduino IDE support
- convert FDEV_SETUP_STREAM macro to fdev_setup_stream for Arduino C++ compatibility
- new define: DISABLE_FDEV_SETP to disable fdev_setup_stream for soft_uart and soft_uart_echo
- added rx-callback mechanism for TIMER3 
- addex .hex and .elf versions for TIMER3
- Arduino Leonardo (atmega32u4) is officially supported
- Arduino UNO (atmega328p) is officially supported just uncomment USE_ARDUINO_UNO define

v1 28-11-2021:
- MIT License
- support for AT90USB128x: rpk2, Teensy++ 2.0, AT90USBKEY ...
- support TIMER1 and TIMER3. ts possible to switch between TIMER1 and TIMER3 with a simple #define
- RX callback SWU_RegisterRxCb, like NeoICSerial
- RX echo support: SWU_RxByte_echo, SWU_RxByte_echo_isprint, SWU_RxByte_echo_isprint_nl, SWU_RxBuffer_echo
- FDEV_SETUP_STREAM support, now you can use printf, scanf, fprintf ...
- main example using all features
