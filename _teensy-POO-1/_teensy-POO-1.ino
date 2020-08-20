#include <stdlib.h>

#include <HardwareSerial.h>

#include "IMidiStream.h"
#include "IDisplayer.h"

#include <MidiApplication.h>

#include "TeenSy_MidiStream.h"
#include "TeenSy_Displayer.h"


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
IMidiStream *midiStream = NULL;
IDisplayer *displayer = NULL;
MidiApplication *midiApplication = NULL;

// ==================================================================

void setup()
{
  Serial.begin(115200);

  Serial1.begin(31250); // open the serial port for MIDI

  // ========================================
  displayer = new TeenSy_Displayer(&Serial);
  midiStream = new TeenSy_MidiStream(&Serial1);
  midiApplication = new MidiApplication(
      midiStream)
  ;
  midiApplication->setDisplayer(displayer);
  midiApplication->init();
}

void loop()
{
  // //*******************METRO*****************************
  // if (midiApplication->play_ok || midiApplication->rec_ok)
  // {
  //   if (serialMetro.check())
  //     midiApplication->i_count += 1 * midiApplication->x_count;
  // }
  // //**************** flag "play" *************
  // if (midiApplication->play_ok)
  // {
  //   midiApplication->play();
  // }

  //****************************************************

  if (Serial1.available())
  {
    midiApplication->handleMidiCode();
  }
}

// void trsf_data_sd()
// {
//   int timesSize = sizeof(midiApplication->times);
//   char linec[timesSize];
//   // Initialize the SD.
//   if (!sd.begin(SD_CONFIG))
//   {
//     sd.initErrorHalt(&Serial);
//     return;
//   }
//   // Remove any existing file.
//   if (sd.exists("ReadCsvDemo.csv"))
//   {
//     sd.remove("ReadCsvDemo.csv");
//   }
//   // Create the file.
//   if (!file.open("ReadCsvDemo.csv", FILE_WRITE))
//   {
//     error("open failed");
//     exit(EXIT_FAILURE);
//   }
//   file.rewind();
//   //************************transfert debute ici****************
//   //******************tab > sd card*****************************
//   Serial.println(sizeof(midiApplication->times));
//   Serial.println(sizeof(midiApplication->times[0]));
//   for (int i = 0; i < sizeof(midiApplication->times) / sizeof(midiApplication->times[0]); i++)
//   {
//   Serial.println(sizeof(midiApplication->times));
//   Serial.println(sizeof(midiApplication->times[0]));
//   Serial.println("-----\r\n");

//     file.println(midiApplication->times[i]); //timer
//     delay(2);
//     file.println(midiApplication->commands[i]); //timer
//     delay(2);
//     file.println(midiApplication->data2s[i]); //timer
//     delay(2);
//     file.println(midiApplication->data3s[i]); //timer
//     if (midiApplication->times[i + 1] == 0.)
//     {
//       file.println("f"); //timer
//       break;
//     }
//   }

//   /*  for (int i=0;i<sizeof(midiApplication->commands)/sizeof(midiApplication->commands[0]);i++) {
//     file.println(midiApplication->commands[i]);//statut
//     delay(5);
//     if(midiApplication->commands[i+1]== 0. ){
//     file.println("f");//timer
//     break;
//     } 
//    }

//     for (int i=0;i<sizeof(midiApplication->data2s)/sizeof(midiApplication->data2s[0]);i++) {
//     file.println(midiApplication->data2s[i]);//note
//     delay(5);
//     if(midiApplication->data2s[i+1]== 0. ){
//     file.println("f");//timer
//     break;
//     } 
//    }

//     for (int i=0;i<sizeof(midiApplication->data3s)/sizeof(midiApplication->data3s[0]);i++) {
//     file.println(midiApplication->data3s[i]);//velocity
//     delay(5);
//     if(midiApplication->commands[i+1]== 0. ){
//     file.println("f");//timer
//     break;
//     } 
//    }*/

//   //*************************************************************
//   file.rewind(); //back file start
//   //*********************************
//   int j = 0;

//   while (file.available())
//   {
//     j++;
//     //***********sd card > tab********************************
//     linec[j] = file.read();
//     Serial.print(linec[j]);
//     if (linec[j - 1] > 0 && linec[j] == 0)
//       exit(EXIT_FAILURE);
//   }
//   //******************************************************
//   file.close();
//   Serial.println(F("Done"));
//   return;
// }
