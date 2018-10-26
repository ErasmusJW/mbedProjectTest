#ifndef ADC_DMA_CPP
#define ADC_DMA_CPP
#include "ADC_Dma.h"
__IO uint32_t conversionCompleteConter = 0;



  __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
 ADC_HandleTypeDef    AdcHandle;

/* Variable used to get converted value */
 __IO uint32_t uhADCxConvertedValue[1000] ;

 ADC_ChannelConfTypeDef sConfig;


void DMA_Config(void)
{


  AdcHandle.Instance          = ADCx;
  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                       /* Continuous mode enabled to have continuous conversion  */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 0;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.DMAContinuousRequests = ENABLE;
  AdcHandle.Init.EOCSelection          = DISABLE;


  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    /* ADC initialization Error */
    led2 = 1;
    pc.printf(" ADC  ######################################### Error");
    wait(1);
    while(1){};
  }


  /*##-2- Configure ADC regular channel ######################################*/
  sConfig.Channel      = ADC_CHANNEL_10;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  sConfig.Offset       = 0;


  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /*  */

    led2 = 1;
    pc.printf(" Channel Configuration Error  ######################################### Error");
    wait(1);
    while(1){};
  }



  if(HAL_ADC_Start_DMA(&AdcHandle, (uint32_t *)uhADCxConvertedValue, 1000) != HAL_OK)
  {
    /* Start Conversation Error */
    led2 = 1;
    pc.printf("  Start Conversation Error  ######################################### Error");
    wait(1);
    while(1){};
  }

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
  /* Turn LED1 on: Transfer process is correct */
  led3 = !led3;
  conversionCompleteConter++;
}



extern "C" {
    void ADCx_DMA_IRQHandler(void)
    {
      HAL_DMA_IRQHandler(AdcHandle.DMA_Handle);
    }

    void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
    {
      GPIO_InitTypeDef          GPIO_InitStruct;
      static DMA_HandleTypeDef  hdma_adc;

      /*##-1- Enable peripherals and GPIO Clocks #################################*/
      /* ADC1 Periph clock enable */
      ADCx_CLK_ENABLE();
      /* Enable GPIO clock ****************************************/
      ADCx_CHANNEL_GPIO_CLK_ENABLE();
      /* Enable DMA2 clock */
      DMAx_CLK_ENABLE();

      /*##-2- Configure peripheral GPIO ##########################################*/
      /* ADC Channel GPIO pin configuration */
      GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

      /*##-3- Configure the DMA streams ##########################################*/
      /* Set the parameters to be configured */
      hdma_adc.Instance = ADCx_DMA_STREAM;

      hdma_adc.Init.Channel  = ADCx_DMA_CHANNEL;
      hdma_adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma_adc.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_adc.Init.MemInc = DMA_MINC_ENABLE;
      hdma_adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
      hdma_adc.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
      hdma_adc.Init.Mode = DMA_CIRCULAR;
      hdma_adc.Init.Priority = DMA_PRIORITY_HIGH;
      hdma_adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
      hdma_adc.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
      hdma_adc.Init.MemBurst = DMA_MBURST_SINGLE;
      hdma_adc.Init.PeriphBurst = DMA_PBURST_SINGLE;

      HAL_DMA_Init(&hdma_adc);

      /* Associate the initialized DMA handle to the ADC handle */
      __HAL_LINKDMA(hadc, DMA_Handle, hdma_adc);

      /*##-4- Configure the NVIC for DMA #########################################*/
      /* NVIC configuration for DMA transfer complete interrupt */
      HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 0, 0);
      HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);
    }
    void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
    {

      /*##-1- Reset peripherals ##################################################*/
      ADCx_FORCE_RESET();
      ADCx_RELEASE_RESET();

      /*##-2- Disable peripherals and GPIO Clocks ################################*/
      /* De-initialize the ADC Channel GPIO pin */
      HAL_GPIO_DeInit(ADCx_CHANNEL_GPIO_PORT, ADCx_CHANNEL_PIN);
    }


}

//     ADC_HandleTypeDef    m_AdcHandle;
//     ADC_ChannelConfTypeDef m_ChanelConfig;
//     __IO uint32_t m_conversionCompleteConter =0;
//     __IO uint32_t m_uiahADCxConvertedValue[ADC_BUFFER_SIZE];


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