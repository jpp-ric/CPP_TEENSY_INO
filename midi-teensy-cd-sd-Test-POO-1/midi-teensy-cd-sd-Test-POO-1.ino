#include <HardwareSerial.h>

#include <MidiApplication.h>
#include "IMidiStream.h"

#include "TeenSy_MidiStream.h"

#include "constantes.h"
#include "Note.h"



//***********midi in out*****************
#include "SdFat.h"

#include <Metro.h> // Include the Metro library



// =========================================================


// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 2

// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif // SDCARD_SS_PIN

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI)
#else // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI)
#endif // HAS_SDIO_CLASS

#if SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;

#else // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif // SD_FAT_TYPE

// Store error strings in flash to save RAM.
#define error(s) sd.errorHalt(&Serial, F(s))


// ====================================================================

Metro serialMetro = Metro(10); // Instantiate an instance
IMidiStream* midiStream = NULL;
MidiApplication* midiApplication = NULL;



// ==================================================================

void setup()
{
  Serial.begin(115200);

  Serial1.begin(31250); // open the serial port for MIDI

  // ========================================

  midiStream = new TeenSy_MidiStream(&Serial1);
  midiApplication = new MidiApplication(
    midiStream
  );

  midiApplication->init();

}



void loop()
{
  //*******************METRO*****************************
  if ( midiApplication->play_ok || midiApplication->rec_ok )
  {
    if (serialMetro.check())
      midiApplication->i_count += 1 * midiApplication->x_count;
  }
  //****************flag "play"*************
  if (midiApplication->play_ok) {
    midiApplication->play();
  }

  //****************************************************

  if (Serial1.available())
  {
    data = midiApplication->getMidiStream()->read();
    midiApplication->treatMidiCode(data);
  }
}