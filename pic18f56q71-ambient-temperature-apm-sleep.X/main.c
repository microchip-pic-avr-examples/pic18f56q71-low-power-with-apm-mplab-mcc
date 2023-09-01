 /*
 * MAIN Generated Driver File
 * 
 * @file main.c
 * 
 * @defgroup main MAIN
 * 
 * @brief This is the generated driver implementation file for the MAIN driver.
 *
 * @version MAIN Driver Version 1.0.0
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
#include "mcc_generated_files/system/system.h"

#define TEMP0_VOLTAGE               0.5f
#define TEMP_COEFFICIENT_MCP9700    0.01f
#define VDD                         3.3f
#define OPAMP_GAIN                  2.67f
#define ADC_RESOLUTION              4096

bool adcContext1Flag = false;

void ADC_UserContext1Callback(void);
void ClearADCFlag(void);
void TemperatureProcessing(void);
void PrintTemperature(float ambientTemperature);

/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();

    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts 
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts 
    // Use the following macros to: 

    // Enable the Global High Interrupts 
    INTERRUPT_GlobalInterruptHighEnable(); 

    // Disable the Global High Interrupts 
    //INTERRUPT_GlobalInterruptHighDisable(); 

    // Enable the Global Low Interrupts 
    INTERRUPT_GlobalInterruptLowEnable(); 

    // Disable the Global Low Interrupts 
    //INTERRUPT_GlobalInterruptLowDisable(); 
    
    ADC_SetContext1ThresholdInterruptHandler(ADC_UserContext1Callback);       
    
    while(1)
    {
        if(adcContext1Flag)
        {
            TemperatureProcessing();

            ClearADCFlag();
        }
        
        SLEEP();
        NOP();
    }    
}

void ADC_UserContext1Callback(void)
{
    adcContext1Flag = true;
}

void ClearADCFlag(void)
{
    adcContext1Flag = false;
}

void TemperatureProcessing(void)
{
    float inputADCVoltage;
    float calculatedTemperature;
    uint16_t adcResult;
    
    adcResult = ADC_GetFilterValue();
    
    inputADCVoltage = (float) adcResult * VDD / ADC_RESOLUTION;
    
    calculatedTemperature = (float) (inputADCVoltage / OPAMP_GAIN - TEMP0_VOLTAGE ) / TEMP_COEFFICIENT_MCP9700;
    
    PrintTemperature(calculatedTemperature);
}

void PrintTemperature(float ambientTemperature)
{
    UART2_Enable();

    printf("%.2f °C \n\r", ambientTemperature);

    while(!UART2_IsTxDone())
        ;
    
    UART2_Disable();
}