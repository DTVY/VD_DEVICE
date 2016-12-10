/*
 * EEPROM.c
 *
 *  Created on: Aug 6, 2013
 *      Author: Admin
 */
#include "include.h"
#include "gui.h"

EeprViewCount_ Eepr_View_Count;
EeprDataSave_ Eepr_Data_Save;

extern float mph;
extern float kmh;
//define trong gui.c
extern GuiNotification_ Gui_Notification;

uint32_t pui32Data[EEPROM_COUNT] = { 0 };
uint32_t pui32Read[EEPROM_COUNT] = { 0 };
volatile uint16_t eeprCount = 0;
uint32_t max = 0;

static EEPROM_MSG EEPROMWrite(uint32_t *pui32_Data, uint32_t ui32WordAddress,
        uint32_t NumOfWords);
static EEPROM_MSG EEPROMReadWords(uint32_t *pui32_Data,
        uint32_t ui32WordAddress, uint32_t NumOfWords);

static EEPROM_MSG EEPROMWrite(uint32_t *pui32_Data, uint32_t ui32WordAddress,
        uint32_t NumOfWords) {
    uint32_t Address;
    if (ui32WordAddress > 0x7ff) {
        return (EEPROM_MSG_INVALID_ADDRESS);
    }
    if ((ui32WordAddress + NumOfWords) > 0x7ff) {
        return (EEPROM_MSG_INVALID_NUMOFWORDS);
    }
    Address = ui32WordAddress << 2;
    ROM_EEPROMProgram(pui32_Data, Address, NumOfWords << 2);

    return EEPROM_MSG_OK;
}

static EEPROM_MSG EEPROMReadWords(uint32_t *pui32_Data,
        uint32_t ui32WordAddress, uint32_t NumOfWords) {
    uint32_t Address;
    if (ui32WordAddress > 0x7ff) {
        return (EEPROM_MSG_INVALID_ADDRESS);
    }
    if ((ui32WordAddress + NumOfWords) > 0x7ff) {
        return (EEPROM_MSG_INVALID_NUMOFWORDS);
    }
    Address = ui32WordAddress << 2;
    ROM_EEPROMRead(pui32_Data, Address, NumOfWords << 2);

    return EEPROM_MSG_OK;
}

void Config_eeprom(void) {
    switch (Gui_Notification.g_ucTypeSpeed) {
    case KMH:
        if (kmh != 0.0) {
            if (eeprCount < EEPROM_COUNT) {
                pui32Data[eeprCount] = kmh;
                eeprCount++;
            }
            if (eeprCount == EEPROM_COUNT)
                eeprCount = 0;
        }
        break;
    case MPH:
        if (mph != 0.0) {
            if (eeprCount < EEPROM_COUNT) {
                pui32Data[eeprCount] = mph;
                eeprCount++;
            }
            if (eeprCount == EEPROM_COUNT)
                eeprCount = 0;
        }
        break;
    case MS:
        break;
    case NONE_CONFIG:
        if (kmh != 0.0) {
            if (eeprCount < EEPROM_COUNT) {
                pui32Data[eeprCount] = kmh;
                eeprCount++;
            }
            if (eeprCount == EEPROM_COUNT) {
                eeprCount = 0;
            }
        }
        break;
    default:
        break;
    }
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    EEPROMInit();
    EEPROMMassErase();
    EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));
    EEPROMProgram(pui32Data, 0x0, sizeof(pui32Data));
    EEPROMRead(pui32Read, 0x0, sizeof(pui32Read));
    uint16_t n, j, k;

    for (n = 0; n < 6; n++) {
        Eepr_Data_Save.da[n] = pui32Read[eeprCount - (n + 1)];
    }
    max = Eepr_Data_Save.da[0];
    for (j = 1; j < 6; j++) {
        if (max < Eepr_Data_Save.da[j])
            max = Eepr_Data_Save.da[j];
    }
    Eepr_View_Count.ac[0] = eeprCount;
    if (Eepr_Data_Save.da[0] > 1000)
        Eepr_Data_Save.da[0] = 0;
    for (k = 1; k < 6; k++) {
        if (Eepr_Data_Save.da[k] != 0) {
            if (Eepr_Data_Save.da[k] < 1000)
                Eepr_View_Count.ac[k] = eeprCount + k;
        }
    }
    //Debug for Eeprom
#ifdef DEBUG_EEPROM_READ
    if (kmh != 0.0 || mph != 0.0)
    UARTprintf("eeproom data: %d-%d\n", pui32Read[eeprCount - 1],
            eeprCount);
#endif
}
