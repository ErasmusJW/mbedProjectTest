#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"

#include "mbedDualDac.h"
//#include "ADC_Dma.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

//AnalogOut   A_out0(PA_5);

 Serial pc(USBTX, USBRX); // tx, rx

//header defines










// extern  __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
// /* Variable used to get converted value */
// extern __IO uint32_t uhADCxConvertedValue[1000] ;


const uint32_t DACTESTOUTDATASIZE = 199;
uint8_t DataToTransmit_I[DACTESTOUTDATASIZE];
uint8_t DataToTransmit_Q[DACTESTOUTDATASIZE];

void CreateSomeDacTestData()
{
    
      const uint16_t halfway = DACTESTOUTDATASIZE/2 +1;
    const uint16_t incPerHalf = 255/halfway;
    
     bool bToggle = 0;
    
    
    
    for (size_t i = 0; i < halfway; i++) //0 - 50
    {
       
       DataToTransmit_I[i] = i * incPerHalf;
       //DataToTransmit_Q[i] = i * incPerHalf;
    }

    for (size_t i = halfway; i < DACTESTOUTDATASIZE; i++) //51 - 99
    {
       DataToTransmit_I[i] = DataToTransmit_I[i-1]- incPerHalf;
       //DataToTransmit_Q[i] = DataToTransmit_Q[i-1]- incPerHalf;
       
    }

    for (size_t i = 0; i < DACTESTOUTDATASIZE; i++) //51 - 99
    {
       //DataToTransmit_I[i] = bToggle * 255;
       DataToTransmit_Q[i] = bToggle * 255;
       bToggle = !bToggle;
       
    }
}


static volatile bool bPrint = false;
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


    CreateSomeDacTestData();




    dacOutputADC Hello;





    // t.start(callback(&queue, &EventQueue::dispatch_forever));
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
    //flipper.attach(&flip, 0.25); // the address of the function to be attached (flip) and the interval (2 seconds)

    OutTimmer.attach_us(&flip,50000);

    //transferErrorDetected = 0;


    //AdcDmaInstance.DMA_Config();
      pc.printf("\n\rEntering while loop  \n\r");
    while (true) 
    {
          // pc.printf("print sucky \n\r");
       
        if(bPrint)
        {
           pc.printf("p\n\r");
           bPrint =false;
           //channel 2
            dacOutputADC::SendDataI(DataToTransmit_I,DACTESTOUTDATASIZE);

            //channel 1;
            dacOutputADC::SendDataQ(DataToTransmit_Q,DACTESTOUTDATASIZE);

         
          
         // pc.printf("v : %i c : %i \n\r",uhADCxConvertedValue[0],conversionCompleteConter);
          //if (HAL_DAC_Start_DMA(&DacHandle, DACx_CHANNEL, (uint32_t *)aEscalator8bit, 30, DAC_ALIGN_8B_R) != HAL_OK)




        }


    }
}



