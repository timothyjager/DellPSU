/*
Copyright (c) 2017, Tim Jager
This library reads data from a Dell laptop power supply using 1 wire protocol
MIT license, check license.txt for more information
*/

#ifndef dell_psu_h
#define dell_psu_h

//this library requires the OneWire library (https://github.com/PaulStoffregen/OneWire)
#include <OneWire.h>

//we try to read up to x bytes from the device (17 bytes is the minimum to extract the power supply wattage, voltage, & amps
#define DELL_PSU_BYTES_TO_READ 17
//For genuine/official chargers, this should be the expected response header
// 'DELL00AC' -> ASCII decimal
const uint8_t OFFICIAL_DELL_HEADER[8] PROGMEM = {68, 69, 76, 76, 48, 48, 65, 67};
class DellPSU
{
private:
  uint16_t _watts = 0;
  uint16_t _millivolts = 0;
  uint16_t _milliamps = 0;
  OneWire _onewire;
  char _resp[DELL_PSU_BYTES_TO_READ];

public:
  DellPSU(uint8_t pin);

  //Determine if a device is present, return true on success
  boolean psu_detected(void);

  //Read the data stream from the psu, return true on success
  boolean read_data(bool allowReadingThirdPartyAdapter = true);

  //get the watts
  uint16_t watts(void);

  //get the millivolts
  uint16_t millivolts(void);

  //get the milliamps
  uint16_t milliamps(void);

  //get the raw response char array
  char *raw_response(void);
};

#endif
