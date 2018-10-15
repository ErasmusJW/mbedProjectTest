#include "mbed.h"
#include "libTest/libTest.h"
#include "ConstalationDiagrams/ModulationMaping.h"


DigitalOut led1(LED2);

 Serial pc(USBTX, USBRX); // tx, rx

// main() runs in its own thread in the OS




AnalogOut   A_out0(PA_4);
AnalogOut   A_out1(PA_5);

AnalogIn   A_in0(A0);
AnalogIn   A_in1(A1);

DigitalOut myled(LED1);

DigitalOut pinOut(D15);

// // Create a queue that can hold a maximum of 32 events
// EventQueue queue(32 * EVENTS_EVENT_SIZE);
// // Create a thread that'll run the event queue's dispatch function
// Thread t;


float EncodeValue(float val)
{
    return((val+1)/2);
}

float DevodeValue(float val)
{
    return((2*val) - 1);
}

Ticker flipper;
void flip() {
    pinOut = !pinOut;
}

Ticker OutTimmer;


int main() {
    pc.baud(115200);
    // t.start(callback(&queue, &EventQueue::dispatch_forever));
   
      std::error_code ec;
    //server_side_http_handler(ec); 
   
    //flipper.attach(&flip, 0.25); // the address of the function to be attached (flip) and the interval (2 seconds)

    OutTimmer.attach_us(&flip,50);

    uint16_t AnalogVal1 =  0;
    uint16_t AnalogVal2 = 32768;

    uint8_t testValue = 0;
    std::complex<float> modulValuse[4];

    std::complex<float> ValuesReceived[4];
   
    //const char * ger =  WatseKopIsGerHard();
    radio::ModulationMaping myModulationMaping;
    
    while (true) 
    {



        std::error_code er;
        uint32_t ValuesInOutBuffer = 0;
        uint32_t ValuesDecoded = 0;

         uint8_t recVal = 0;
        std::tie(er,ValuesInOutBuffer)= myModulationMaping.ModulateBuffer(&testValue,1,modulValuse,4);
        
        for(size_t i = 0 ; i<ValuesInOutBuffer ; ++i)
        {
            float Q = modulValuse[i].real();
            float I = modulValuse[i].imag();
            A_out0.write(EncodeValue(Q));
            A_out1.write(EncodeValue(I));

            float QRead = DevodeValue(A_in0.read());
            float IRead = DevodeValue(A_in1.read());

            ValuesReceived[i] = {QRead,IRead};

            // pc.printf(" Q Sent : %f \t\t  Q recieved : %f  \n\r",Q,QRead);
            // pc.printf(" I Sent : %f \t\t  I recieved : %f  \n\r\n\r",I,IRead);
            wait(0.1);

        }

        std::tie(er,ValuesDecoded)  = myModulationMaping.DemodulateBuffer(ValuesReceived,4,&recVal,1);
        pc.printf("  Sent : %i \t\t   recieved : %i  \n\r",testValue,recVal);
        wait(0.1);

        testValue++;


        //wait(1.5);
        // uint16_t read0 = A_in0.read_u16();
        // uint16_t read1 = A_in1.read_u16();
        //  pc.printf(" loop \n\r\n\r");
        //  pc.printf(" Val 1 Sent : %i \t\t recieved : %i  \n\r",AnalogVal1,read0);
        //  pc.printf(" Val 1 Sent : %i \t\t recieved : %i  \n\r\n\r",AnalogVal2,read1);

        // AnalogVal1 += 4096;
        // AnalogVal2 += 4096;



        
 
        // wait(1.5);
        // pc.printf(LIBTEST_NOTOVERRIDE);
        // pc.printf("\n\r");
        // pc.printf(LIBTEST_OVERRIDE);
        // pc.printf("\n\r");
        // pc.printf("%f",MBED_BUILD_TIMESTAMP);
        // pc.printf("\n\r");


        
        // pc.printf("\n\r");
        // for (size_t i = 0; i < MBED_CONF_CONSTALATIONDIAGRAMS_NUMBEROFPOINTS +1; i++)
        // {

            // uint8_t testValue1 = 0b11011111;
            // uint8_t testValue2 = 0b10101111;
            // uint8_t pInputBuffer[2];
            // uint8_t pOutBuffer[2];
            
            // pInputBuffer[0] = testValue1;
            // pInputBuffer[1] = testValue2;

            // std::complex<float> modulValuse[8];
            // std::error_code er;
            // uint32_t valuesInOutBuf=0; 
            // std::tie(er,valuesInOutBuf)= myModulationMaping.ModulateBuffer(pInputBuffer,2,modulValuse,8);

            // pc.printf("Modulate Value1 : %i \n\r",testValue1);
            // pc.printf("Modulate Value2 : %i \n\r",testValue2);
            // pc.printf("valuesInOutBuf : %i \n\r",valuesInOutBuf);



            // pc.printf("Modulate Error: %i \n\r",er );
            //   valuesInOutBuf = 0;
            // std::tie(er,valuesInOutBuf)  = myModulationMaping.DemodulateBuffer(modulValuse,4,pOutBuffer,2);

            // pc.printf("demoModulate Error: %i \n\r",er );
            // pc.printf("De modulate Value1: %i \n\r",pOutBuffer[0] );
            // pc.printf("De modulate Value2: %i \n\r\n\r",pOutBuffer[1] );
            // pc.printf("valuesInOutBuf : %i \n\r",valuesInOutBuf);


      
 
       // auto cunt = 10; 
    }
}

