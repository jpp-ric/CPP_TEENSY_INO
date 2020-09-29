#include <MsTimer2.h>

#include <stdlib.h>
#include <stdio.h>

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
//Metro VelocityDecreasing = Metro(10); // replace runing velocity
IMidiStream *midiStream = NULL;
IDisplayer *displayer = NULL;
MidiApplication *midiApplication = NULL;

// ==================================================================

IntervalTimer myTimer; //type interrups timer

void setup()
{

  Serial.begin(115200);

  Serial1.begin(31250); // open the serial port for MIDI
  myTimer.begin(Ticks_mid, 12000);

  // ========================================
  displayer = new TeenSy_Displayer(&Serial);
  midiStream = new TeenSy_MidiStream(&Serial1);
  midiApplication = new MidiApplication(midiStream);
  midiApplication->setDisplayer(displayer);
  midiApplication->init();
  
}

void loop()
{
  if (midiApplication->SwitchMetro)
  {
    midiApplication->Metronome();
  }

  if (midiApplication->start_rec_1 || midiApplication->start_rec_2 ||
      midiApplication->start_rec_3 || midiApplication->start_rec_4 ||
      midiApplication->start_rec_Trk5)
  {
    midiApplication->FlashingLed();
  }
  //============flashing led==============
  if (!midiApplication->TimerLed)
  {

    digitalWrite(13, LOW);
  }
  if (midiApplication->TimerLed)
  {

    digitalWrite(13, HIGH);
  }
  //==========================================

  //*******************METRO*****************************

  //midiApplication->StartRecTrack5();
  //midiApplication->StartStopPlayTrk5();

  // if (VelocityDecreasing.check())
  // {

  //   if (!midiApplication->Bang) //permanent alternate 0/1 like pulse generator
  //   {
  //     //midiApplication->beat += 1.0;
  //     midiApplication->Bang = true; //1
  //   }
  //   else
  //   {
  //     midiApplication->Bang = false; //0
  //   }

  //   /*for (int index_vel_run = 0; index_vel_run < 100; index_vel_run++)
  //   {
  //     if (midiApplication->X_velocitych1[index_vel_run] > 0)
  //     {
  //       midiApplication->X_velocitych1[index_vel_run] -= 1; //permanent decraese velocity
  //     }
  //   }*/
  // }
  //**************** flag "play" *************

  if (midiApplication->play_1_ok)
  {
    midiApplication->Ticks = 0; //reset ticks

    midiApplication->play_loop = true;
    midiApplication->play_1_ok = false;

    midiApplication->midiCodeIndex_1 = 0;
    midiApplication->midiCodeIndex_2 = 0;
    midiApplication->midiCodeIndex_3 = 0;
    midiApplication->play_1();
  }

  //**************** flag "play2" *************
  //Serial.println(midiApplication->Bang);
  if (midiApplication->play_2_ok)
  {
    if (!midiApplication->play_loop)
    {
      midiApplication->Ticks = 0; //reset ticks
      midiApplication->midiCodeIndex_2 = 0;
    }

    midiApplication->play_loop2 = true;
    midiApplication->play_2_ok = false;

    midiApplication->play_2();
  }

  //**************** flag "play3" *************
  //Serial.println(midiApplication->Bang);
  if (midiApplication->play_3_ok)
  {
    if (!midiApplication->play_loop)
    {
      midiApplication->Ticks = 0; //reset ticks
      midiApplication->midiCodeIndex_3 = 0;
    }
    midiApplication->play_loop3 = true;
    midiApplication->play_3_ok = false;

    midiApplication->play_3();
  }
  if (midiApplication->play_loop3)
  {
    //Serial.println("play loop");
    midiApplication->play_3();
  }
  //=============================================================
  if (midiApplication->play_4_ok)
  {
    if (!midiApplication->play_loop)
    {
      midiApplication->Ticks = 0; //reset ticks
      midiApplication->midiCodeIndex_4 = 0;
    }
    midiApplication->play_loop4 = true;
    midiApplication->play_4_ok = false;

    midiApplication->play_4();
  }

  //=====================================================
  if (midiApplication->play_loop)
  {
    //Serial.println("play loop");
    midiApplication->play_1();
  }
  if (midiApplication->play_loop2)
  {
    midiApplication->play_2();
  }
  //===============================================

  //==============================================================

  //==================================================
  if (midiApplication->play_loop4)
  {
    //Serial.println("play loop");
    midiApplication->play_4();
  }
  //==================================================
  //**************** flag playTrk5 *************

  if (midiApplication->play_Trk5)
  {
    if (!midiApplication->play_loop)
    {
      midiApplication->TicksTrk5 = 0; //reset ticks
      midiApplication->midiCodeIndex_Trk5 = 0;
    }
    midiApplication->play_loop_Trk5 = true;
    midiApplication->play_Trk5 = false;

    midiApplication->playTrk5();
  }
  if (midiApplication->play_loop_Trk5)
  {
    //midiApplication->StartStopPlayTrk5();
    midiApplication->playTrk5();
  }
  //================================================
  

  //****************************************************

  if (Serial1.available())
  {
    midiApplication->handleMidiCode();
  }

  if (midiApplication->DataMidtoSd)
  {
    midiApplication->DataMidtoSd = false;

    filewrite();
  }
  //=============================================
  if (midiApplication->SdtoDataMid)
  {
    midiApplication->SdtoDataMid = false;

    readfile();
    
  }
}
//************interrups function***********
void Ticks_mid()
{
  if ((midiApplication->play_loop) || (midiApplication->start_rec_1) ||
      (midiApplication->start_rec_2) || (midiApplication->play_2_ok) ||
      (midiApplication->play_3_ok) || (midiApplication->start_rec_3) ||
      (midiApplication->play_loop2) || (midiApplication->play_loop3) ||
      (midiApplication->play_1_ok) || (midiApplication->play_loop4) ||
      (midiApplication->play_4_ok) || (midiApplication->start_rec_4))

  {

    midiApplication->Ticks += (1 * midiApplication->x_count); //incr ticks
    midiApplication->beat1 += (1 * midiApplication->x_count);
  }
  if ((midiApplication->play_loop_Trk5) || (midiApplication->start_rec_Trk5) ||
      (midiApplication->play_Trk5))
  {

    midiApplication->TicksTrk5 += (1 * midiApplication->x_count);
  }
}
//==============================================

//=========================================
//Serial.println(midiApplication->Ticks);
void trsf_data_sd()
{
  int ess = 0;
  int timesSize = sizeof(midiApplication->time1s);
  int linec[timesSize];
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("jppessai.cvs"))
  {
    sd.remove("jppessai.cvs");
  }
  // Create the file.
  if (!file.open("jppessai.cvs", FILE_WRITE))
  {
    error("open failed");
    exit(EXIT_FAILURE);
  }
  file.rewind();
  //************************transfert debute ici****************
  //******************tab > sd card*****************************
  Serial.println(sizeof(midiApplication->time1s));
  Serial.println(sizeof(midiApplication->time1s[0]));
  for (int i = 0; i < sizeof(midiApplication->time1s) / sizeof(midiApplication->time1s[0]); i++)
  {
    

    file.println(midiApplication->time1s[i]); //timer
    delay(2);
    ess = midiApplication->command1s[i]; //>> 16 & 0x000000FF;
    //file.println(midiApplication->command1s[i] >>16 );//& 0x00000000000000FF); //timer
    file.println(ess);
    delay(2);
    Serial.println(midiApplication->time1s[i]); //timer

    Serial.println(ess);
    //file.println(midiApplication->data21s[i]); //timer
    //delay(2);
    //file.println(midiApplication->data31s[i]); //timer
    if (midiApplication->time1s[i + 1] == 0.)
    {
      file.println("f"); //timer
      file.close();
      Serial.println(F("Done"));

      break;
    }
  }
}
//===============================================================
//======================================================
void filewrite()
{
  

  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("jppessai.cvs"))
  {
    sd.remove("jppessai.cvs");
  }
  // Create the file.
  if (!file.open("jppessai.cvs", FILE_WRITE))
  {
    error("open failed");
  }
  
  
 
 
  
  for (int i = 0; i <10000; i++)
  {
    Serial.println(midiApplication->time1s[i]);
    Serial.println(midiApplication->command1s[i]);
    
  file.print(midiApplication->time1s[i]);
  file.println("/n"); 
  
    file.print(midiApplication->command1s[i]);
    file.println("/n");

    delay(2);
    
    //Serial.println(midiApplication->command1s[i]);
    //i++;
    if(midiApplication->command1s[i]==98)
    {

    //   for (int i = 0; i < 200; i++)
    //   {
    //   midiApplication->time1s[i]=0;
    // midiApplication->command1s[i]=0; 

    //   }

      break;
    }
    
  }
  
   
  file.close();
  Serial.println(F("Done"));
  return;
}
//======================readfile===========================================
void readfile()
{
  String  buffer;

  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  
  if (!file.open("jppessai.cvs", FILE_WRITE))
  {
    error("open failed");
  }
   
  
  //*************************************************************
  file.rewind(); //back file start  
  
  int i = 0;  

  //while (file.available() >= 98)
  for(i=0;i<10000;i++)
  {
        
    
    buffer = file.readStringUntil('/n');    
    
    midiApplication->time1s[i]=buffer.toFloat();

    buffer = file.readStringUntil('/n');    
    
    midiApplication->command1s[i]=buffer.toInt();

    Serial.println(midiApplication->time1s[i]);
    Serial.println(midiApplication->command1s[i]);
    
    
    if(buffer.toInt()==98){
     // midiApplication->sd_cardtrk1=true;
      midiApplication->data_trk_1=true;
      midiApplication->x_count = 1.0;
      midiApplication->inst1=midiApplication->command1s[1]>>24 & 0x000000FF;
      Serial.println(midiApplication->inst1);
      midiApplication->inst = midiApplication->inst1;
      midiApplication->command=192; 
      midiApplication->sendProgramChange();
      

    break;

    }
    
  }
 
  
  //******************************************************
  file.close();
  Serial.println(F("Done"));
  return;
}
