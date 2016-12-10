#ifndef EEPROM_H_
#define EEPROM_H_

//#define DEBUG_EEPROM_READ
#define EEPROM_COUNT  	30

typedef enum {
	EEPROM_MSG_INVALID_ADDRESS = 0, EEPROM_MSG_INVALID_NUMOFWORDS, EEPROM_MSG_OK
} EEPROM_MSG;

typedef struct EeprDataSave {
	uint32_t da[6];
} EeprDataSave_;

typedef struct EeprViewCount {
	uint16_t ac[6];
} EeprViewCount_;

void Config_eeprom(void);

#endif /* EEPROM_H_ */
