#include "include.h"
#include "gui.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "grlib/checkbox.h"
#include "grlib/container.h"
#include "grlib/radiobutton.h"
#include "grlib/slider.h"
#include "sampling.h"
#include "utils/ustdlib.h"  // Defines usprintf
#include "utils/ustdlib.c"
#include "images.h"

#include "inc/hw_nvic.h "
#include "inc/hw_adc.h"
#include "driverlib/adc.h"

extern SAMPLING_ADC adcNode[2];
extern FFT fftNode[2];

extern float mph, kmh;
extern float32_t Speed_MeterPerSecond;
extern uint16_t k, g;
extern volatile unsigned short int countTimerISRLoopSample;

extern EeprDataSave_ Eepr_Data_Save;
extern EeprViewCount_ Eepr_View_Count;
extern uint32_t max;

extern tCanvasWidget g_psPanels[];

tCanvasWidget g_sBackground, g_sSetType, ConfigBottom, ViewData, g_psRadio;
tPushButtonWidget g_sPushBtn1, g_sPushBtn2, g_sPushBtn3, g_sPushBtn4;
tPushButtonWidget g_psTopButtons[];

StateButton_ State_Button;
GuiNotification_ Gui_Notification;
char Deny_State = 0;
uint8_t stop = 0;
uint8_t stopped = 0;
//-------------------------------------------------------------------------------------------//
Update_Data Up_D;
Er_sData_ Er_sD;
//*****************************************************************************
//
// The first panel, which displays data
//
//
//*****************************************************************************
Canvas(er_cData5, &g_sBackground, 0, 0, &g_sLCD_SSD2119, 0, 180, 27, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrLightCyan, ClrLightCyan, ClrRed, g_psFontCmtt14,
        Er_sD.Er_cDataText[5], 0, 0);
Canvas(er_cData4, &g_sBackground, &er_cData5, 0, &g_sLCD_SSD2119, 0, 160, 27,
        20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrLightCyan, ClrLightCyan, ClrRed, g_psFontCmtt14,
        Er_sD.Er_cDataText[4], 0, 0);
Canvas(er_cData3, &g_sBackground, &er_cData4, 0, &g_sLCD_SSD2119, 0, 140, 27,
        20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrLightCyan, ClrLightCyan, ClrRed, g_psFontCmtt14,
        Er_sD.Er_cDataText[3], 0, 0);
Canvas(er_cData2, &g_sBackground, &er_cData3, 0, &g_sLCD_SSD2119, 0, 120, 27,
        20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrLightCyan, ClrLightCyan, ClrRed, g_psFontCmtt14,
        Er_sD.Er_cDataText[2], 0, 0);
Canvas(er_cData1, &g_sBackground, &er_cData2, 0, &g_sLCD_SSD2119, 0, 100, 27,
        20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrLightCyan, ClrLightCyan, ClrRed, g_psFontCmtt14,
        Er_sD.Er_cDataText[1], 0, 0);
Canvas(er_cData, &g_sBackground, &er_cData1, 0, &g_sLCD_SSD2119, 0, 80, 27, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrLightCyan, ClrLightCyan, ClrRed, g_psFontCmtt14,
        Er_sD.Er_cDataText[0], 0, 0);
//--------------------
Canvas(g_sMaxValue, &g_sBackground, &er_cData, 0, &g_sLCD_SSD2119, 0, 50, 160,
        30,
        (CANVAS_STYLE_FILL|CANVAS_STYLE_OUTLINE|CANVAS_STYLE_TEXT|CANVAS_STYLE_TEXT_OPAQUE),
        ClrHoneydew, ClrSkyBlue, ClrBlue, g_psFontCm20, Up_D.maxValue, 0, 0);
//----EEPROM DATA------------------
Canvas(g_sDataNumb5, &g_sBackground, &g_sMaxValue, 0, &g_sLCD_SSD2119, 27, 180,
        53, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAliceBlue, ClrAliceBlue, ClrBlue, g_psFontCmtt16,
        Er_sD.DataSaveText5, 0, 0);
Canvas(g_sDataNumb4, &g_sBackground, &g_sDataNumb5, 0, &g_sLCD_SSD2119, 27, 160,
        53, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAzure, ClrAliceBlue, ClrBlue, g_psFontCmtt16, Er_sD.DataSaveText4, 0,
        0);
Canvas(g_sDataNumb3, &g_sBackground, &g_sDataNumb4, 0, &g_sLCD_SSD2119, 27, 140,
        53, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAliceBlue, ClrAliceBlue, ClrBlue, g_psFontCmtt16,
        Er_sD.DataSaveText3, 0, 0);
Canvas(g_sDataNumb2, &g_sBackground, &g_sDataNumb3, 0, &g_sLCD_SSD2119, 27, 120,
        53, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAzure, ClrAliceBlue, ClrBlue, g_psFontCmtt16, Er_sD.DataSaveText2, 0,
        0);
Canvas(g_sDataNumb1, &g_sBackground, &g_sDataNumb2, 0, &g_sLCD_SSD2119, 27, 100,
        53, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAliceBlue, ClrAliceBlue, ClrBlue, g_psFontCmtt16,
        Er_sD.DataSaveText1, 0, 0);
Canvas(g_sDataNumb, &g_sBackground, &g_sDataNumb1, 0, &g_sLCD_SSD2119, 27, 80,
        53, 20,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAzure, ClrAliceBlue, ClrBlue, g_psFontCmtt16, Er_sD.DataSaveText, 0,
        0);
//-----------------------------
Canvas(heztText, &g_sBackground, &g_sDataNumb, 0, &g_sLCD_SSD2119, 280, 20, 40,
        30, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
        ClrAliceBlue, ClrAliceBlue, ClrSkyBlue, g_psFontCm26, "Hz", 0, 0);
Canvas(g_sTypeSpeed, &g_sBackground, &heztText, 0, &g_sLCD_SSD2119, 270, 80, 50,
        120,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE|CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAliceBlue, ClrAliceBlue, ClrBlue, g_psFontCm24, "kmh", 0, 0);
Canvas(g_sSpeedNumber, &g_sBackground, &g_sTypeSpeed, 0, &g_sLCD_SSD2119, 80,
        80, 190, 120,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE |CANVAS_STYLE_TEXT_OPAQUE | CANVAS_STYLE_TEXT),
        ClrAliceBlue, ClrAliceBlue, ClrBlue, g_psFontCmtt46, Up_D.speedNumber,
        0, 0);
Canvas(g_sDistance, &g_sBackground, &g_sSpeedNumber, 0, &g_sLCD_SSD2119, 160, 50,
        160, 30,
        (CANVAS_STYLE_FILL|CANVAS_STYLE_OUTLINE|CANVAS_STYLE_TEXT|CANVAS_STYLE_TEXT_OPAQUE),
        ClrHoneydew, ClrHoneydew, ClrBlue, g_psFontCm20b, Up_D.distanceText, 0,
        0);
Canvas(FFThezt, &g_sBackground, &g_sDistance, 0, &g_sLCD_SSD2119, 160, 20, 120,
        30,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT|CANVAS_STYLE_TEXT_OPAQUE),
        ClrAliceBlue, ClrAliceBlue, ClrSkyBlue, g_psFontCm22, Up_D.HeztText, 0,
        0);
Canvas(g_sSampFreq, &g_sBackground, &FFThezt, 0, &g_sLCD_SSD2119, 0, 20, 160,
        30, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
        ClrWhite, ClrSkyBlue, ClrSkyBlue, g_psFontCm18, Up_D.sampFreqText, 0,
        0);
Canvas(g_sTitle, &g_sBackground, &g_sSampFreq, 0, &g_sLCD_SSD2119, 0, 0, 320,
        20, (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT),
        ClrGreenYellow, ClrGreenYellow, ClrRed, g_psFontCm20i,
        "Velocity Detector", 0, 0);
Canvas(g_sBackground, g_psPanels, 0, &g_sTitle, &g_sLCD_SSD2119, 0, 0, 320, 180,
        CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

//*****************************************************************************
//
// The third panel, which demonstrates the canvas widget.
//
//*****************************************************************************
Canvas(ConfigBottom, g_psPanels + 2, 0, &g_sPushBtn1, &g_sLCD_SSD2119, 0, 200,
        180, 40, CANVAS_STYLE_FILL, ClrMintCream, ClrSkyBlue, 0, g_psFontCm16,
        0, 0, 0);
RectangularButton(g_sPushBtn4, &ConfigBottom, 0, 0, &g_sLCD_SSD2119, 180, 200,
        80, 40,
        (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT | PB_STYLE_FILL),
        ClrLavenderBlush, ClrLavenderBlush, ClrSkyBlue, ClrSkyBlue,
        g_psFontCm26i, "Reset", 0, 0, 0, 0, OnButtonReset);
RectangularButton(g_sPushBtn2, &ConfigBottom, &g_sPushBtn4, 0, &g_sLCD_SSD2119,
        0, 200, 90, 40,
        (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT | PB_STYLE_FILL),
        ClrFloralWhite, ClrLightCyan, ClrSkyBlue, ClrSkyBlue, g_psFontCm16i,
        "Information", 0, 0, 0, 0, OnViewData);
RectangularButton(g_sPushBtn1, &ConfigBottom, &g_sPushBtn2, 0, &g_sLCD_SSD2119,
        90, 200, 90, 40,
        (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT | PB_STYLE_FILL),
        ClrCornsilk, ClrLightCyan, ClrSkyBlue, ClrSkyBlue, g_psFontCm28i,
        "Start", 0, 0, 0, 0, OnButtonPress);

//*****************************************************************************
//
// The second panel, which demonstrates the graphics primitives.
//
//*****************************************************************************
Canvas(g_sSetType, g_psPanels + 1, 0, &g_sPushBtn3, &g_sLCD_SSD2119, 260, 200,
        60, 40, CANVAS_STYLE_FILL, ClrSkyBlue, ClrSkyBlue, 0, g_psFontCm16, 0,
        0, 0);
RectangularButton(g_sPushBtn3, &g_sSetType, 0, 0, &g_sLCD_SSD2119, 261, 200, 58,
        40,
        (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT | PB_STYLE_FILL),
        ClrCadetBlue, ClrCyan, ClrSkyBlue, ClrWhite, g_psFontCm18b, "Unit", 0,
        0, 0, 0, OnConfigPress);

//*****************************************************************************
//
// The fourth panel, which contains a selection of radio buttons.
//
//*****************************************************************************

tCanvasWidget g_psRadioButtonIndicators[] = {
CanvasStruct(&g_psRadio, g_psRadioButtonIndicators + 1, 0,
        &g_sLCD_SSD2119, 95, 62, 20, 20, CANVAS_STYLE_IMG, 0, 0, 0, 0, 0,
        g_pucLightOff, 0),
CanvasStruct(&g_psRadio,
        g_psRadioButtonIndicators + 2, 0, &g_sLCD_SSD2119, 95, 107, 20, 20,
        CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, g_pucLightOff, 0),
CanvasStruct(
        &g_psRadio, 0, 0, &g_sLCD_SSD2119, 95, 152, 20, 20,
        CANVAS_STYLE_IMG, 0, 0, 0, 0, 0, g_pucLightOff, 0), };
tRadioButtonWidget g_psRadioButtons1[] = {
RadioButtonStruct(&g_psRadio, g_psRadioButtons1 + 1, 0, &g_sLCD_SSD2119,
        10, 50, 80, 45, RB_STYLE_TEXT, 16, 0, ClrLightCyan,
        ClrCornsilk, &g_sFontCm20, "MPH", 0, OnRadioChange),
RadioButtonStruct(&g_psRadio, g_psRadioButtons1 + 2, 0, &g_sLCD_SSD2119,
        10, 95, 80, 45, RB_STYLE_TEXT, 16, 0, ClrLightCyan,
        ClrCornsilk, &g_sFontCm20, "KMH", 0, OnRadioChange),
RadioButtonStruct(&g_psRadio, g_psRadioButtonIndicators, 0,
        &g_sLCD_SSD2119, 10, 140, 80, 45, RB_STYLE_TEXT, 24, 0,
        ClrLightCyan, ClrCornsilk, &g_sFontCm20, "M/S", 0,
        OnRadioChange) };
#define NUM_RADIO1_BUTTONS      (sizeof(g_psRadioButtons1) /   \
                                         sizeof(g_psRadioButtons1[0]))
tCanvasWidget g_psRadio =
CanvasStruct(g_psPanels+3, 0, g_psRadioButtons1, &g_sLCD_SSD2119, 0,
        20, 320, 180, CANVAS_STYLE_FILL, 0, 0, 0, 0, 0, 0, 0);
//*****************************************************************************
//
// The fifth panel, which demonstrates the container widget.
//
//*****************************************************************************
Canvas(TextData, &ViewData, 0, 0, &g_sLCD_SSD2119, 0, 40, 100, 40,
        (CANVAS_STYLE_FILL | CANVAS_STYLE_OUTLINE | CANVAS_STYLE_TEXT), 0, 0,
        ClrWhite, g_psFontCm16, "test data", 0, 0);
Canvas(ViewData, g_psPanels + 4, 0, &TextData, &g_sLCD_SSD2119, 0, 20, 320, 180,
        (CANVAS_STYLE_FILL), ClrBlack, 0, 0, 0, 0, 0, 0);
//*****************************************************************************
//
// An array of canvas widgets, one per panel.  Each canvas is filled with
// black, overwriting the contents of the previous panel.
//
//*****************************************************************************
tCanvasWidget g_psPanels[] = {
CanvasStruct(0, 0, &g_sBackground, &g_sLCD_SSD2119, 0,
        0, 320, 180, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
CanvasStruct(0, 0, &g_sSetType, &g_sLCD_SSD2119, 0, 260,
        60, 40, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
CanvasStruct(0, 0, &ConfigBottom, &g_sLCD_SSD2119, 0, 200, 180,
        40, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
CanvasStruct(0, 0, &g_psRadio, &g_sLCD_SSD2119, 0,
        24, 320, 180, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0),
CanvasStruct(0, 0, &ViewData, &g_sLCD_SSD2119, 0, 20,
        320, 180, CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0), };
//-----------------------------------------------------------------------------------------------------------//
/*
 * */
void Timer3AIntHandler(void) {
    TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    Gui_Notification.g_ucDispRefresh = 1;
}
/*
 * */
static void InitDisplayTimer(void) {
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
    TimerConfigure(TIMER3_BASE, TIMER_CFG_ONE_SHOT);
    TimerLoadSet(TIMER3_BASE, TIMER_A, SysCtlClockGet() / REFRESH_RATE);
    IntEnable(INT_TIMER3A);
    TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER3_BASE, TIMER_A);
}
/*
 * */
static void OnButtonPress(tWidget *pWidget) {
    State_Button.StateButton1 = !State_Button.StateButton1;
    if (State_Button.StateButton1) {
        if (State_Button.StateButton1) {
            Deny_State = 1;
            PushButtonTextSet(&g_sPushBtn1, "Stop");
            WidgetPaint((tWidget * )&g_sPushBtn1);
            stopSampling();
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x08);
        }
    } else {
        if (!State_Button.StateButton1) {
            Deny_State = 0;
            PushButtonTextSet(&g_sPushBtn1, "Start");
            WidgetPaint((tWidget * )&g_sPushBtn1);
            resumeSampling();
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0x00);
        }
    }
}
/*
 *
 * */
static void OnButtonReset(tWidget *pWidget) {
    //Reset All
    HWREG(NVIC_APINT) = NVIC_APINT_VECTKEY | NVIC_APINT_SYSRESETREQ;
}

/*
 *
 * */
static void OnConfigPress(tWidget *pWidget) {
    State_Button.StateButton3 = !State_Button.StateButton3;
    if (State_Button.StateButton3) {
        if (State_Button.StateButton3) {
            //Deny_State = 1;
            TimerDisable(TIMER3_BASE, TIMER_A);
            Gui_Notification.g_ucDispRefresh = 0;
            WidgetRemove((tWidget *) &g_sBackground);
            WidgetAdd(WIDGET_ROOT, (tWidget *) &g_psRadio);
            WidgetPaint((tWidget * )&g_psRadio);
        }
    } else {
        if (!State_Button.StateButton3) {
            //Deny_State = 0;
            WidgetRemove((tWidget *) &g_psRadio);
            WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sBackground);
            WidgetPaint((tWidget * )&g_sBackground);
            Gui_Notification.g_ucDispRefresh = 1;
        }
    }
}
/*
 *
 * */
static void OnViewData(tWidget *pWidget) {
    State_Button.StateButton2 = !State_Button.StateButton2;
    if (State_Button.StateButton2) {
        if (State_Button.StateButton2) {
            Deny_State = 1;
            TimerDisable(TIMER3_BASE, TIMER_A);
            Gui_Notification.g_ucDispRefresh = 0;
            WidgetRemove((tWidget *) &g_sBackground);
            WidgetAdd(WIDGET_ROOT, (tWidget *) &ViewData);
            WidgetPaint((tWidget * )&ViewData);
        }
    } else {
        if (!State_Button.StateButton2) {
            Deny_State = 0;
            WidgetRemove((tWidget *) &ViewData);
            WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sBackground);
            WidgetPaint((tWidget * )&g_sBackground);
            WidgetAdd(WIDGET_ROOT, (tWidget *) &ConfigBottom);
            WidgetPaint((tWidget * )&ConfigBottom);
//          SysCtlDelay(30);
            WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sSetType);
            WidgetPaint((tWidget * )&g_sSetType);
            Gui_Notification.g_ucDispRefresh = 1;
        }
    }
}

//bool ButtonTF = false;
//void OnConfigPress(tWidget *pWidget) {
//  WidgetRemove((tWidget *) &g_sBackground);
//  WidgetRemove((tWidget *) &g_sBackground);
//  ButtonTF = !ButtonTF;
//  if (ButtonTF) {
//
//      if (stop == 1) {
//          stopped = 1;
//      } else {
//          stop = 1;
//          stopped = 0;
//      }
//      WidgetAdd(WIDGET_ROOT, (tWidget *) &g_psRadio);
//      WidgetPaint((tWidget * ) &g_psRadio);
//  } else {
//      ClrMyWidget();
//      if (stopped == 1) {
//          stopped = 0;
//      } else {
//          stop = 0;
//          stopped = 0;
//      }
//  }
//
//}
/*
 * bSelected : con tro *pIm, tro toi bSelected, kiem tra xem hinh anh la Light On hay Off
 * */
static void OnRadioChange(tWidget *pWidget, uint32_t bSelected) {
//
// Find the index of this radio button in the first group.
//
    uint32_t ulIdx;
    for (ulIdx = 0; ulIdx < NUM_RADIO1_BUTTONS; ulIdx++) {
        if (pWidget == (tWidget *) (g_psRadioButtons1 + ulIdx)) {
            if (ulIdx == 0) {
                if (bSelected) {
                    Gui_Notification.g_ucTypeSpeed = MPH;
                    CanvasImageSet(g_psRadioButtonIndicators + 1,
                            g_pucLightOff);
                    WidgetPaint((tWidget * )(g_psRadioButtonIndicators + 1));
                } else {
                    Gui_Notification.g_ucTypeSpeed = NONE_CONFIG;
                }
                SysCtlDelay(30);
                //
                // Set the matching indicator based on the selected state of the radio
                // button.
                //
                CanvasImageSet(g_psRadioButtonIndicators + ulIdx,
                        bSelected ? g_pucLightOn : g_pucLightOff);
                WidgetPaint((tWidget * )(g_psRadioButtonIndicators + ulIdx));
            } else if (ulIdx == 1) {
                if (bSelected) {
                    Gui_Notification.g_ucTypeSpeed = KMH;
                } else {
                    Gui_Notification.g_ucTypeSpeed = NONE_CONFIG;
                }
                SysCtlDelay(30);
                CanvasImageSet(g_psRadioButtonIndicators + ulIdx,
                        bSelected ? g_pucLightOn : g_pucLightOff);
                WidgetPaint((tWidget * )(g_psRadioButtonIndicators + ulIdx));
            } else if (ulIdx == 2) {
                if (bSelected) {
                    Gui_Notification.g_ucTypeSpeed = MS;
                    CanvasImageSet(g_psRadioButtonIndicators + 1,
                            g_pucLightOff);
                    WidgetPaint((tWidget * )(g_psRadioButtonIndicators + 1));
                } else {
                    Gui_Notification.g_ucTypeSpeed = NONE_CONFIG;
                }
                SysCtlDelay(30);
                CanvasImageSet(g_psRadioButtonIndicators + ulIdx,
                        bSelected ? g_pucLightOn : g_pucLightOff);
                WidgetPaint((tWidget * )(g_psRadioButtonIndicators + ulIdx));
            }
        }
    }
}
/*
 *
 * */
void GUIUpdateDisplay(void) {
    if (Gui_Notification.g_ucDispRefresh) {
        if (!Deny_State) {
            usprintf(Up_D.HeztText, "Signal %d", (uint16_t) fftNode[0].hertz);
            CanvasTextSet(&FFThezt, Up_D.HeztText);
            WidgetPaint((tWidget * )&FFThezt);

            usprintf(Up_D.maxValue, "Maximum %d", max);
            CanvasTextSet(&g_sMaxValue, Up_D.maxValue);
            WidgetPaint((tWidget * )&g_sMaxValue);

            //------------- 0 -----------------
            usprintf(Er_sD.Er_cDataText[0], "%d", Eepr_View_Count.ac[0]);
            CanvasTextSet(&er_cData, Er_sD.Er_cDataText[0]);
            WidgetPaint((tWidget * )&er_cData);

            usprintf(Er_sD.DataSaveText, "%d", (uint16_t) Eepr_Data_Save.da[0]);
            CanvasTextSet(&g_sDataNumb, Er_sD.DataSaveText);
            WidgetPaint((tWidget * )&g_sDataNumb);
            //-------------- 1 ---------------------
            usprintf(Er_sD.Er_cDataText[1], "%d", Eepr_View_Count.ac[1]);
            CanvasTextSet(&er_cData1, Er_sD.Er_cDataText[1]);
            WidgetPaint((tWidget * )&er_cData1);

            usprintf(Er_sD.DataSaveText1, "%d",
                    (uint16_t) Eepr_Data_Save.da[1]);
            CanvasTextSet(&g_sDataNumb1, Er_sD.DataSaveText1);
            WidgetPaint((tWidget * )&g_sDataNumb1);
            //--------------- 2 --------------------
            usprintf(Er_sD.Er_cDataText[2], "%d", Eepr_View_Count.ac[2]);
            CanvasTextSet(&er_cData2, Er_sD.Er_cDataText[2]);
            WidgetPaint((tWidget * )&er_cData2);

            usprintf(Er_sD.DataSaveText2, "%d",
                    (uint16_t) Eepr_Data_Save.da[2]);
            CanvasTextSet(&g_sDataNumb2, Er_sD.DataSaveText2);
            WidgetPaint((tWidget * )&g_sDataNumb2);
            //--------------- 3 --------------------
            usprintf(Er_sD.Er_cDataText[3], "%d", Eepr_View_Count.ac[3]);
            CanvasTextSet(&er_cData3, Er_sD.Er_cDataText[3]);
            WidgetPaint((tWidget * )&er_cData3);

            usprintf(Er_sD.DataSaveText3, "%d",
                    (uint16_t) Eepr_Data_Save.da[3]);
            CanvasTextSet(&g_sDataNumb3, Er_sD.DataSaveText3);
            WidgetPaint((tWidget * )&g_sDataNumb3);
            //---------------- 4 --------------------
            usprintf(Er_sD.Er_cDataText[4], "%d", Eepr_View_Count.ac[4]);
            CanvasTextSet(&er_cData4, Er_sD.Er_cDataText[4]);
            WidgetPaint((tWidget * )&er_cData4);

            usprintf(Er_sD.DataSaveText4, "%d",
                    (uint16_t) Eepr_Data_Save.da[4]);
            CanvasTextSet(&g_sDataNumb4, Er_sD.DataSaveText4);
            WidgetPaint((tWidget * )&g_sDataNumb4);
            //--------------- 5 ---------------------
            usprintf(Er_sD.Er_cDataText[5], "%d", Eepr_View_Count.ac[5]);
            CanvasTextSet(&er_cData5, Er_sD.Er_cDataText[5]);
            WidgetPaint((tWidget * )&er_cData5);

            usprintf(Er_sD.DataSaveText5, "%d",
                    (uint16_t) Eepr_Data_Save.da[5]);
            CanvasTextSet(&g_sDataNumb5, Er_sD.DataSaveText5);
            WidgetPaint((tWidget * )&g_sDataNumb5);
            //------------------------------------
            switch (Gui_Notification.g_ucTypeSpeed) {
            case MPH:
                usprintf(Up_D.speedNumber, "%d.%d", (uint16_t) mph, g);
                CanvasTextSet(&g_sSpeedNumber, Up_D.speedNumber);
                WidgetPaint((tWidget * )&g_sSpeedNumber);
                CanvasTextSet(&g_sTypeSpeed, "mph");
                WidgetPaint((tWidget * )&g_sTypeSpeed);
                break;
            case KMH:
                usprintf(Up_D.speedNumber, "%d.%d", (uint16_t) kmh, k);
                if (kmh == 0) {
                    CanvasTextSet(&g_sTypeSpeed, "kmh");
                    WidgetPaint((tWidget * )&g_sTypeSpeed);
                } else {
                    CanvasTextSet(&g_sSpeedNumber, Up_D.speedNumber);
                    WidgetPaint((tWidget * )&g_sSpeedNumber);
                    CanvasTextSet(&g_sTypeSpeed, "kmh");
                    WidgetPaint((tWidget * )&g_sTypeSpeed);
                }
                break;
            case MS:
                usprintf(Up_D.speedNumber, "%d",
                        (uint16_t) Speed_MeterPerSecond);
                CanvasTextSet(&g_sSpeedNumber, Up_D.speedNumber);
                WidgetPaint((tWidget * )&g_sSpeedNumber);
                CanvasTextSet(&g_sTypeSpeed, "m/s");
                WidgetPaint((tWidget * )&g_sTypeSpeed);
                break;
            case NONE_CONFIG:
            default:
                usprintf(Up_D.speedNumber, "%d.%d", (uint16_t) kmh, k);
                CanvasTextSet(&g_sSpeedNumber, Up_D.speedNumber);
                WidgetPaint((tWidget * )&g_sSpeedNumber);
                CanvasTextSet(&g_sTypeSpeed, "kmh");
                WidgetPaint((tWidget * )&g_sTypeSpeed);
                break;
            }
            CanvasTextSet(&g_sDistance, Up_D.distanceText);
            WidgetPaint((tWidget * )&g_sDistance);
            Gui_Notification.g_ucDispRefresh = 0;
            TimerEnable(TIMER3_BASE, TIMER_A);
        }
    }
}
/*
 *
 * */
void InitGUI(void) {
    Gui_Notification.g_ucTypeSpeed = 0;
    InitDisplayTimer();
    LCD_SSD2119Init();

    TouchScreenInit();
    TouchScreenCallbackSet(WidgetPointerMessage);

    SysCtlDelay(100);
    CanvasImageSet(g_psRadioButtonIndicators + 1, g_pucLightOn);
    WidgetPaint((tWidget * )(g_psRadioButtonIndicators + 1));
    SysCtlDelay(30);

    WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sBackground);
    WidgetPaint((tWidget * )&g_sBackground);
    SysCtlDelay(30);

    WidgetAdd(WIDGET_ROOT, (tWidget *) &g_sSetType);
    WidgetPaint((tWidget * )&g_sSetType);
    SysCtlDelay(30);

    WidgetAdd(WIDGET_ROOT, (tWidget *) &ConfigBottom);
    WidgetPaint((tWidget * )&ConfigBottom);
    SysCtlDelay(30);

    usprintf(Up_D.sampFreqText, "SampFreq: %d Hz", adcNode[0].g_uiSamplingFreq);
    CanvasTextSet(&g_sSampFreq, Up_D.sampFreqText);
    WidgetPaint((tWidget * )&g_sSampFreq);

    SysCtlDelay(30);
    WidgetPaint(WIDGET_ROOT);
}
/*
 *
 * */
void ClrMyWidget(void) {
    WidgetRemove((tWidget *) &g_psRadio);

    WidgetPaint((tWidget * )&g_sSetType);
    WidgetPaint((tWidget * )&g_sBackground);
}
/*
 *
 * */
void stopSampling(void) {
    countTimerISRLoopSample = 0;
    TimerDisable(TIMER0_BASE, TIMER_A);
    IntPendClear(INT_TIMER0A);
    IntPendClear(INT_ADC0SS3);
    ADCIntDisable(ADC0_BASE, 3);
    uDMADisable();
}
/*
 *
 * */
void resumeSampling(void) {
    TimerEnable(TIMER0_BASE, TIMER_A);
    IntEnable(INT_TIMER0A);
    IntEnable(INT_ADC0SS3);
    ADCIntEnable(ADC0_BASE, 3);
    uDMAEnable();
}
