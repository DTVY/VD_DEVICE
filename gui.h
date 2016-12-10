//*****************************************************************************
//
// gui.h - Predefines, public functions, and globals for the graphical user
// interface portion of the code.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
//
//*****************************************************************************

#ifndef __GUI_H__
#define __GUI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define REFRESH_RATE	500

typedef enum UnitS {
    NONE_CONFIG = 0, MPH, KMH, MS
} UnitSpeed;
//-------------------------------------------------------------------------------------------//
// Flag used to inform the system that a screen refresh needs to happen as soon
// as sanely possible
//
typedef struct GuiNotification {
    volatile unsigned short int g_ucDispRefresh; //de kieu short int de xem debug
    volatile unsigned short int g_ucTypeSpeed;
    volatile unsigned short int g_ucCfgDisplay;
} GuiNotification_;

typedef struct StateButton {
    volatile unsigned short int StateButton1, StateButton2, StateButton3,
            StateButton4;
} StateButton_;

typedef struct up_data {
    char HeztText[20], speedNumber[10], sampFreqText[20], distanceText[20],
            maxValue[20];
} Update_Data;

typedef struct Er_sData {
    char Er_cDataText[6][3];
    char DataSaveText[10], DataSaveText1[10], DataSaveText2[10],
            DataSaveText3[10], DataSaveText4[10], DataSaveText5[10];
} Er_sData_;
//----------------------------------------------------------------//
static void OnConfigPress(tWidget *pWidget);
static void OnViewData(tWidget *pWidget);
static void OnRadioChange(tWidget *pWidget, uint32_t bSelected);
static void OnButtonPress(tWidget *pWidget);
static void OnButtonReset(tWidget *pWidget);
//---------------------------------------------------------------//
void InitGUI(void);
void GUIUpdateDisplay(void);
void ClrMyWidget(void);
static void InitDisplayTimer(void);

void stopSampling(void);
void resumeSampling(void);

#endif // __GUI_H__
