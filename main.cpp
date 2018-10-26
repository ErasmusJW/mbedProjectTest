#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"
#include "ADC_Dma.h"

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

AnalogOut   A_out0(PA_5);

 Serial pc(USBTX, USBRX); // tx, rx

//header defines




//* header defines 



extern  __IO uint32_t transferErrorDetected; /* Set to 1 if an error transfer is detected */
extern ADC_HandleTypeDef    AdcHandle;

/* Variable used to get converted value */
extern __IO uint32_t uhADCxConvertedValue[1000] ;

/* Private function prototypes -----------------------------------------------*/

static void Error_Handler(void);
static void CPU_CACHE_Enable(void);
 void DMA_Config(void);
/* Private functions ---------------------------------------------------------*/


//het hom self promote na global;
extern ADC_ChannelConfTypeDef sConfig;



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





