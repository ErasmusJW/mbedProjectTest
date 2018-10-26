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
extern ADC_HandleTypeDef    AdcHandle;

/* Variable used to get converted value */
extern __IO uint32_t uhADCxConvertedValue[1000] ;

extern ADC_ChannelConfTypeDef sConfig;


extern DigitalOut led1;
extern DigitalOut led2;
extern DigitalOut led3;

extern Serial pc;

extern __IO uint32_t conversionCompleteConter ;

void DMA_Config(void);

 
  
 void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle);




// #define ADCx                            ADC1
// #define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()
// #define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()
// #define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()

// #define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
// #define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

// /* Definition for ADCx Channel Pin */
// #define ADCx_CHANNEL_PIN                GPIO_PIN_0
// #define ADCx_CHANNEL_GPIO_PORT          GPIOC

// /* Definition for ADCx's Channel */
// #define ADCx_CHANNEL                    ADC_CHANNEL_10

// /* Definition for ADCx's DMA */
// #define ADCx_DMA_CHANNEL                DMA_CHANNEL_0
// #define ADCx_DMA_STREAM                 DMA2_Stream0

// /* Definition for ADCx's NVIC */
// #define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
// #define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler

// #define ADC_BUFFER_SIZE             250

// extern DigitalOut led2;
// extern DigitalOut led3;


//     ADC_HandleTypeDef    m_AdcHandle;
//     ADC_ChannelConfTypeDef m_ChanelConfig;
//     __IO uint32_t m_conversionCompleteConter ;
//     __IO uint32_t m_uiahADCxConvertedValue[ADC_BUFFER_SIZE];

// extern Serial pc;
// void SetAdcDma() ;

// // class AdcDma
// // {
// // public:
// //     AdcDma();

// //     AdcDma(AdcDma &&) = default;
// //     AdcDma(const AdcDma &) = default;
// //     AdcDma &operator=(AdcDma &&) = default;
// //     AdcDma &operator=(const AdcDma &) = default;
// //     ~AdcDma();


// //     static ADC_HandleTypeDef    m_AdcHandle;
// //     static ADC_ChannelConfTypeDef m_ChanelConfig;
// //      static __IO uint32_t m_conversionCompleteConter ;
// //     static __IO uint32_t m_uiahADCxConvertedValue[ADC_BUFFER_SIZE] ;
   

// // private:
// //     void SetAdcDma() ;

// //     //need to pass this shit to a c library, I think going  to try a few things strating with static instead of global




    
// // };

// void PrintLine(int line)
// {
//     pc.printf(" Line %i \n\r",line);
//     wait(0.4);
// }

// void SetAdcDma() 
// {
//   PrintLine(__LINE__);
//   m_AdcHandle.Instance                   = ADCx;
  
  
//   m_AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.ContinuousConvMode    = ENABLE;                       /* Continuous mode enabled to have continuous conversion  */
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.NbrOfDiscConversion   = 0;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.NbrOfConversion       = 1;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.DMAContinuousRequests = ENABLE;
//   PrintLine(__LINE__);
//   m_AdcHandle.Init.EOCSelection          = DISABLE;
// PrintLine(__LINE__);
// PrintLine(__LINE__);
//   if (HAL_ADC_Init(&m_AdcHandle) != HAL_OK)
//   {
//     /* ADC initialization Error */
//     led2 = 1;
//     PrintLine(__LINE__);
//     pc.printf(" ADC  ######################################### Error");
//     wait(1);
//     while(1){};
//   }
//     PrintLine(__LINE__);

//   /*##-2- Configure ADC regular channel ######################################*/
  
//   m_ChanelConfig.Channel      = ADC_CHANNEL_10;
//   PrintLine(__LINE__);
//   m_ChanelConfig.Rank         = 1;
//   PrintLine(__LINE__);
//   m_ChanelConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
//   PrintLine(__LINE__);
//   m_ChanelConfig.Offset       = 0;
//     PrintLine(__LINE__);

//   if (HAL_ADC_ConfigChannel(&m_AdcHandle, &m_ChanelConfig) != HAL_OK)
//   {
//     /*  */
// PrintLine(__LINE__);
//     led2 = 1;
//     pc.printf(" Channel Configuration Error  ######################################### Error");
//     wait(1);
//     while(1){};
//   }
//     PrintLine(__LINE__);


//   if(HAL_ADC_Start_DMA(&m_AdcHandle, (uint32_t *)m_uiahADCxConvertedValue, 1000) != HAL_OK)
//   {
//     /* Start Conversation Error */
//     led2 = 1;
//     PrintLine(__LINE__);
//     pc.printf("  Start Conversation Error  ######################################### Error");
//     wait(1);
//     while(1){};
//   }
//   PrintLine(__LINE__);
// }




//  void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
// {
//   /* Turn LED1 on: Transfer process is correct */
//   led3 = !led3;
//   m_conversionCompleteConter++;
//   PrintLine(__LINE__);
// }

// extern "C" {
//     void ADCx_DMA_IRQHandler(void)
//     {
//       HAL_DMA_IRQHandler(m_AdcHandle.DMA_Handle);
//     }

//     void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
//     {
//       GPIO_InitTypeDef          GPIO_InitStruct;
//       static DMA_HandleTypeDef  hdma_adc;

//       /*##-1- Enable peripherals and GPIO Clocks #################################*/
//       /* ADC1 Periph clock enable */
//       ADCx_CLK_ENABLE();
//       /* Enable GPIO clock ****************************************/
//       ADCx_CHANNEL_GPIO_CLK_ENABLE();
//       /* Enable DMA2 clock */
//       DMAx_CLK_ENABLE();

//       /*##-2- Configure peripheral GPIO ##########################################*/
//       /* ADC Channel GPIO pin configuration */
//       GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
//       GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//       GPIO_InitStruct.Pull = GPIO_NOPULL;
//       HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

//       /*##-3- Configure the DMA streams ##########################################*/
//       /* Set the parameters to be configured */
//       hdma_adc.Instance = ADCx_DMA_STREAM;

//       hdma_adc.Init.Channel  = ADCx_DMA_CHANNEL;
//       hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
//       hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
//       hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
//       hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
//       hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
//       hdma_adc.Init.Mode = DMA_CIRCULAR;
//       hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
//       hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//       hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
//       hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
//       hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

//       HAL_DMA_Init(&hdma_adc);

//       /* Associate the initialized DMA handle to the ADC handle */
//       __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

//       /*##-4- Configure the NVIC for DMA #########################################*/
//       /* NVIC configuration for DMA transfer complete interrupt */
//       HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
//       HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
//     }
//     void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
//     {

//       /*##-1- Reset peripherals ##################################################*/
//       ADCx_FORCE_RESET();
//       ADCx_RELEASE_RESET();

//       /*##-2- Disable peripherals and GPIO Clocks ################################*/
//       /* De-initialize the ADC Channel GPIO pin */
//       HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
//     }


// }

#endif
