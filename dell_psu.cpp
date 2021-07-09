/*
Copyright (c) 2017, Tim Jager
This library reads data from a Dell laptop power supply using 1 wire protocol
MIT license, check license.txt for more information
*/

#include "dell_psu.h"

DellPSU::DellPSU(uint8_t pin) : _onewire(pin)
{
  _watts = 0;
  _millivolts = 0;
  _milliamps = 0;
}

//Determine if a device is detected on the bus
boolean DellPSU::psu_detected()
{
  return _onewire.reset(); // OneWire bus reset, always needed to start operation on the bus, returns TRUE if there's a device present.
}

//Read all of the data from the device
boolean DellPSU::read_data(bool allowReadingThirdPartyAdapter = true)
{
  byte i;
  byte command_bytes[3] = {                   // commands array to initiate a read command,LSB,MSB addresses
                           0xF0, 0x00, 0x00}; // 0xF0 is the Read Data command, 00h 00h is the starting address
  byte ccrc;                                  // Variable to store the command CRC
  byte ccrc_calc;

  //reset the bus and make sure a device is present
  if (!psu_detected())
  {
    return false;
  }

  //we found a device, so let's try to read from it.
  _onewire.skip();                             // Skip ROM search
  _onewire.write_bytes(command_bytes, 3);      // 0xF0 Read data command, 0x00 LSB starting address, 0x00 MSB starting address
  ccrc = _onewire.read();                      // Device generates a CRC for the command we sent
  ccrc_calc = OneWire::crc8(command_bytes, 3); // We calculate the CRC of the commands we sent using the library function

  //make sure the CRC matches
  if (ccrc_calc != ccrc)
  {
    return false; // if not, return
  }

  //read all of the bytes from the device
  for (i = 0; i < DELL_PSU_BYTES_TO_READ; i++)
  {                             // Now it's time to read the PROM data itself, each page is 32 bytes so we need 32 read commands
    _resp[i] = _onewire.read(); // we store each read byte to a different position in the data array
  }

  //Parse the values from the string (characters 8,9,10 are the wattage. 11,12,13 are the voltage. 14,15,16 are the current)
  //the string below is an example of what is read from the device
  //DELL00AC180195092CN0WW4XY4866139S4SC9A03
  //
  //Header  | Watts | Volts*10 | Amps*10 | Serial Number
  //DELL00AC  180     195        092       CN0WW4XY4866139S4SC9A03 <-these are the characters we are parsing
  //DELL00AC  180     19.5       9.2       CN0WW4XY4866139S4SC9A03 <-these are interpreted values from those characters (180Watts, 19.5 volts, 9.2amps)
  //
  //00000000  001     111        111       11122222222223333333333 <-these numbers are just index values used for counting character positions
  //01234567  890     123        456       78901234567890123456789 <-these numbers are just index values used for counting character positions

  //3rd-party 'Dell Compatible' chargers don't usually contain the official 'DELL' header(for obvious legal reasons), however,
  //they do follow the rest of the data structure. To facilitate reading of these compatible chargers, the 'allowReadingThirdPartyAdapter'
  //flag can be set TRUE, which skips the 'DELL official' header check.
  //EXAMPLE: I have a 330W 'Dell compatible' charger, which reports the following:

  //Header  | Watts | Volts*10 | Amps*10
  //45CC0013   330       195      169

  if (!allowReadingThirdPartyAdapter)
  {
    // 3rd-party adapter reading not permitted. Verify DELL header
    uint8_t headerResult = memcmp_P(_resp, OFFICIAL_DELL_HEADER, sizeof(OFFICIAL_DELL_HEADER));
    if ( headerResult != 0)
    {
      // Header doesn't match expected. Bail
      return false;
    }
  }

  //now convert from ASCII characters to integer values
  //In order to keep everything as integers, we are using units like millivolts, milliamps so we don't have to represent 19.5 volts & 9.2 amps as floats.
  _watts = 100 * (uint16_t)(_resp[8] - 48) + 10 * (uint16_t)(_resp[9] - 48) + (_resp[10] - 48);
  _millivolts = 10000 * (uint16_t)(_resp[11] - 48) + 1000 * (uint16_t)(_resp[12] - 48) + 100 * (uint16_t)(_resp[13] - 48);
  _milliamps = 10000 * (uint16_t)(_resp[14] - 48) + 1000 * (uint16_t)(_resp[15] - 48) + 100 * (uint16_t)(_resp[16] - 48);
  
  return true;
}

//return the watts
uint16_t DellPSU::watts()
{
  return _watts;
}

//return the millivolts
uint16_t DellPSU::millivolts()
{
  return _millivolts;
}
//return the milliamps
uint16_t DellPSU::milliamps()
{
  return _milliamps;
}

//return the raw response char array
char *DellPSU::raw_response()
{
  return _resp;
}
