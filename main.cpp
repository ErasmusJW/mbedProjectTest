#include "mbed.h"
#include "libTest/libTest.h"

DigitalOut led1(LED1);

Serial pc(USBTX, USBRX); // tx, rx

// main() runs in its own thread in the OS
int main() {
    //const char * ger =  WatseKopIsGerHard();
    while (true) {
        led1 = !led1;
        wait(1.75);
        pc.printf(LIBTEST_NOTOVERRIDE);
        pc.printf(LIBTEST_OVERRIDE);
        pc.printf("%f",MBED_BUILD_TIMESTAMP);
        pc.printf("%f",MBED_LIBRARY_VERSION);  
        pc.printf("%f",JOU_MA_SE_POES);  
        auto cunt = 10; 
    }
}

