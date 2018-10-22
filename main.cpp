#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"


DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

AnalogOut   A_out0(PA_5);

 Serial pc(USBTX, USBRX); // tx, rx

//header defines

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


//* header defines 



static __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
ADC_HandleTypeDef    AdcHandle;

/* Variable used to get converted value */
__IO uint32_t uhADCxConvertedValue[1000] ;

/* Private function prototypes -----------------------------------------------*/

static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
static void DMA_Config(void);
/* Private functions ---------------------------------------------------------*/


//het hom self promote na global;
ADC_ChannelConfTypeDef sConfig;



static bool bPrint = false;
Ticker flipper;
void flip() {
    led1 = !led1;
    bPrint = true;
}

Ticker OutTimmer;

__IO uint32_t conversionCompleteConter = 0;

uint16_t analogOutCount = 0;
int main() {
    pc.baud(115200);
    pc.printf("Startup \n\r");
 
    led1 = 1;
    led2 = 1;
    led3 = 1;
    wait(2);
    led1 = 0;
    led2 = 0;
    led3 = 0;
    wait(2);
    // t.start(callback(&queue, &EventQueue::dispatch_forever));
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
    //flipper.attach(&flip, 0.25); // the address of the function to be attached (flip) and the interval (2 seconds)

    OutTimmer.attach_us(&flip,1000000);

    transferErrorDetected = 0;


    DMA_Config();

    while (true) 
    {

        A_out0.write_u16(analogOutCount);
        analogOutCount += 2048;
        if (transferErrorDetected == 1)
        {
          /* Turn LED2 on*/
            led2 =1;
            pc.printf("Transfer Error detected ********* \n\r");
            wait(1);
            transferErrorDetected = 0;
        }
        if(bPrint)
        {
          bPrint =false;
          pc.printf("v : %i c : %i \n\r",uhADCxConvertedValue[0],conversionCompleteConter);
        }


    }
}

static void DMA_Config(void)
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