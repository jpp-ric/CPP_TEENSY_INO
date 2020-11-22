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
//=================================================================
  
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
  //========================essainotestri==============================
//   midiApplication->tab[0]=10;
//  midiApplication->tab[1]=34;
//  midiApplication->tab[2]=3;
//  midiApplication->tab[3]=23;
//  midiApplication->tab[4]=5;
//  midiApplication->tab[5]=54;

//  midiApplication->nbr=5;
 
//   midiApplication->triFusion(0, midiApplication->nbr-1, midiApplication->tab, midiApplication->tmp);//aligne nt ordre croissant  
 
  
//   for(int i = 0; i < 5; i++) 
//    {
//      Serial.print(midiApplication->tab[i]);
//   }

 
}
//==========================================================

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
  //===============save tracks to sdcard=========================================

  if (midiApplication->DataMidtoSd)
  {
    
    if(midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==88)
    {
      midiApplication->data2=0;
      midiApplication->SdtoDataMid = false;
     
    filewrite_trk1();
    }
    else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==89 &&
     midiApplication->data3>0)
    {
      midiApplication->data3=0;
      midiApplication->SdtoDataMid = false;
      Serial.print("ooook");
    filewrite_trk2();
    }
    
    else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==91)
    {
      midiApplication->data2=0;
      midiApplication->SdtoDataMid = false;
      
    filewrite_trk3();
    }
    // else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==93)
    // {
     // midiApplication->data2=0;
      
    //   midiApplication->DataMidtoSd = false;
    // filewrite_trk4();
    // }
    else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==95)
    {
      midiApplication->data2=0;
      midiApplication->SdtoDataMid = false;
      
    filewrite_trk5();
    }


    
  }
  //=============================================
  if (midiApplication->SdtoDataMid)
  {
    

    if(midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==88)
    {
      midiApplication->data2=0;
      midiApplication->SdtoDataMid = false;
    readfile_trk1();
    }
    else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==89 &&
      midiApplication->data3>0)
    {
      midiApplication->data3=0;
      midiApplication->SdtoDataMid = false;
      Serial.print("ok");
    filewrite_trk2();
    }
    else if(midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==91)
    {
      midiApplication->data2=0;
      midiApplication->SdtoDataMid = false;
     
    readfile_trk3();
    }
    // else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==93)
    // {
     // midiApplication->data2=0;
    //  midiApplication->SdtoDataMid = false;
    // filewrite_trk4();
    // }
    else if (midiApplication->isNoteOnCommand(midiApplication->command) && midiApplication->data2==95)
    {
     midiApplication->data2=0;
     
      midiApplication->SdtoDataMid = false;
    readfile_trk5();
    }

    


    
  }
}
//============================================================================
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

//==========================filetrk1============================================================
//======================================================
void filewrite_trk1()
{
   if(midiApplication->command1s[1] >0){
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("sdtrk1.cvs"))
  {
    sd.remove("sdtrk1.cvs");
  }
  // Create the file.
  if (!file.open("sdtrk1.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //==============writetofilesd=================================
  file.print(midiApplication->tempotrk1);
    file.println("/n");
  int i=0;
  bool flag_write = true;
  while (flag_write) 
  {
    // Serial.println(midiApplication->time1s[i]);
    // Serial.println(midiApplication->command1s[i]);
    //this->x_count = midiApplication->tempo;
     

    file.print(midiApplication->time1s[i]);
    file.println("/n");

    file.print(midiApplication->command1s[i]);
    file.println("/n");

    //Serial.println(midiApplication->command1s[i]);
   
    if (midiApplication->command1s[i] == 98)
    {
      flag_write = false;
      break;
    }
   i++;
  }

  file.close();
  Serial.println(F("Done"));
  return;
   }
}
//======================readfile===========================================
void readfile_trk1()
{
 
  String buffer;

  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }

  if (!file.open("sdtrk1.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //*************************************************************
  file.rewind(); //back file start

  buffer = file.readStringUntil('/n');
   midiApplication->x_count = buffer.toFloat();

  int i = 0;

  bool flag_read=true;
  while(flag_read)
  {

    buffer = file.readStringUntil('/n');

    midiApplication->time1s[i] = buffer.toFloat();

    buffer = file.readStringUntil('/n');

    midiApplication->command1s[i] = buffer.toInt();

    //Serial.println(midiApplication->time1s[i]);
    //Serial.println(midiApplication->command1s[i]);

    if (buffer.toInt() == 98)
    {
      // midiApplication->sd_cardtrk1=true;
      midiApplication->data_trk_1 = true;
      //midiApplication->x_count = 1.0;
      midiApplication->inst1 = midiApplication->command1s[1] >> 24 & 0x000000FF;
      Serial.println(midiApplication->inst1);
      midiApplication->inst = midiApplication->inst1;
      midiApplication->command = 192;
      midiApplication->sendProgramChange();
      flag_read=false;

      break;
    }
    i++;
  }

  //******************************************************
  file.close();
  Serial.println(F("Done"));
  return;
  
}

//===================================================================================
void filewrite_trk2()
{
   if(midiApplication->command2s[1] >0){
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("sdtrk2.cvs"))
  {
    sd.remove("sdtrk2.cvs");
  }
  // Create the file.
  if (!file.open("sdtrk2.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //==============writetofilesd=================================
  file.print(midiApplication->tempotrk2);
    file.println("/n");
  int i=0;
  bool flag_write = true;
  while (flag_write) 
  {
     Serial.println(midiApplication->time2s[i]);
     Serial.println(midiApplication->command2s[i]);
    //this->x_count = midiApplication->tempo;
     

    file.print(midiApplication->time2s[i]);
    file.println("/n");

    file.print(midiApplication->command2s[i]);
    file.println("/n");

    //Serial.println(midiApplication->command1s[i]);
   
    if (midiApplication->command2s[i] == 98)
    {
      flag_write = false;
      break;
    }
   i++;
  }

  file.close();
  Serial.println(F("Done"));
  return;
   }
}
//======================readfile===========================================
void readfile_trk2()
{
 
  String buffer;

  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }

  if (!file.open("sdtrk2.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //*************************************************************
  file.rewind(); //back file start

  buffer = file.readStringUntil('/n');
   midiApplication->x_count = buffer.toFloat();

  int i = 0;

  bool flag_read=true;
  while(flag_read)
  {

    buffer = file.readStringUntil('/n');

    midiApplication->time2s[i] = buffer.toFloat();

    buffer = file.readStringUntil('/n');

    midiApplication->command2s[i] = buffer.toInt();

    Serial.println(midiApplication->time2s[i]);
    Serial.println(midiApplication->command2s[i]);

    if (buffer.toInt() == 98)
    {
      // midiApplication->sd_cardtrk1=true;
      midiApplication->data_trk_2 = true;
      //midiApplication->x_count = 1.0;
      midiApplication->inst2 = midiApplication->command2s[1] >> 24 & 0x000000FF;
      Serial.println(midiApplication->inst2);
      midiApplication->inst = midiApplication->inst2;
      midiApplication->command = 193;
      midiApplication->sendProgramChange();
      flag_read=false;

      break;
    }
    i++;
  }

  //******************************************************
  file.close();
  Serial.println(F("Done"));
  return;
  
}
//==================================================================
void filewrite_trk3()
{
   if(midiApplication->command3s[1] >0){
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("sdtrk3.cvs"))
  {
    sd.remove("sdtrk3.cvs");
  }
  // Create the file.
  if (!file.open("sdtrk3.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //==============writetofilesd=================================
  file.print(midiApplication->tempotrk3);
    file.println("/n");
  int i=0;
  bool flag_write = true;
  while (flag_write) 
  {
    // Serial.println(midiApplication->time1s[i]);
    // Serial.println(midiApplication->command1s[i]);
    //this->x_count = midiApplication->tempo;
     

    file.print(midiApplication->time3s[i]);
    file.println("/n");

    file.print(midiApplication->command3s[i]);
    file.println("/n");

    //Serial.println(midiApplication->command1s[i]);
   
    if (midiApplication->command3s[i] == 98)
    {
      flag_write = false;
      break;
    }
   i++;
  }

  file.close();
  Serial.println(F("Done"));
  return;
   }
}
//======================readfile===========================================
void readfile_trk3()
{
 
  String buffer;

  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }

  if (!file.open("sdtrk3.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //*************************************************************
  file.rewind(); //back file start

  buffer = file.readStringUntil('/n');
   midiApplication->x_count = buffer.toFloat();

  int i = 0;

  bool flag_read=true;
  while(flag_read)
  {

    buffer = file.readStringUntil('/n');

    midiApplication->time3s[i] = buffer.toFloat();

    buffer = file.readStringUntil('/n');

    midiApplication->command3s[i] = buffer.toInt();

    //Serial.println(midiApplication->time1s[i]);
    //Serial.println(midiApplication->command1s[i]);

    if (buffer.toInt() == 98)
    {
      // midiApplication->sd_cardtrk1=true;
      midiApplication->data_trk_3 = true;
      //midiApplication->x_count = 1.0;
      midiApplication->inst3 = midiApplication->command3s[1] >> 24 & 0x000000FF;
      Serial.println(midiApplication->inst3);
      midiApplication->inst = midiApplication->inst3;
      midiApplication->command = 194;
      midiApplication->sendProgramChange();
      flag_read=false;

      break;
    }
    i++;
  }

  //******************************************************
  file.close();
  Serial.println(F("Done"));
  return;
  
}

//===================================filetrk5======================================

void filewrite_trk5()
{
   if(midiApplication->commandTrk5[1] >0){
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("sdtrk5.cvs"))
  {
    sd.remove("sdtrk5.cvs");
  }
  // Create the file.
  if (!file.open("sdtrk5.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //==============writetofilesd=================================
  file.print(midiApplication->tempotrk5);
    file.println("/n");
  int i=0;
  bool flag_write = true;
  while (flag_write) 
  {
    // Serial.println(midiApplication->time1s[i]);
    // Serial.println(midiApplication->command1s[i]);

    file.print(midiApplication->timeTrk5[i]);
    file.println("/n");

    file.print(midiApplication->commandTrk5[i]);
    file.println("/n");

    //Serial.println(midiApplication->command1s[i]);
   
    if (midiApplication->commandTrk5[i] == 99)
    {
      flag_write = false;
      break;
    }
   i++;
  }

  file.close();
  Serial.println(F("Done"));
  return;
   }
}
//======================readfiletrk5===========================================
void readfile_trk5()
{
 
  String buffer;

  // Initialize the SD.
  if (!sd.begin(SD_CONFIG))
  {
    sd.initErrorHalt(&Serial);
    return;
  }

  if (!file.open("sdtrk5.cvs", FILE_WRITE))
  {
    error("open failed");
  }

  //*************************************************************
  file.rewind(); //back file start
  buffer = file.readStringUntil('/n');
   midiApplication->x_count = buffer.toFloat();

  int i = 0;

  bool flag_read=true;
  while(flag_read)
  {

    buffer = file.readStringUntil('/n');

    midiApplication->timeTrk5[i] = buffer.toFloat();

    buffer = file.readStringUntil('/n');

    midiApplication->commandTrk5[i] = buffer.toInt();

    //  Serial.println(midiApplication->timeTrk5[i]);
    // Serial.println(midiApplication->commandTrk5[i]);

    if (buffer.toInt() == 99)
    {
      // midiApplication->sd_cardtrk1=true;
      midiApplication->data_Trk5 = true;
     // midiApplication->x_count = 1.0;
     // midiApplication->inst1 = midiApplication->commandTrk5[1] >> 24 & 0x000000FF;
     // Serial.println(midiApplication->inst1);
     // midiApplication->inst = midiApplication->inst1;
     // midiApplication->command = 192;
     // midiApplication->sendProgramChange();
      flag_read=false;

      break;
    }
    i++;
  }

  //******************************************************
  file.close();
  Serial.println(F("Done"));
  return;
  
}

