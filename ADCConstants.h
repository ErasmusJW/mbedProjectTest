#ifndef ADCCONSTANTS_H
#define ADCCONSTANTS_H


//can't find mbed define for this;
#define SYSTEM_FREQUENCY 216000000.0
#define PRE_SCALE_TO_ADC 2.0

#define ADC_SAMPLING_DESIRED 40000.0
#define CONVERSION_ADC_CYCLES 12.0
#define MIN_NUMBER_OF_SAMPLE 4.0


static const float  ui_maxPeriodPerSampleInMicro= []()->float{
    return (1000000.0/(ADC_SAMPLING_DESIRED*4.0));

}();

struct ConfigADC
{
    uint16_t clockPreScaller;
    uint16_t numberOfCycles;
};

static  ConfigADC adcConfig = []()->ConfigADC{

    const float effectiveFrequency = SYSTEM_FREQUENCY/PRE_SCALE_TO_ADC;
     float difference = 999999999999;
    ConfigADC returnVal = {0,0};

    for (uint8_t prescallerbeingTested : {1, 2, 4, 6, 8})
    {
        const float adcFrequency = effectiveFrequency/(float)prescallerbeingTested;
        const float adcPeriodInMicro = 1000000/adcFrequency;
        for (uint16_t sampleCyclesBeingTested : {480, 144, 112, 84, 56,28,15,3})
        {
            const uint16_t totalCycles = sampleCyclesBeingTested + CONVERSION_ADC_CYCLES;
            const float totalConversionTime = adcPeriodInMicro * totalCycles;
            if(totalConversionTime <= ui_maxPeriodPerSampleInMicro)
            {
                const float delta = ui_maxPeriodPerSampleInMicro - totalConversionTime  ;
                if(delta < difference)
                {
                    returnVal.clockPreScaller = prescallerbeingTested;
                    returnVal.numberOfCycles = sampleCyclesBeingTested;
                    difference = delta;
                }
            }


        }

    }
    return returnVal;
}();

//static const double doosPoesMax =  (1000000.0/SYSTEM_FREQUENCY/PRE_SCALE_TO_ADC/1) * (480.0 + 12.0);
// static const float doosPoesMax =    1000000.0 /  (216000000.0 / 2.0 / 1.0 )  * (480.0+12.0);
// static const float doosPoesMin =    1000000.0 /  (216000000.0 / 2.0 / 1.0 )  * (3+12.0);

// static const uint16_t SampleCycles = []()->uint16_t{
//     const float adcFrequency = SYSTEM_FREQUENCY/PRE_SCALE_TO_ADC/clockPrescaller;
//     const float adcPeriodInMicro = 1000000/adcFrequency;
    
//     for (uint16_t sampleCyclesBeingTested : {480, 144, 112, 84, 56,28,15,3})
//     {
//         const uint16_t totalCycles = sampleCyclesBeingTested + CONVERSION_ADC_CYCLES;
//         const uint16_t totalConversionTime = adcPeriodInMicro * totalCycles;


//         if(ui_maxPeriodPerSampleInMicro >= totalConversionTime )
//             return sampleCyclesBeingTested;

//     }
//     return 0;
// }();


// #define ADC_CLOCKPRESCALER_PCLK_DIV1    ADC_CLOCK_SYNC_PCLK_DIV1
// #define ADC_CLOCKPRESCALER_PCLK_DIV2    ADC_CLOCK_SYNC_PCLK_DIV2
// #define ADC_CLOCKPRESCALER_PCLK_DIV4    ADC_CLOCK_SYNC_PCLK_DIV4
// #define ADC_CLOCKPRESCALER_PCLK_DIV6    ADC_CLOCK_SYNC_PCLK_DIV6
// #define ADC_CLOCKPRESCALER_PCLK_DIV8    ADC_CLOCK_SYNC_PCLK_DIV8

// #define ADC_SAMPLETIME_3CYCLES    0x00000000U
// #define ADC_SAMPLETIME_15CYCLES   ((uint32_t)ADC_SMPR1_SMP10_0)
// #define ADC_SAMPLETIME_28CYCLES   ((uint32_t)ADC_SMPR1_SMP10_1)
// #define ADC_SAMPLETIME_56CYCLES   ((uint32_t)(ADC_SMPR1_SMP10_1 | ADC_SMPR1_SMP10_0))
// #define ADC_SAMPLETIME_84CYCLES   ((uint32_t)ADC_SMPR1_SMP10_2)
// #define ADC_SAMPLETIME_112CYCLES  ((uint32_t)(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_0))
// #define ADC_SAMPLETIME_144CYCLES  ((uint32_t)(ADC_SMPR1_SMP10_2 | ADC_SMPR1_SMP10_1))
// #define ADC_SAMPLETIME_480CYCLES  ((uint32_t)ADC_SMPR1_SMP10)


#endif