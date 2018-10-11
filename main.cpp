#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"


DigitalOut led1(LED1);

 Serial pc(USBTX, USBRX); // tx, rx

// main() runs in its own thread in the OS











int main() {
   
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
   
   
   
   
    //const char * ger =  WatseKopIsGerHard();
    radio::ModulationMaping myModulationMaping;
    while (true) {
        led1 = !led1;
        wait(9.0);
        // pc.printf(LIBTEST_NOTOVERRIDE);
        // pc.printf("\n\r");
        // pc.printf(LIBTEST_OVERRIDE);
        // pc.printf("\n\r");
        // pc.printf("%f",MBED_BUILD_TIMESTAMP);
        pc.printf("\n\r");


        
        pc.printf("\n\r");
        // for (size_t i = 0; i < MBED_CONF_CONSTALATIONDIAGRAMS_NUMBEROFPOINTS +1; i++)
        // {

            uint8_t testValue1 = 0b11011111;
            uint8_t testValue2 = 0b10101111;
            uint8_t pInputBuffer[2];
            uint8_t pOutBuffer[2];
            
            pInputBuffer[0] = testValue1;
            pInputBuffer[1] = testValue2;

            std::complex<float> modulValuse[8];
            std::error_code er;
            uint32_t valuesInOutBuf=0; 
            std::tie(er,valuesInOutBuf)= myModulationMaping.ModulateBuffer(pInputBuffer,2,modulValuse,8);

            pc.printf("Modulate Value1 : %i \n\r",testValue1);
            pc.printf("Modulate Value2 : %i \n\r",testValue2);
            pc.printf("valuesInOutBuf : %i \n\r",valuesInOutBuf);



            pc.printf("Modulate Error: %i \n\r",er );
              valuesInOutBuf = 0;
            std::tie(er,valuesInOutBuf)  = myModulationMaping.DemodulateBuffer(modulValuse,8,pOutBuffer,2);

            pc.printf("demoModulate Error: %i \n\r",er );
            pc.printf("De modulate Value1: %i \n\r",pOutBuffer[0] );
            pc.printf("De modulate Value2: %i \n\r\n\r",pOutBuffer[1] );
            pc.printf("valuesInOutBuf : %i \n\r",valuesInOutBuf);

            // pc.printf("RE : %f  \t\t IMG : %f",myConstalation.m_aConstalationMapping[i].real(),myConstalation.m_aConstalationMapping[i].imag());
        // }
 
       // auto cunt = 10; 
    }
}

