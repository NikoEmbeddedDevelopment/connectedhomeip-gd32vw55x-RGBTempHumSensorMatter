/*!
    \file    la_mem.h
    \brief   Header file for logic analyzer memory.

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

#ifndef _LA_MEM_H_
#define _LA_MEM_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "co_int.h"

/*
 * DEFINES
 ****************************************************************************************
 */
// Number of 32-bit words in each LA memory line
#define LA_MEM_WORD_COUNT  4

// Number of line in the LA memory
#define LA_MEM_LINE_COUNT (16 * 1024)

/*
 * STRUCTS
 ****************************************************************************************
 */
// Structure describing the format of a line of LA memory
struct la_mem_format
{
    // Line of embedded LA memory
    uint32_t word[LA_MEM_WORD_COUNT];
};

// Embedded LA RAM for MAC
extern struct la_mem_format la_mem_mac[LA_MEM_LINE_COUNT];

#endif // _LA_MEM_H_
