/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2013, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP432 CODE EXAMPLE DISCLAIMER
 *
 * MSP432 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see MSP432 SDK http://www.ti.com/tool/simplelink-msp432-sdk
 * for an API library & https://dev.ti.com/pinmux/ for a GUI-based approach
 * to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//   MSP432P4111 Demo - Device configuration for operation @ MCLK = DCO = 48MHz
//
//   Description: Proper device configuration to enable operation at MCLK=48MHz
//   including:
//   1. Configure VCORE level to 1
//   2. Configure flash wait-state to 3
//   3. Configure DCO frequency to 48MHz
//   4. Ensure MCLK is sourced by DCO
//
//   After configuration is complete, MCLK is output to port pin P4.3.
//
//                MSP432P411x
//             -----------------
//         /|\|                 |
//          | |                 |
//          --|RST              |
//            |             P4.3|----> MCLK
//            |                 |
//
//   Bob Landers
//   Texas Instruments Inc.
//   August 2017
//   Built with CCSv7.1, IAR EWARM V8.11, Keil 5.23, GCC 4.9.3
//******************************************************************************
#include "ti/devices/msp432p4xx/inc/msp.h"
#include <stdint.h>


void error(void);

int main(void)
{
    volatile uint32_t i;

    WDT_A->CTL = WDT_A_CTL_PW |
                 WDT_A_CTL_HOLD;            // Stop WDT

    P1->DIR |= BIT0;                        // P1.0 set as output

    /*Set Vcore =1, and flash wait state = 3 for operation up to 48MHz
     * Refer to device datasheet for execution frequency Vs flash wait-state
     * and for minimum supply voltage at Vcore = 1
     */

    /* Step 1: Transition to VCORE Level 1: AM_LDO_VCORE0 --> AM_LDO_VCORE1 */
    PCM->CTL0 = PCM_CTL0_KEY_VAL | PCM_CTL0_AMR_1;
    while ((PCM->CTL1 & PCM_CTL1_PMR_BUSY));
    //check errors
    if (PCM->IFG & PCM_IFG_AM_INVALID_TR_IFG)
        error();                            // Error if transition was not successful
    if ((PCM->CTL0 & PCM_CTL0_CPM_MASK) != PCM_CTL0_CPM_1)
        error();                            // Error if device is not in AM_LDO_VCORE1 mode

    /* Step 2: Configure Flash wait-state to 3 for both banks 0 & 1 */
    FLCTL_A->BANK0_RDCTL = (FLCTL_A->BANK0_RDCTL & ~(FLCTL_A_BANK0_RDCTL_WAIT_MASK)) |
            FLCTL_A_BANK0_RDCTL_WAIT_3;
    FLCTL_A->BANK1_RDCTL  = (FLCTL_A->BANK0_RDCTL & ~(FLCTL_A_BANK1_RDCTL_WAIT_MASK)) |
            FLCTL_A_BANK1_RDCTL_WAIT_3;

    /* Step 3: Configure DCO to 48MHz, ensure MCLK uses DCO as source*/
    CS->KEY = CS_KEY_VAL ;                  // Unlock CS module for register access
    CS->CTL0 = 0;                           // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_5;           // Set DCO to 48MHz
    /* Select MCLK = DCO, no divider */
    CS->CTL1 = (CS->CTL1 & ~(CS_CTL1_SELM_MASK | CS_CTL1_DIVM_MASK)) |
            CS_CTL1_SELM_3;
    CS->KEY = 0;                            // Lock CS module from unintended accesses

    /* Step 4: Output MCLK to port pin to demonstrate 48MHz operation */
    P4->DIR |= BIT3;
    P4->SEL0 |=BIT3;                        // Output MCLK
    P4->SEL1 &= ~(BIT3);

    while (1)                               // continuous loop
    {
        P1->OUT ^= BIT0;                    // Blink P1.0 LED
        for (i = 200000; i > 0; i--);       // Delay
    }
}

void error(void)
{
    //leave on LED to indicate error
    P1->OUT |= BIT0;
    while (1);
}
