#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"
#include "ADC_Dma.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

//AnalogOut   A_out0(PA_5);

 Serial pc(USBTX, USBRX); // tx, rx

//header defines




//* header defines 



#define DACx                            DAC
#define DACx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()


#define DACx_CLK_ENABLE()               __HAL_RCC_DAC_CLK_ENABLE()
#define DACx_FORCE_RESET()              __HAL_RCC_DAC_FORCE_RESET()
#define DACx_RELEASE_RESET()            __HAL_RCC_DAC_RELEASE_RESET()

/* Definition for DACx Channel Pin */
#define DACx_CHANNEL_PIN                GPIO_PIN_4
#define DACx_CHANNEL_GPIO_PORT          GPIOA

/* Definition for DACx's Channel */
#define DACx_DMA_CHANNEL                DMA_CHANNEL_7

#define DACx_CHANNEL                    DAC_CHANNEL_1

/* Definition for DACx's DMA_STREAM */
#define DACx_DMA_INSTANCE               DMA1_Stream5

/* Definition for DACx's NVIC */
#define DACx_DMA_IRQn                   DMA1_Stream5_IRQn
#define DACx_DMA_IRQHandler             DMA1_Stream5_IRQHandler




DAC_HandleTypeDef    DacHandle;
static DAC_ChannelConfTypeDef DACsConfig;
//const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
 uint8_t aEscalator8bit[2] = {0x00, 0xFF};
__IO uint8_t ubSelectedWavesForm = 1;
__IO uint8_t ubKeyPressed = SET;





// static void DAC_Ch1_EscalatorConfig(void);
  static void TIM6_Config(void);
  static void DAC_Ch1_EscalatorConfig(void);






















extern  __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
/* Variable used to get converted value */
extern __IO uint32_t uhADCxConvertedValue[1000] ;


static bool bPrint = false;
Ticker flipper;
void flip() {
    led1 = !led1;
    bPrint = true;
}

Ticker OutTimmer;

extern __IO uint32_t conversionCompleteConter ;

uint16_t analogOutCount = 0;
int main() {
    pc.baud(115200);
    pc.printf("Startup \n\r");
    
    AdcDma  AdcDmaInstance;
 
    led1 = 1;
    led2 = 1;
    led3 = 1;
    wait(2);
    led1 = 0;
    led2 = 0;
    led3 = 0;
    wait(2);






    DacHandle.Instance = DACx;
    TIM6_Config();


    DAC_Ch1_EscalatorConfig();










    // t.start(callback(&queue, &EventQueue::dispatch_forever));
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
    //flipper.attach(&flip, 0.25); // the address of the function to be attached (flip) and the interval (2 seconds)

    OutTimmer.attach_us(&flip,1000000);

    transferErrorDetected = 0;


    AdcDmaInstance.DMA_Config();

    while (true) 
    {
         //pc.printf("poes ");
        //A_out0.write_u16(analogOutCount);
        analogOutCount += 270;
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
          pc.printf("print ");
          pc.printf("v : %i c : %i \n\r",uhADCxConvertedValue[0],conversionCompleteConter);
        }


    }
}





void TIM6_Config(void)
{
  static TIM_HandleTypeDef  htim;
  TIM_MasterConfigTypeDef sMasterConfig;

  /*##-1- Configure the TIM peripheral #######################################*/
  /* Time base configuration */
  htim.Instance = TIM7;
    // 2700 40 khz

  //(freq / 2) = desiredfreqeny ( x)
    // x = 108 000 000 / desired freqenxy

  htim.Init.Period            = 2700;
  htim.Init.Prescaler         = 0;
  htim.Init.ClockDivision     = 0;
  htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim.Init.RepetitionCounter = 0;
  htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; 
  HAL_TIM_Base_Init(&htim);

  /* TIM6 TRGO selection */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

  /*##-2- Enable TIM peripheral counter ######################################*/
  HAL_TIM_Base_Start(&htim);
}





static void DAC_Ch1_EscalatorConfig(void)
{
  /*##-1- Initialize the DAC peripheral ######################################*/
  if (HAL_DAC_Init(&DacHandle) != HAL_OK)
  {
    /* Initialization Error */
    led2 = 1;
    pc.printf(" DAC Initialization  ######################################### Error");
    wait(1);
    while(1){};
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  DACsConfig.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
  DACsConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandle, &DACsConfig, DACx_CHANNEL) != HAL_OK)
  {
    /* Channel configuration Error */
    led2 = 1;
    pc.printf(" DAC Channel configuration  ######################################### Error");
    wait(1);
    while(1){};
  }

  pc.printf(" DAC Channel configuration complete");

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)aEscalator8bit, 2, DAC_ALIGN_8B_R) != HAL_OK)
  {
    /* Start DMA Error */
    led2 = 1;
    pc.printf(" DAC Start DMA Error  ######################################### Error");
    wait(1);
    while(1){};
  }

 pc.printf(" DAC dma start");
}




extern "C" {

    
    //has to be fucking c poes
    void DACx_DMA_IRQHandler(void)
    {
     HAL_DMA_IRQHandler(DacHandle.DMA_Handle1);
    }


    void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
    {
        GPIO_InitTypeDef          GPIO_InitStruct;
        static DMA_HandleTypeDef  hdma_dac1;

        /* DAC Periph clock enable */
        DACx_CLK_ENABLE();


        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO clock ****************************************/
        DACx_CHANNEL_GPIO_CLK_ENABLE();

        /* DMA1 clock enable */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* DAC Channel1 GPIO pin configuration */
        GPIO_InitStruct.Pin = DACx_CHANNEL_PIN;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DACx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

        /*##-3- Configure the DMA ##########################################*/
        /* Set the parameters to be configured for DACx_DMA_STREAM */
        hdma_dac1.Instance = DACx_DMA_INSTANCE;

        hdma_dac1.Init.Channel  = DACx_DMA_CHANNEL;

        hdma_dac1.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_dac1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_dac1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_dac1.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_dac1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_dac1.Init.Mode = DMA_CIRCULAR;
        hdma_dac1.Init.Priority = DMA_PRIORITY_HIGH;

        HAL_DMA_Init(&hdma_dac1);

        /* Associate the initialized DMA handle to the DAC handle */
        __HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1);

        /*##-4- Configure the NVIC for DMA #########################################*/
        /* Enable the DMA1_Stream5 IRQ Channel */
        HAL_NVIC_SetPriority(DACx_DMA_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(DACx_DMA_IRQn);

    }

    /**
     * @brief  DeInitializes the DAC MSP.
     * @param  hdac: pointer to a DAC_HandleTypeDef structure that contains
     *         the configuration information for the specified DAC.
     * @retval None
     */
    void HAL_DAC_MspDeInit(DAC_HandleTypeDef *hdac)
    {
        /*##-1- Reset peripherals ##################################################*/
        DACx_FORCE_RESET();
        DACx_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* De-initialize the DAC Channel1 GPIO pin */
        HAL_GPIO_DeInit(DACx_CHANNEL_GPIO_PORT, DACx_CHANNEL_PIN);

        // /*##-3- Disable the DMA Stream ############################################*/
        // /* De-Initialize the DMA Stream associate to DAC_Channel1 */
        // HAL_DMA_DeInit(hdac->DMA_Handle1);

        // /*##-4- Disable the NVIC for DMA ###########################################*/
        // HAL_NVIC_DisableIRQ(DACx_DMA_IRQn);
    }

    /**
     * @brief TIM MSP Initialization
     *        This function configures the hardware resources used in this example:
     *           - Peripheral's clock enable
     *           - Peripheral's GPIO Configuration
     * @param htim: TIM handle pointer
     * @retval None
     */
    void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
    {
        /* TIM6 Periph clock enable */
        __HAL_RCC_TIM7_CLK_ENABLE();
    }

    /**
     * @brief TIM MSP De-Initialization
     *        This function frees the hardware resources used in this example:
     *          - Disable the Peripheral's clock
     *          - Revert GPIO to their default state
     * @param htim: TIM handle pointer
     * @retval None
     */
    void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
    {

        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_TIM7_FORCE_RESET();
        __HAL_RCC_TIM7_RELEASE_RESET();
    }


}