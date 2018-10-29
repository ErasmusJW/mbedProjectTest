#ifndef ADC_DMA_H
#define ADC_DMA_H

#include "mbed.h"

#define ADCx                            ADC1
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_0
#define ADCx_CHANNEL_GPIO_PORT          GPIOC

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_10

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_0
#define ADCx_DMA_STREAM                 DMA2_Stream0

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler

extern  __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
//extern ADC_HandleTypeDef    AdcHandle;

/* Variable used to get converted value */
extern __IO uint32_t uhADCxConvertedValue[1000] ;

extern ADC_ChannelConfTypeDef sConfig;


extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;

extern Serial pc;

extern __IO uint32_t conversionCompleteConter ;



 
  
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle);

class AdcDma
{
public:
    AdcDma();
    AdcDma(AdcDma &&) = default;
    AdcDma(const AdcDma &) = default;
    AdcDma &operator=(AdcDma &&) = default;
    AdcDma &operator=(const AdcDma &) = default;
    ~AdcDma();
    static ADC_HandleTypeDef    AdcHandle;
    void DMA_Config(void);

private:
    
};






#endif
