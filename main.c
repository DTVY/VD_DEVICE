#include "include.h"
/* Include arm_math.h mathematic functions */

#include <stdarg.h>
#include "main.h"
#include "gui.h"

//#include "utils/uartstdio.c"
extern FFT fftNode[2];

extern volatile unsigned char flagTransBuffDone;
extern volatile char flag_OnButtonPress_;
//------------------------------------
uint32_t output3[NUM_SAMPLES ] = { 0 };
//------------------------------------
//------------------------------------
extern volatile unsigned char LPFdone;
extern uint32_t g_fFIRResult[NUM_SAMPLES ];

extern SAMPLING_ADC adcNode[2];
extern uint32_t g_ulADCValues[NUM_SAMPLES ];
extern uint32_t *currentAdcBuffer;

extern GuiNotification_ Gui_Notification;

volatile uint16_t g_uiDSPPerSec = 0; // khong dat khoa static o day de tinh so vong lap
float32_t g_fFFTResult[NUM_SAMPLES * 2] = { 0 };
/*------------------------------------------------------------------------------*/
float32_t Speed_MeterPerSecond = 0;
float mph = 0.0f;
float kmh = 0.0f;
/*-------------------------------------------------------------------------------*/
#if 1
int main(void) {
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    uint32_t *h, *w, *p; //test cfir filter, ok!
    g_uiDSPPerSec = 0;
    /*
     * test thay chia Max Freq chon 40 MHz chay on dinh hon, chua test may do voi 80 Mhz
     * */
    ROM_SysCtlClockSet(
    SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //200 / 5 = 40MHZ, /2.5 = 80MHZ

    InitSamplingTimer();
    InitADC3Transfer();
    //InitDebugTimer(); //Timer2A init
    InitGPIO();
    InitConsole();
    SysCtlDelay(50);
    InitGUI();
#ifdef FFT1_
    InitDSP1();
#else
    InitDSP2();
#endif
    ROM_IntEnable(INT_ADC0SS3);
    ROM_IntMasterEnable();

    int firloop1 = 0, i = 0;
    int read = 0;

    while (1) {
        /*
         * Timer3 dem len 1 refesh man hinh 1 lan
         * */
        if (Gui_Notification.g_ucDispRefresh == 1)
            GUIUpdateDisplay();
        if (adcNode[0].g_ucDataReady) {
            switch (choiceTypeProcessing(NONE)) {
            case LOWPASS_FILTER:
                LowpassFilter(g_ulADCValues, output3, 10,
                        adcNode[0].g_uiSamplingFreq);
                break;
            case HIGHPASS_FILTER:
                HighPassFilter(g_ulADCValues, output3, 5,
                        adcNode[0].g_uiSamplingFreq);
                break;
            case MY_FIR_FILTER:
                for (firloop1 = 0; firloop1 < NUM_SAMPLES ; firloop1++)
                    output3[firloop1] = cfir(NUM_SAMPLES, h, w, &p,
                            g_ulADCValues[firloop1]);
                break;
            case STANDARD_FIR_FILTER:
                FIRprocessing(g_ulADCValues, g_fFIRResult);
                break;
            default:
                break;
            } //switch
            SysCtlDelay(300);
// for debugging ------------------------------------------------------------
#ifdef FFT1_
            FFTprocessing1(g_ulADCValues, g_fFFTResult, FFT_LENGTH_256);
#else
            FFTprocessing2(g_ulADCValues, g_fFFTResult);
#endif
            g_uiDSPPerSec++; //neu gia tri cua bien khong tang thi "Processing" qua trinh bi loi

            DisplayData(fftNode[0].hertz);
            //
            // Display the setup on the console.
            //

            //FIX HERE AGAIN
            /*
             * FFT length: 256 -> real points only 126
             * 23.2.16: do freq thay gia tri "MaxFreqIndex" khong on dinh ngoai khoang [10,126]
             * */
            //if (g_fFFTResult[fftNode[0].maxIndex] < 10)
            //  fftNode[0].hertz = 0.0f;
//          }
//------------------------------------------------------------------------
        }

        WidgetMessageQueueProcess();
    }
//while
}
#endif

/*
 * */
void delayMS(int ms) {
    ROM_SysCtlDelay((SysCtlClockGet() / (3 * 1000)) * ms);
}
/*
 *
 * */
void InitConsole(void) {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);

    ROM_UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTStdioConfig(0, 115200, 16000000);
}
/*
 *
 * */
void InitGPIO(void) {
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
    GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0);
}
/*
 *
 * */
uint16_t k = 0;
uint16_t g = 0;
void DisplayData(float32_t hertz) {
// calculate the number of Doppler Hertz per m/s for this radar frequency;
    /*
     * lamda = c/f_radar
     * speed = (f_d*lamda)/2
     * */
    //static float32_t HertzPerMs = 2.0f * RADAR_FREQUENCY / SPEED_OF_LIGHT;
    static float32_t Lamda = SPEED_OF_LIGHT / RADAR_FREQUENCY;

    Speed_MeterPerSecond = 0.0f;
    //MetersPerSecond = hertz / HertzPerMs; //speed
    Speed_MeterPerSecond = (hertz * Lamda) / 2.0f;
    float d = 0, c = 0;
    switch (Gui_Notification.g_ucTypeSpeed) {
    case MPH:
        mph = Speed_MeterPerSecond * MPH_FACTOR;
        c = mph - (uint16_t) mph;
        g = c * 100;
        Config_eeprom();
#ifdef DEBUG_UART
//      UARTprintf("%d.%3d\t%d\n", (uint16_t) mph, g, fftNode[0].maxIndex);
        UARTprintf("m%2d%2d%2d%3d\n", (uint16_t) mph, g, fftNode[0].maxIndex,
                (uint16_t) hertz);
//      sprintf(str, "%3.2f", mph); // da test nhung gia tri khong cap nhat, khong hieu cho nay
//      UARTprintf(str);
#endif
        break;
    case KMH:
        kmh = Speed_MeterPerSecond * KMH_FACTOR;
        d = kmh - (uint16_t) kmh;
        k = d * 100;
        Config_eeprom();
#ifdef DEBUG_UART
        if ((uint16_t) kmh >= 10)
            UARTprintf("k%2d%2d%2d%3d\n", (uint16_t) kmh * 10, k,
                    fftNode[0].maxIndex, (uint16_t) hertz);
        else
            UARTprintf("k%2d%2d%2d%3d\n", (uint16_t) kmh, k,
                    fftNode[0].maxIndex, (uint16_t) hertz);
#endif
        break;
    case MS:

        break;
    case NONE_CONFIG:
        kmh = Speed_MeterPerSecond * KMH_FACTOR;
        d = kmh - (uint16_t) kmh;
        k = d * 100;
        Config_eeprom();
        if ((uint16_t) kmh >= 10)
            UARTprintf("k%2d%2d%2d%3d\n", (uint16_t) kmh * 10, k,
                    fftNode[0].maxIndex, (uint16_t) hertz);
        else
            UARTprintf("k%2d%2d%2d%3d\n", (uint16_t) kmh, k,
                    fftNode[0].maxIndex, (uint16_t) hertz);
        break;
    default:
        break;
    }
}
/*
 *
 * */
uint8_t static MenuGetSpeedMode(typeSpeedMode type_speedmode) {
    return type_speedmode;
}
uint8_t static MenuGetDisplayMode(typeDisplayMode displaymode) {
    return displaymode;
}
/*
 *
 * */
void PeripheralPrintf(uint16_t peripheral, char *format, ...) {
    char buffer[64] = { 0 };

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    switch (peripheral) {
    case PERIPHERAL_LCD:
        break;
    case PERIPHERAL_UART:
        break;
    default:
        break;
    }
}

#if 0 //specific test lcd
int main(void) {
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    ROM_SysCtlClockSet(
            SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //200 / 5 = 40MHZ, /2.5 = 80MHZ

    InitGPIO();
    SysCtlDelay(50);
    InitGUI();
    ROM_IntMasterEnable();
    while (1) {
        /*
         * Timer3 dem len 1 refesh man hinh 1 lan
         * */
        if (Gui_Notification.g_ucDispRefresh == 1)
        GUIUpdateDisplay();

        SysCtlDelay(300);

        WidgetMessageQueueProcess();
    }
}

#endif
