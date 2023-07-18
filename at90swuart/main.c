/*
MIT License - Copyright 2021
-
David Reguera Garcia aka Dreg - dreg@fr33project.org
-
http://www.rootkit.es/
http://github.com/David-Reguera-Garcia-Dreg/
http://www.fr33project.org/
-
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

Credits:
    A second UART in software
    P. Dannegger danni@specs.de
    https://community.atmel.com/projects/second-uart-software

    AVR_SWUART
    kiki (eziya)
    https://github.com/eziya/AVR_SWUART/blob/master/AVR_SWUART/328P_SWUART.c
    https://blog.naver.com/eziya76/221244587970
*/

#include "at90swuart.h"

#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

#define clear_bit(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define set_bit(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define toogle_bit(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit))

volatile bool rx_flag;
volatile uint8_t rx_bff[200];
volatile uint8_t curr_buff_pos = 0xFF;

void my_rx_handler(uint8_t byte)
{
    // Never call to SWU_* functions in this handler, this handler must be short and fast (called from the RX interrupt)
    static int i = 0;
    i = i % (sizeof(rx_bff) - 1);
    rx_bff[i] = byte;
    curr_buff_pos = i;
    i++;
    if (byte == '\r' || byte == '\n')
    {
        rx_flag = true;
    }
}


int main(int argc, char** argv)
{
    MCUSR &= ~_BV(WDRF);
    wdt_disable();

    CLKPR = (1 << CLKPCE);
    CLKPR = 0;

    SWU_Initialize();

    _delay_ms(1000);

    stdin = stdout = &soft_uart;

    for (int i = 0; i <  20; i++)
    {
        SWU_TxByte('d', NULL);
    }
    SWU_TxByte('\r', NULL);
    SWU_TxByte('\n', NULL);

    printf("\r\n\r\nAT90SWUART version: %s\r\n\r\n", AT90SWUART_VER_STR);

    fprintf(&soft_uart, "\r\nhi dreg from fprintf\r\n");

    printf("\r\nhi dreg from printf\r\n");

#define TXBUFFSTR "\r\nhi dreg from SWU_TxBuffer\r\n\r\n"
    SWU_TxBuffer((uint8_t*)TXBUFFSTR, sizeof(TXBUFFSTR) - 1);

    uint8_t bytes_readed[4];

    memset(bytes_readed, 0, 4);
    printf("please, type 3 characters (echo is ENABLED): ");
    SWU_RxBuffer_echo(bytes_readed, 3);
    printf("\r\nbytes readed: %s\r\n", bytes_readed);

    memset(bytes_readed, 0, 4);
    printf("please, type 3 characters (echo is disabled): ");
    SWU_RxBuffer(bytes_readed, 3);
    printf("\r\nbytes readed: %s\r\n", bytes_readed);

    int number;

    number = 0;
    printf("please, type a decimal number and press ENTER (echo is ENABLED): ");
    fscanf(&soft_uart_echo, "%d", &number);
    printf("\r\nnumber in hex: 0x%X\r\n", number);

    number = 0;
    printf("please, type a decimal number and press ENTER (echo is disabled): ");
    scanf("%d", &number);
    printf("\r\nnumber in hex: 0x%X\r\n", number);

    stdin = stdout = &soft_uart_echo;
    number = 0;
    printf("please, type a decimal number and press ENTER (echo is FORCED ENABLE): ");
    scanf("%d", &number);
    printf("\r\nnumber in hex: 0x%X\r\n", number);

    printf("\r\ntype some characters and press ENTER to test my_rx_handler\r\n");
    rx_flag = false;
    uint8_t last_up = curr_buff_pos;
    SWU_RegisterRxCb(my_rx_handler);
    while (!rx_flag)
    {
        if (last_up != curr_buff_pos)
        {
            last_up = curr_buff_pos;
            printf("\r\ncurr buff: %s\r\n", rx_bff);
        }
    }
    SWU_RegisterRxCb(NULL);

    printf("\r\n\r\nmy_rx_handler executed!, last chars typed: %s\r\n", rx_bff);

    printf("\r\n\r\nTYPE AND ECHO (only valid chars):\r\n");
    // Serial ECHO
    while (1)
    {
        if (sRxDone)
        {
            SWU_RxByte_echo_isprint_nl(NULL);
        }
    }
}

