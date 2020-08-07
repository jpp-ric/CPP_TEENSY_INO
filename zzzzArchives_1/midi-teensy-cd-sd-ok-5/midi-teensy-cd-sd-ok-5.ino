
//***********midi in out*****************
#include "Note.h"
#define VEL 1
#include "SdFat.h"

#include <Metro.h> // Include the Metro library
Note note(34);

Metro serialMetro = Metro(10);  // Instantiate an instance
int inst_1= 1;
int inst_2 = 1;



int switch_splt = 0;

// SD_FAT_TYPE = 0 for SdFat/File as defined in SdFatConfig.h,
// 1 for FAT16/FAT32, 2 for exFAT, 3 for FAT16/FAT32 and exFAT.
#define SD_FAT_TYPE 2


// SDCARD_SS_PIN is defined for the built-in SD on some boards.
#ifndef SDCARD_SS_PIN
const uint8_t SD_CS_PIN = SS;
#else  // SDCARD_SS_PIN
// Assume built-in SD is used.
const uint8_t SD_CS_PIN = SDCARD_SS_PIN;
#endif  // SDCARD_SS_PIN

// Try to select the best SD card configuration.
#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI)
#endif  // HAS_SDIO_CLASS


#if SD_FAT_TYPE == 2
SdExFat sd;
ExFile file;

#else  // SD_FAT_TYPE
#error Invalid SD_FAT_TYPE
#endif  // SD_FAT_TYPE

char line[40];

//------------------------------------------------------------------------------
// Store error strings in flash to save RAM.
#define error(s) sd.errorHalt(&Serial, F(s))
//------------------------------------------------------------------------------
// Check for extra characters in field or find minus sign.
char* skipSpace(char* str) {
  while (isspace(*str)) str++;
  return str;
}
//------------------------------------------------------------------------------
bool parseLine(char* str) {
  char* ptr;

  // Set strtok start of line.
  str = strtok(str, ",");
  if (!str) return false;
  
  // Print text field.
  Serial.println(str);
  
  // Subsequent calls to strtok expects a null pointer.
  str = strtok(nullptr, ",");
  if (!str) return false;
  
  // Convert string to long integer.
  int32_t i32 = strtol(str, &ptr, 0);
  if (str == ptr || *skipSpace(ptr)) return false;
  Serial.println(i32);
  
  str = strtok(nullptr, ",");
  if (!str) return false;
  
  // strtoul accepts a leading minus with unexpected results.
  if (*skipSpace(str) == '-') return false;
  
  // Convert string to unsigned long integer.
  uint32_t u32 = strtoul(str, &ptr, 0);
  if (str == ptr || *skipSpace(ptr)) return false;
  Serial.println(u32);

  str = strtok(nullptr, ",");
  if (!str) return false;
  
  // Convert string to double.
  double d = strtod(str, &ptr);
  if (str == ptr || *skipSpace(ptr)) return false;
  Serial.println(d);
  
  // Check for extra fields.
  return strtok(nullptr, ",") == nullptr;
}
//------------------------------------------------------------------------------



float i_count = 0.1;
int i_clignt = 0;
float  x_count = 0.1;
int comand_nt = 144;
int st_rec = 0;
int play_ok = 0;
int rec_ok = 0;
int data2 = 0;
int data3 = 0;
int flag = 0;

int flag_split = 0;

int inst =1;

int  indxmid = 0;

int data = 0;
int comand = 0;
int Data2 = 60;
int Data3 = 100;
boolean nORv = Data2;

void setup() {
  
  Serial1.begin(31250); // open the serial port for MIDI
  Serial.begin(115200);
  
  Serial1.write(192);
  Serial1.write(0);
  Serial1.write(0);
  
  Serial1.write(193);
  Serial1.write(0);
  Serial1.write(1);
  
  Serial1.write(193);
  Serial1.write(33);
  
  
  
  
  Serial1.write(196);
  Serial1.write(0);
  Serial1.write(0);
  
   Serial1.write(199);
  Serial1.write(0);
  Serial1.write(0);
  
  Serial1.write(183);
  Serial1.write(7);
  Serial1.write(90);
  
  Serial1.write(176);
  Serial1.write(7);
  Serial1.write(90);
  
   Serial1.write(177);
  Serial1.write(7);
  Serial1.write(90);
  
  
  Serial1.write(180);
  Serial1.write(7);
  Serial1.write(90);
}

void loop() {

 
//*******************METRO*****************************
  if(play_ok == 1 | rec_ok == 1){ 
    if (serialMetro.check() == 1)
    i_count +=1 * x_count;
    i_clignt +=1; 
  }
  //****************flag "play"*************
   if(play_ok == 1)
  //  play();
   //comand = 0; 
    
  //****************************************************
    
  if (Serial1.available()) {
  data = Serial1.read();
    
  //*****************************data mid********************************
  //**********************************************************************
    if (data > 253) {
      return;  // other MIDI comand
    }
   //*************************statu-mid********************************************** 
    else if (data > 127) {
      comand = data;  // Data2 OFF or ON comand
           
    }
    //*********************second byte*********************************************************
    else if (flag == 0) {
      Data2 = data;
      
      flag = 1;
            
    }
    //**********************third byte******************************************************
    else if (flag == 1) {
      Data3 = data;
      flag= 0;
      //***********************note on off******************************************************    
    
      if(comand < 160 && comand > 0){
       
     /* if (comand == 144 && flag_split == 1)   
      split_kb();*/
      
      if (comand >143 && comand < 160)
      comand_nt = comand; 
      if(flag_split == 1 && Data2 < 54 && comand == 144){
      comand = 145;
      }
      if(flag_split == 1 && Data2 < 54 && comand == 128){
       comand = 129;
       } 
     
      Serial1.write(comand);       
      Serial1.write(Data2);
      Serial1.write(Data3);
      
     if(flag_split == 1){  
     if(comand == 129)
     comand = 128;
     if(comand == 145)
     comand = 144;}
     
      //********************switch rec_ok note 96******************
      //*****************rec on*********************************
      if(Data2 == 96 && rec_ok == 0){
       comand = 128;              
      rec_ok = 1;
      st_rec = 1;
      //indxmid = 0;
      i_count = 0.;
      x_count = 1.; 
     // Serial.println(i_count);
      }
           
      if(rec_ok == 1)
     note.record(indxmid,Data2,Data3,i_count,flag_split,st_rec,rec_ok,comand_nt,comand,play_ok,i_clignt);
           
      }
      
      //Serial.println(comand);
   //****************************control********************   
       if(comand > 159){
    //*********************flag split*************** 
      if(Data2 == 19 && Data3 > 0 && switch_splt == 0){
      flag_split = 1;
      Data2 = 0;//on
      switch_splt = 1; 
      Serial.print(switch_splt);
      }
      if(Data2 == 19 && Data3 > 0 && switch_splt == 1){
      flag_split = 0; //on
      switch_splt = 0;
      Data2 = 0;
      Serial.print(switch_splt); 
      }
  //********************stop***************************** 
  
       if(Data2 == 14 && Data3 > 0){
        for(int i = 1;i< 17; i++){
         Serial1.write(175+i);
         Serial1.write(123);
         Serial1.write(0);
        }
       play_ok = 0;
      rec_ok = 0;
      Data2 = 200;
      //Serial.println("ok");
     } 
     if(Data2 == 10 && Data3 > 0){
      Data3 = 0;
      //trsf_data_sd();
     }
     //********************play*************************
     if(Data2 == 15 && Data3 > 0){
       play_ok = 1;
       i_count = 0.;
       //indxmid = 0;
       
      // play();
       Data2 = 200;
       }
    //**************************************************     
    //**********************level****************************     
     if(Data2 == 1) {
      Data2 = 7; //level
      comand = (comand_nt - 143) + 175;
      }
    //************************reverb*************************  
      if(Data2 == 2){
      Data2 = 91;//reverb
      comand = (comand_nt - 143) + 175;
      }
     //************************instr chang******************** 
     else  if(Data2 == 11){
      comand = (comand_nt - 143) + 191;
      prgm_chg_instm(); //instr chang decr
      return; 
       } 
      else   if(Data2 == 12){
       comand = (comand_nt - 143) + 191; 
      prgm_chg_instp(); //instr chang incr
      return; 
       } 
   //*********************send control************************    
      Serial1.write(comand);//2 bytes only to prgm chang******
      Serial1.write(Data2);//***********************************
      
      if(comand != 192 && comand > 0)// if not prg chang send third byte
      Serial1.write(Data3);
      //***********************************************************
      }
          
     
    }
  }
}

//***********************************functions*****************************o

  
  void prgm_chg_instm() {
 /*  if(comand == 193) {
  Serial1.write(177);
  Serial1.write(0);
  Serial1.write(11);
  if(inst_2 > 1 )
  inst_2 -=1;
  inst = inst_2;  
   }  
  if(comand == 192) {
  Serial1.write(176);
  Serial1.write(0);
  Serial1.write(0); 
  if(inst_1 > 1 )
  inst_1 -=1;
  inst = inst_1;   
  }*/
  
  if(Data3 != 0){  
  if(inst > 1)
  inst -=1;  
  Serial1.write(comand);
  Serial1.write(inst);
  }
  
  }
  
   
  void prgm_chg_instp() {
 /* if(comand == 193) {
  Serial1.write(177);
  Serial1.write(0);
  Serial1.write(11);
  if(inst_2 < 128 )
  inst_2 +=1;  
  inst = inst_2;  
  }
  
  if(comand == 192 ){
  Serial1.write(176);
  Serial1.write(0);
  Serial1.write(0);
  if(inst_1 < 128 )
  inst_1 +=1;  
  inst = inst_1;  
  }*/
    
  if(Data3 !=0){  
  if(inst < 128 )
  inst +=1;  
  Serial1.write(comand);
  Serial1.write(inst);
  }
  
  }
 
    
  /*void play() {
   
  if(indxmid < 10000 ){ 
  if(i_count >= TimerMid[indxmid]){
    
  if(data2mid[indxmid] == 98){ //rebouclage   
  i_count = 0.;
indxmid = 0;
return;
} 
 
 Serial1.write(comandmid[indxmid]);
  
 Serial1.write(data2mid[indxmid]);
Serial1.write(data3mid[indxmid]);


indxmid +=1;
}
}  
  return;
 }  */
 
 void split_kb(){
   
   if(comand == 128 && Data2 < 55)
     comand = 129; 
      //if (Data2 < 55)
     if(comand == 144 && Data2 < 55)
     comand = 145;
     
      if(comand == 128 && Data2 > 55)
     comand = 128; 
      //if (Data2 < 55)
     if(comand == 144 && Data2 > 55)
     comand = 144;
 return;
 }
 /*
void trsf_data_sd() {
  int h = sizeof(TimerMid);
 char linec [h];   
  // Initialize the SD.
  if (!sd.begin(SD_CONFIG)) {
    sd.initErrorHalt(&Serial);
    return;
  }
  // Remove any existing file.
  if (sd.exists("ReadCsvDemo.csv")) {
    sd.remove("ReadCsvDemo.csv"); 
  }
  // Create the file.
  if (!file.open("ReadCsvDemo.csv", FILE_WRITE)) {
    error("open failed");
    exit;
  }
  file.rewind();
 //************************transfert debute ici****************
 //******************tab > sd card***************************** 
   for (int i=0;i<sizeof(TimerMid)/sizeof(TimerMid[0]);i++) {
    file.println(TimerMid[i]);//timer
    delay(2);
    file.println(comandmid[i]);//timer
    delay(2);
    file.println(data2mid[i]);//timer
    delay(2);
    file.println(data3mid[i]);//timer
    if(TimerMid[i+1]== 0. ){
    file.println("f");//timer
    break;
    } 
   }

  /*  for (int i=0;i<sizeof(comandmid)/sizeof(comandmid[0]);i++) {
    file.println(comandmid[i]);//statut
    delay(5);
    if(comandmid[i+1]== 0. ){
    file.println("f");//timer
    break;
    } 
   }

    for (int i=0;i<sizeof(data2mid)/sizeof(data2mid[0]);i++) {
    file.println(data2mid[i]);//note
    delay(5);
    if(data2mid[i+1]== 0. ){
    file.println("f");//timer
    break;
    } 
   }

    for (int i=0;i<sizeof(data3mid)/sizeof(data3mid[0]);i++) {
    file.println(data3mid[i]);//velocity
    delay(5);
    if(comandmid[i+1]== 0. ){
    file.println("f");//timer
    break;
    } 
   }*/
   
 //*************************************************************      
 /* file.rewind();//back file start
  //*********************************
  int j=0;
  
  while (file.available()) {
    j++;
  //***********sd card > tab******************************** 
   linec[j] = file.read();    
   Serial.print(linec[j]);
   if(linec[j-1]> 0 && linec[j] == 0)
   exit;
  }
  //******************************************************
  file.close();
  Serial.println(F("Done"));
  return;
}*/


 
