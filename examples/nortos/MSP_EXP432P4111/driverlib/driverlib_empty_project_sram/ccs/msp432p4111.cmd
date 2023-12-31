/******************************************************************************
*
* Copyright (C) 2012 - 2016 Texas Instruments Incorporated - http://www.ti.com/
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
*  Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*
*  Redistributions in binary form must reproduce the above copyright
*  notice, this list of conditions and the following disclaimer in the
*  documentation and/or other materials provided with the
*  distribution.
*
*  Neither the name of Texas Instruments Incorporated nor the names of
*  its contributors may be used to endorse or promote products derived
*  from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Default linker command file for Texas Instruments MSP432P4111
*
* File creation date: 2016-12-07
*
*****************************************************************************/
--retain=flashMailbox

MEMORY
{
    MAIN       (RX) : origin = 0x00000000, length = 0x00200000
    INFO       (RX) : origin = 0x00200000, length = 0x00008000
    SRAM_CODE  (RWX): origin = 0x01000000, length = 0x00040000
    SRAM_DATA  (RW) : origin = 0x20000000, length = 0x00040000
}

/* Section allocation in memory */
SECTIONS
{
    .intvecs:   > 0x00000000
    .text   :   > SRAM_CODE
    .const  :   > SRAM_CODE
    .cinit  :   > SRAM_CODE
    .pinit  :   > SRAM_CODE
    .init_array   :     > SRAM_CODE
    .binit        : {}  > SRAM_CODE

    /* The following sections show the usage of the INFO flash memory        */
    /* INFO flash memory is intended to be used for the following            */
    /* device specific purposes:                                             */
    /* Flash mailbox for device security operations                          */
    .flashMailbox : > 0x00200000
    /* TLV table for device identification and characterization              */
    .tlvTable     : > 0x00201000
    /* BSL area for device bootstrap loader                                  */
    .bslArea      : > 0x00202000

    .vtable :   > 0x20000000
    .data   :   > SRAM_CODE
    .bss    :   > SRAM_CODE
    .sysmem :   > SRAM_CODE
    .stack  :   > SRAM_CODE (HIGH)
}

/* Symbolic definition of the WDTCTL register for RTS */
WDTCTL_SYM = 0x4000480C;

