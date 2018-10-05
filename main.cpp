#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"
#include "TempGlobals.h"

DigitalOut led1(LED1);

// Serial pc(USBTX, USBRX); // tx, rx

// main() runs in its own thread in the OS











int main() {
   
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
   
   
   
   
    //const char * ger =  WatseKopIsGerHard();
    radio::ModulationMaping myModulationMaping;
    while (true) {
        led1 = !led1;
        wait(3.0);
        // pc.printf(LIBTEST_NOTOVERRIDE);
        // pc.printf("\n\r");
        // pc.printf(LIBTEST_OVERRIDE);
        // pc.printf("\n\r");
        // pc.printf("%f",MBED_BUILD_TIMESTAMP);
        pc.printf("\n\r");


        
        pc.printf("\n\r");
        for (size_t i = 0; i < MBED_CONF_CONSTALATIONDIAGRAMS_NUMBEROFPOINTS +1; i++)
        {

            auto val = myModulationMaping.getMappedValue(i);
            if(val.err == encoding_error::OK)
              pc.printf("ERR : %i \t\t RE:\t%f  \t\t IM:\t%f  \n\r",val.err ,val.value.real(),val.value.imag() );
            else
               pc.printf("ERR : %i ",val.err);
            // pc.printf("RE : %f  \t\t IMG : %f",myConstalation.m_aConstalationMapping[i].real(),myConstalation.m_aConstalationMapping[i].imag());
        }
 
       // auto cunt = 10; 
    }
}

