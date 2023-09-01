/**
 * APM Generated Driver API Header File
 * 
 * @file apm.h
 * 
 * @defgroup  apm APM
 * 
 * @brief This file contains the API prototypes for the APM module.
 *
 * @version APM Driver Version 1.0.1
*/
/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef APM_H
#define APM_H

/**
  Section: Included Files
*/

#include <stdint.h>

/**
  Section: APM APIs
*/

/**
 * @ingroup apm
 * @brief Initializes the APM module. This is called only once before calling other APM APIs.
 * @param None.
 * @return None.
 */ 
void APM_Initialize(void);

/**
 * @ingroup apm
 * @brief  Starts the APM operation.
 * @pre APM_Initialize() is already called.
 * @param None.
 * @return None.
 */ 
void APM_Start(void);

/**
 * @ingroup apm
 * @brief Stops the APM operation.
 * @pre APM_Initialize() is already called.
 * @param None.
 * @return None.
 */ 
void APM_Stop(void);

/**
 * @ingroup apm
 * @brief Sets the prescaler value for the APM 16-bit internal timer.
 * @pre APM_Initialize() is already called.
 * @param prescalerVal - 16-bit count value for the APM host timer clock prescaler.
 * @return None.
 */ 
void APM_PrescalerSet(uint16_t prescalerVal);

/**
 * @ingroup apm
 * @brief Sets the initial value for the 16-bit period counter.
 * @pre APM_Initialize() is already called.
 * @param periodVal - Initial 16-bit count value for the APM host timer clock period.
 * @return None.
 */ 
void APM_PeriodSet(uint16_t periodVal);

/**
 * @ingroup apm
 * @brief Sets the number of ticks of the 16-bit timer before the Start 1 Event occurs.
 * @pre APM_Initialize() is already called.
 * @param Start1Val - Number of ticks of the 16-bit timer.
 * @return None.
 */ 
void APM_Start1CountSet(uint16_t Start1Val);

/**
 * @ingroup apm
 * @brief Sets the number of ticks of the 24-bit timer before the Start 2 Event occurs. The 24-bit offset timer begins to tick after the Start 1 Event occurs.
 * @pre APM_Initialize() and APM_Start1CountSet() are already called.
 * @param Start2Val - Number of ticks of the 24-bit offset timer.
 * @return None.
 */ 
void APM_Start2CountSet(uint24_t Start2Val);

/**
 * @ingroup apm
 * @brief Sets the number of ticks of the 24-bit timer before the End 1 Event occurs. The 24-bit offset timer begins ticking after the Start 1 Event occurs.
 * @pre APM_Initialize() and APM_Start1CountSet() are already called.
 * @param End1Val - Number of ticks of the 24-bit offset timer.
 * @return None.
 */ 
void APM_End1CountSet(uint24_t End1Val);

/**
 * @ingroup apm
 * @brief Sets the number of ticks of the 24-bit timer before the End 2 Event occurs. The 24-bit offset timer begins ticking after the Start 1 Event occurs.
 * @pre APM_Initialize(), APM_Start1CountSet(), APM_Start2CountSet() and APM_End1CountSet() are already called.
 * @param End2Val - Number of ticks of the 24-bit offset timer.
 * @return None.
 */ 
void APM_End2CountSet(uint24_t End2Val);

/**
 * @ingroup apm
 * @brief Returns the status of the peripherals of the APM module.
 * @pre APM_Initialize() is already called.
 * @param None.
 * @return uint16_t - 16-bit value that represents the enabled and disabled analog peripherals.
 */ 
uint16_t APM_StatusGet(void);

#endif // APM_H
/**
 End of File
*/
