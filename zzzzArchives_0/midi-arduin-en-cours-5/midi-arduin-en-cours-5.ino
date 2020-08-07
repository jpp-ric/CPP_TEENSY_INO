
//***********midi in out*****************

 

 
#define VEL 1
float TimerMid [500];
float i_count = 0.1;
float plloop = 0.1;
float  x_count = 0.1;
int command_nt = 144;
int st_rec = 0;
int play_ok = 0;
int rec_ok = 0;
int data2 = 0;
int data3 = 0;
int flag = 0;
int inc_val = 0;

int bb = 0;
int inst =1;

unsigned long  indxmid = 0;
int data3mid[500];
int data2mid[500];
int commandmid[500];
int data = 0;
int command = 0;
int Data2 = 60;
int Data3 = 100;
boolean nORv = Data2;

void setup() {
  
  Serial1.begin(31250); // open the serial port for MIDI
  Serial.begin(9600);
  
  Serial1.write(192);
  Serial1.write(0);
  Serial1.write(0);
  
  Serial1.write(196);
  Serial1.write(0);
  Serial1.write(0);
  
   Serial1.write(199);
  Serial1.write(0);
  Serial1.write(0);
  
  Serial1.write(183);
  Serial1.write(7);
  Serial1.write(100);
  
  Serial1.write(176);
  Serial1.write(7);
  Serial1.write(100);
  
  Serial1.write(180);
  Serial1.write(7);
  Serial1.write(100);
}

void loop() {
  
   if(play_ok == 1)
    play();
    
    
  //currentMillis = millis();
  //currentMillis = currentMillis/10;
  if(play_ok == 1 | rec_ok == 1) 
  i_count +=1 * x_count;
  
  if (Serial1.available()) {
  data = Serial1.read();
  
  //*****************************data mid********************************
  //**********************************************************************
    if (data > 253) {
      return;  // other MIDI command
    }
   //*************************statu-mid********************************************** 
    else if (data > 127) {
      command = data;  // Data2 OFF or ON command
           
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
    
      if(command < 160){
      Serial1.write(command);
      Serial1.write(Data2);
      Serial1.write(Data3);
      //********************switch rec_ok note 96******************
      //*****************rec on*********************************
      if(Data2 == 96 && command == 144 && rec_ok == 0){
       command = 128;              
      rec_ok = 1;
      st_rec = 1;
      indxmid = 0;
      i_count = 0.;
      x_count = 1.; 
     // Serial.println(i_count);
      }
           
      if(rec_ok == 1)
      record();
           
      }
      if (command >143 && command < 160)
    command_nt = command;  
      //Serial.println(command);
   //****************************control********************   
       if(command > 159){
  //********************stop***************************** 
  
       if(Data2 == 14 && Data3 > 0){
         Serial1.write(176);
         Serial1.write(123);
         Serial1.write(0);
       play_ok = 0;
      rec_ok = 0;
      Data2 = 200;
      //Serial.println("ok");
     } 
     
     //********************play*************************
     if(Data2 == 15 && Data3 > 0){
       play_ok = 1;
       i_count = 0.;
       indxmid = 0;
       
       play();
       Data2 = 200;
       }
    //**************************************************     
    //**********************level****************************     
     if(Data2 == 1) {
      Data2 = 7; //level
      command = (command_nt - 143) + 175;
      }
    //************************reverb*************************  
      if(Data2 == 2){
      Data2 = 91;//reverb
      command = (command_nt - 143) + 175;
      }
     //************************instr chang******************** 
     else  if(Data2 == 11){
      command = (command_nt - 143) + 191;
      prgm_chg_instm(); //instr chang decr
      return; 
       } 
      else   if(Data2 == 12){
       command = (command_nt - 143) + 191; 
      prgm_chg_instp(); //instr chang incr
      return; 
       } 
   //*********************send control************************    
      Serial1.write(command);//2 bytes only to prgm chang******
      Serial1.write(Data2);//***********************************
      
      if(command != 192)// if not prg chang send third byte
      Serial1.write(Data3);
      //***********************************************************
      }
          
     
    }
  }
}

//***********************************functions*****************************o

  
  void prgm_chg_instm() {
  if(Data3 != 0){  
  if(inst > 1)
  inst -=1;  
  Serial1.write(command);
  Serial1.write(inst);
  }
  
  }
  
   
  void prgm_chg_instp() {
  if(Data3 !=0){  
  if(inst < 128 )
  inst +=1;  
  Serial1.write(command);
  Serial1.write(inst);
  }
  
  }
  
  void record() {
   
  if(indxmid < 200 ){ 
  if(Data2 != 96 ){
   if(st_rec == 1){
    st_rec = 0;
   i_count = 0.;
  } 
    
 TimerMid[indxmid] = i_count;
 commandmid[indxmid] = command; 
 data2mid[indxmid] = Data2;
data3mid[indxmid] = Data3;


if(Data2 == 37 && command == 144  && rec_ok == 1){
  data2mid[indxmid] = 98;
      rec_ok = 0;
      command = 128;
      
      play_ok = 1;
      indxmid = 0;
      i_count = 0.;
      x_count = 1.45; 
      play();           
      }

indxmid +=1;

}
}
    }
    
 void play() {
   
  if(indxmid < 200 ){ 
  if(i_count >= TimerMid[indxmid]){
    
  if(data2mid[indxmid] == 98){    
  i_count = 0.;
indxmid = 0;
return;
}  
 Serial1.write(commandmid[indxmid]);
 
 Serial1.write(data2mid[indxmid]);
Serial1.write(data3mid[indxmid]);


indxmid +=1;
}
}  
  return;
 }  
  
 
