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
© [2022] Microchip Technology Inc. and its subsidiaries.

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
#include <util/delay.h>
#include <stdio.h>

#define TOP_VALUE 0xFFFF
#define DATA_STREAMER_START_BYTE 0x5F //trivial Data Streamer Protocol start of frame token
#define DATA_STREAMER_END_BYTE 0xA0 
#define PRESCALAR 0x01
#define ADC_CHANNEL 19 /*   read ADC CHANNEL 19   : pin PF3   */
//ADC_SAMPNUM_t numSamples = ADC_SAMPNUM_NONE_gc;
ADC_SAMPNUM_t numSamples = ADC_SAMPNUM_ACC32_gc;
//ADC_SAMPNUM_t numSamples = ADC_SAMPNUM_ACC64_gc;
//ADC_SAMPNUM_t numSamples = ADC_SAMPNUM_ACC128_gc;

ADC_SAMPDLY_t sampDelay = ADC_SAMPDLY_DLY0_gc;

uint16_t adc_result;
uint8_t adc_lsb;
uint8_t adc_msb;

void USART0_Send(uint8_t);
void SW0_callback(void);
uint8_t getNumSamples();


void Terminal()
{
    printf("\nADC Result ----------------------------------------\n");
    printf("Number of samples accumulated: %u\n", getNumSamples());
    
    adc_result = ADC0_GetConversionResult();
    printf("adc_result before averaging is %u\n", adc_result);
    
    //adc_result = (numSamples = 7) ? adc_result >> 6 : adc_result >> numSamples;
    
    // Check to see if sample number accumulation requires truncating
    if(numSamples == ADC_SAMPNUM_ACC128_gc) {
        adc_result = adc_result >> 6;
    }
    else {
        adc_result = adc_result >> numSamples;
    }
    printf("adc_result after averaging is %u\n", adc_result);
    
    adc_lsb = (uint8_t)(adc_result);
    adc_msb = (uint8_t)(adc_result >> 8);
    
    _delay_ms(5000);
}

void Variable_Stream()
{
    //Get new result
	adc_result = ADC0_GetConversionResult();
    
    //Average based on number of samples (10-bit Resolution)
    // Check to see if accumulation requires truncating
    if(numSamples == ADC_SAMPNUM_ACC128_gc) {
        adc_result = adc_result >> 6;
    }
    else {
        adc_result = adc_result >> numSamples;
    }
    
    //Pack into Byte Stream Format
    adc_lsb = (uint8_t)(adc_result);
    adc_msb = (uint8_t)(adc_result >> 8);
    
    //Stream Data
	USART0_Send(DATA_STREAMER_START_BYTE);    /* Data Streamer protocol START */
    USART0_Send(adc_lsb);                     /* ADC sample LSB */
    USART0_Send(adc_msb);                     /* ADC sample MSB */
    USART0_Send(numSamples);  /* Number of Samples Accumulated */  
    //USART0_Send(sampDelay);                   /* Sample Delay */
    USART0_Send(PRESCALAR);                   /* Prescalar    */
    USART0_Send(DATA_STREAMER_END_BYTE);      /* Data Streamer protocol END */
}
/*
    Main application
*/

int main(void)
{
    SYSTEM_Initialize();
    
    TCB0_CaptureCallbackRegister(&SW0_callback);
    
      
    // Set Number of Accumulations 
    ADC0.CTRLB = numSamples;
     
    ADC0_StartConversion(ADC_CHANNEL);
    while(1)
    {
        while(!ADC0_IsConversionDone()){;}
        
        Variable_Stream();
        //Terminal();
    }
}

void USART0_Send(uint8_t byte) {
    USART0_Write(byte);
    
    while(!USART0_IsTxReady()) {;}
    
    USART0.STATUS |= USART_TXCIF_bm;
}

void SW0_callback() {
    if(SW0_GetValue())
    {
        ADC0_StopConversion();
        LED0_SetHigh();
               
        //Increment the Samples Accumulated 0, 2, 4, 8, 16, 32, 64, 128
        numSamples++;
        
        if(numSamples == 8) {
            numSamples = 0;
        }
        
        // Set Number of Accumulations 
        ADC0.CTRLB = numSamples;
        
//        // This bit field defines the delay between consecutive ADC samples. This allows modifying the sampling frequency
//        // used during hardware accumulation, to suppress periodic noise that may otherwise disturb the sampling. The delay is
//        // expressed as CLK_ADC cycles and is given directly by the bit field setting.
//        sampDelay++;
//        
//        if(sampDelay == 16) {
//            sampDelay = 0;
//        }
//        
//        // Set the Sampling Delay
//        ADC0.CTRLD = sampDelay;
        
        //Restart Conversion
        ADC0_StartConversion(ADC_CHANNEL);
    }
    else 
    {
        LED0_SetLow();
    }
}

//Returns the number samples accumulated based on ADC_SAMPNUM_t numSamples
uint8_t getNumSamples() {
    uint8_t div;
    switch(numSamples) {
        case ADC_SAMPNUM_NONE_gc:         // ADC_SAMPNUM_NONE_gc
            div = 0;
            break;
        case ADC_SAMPNUM_ACC2_gc:         // ADC_SAMPNUM_ACC2_gc
            div = 2;
            break;
        case ADC_SAMPNUM_ACC4_gc:         // ADC_SAMPNUM_ACC4_gc
            div = 4; 
            break;
        case ADC_SAMPNUM_ACC8_gc:         // ADC_SAMPNUM_ACC8_gc
            div = 8;
            break;
        case ADC_SAMPNUM_ACC16_gc:         // ADC_SAMPNUM_ACC16_gc
            div = 16;
            break;
        case ADC_SAMPNUM_ACC32_gc:         // ADC_SAMPNUM_ACC32_gc
            div = 32;
            break;
        case ADC_SAMPNUM_ACC64_gc:         // ADC_SAMPNUM_ACC64_gc
            div = 64;
            break;
        case ADC_SAMPNUM_ACC128_gc:         // ADC_SAMPNUM_ACC128_gc
            div = 128;
            break;
        default:
            div = 0;
            break; 
    }
    return div;
}