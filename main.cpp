#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"
//#include "ADC_Dma.h"

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
#define DACx_CHANNEL_PIN_I                GPIO_PIN_5
#define DACx_CHANNEL_PIN_Q                GPIO_PIN_4
#define DACx_CHANNEL_GPIO_PORT            GPIOA

/* Definition for DACx's Channel */
#define DACx_DMA_CHANNEL                DMA_CHANNEL_7

#define DACx_CHANNEL_I                    DAC_CHANNEL_2
#define DACx_CHANNEL_Q                    DAC_CHANNEL_1

/* Definition for DACx's DMA_STREAM */
#define DACx_DMA_INSTANCE_I               DMA1_Stream6
#define DACx_DMA_INSTANCE_Q               DMA1_Stream5

/* Definition for DACx's NVIC */
#define DACx_DMA_IRQn_I                  DMA1_Stream6_IRQn
#define DACx_DMA_IRQn_Q                  DMA1_Stream5_IRQn

#define DACx_DMA_IRQHandler_I             DMA1_Stream6_IRQHandler
#define DACx_DMA_IRQHandler_Q             DMA1_Stream5_IRQHandler


#define SYMBOLS_PERTRANSFER 100

uint8_t DataToTransmit_I [SYMBOLS_PERTRANSFER] ;
uint8_t DataToTransmit_Q [SYMBOLS_PERTRANSFER] ;


DAC_HandleTypeDef    DacHandleI;
DAC_HandleTypeDef    DacHandleQ;
static DAC_ChannelConfTypeDef DACsConfigI;
static DAC_ChannelConfTypeDef DACsConfigQ;
//const uint8_t aEscalator8bit[6] = {0x0, 0x33, 0x66, 0x99, 0xCC, 0xFF};
 
__IO uint8_t ubSelectedWavesForm = 1;
__IO uint8_t ubKeyPressed = SET;





// static void DAC_Ch1_EscalatorConfig(void);
  static void TIM6_Config(void);
  static void DAC_Ch1_EscalatorConfig(void);






















// extern  __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
// /* Variable used to get converted value */
// extern __IO uint32_t uhADCxConvertedValue[1000] ;


static bool bPrint = false;
Ticker flipper;
void flip() {
    led1 = !led1;
    bPrint = true;
}

Ticker OutTimmer;

// extern __IO uint32_t conversionCompleteConter ;

uint16_t analogOutCount = 0;
int main() {
    pc.baud(115200);
    pc.printf("Startup \n\r");
    
    //AdcDma  AdcDmaInstance;
 
    led1 = 1;
    led2 = 1;
    led3 = 1;
    wait(2);
    led1 = 0;
    led2 = 0;
    led3 = 0;
    wait(2);


    const uint8_t halfway = SYMBOLS_PERTRANSFER/2 +1;
    const uint8_t incPerHalf = 255/halfway;
    
     
    for (size_t i = 0; i < halfway; i++) //0 - 50
    {
       DataToTransmit_I[i] = i * incPerHalf;
       DataToTransmit_Q[i] = i * incPerHalf;
    }

    for (size_t i = halfway; i < SYMBOLS_PERTRANSFER; i++) //51 - 99
    {
       DataToTransmit_I[i] = DataToTransmit_I[i-1]- incPerHalf;
       DataToTransmit_Q[i] = DataToTransmit_Q[i-1]- incPerHalf;
       
    }


    DacHandleI.Instance = DACx;
    DacHandleQ.Instance = DACx;
    TIM6_Config();


    DAC_Ch1_EscalatorConfig();










    // t.start(callback(&queue, &EventQueue::dispatch_forever));
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
    //flipper.attach(&flip, 0.25); // the address of the function to be attached (flip) and the interval (2 seconds)

    OutTimmer.attach_us(&flip,1000000);

    //transferErrorDetected = 0;


    //AdcDmaInstance.DMA_Config();

    while (true) 
    {
         //pc.printf("poes ");
        //A_out0.write_u16(analogOutCount);
        //analogOutCount += 270;
        // if (transferErrorDetected == 1)
        // {
        //   /* Turn LED2 on*/
        //     led2 =1;
        //     pc.printf("Transfer Error detected ********* \n\r");
        //     wait(1);
        //     transferErrorDetected = 0;
        // }
        if(bPrint)
        {
          bPrint =false;
         //if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)aEscalator8bit, 30, DAC_ALIGN_8B_R) != HAL_OK)


          pc.printf("print ");
         // pc.printf("v : %i c : %i \n\r",uhADCxConvertedValue[0],conversionCompleteConter);




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
  if (HAL_DAC_Init(&DacHandleI) != HAL_OK)
  {
    /* Initialization Error */
    led2 = 1;
    pc.printf(" DAC Initialization I  ######################################### Error");
    wait(1);
    while(1){};
  }
  if (HAL_DAC_Init(&DacHandleQ) != HAL_OK)
  {
    /* Initialization Error */
    led2 = 1;
    pc.printf(" DAC Initialization Q  ######################################### Error");
    wait(1);
    while(1){};
  }

  /*##-1- DAC channel1 Configuration #########################################*/
  DACsConfigI.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
  DACsConfigI.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  DACsConfigQ.DAC_Trigger = DAC_TRIGGER_T7_TRGO;
  DACsConfigQ.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;

  if (HAL_DAC_ConfigChannel(&DacHandleI, &DACsConfigI, DACx_CHANNEL_I) != HAL_OK)
  {
    /* Channel configuration Error */
    led2 = 1;
    pc.printf(" DAC Channel configuration I ######################################### Error");
    wait(1);
    while(1){};
  }
  if (HAL_DAC_ConfigChannel(&DacHandleQ, &DACsConfigQ, DACx_CHANNEL_Q) != HAL_OK)
  {
    /* Channel configuration Error */
    led2 = 1;
    pc.printf(" DAC Channel configuration Q  ######################################### Error");
    wait(1);
    while(1){};
  }

  pc.printf(" DAC Channel configuration complete");

  /*##-2- Enable DAC selected channel and associated DMA #############################*/
  if (HAL_DAC_Start_DMA(&DacHandleI, DACx_CHANNEL_I, (uint32_t *)DataToTransmit_I, SYMBOLS_PERTRANSFER, DAC_ALIGN_8B_R) != HAL_OK)
  {
    /* Start DMA Error */
    led2 = 1;
    pc.printf(" DAC Start DMA Error  ######################################### Error");
    wait(1);
    while(1){};
  }

    if (HAL_DAC_Start_DMA(&DacHandleQ, DACx_CHANNEL_Q, (uint32_t *)DataToTransmit_Q, SYMBOLS_PERTRANSFER, DAC_ALIGN_8B_R) != HAL_OK)
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
    void DACx_DMA_IRQHandler_I(void)
    {
     HAL_DMA_IRQHandler(DacHandleI.DMA_Handle2);
    }
    void DACx_DMA_IRQHandler_Q(void)
    {
     HAL_DMA_IRQHandler(DacHandleI.DMA_Handle1);
    }

    void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
    {
        GPIO_InitTypeDef          GPIO_InitStructI;
        GPIO_InitTypeDef          GPIO_InitStructQ;
        static DMA_HandleTypeDef  hdma_dac1I;
        static DMA_HandleTypeDef  hdma_dac1Q;

        /* DAC Periph clock enable */
        DACx_CLK_ENABLE();


        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO clock ****************************************/
        DACx_CHANNEL_GPIO_CLK_ENABLE();

        /* DMA1 clock enable */
        __HAL_RCC_DMA1_CLK_ENABLE();

        /*##-2- Configure peripheral GPIO ##########################################*/
        /* DAC Channel1 GPIO pin configuration */
        GPIO_InitStructI.Pin = DACx_CHANNEL_PIN_I;
        GPIO_InitStructI.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStructI.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DACx_CHANNEL_GPIO_PORT, &GPIO_InitStructI);

                /* DAC Channel1 GPIO pin configuration */
        GPIO_InitStructQ.Pin = DACx_CHANNEL_PIN_Q;
        GPIO_InitStructQ.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStructQ.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(DACx_CHANNEL_GPIO_PORT, &GPIO_InitStructQ);

        /*##-3- Configure the DMA ##########################################*/
        /* Set the parameters to be configured for DACx_DMA_STREAM */
        hdma_dac1I.Instance = DACx_DMA_INSTANCE_I;

        hdma_dac1I.Init.Channel  = DACx_DMA_CHANNEL;

        hdma_dac1I.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_dac1I.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_dac1I.Init.MemInc = DMA_MINC_ENABLE;
        hdma_dac1I.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_dac1I.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        //hdma_dac1I.Init.Mode = DMA_NORMAL;
        hdma_dac1I.Init.Mode = DMA_CIRCULAR;
        hdma_dac1I.Init.Priority = DMA_PRIORITY_HIGH;
        //hdma_dac1I.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
        HAL_DMA_Init(&hdma_dac1I);


                /*##-3- Configure the DMA ##########################################*/
        /* Set the parameters to be configured for DACx_DMA_STREAM */
        hdma_dac1Q.Instance = DACx_DMA_INSTANCE_Q;

        hdma_dac1Q.Init.Channel  = DACx_DMA_CHANNEL;

        hdma_dac1Q.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_dac1Q.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_dac1Q.Init.MemInc = DMA_MINC_ENABLE;
        hdma_dac1Q.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_dac1Q.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        //hdma_dac1Q.Init.Mode = DMA_NORMAL;
        hdma_dac1Q.Init.Mode = DMA_CIRCULAR;
        hdma_dac1Q.Init.Priority = DMA_PRIORITY_HIGH;
        //hdma_dac1Q.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
         HAL_DMA_Init(&hdma_dac1I);
        HAL_DMA_Init(&hdma_dac1Q);
        

        /* Associate the initialized DMA handle to the DAC handle */
        __HAL_LINKDMA(hdac, DMA_Handle2, hdma_dac1I);
        __HAL_LINKDMA(hdac, DMA_Handle1, hdma_dac1Q);

        /*##-4- Configure the NVIC for DMA #########################################*/
        /* Enable the DMA1_Stream5 IRQ Channel */
        HAL_NVIC_SetPriority(DACx_DMA_IRQn_I, 2, 0);
        HAL_NVIC_SetPriority(DACx_DMA_IRQn_Q, 2, 0);
        HAL_NVIC_EnableIRQ(DACx_DMA_IRQn_I);
        HAL_NVIC_EnableIRQ(DACx_DMA_IRQn_Q);

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
        HAL_GPIO_DeInit(DACx_CHANNEL_GPIO_PORT, DACx_CHANNEL_PIN_I);
         HAL_GPIO_DeInit(DACx_CHANNEL_GPIO_PORT, DACx_CHANNEL_PIN_Q);

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