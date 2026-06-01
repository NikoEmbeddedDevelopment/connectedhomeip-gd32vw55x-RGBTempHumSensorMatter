/*!
    \file    console_uart.c
    \brief   CONSOLE UART for GD32VW55x SDK.

    \version 2023-07-20, V1.0.0, firmware for GD32VW55x
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include <ctype.h>
#include "gd32vw55x.h"
#include <stdio.h>
#include "console_uart.h"
#include "wrapper_os.h"
#include "dbg_print.h"
#include "wakelock.h"

#ifdef LOG_UART
static cyclic_buf_t uart_cyc_buf;
extern int cmd_info_send(int id, void *msg_data, uint16_t len);

char uart_buf[UART_BUFFER_SIZE];
uint32_t uart_index = 0;

#if defined(__ARMCC_VERSION)
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    while (RESET == usart_flag_get(LOG_UART, USART_FLAG_TBE));
    usart_data_transmit(LOG_UART, (uint8_t)ch);
    return ch;
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
    /* Send byte to USART */
    while (RESET == usart_flag_get(LOG_UART, USART_FLAG_TBE));
    usart_data_transmit(LOG_UART, (uint8_t)ch);
    /* Return character written */
    return ch;
}
#elif defined(__GNUC__)
int _write(int fd, char *str, int len)
{
    (void)fd;
    int32_t i = 0;

    /* Send string and return the number of characters written */
    while (i != len) {
        while (RESET == usart_flag_get(LOG_UART, USART_FLAG_TBE));
        usart_data_transmit(LOG_UART, *str);
        str++;
        i++;
    }

    while (RESET == usart_flag_get(LOG_UART, USART_FLAG_TC));

    return i;
}
#endif

static void uart_cmd_rx_indicate(void)
{
    if (cmd_info_send(0x23, (void *)(&uart_cyc_buf), uart_index + 1) == 0) {
        if (cyclic_buf_write(&uart_cyc_buf, (uint8_t *)uart_buf, uart_index + 1) == false) {
            dbg_print(ERR, "uart cyclic buffer full\r\n");
        }
    } else {
        /* queue was full */
        dbg_print(ERR, "queue full\r\n");
        /* TODO: report 'message ignored' status */
    }

    uart_index = 0;
}

void log_uart_putc_noint(uint8_t c)
{
    while (RESET == usart_flag_get(LOG_UART, USART_FLAG_TBE));
    usart_data_transmit(LOG_UART, (uint8_t)c);
}

void log_uart_put_data(const uint8_t *d, int size)
{
    uart_put_data(LOG_UART, d, size);
}

static void log_uart_rx_irq_hdl(uint32_t uart_port)
{
    char ch;
    usart_interrupt_disable(uart_port, USART_INT_RBNE);
    while (1) {
        // We should have chance to check overflow error
        // Otherwise it may cause dead loop handle rx interrupt
        if (RESET != usart_flag_get(uart_port, USART_FLAG_ORERR)) {
            usart_flag_clear(uart_port, USART_FLAG_ORERR);
        }

        if ((RESET != usart_flag_get(uart_port, USART_FLAG_RBNE))) {
            ch = (char)usart_data_receive(uart_port);
        } else {
            break;
        }

        if (ch == '\0') {
            break;
        }

        if (isprint(ch)) {
            uart_buf[uart_index++] = ch;
            if (uart_index >= UART_BUFFER_SIZE) {
                uart_index = 0;
            }
            log_uart_putc_noint(ch);
        } else if (ch == '\r') { /* putty doesn't transmit '\n' */
            uart_buf[uart_index] = '\0';

            log_uart_putc_noint('\r');
            log_uart_putc_noint('\n');

            if (uart_index > 0) {
                uart_cmd_rx_indicate();
            } else {
                log_uart_putc_noint('#');
                log_uart_putc_noint(' ');
            }
            sys_wakelock_release(LOCK_ID_USART);
        } else if (ch == '\b') { /* non-destructive backspace */
            if (uart_index > 0) {
                uart_buf[--uart_index] = '\0';
            }
        }
    }

    usart_interrupt_enable(uart_port, USART_INT_RBNE);
}

void console_uart_init(void)
{
    memset(uart_buf, 0, UART_BUFFER_SIZE);
    uart_index = 0;
    cyclic_buf_init(&uart_cyc_buf, 4 * UART_BUFFER_SIZE);

    uart_config(LOG_UART, DEFAULT_LOG_BAUDRATE, false, false, false);
    uart_irq_callback_register(LOG_UART, log_uart_rx_irq_hdl);
}

char log_uart_getc(void)
{
    char ch;
    while (1) {
        if (RESET != usart_flag_get(LOG_UART, USART_FLAG_ORERR)) {
            usart_flag_clear(LOG_UART, USART_FLAG_ORERR);
        }

        if ((RESET != usart_flag_get(LOG_UART, USART_FLAG_RBNE))) {
            ch = (char)usart_data_receive(LOG_UART);
            return ch;
        }
    }
}
#else /* LOG_UART */
#if defined(__ARMCC_VERSION)
/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    return ch;
}
#elif defined(__ICCARM__)
int putchar(int ch)
{
    return ch;
}
#elif defined(__GNUC__)
int _write(int fd, char *str, int len)
{
    (void)fd;

    return 0;
}
#endif

void console_uart_init(void)
{
    return;
}

void log_uart_putc_noint(uint8_t c)
{
    return;
}

char log_uart_getc(void)
{
    return '\0';
}

#endif /* LOG_UART */
