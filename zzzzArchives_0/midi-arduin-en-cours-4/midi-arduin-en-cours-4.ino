
//***********midi in out*****************

 unsigned long currentMillis = millis();
 float TimerMid [200];
 unsigned long prevmillis = 0;

#define VEL 1
#define POT_1 1
unsigned long i_count = 0;
 unsigned long plloop = 0;
float  x_count = 1;
int play_ok = 0;
int rec_ok = 0;
int data2 = 0;
int data3 = 0;
int flag = 0;
int inc_val = 0;
unsigned i =0; 
int bb = 0;
int inst =1;
int indxmidp = 0;
int indxmid = 0;
int data3mid[200];
int data2mid[200];
int commandmid[200];
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
      // if(command > 159)
      //Contrl_X();//controleS X 
      
      
      
      //contrl_chg();
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
      rec_ok = 1;
      Data2 = 97;
      indxmid = 0;
      i_count = 0;
      x_count = 1; 
     // Serial.println(i_count);
      }
      //************rec off play on**************************
      if(Data2 == 96 && command == 144  && rec_ok == 1){
      rec_ok = 0;
      Data2 = 97;
      play_ok = 1;
      indxmid = 0;
      i_count = 0;
      x_count = 1.8; 
      //plloop = data2mid[indxmid]; 
     // Serial.println(i_count);
      }
      if(rec_ok == 1)
      record();
      
     
      
      
      //*****************************************************
      //Serial.println(i_count);
      
      }
   //****************************control********************   
       if(command > 159){
  //********************stop***************************** 
  //Serial.println(Data2);
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
       i_count = 0;
       indxmid = 0;
       play();
       Data2 = 200;
       }
    //**************************************************     
    //**********************level****************************     
     if(Data2 == 1) 
      Data2 = 7; //level
    //************************reverb*************************  
      if(Data2 == 2)
      Data2 = 91;//reverb
     //************************instr chang******************** 
     else  if(Data2 == 11){
      command = 192;
      prgm_chg_instm(); //instr chang decr
      return; 
       } 
      else   if(Data2 == 12){
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
      
     
      
      
     /* Serial1.write(command+7);
      Serial1.write(Data2);
      Serial1.write(Data3);
      */
     /* Serial1.write(command+4);
      Serial1.write(Data2);
      Serial1.write(Data3);*/
      
      /*Serial.println(command);
      Serial.println(Data2);
      Serial.println(Data3);*/
     
     // Serial.println(currentMillis/10);
     
    }
  }
}

//***********************************functions*****************************o
/*
void datamid(){

if(Data2mid[indxmidp] != 60 ) {
  
if (currentMillis - prevmillis >= TimerMid[indxmidp]) {
Serial1.write(commandmid[indxmidp]);
Serial1.write(Data2mid[indxmidp]);

Serial1.write(velmid[indxmidp]);
 
 currentMillis = millis();
 currentMillis = currentMillis/10;
 prevmillis = currentMillis;
indxmidp +=1;
}

}
return;
}*/

  
  void prgm_chg_instm() {
  if(Data3 != 0){  
  if(inst > 1)
  inst -=1;  
  Serial1.write(192);
  Serial1.write(inst);
  }
  
  }
  
   
  void prgm_chg_instp() {
  if(Data3 !=0){  
  if(inst < 128 )
  inst +=1;  
  Serial1.write(192);
  Serial1.write(inst);
  }
  
  }
  
  void record() {
   
  if(indxmid < 200 ){ 
 TimerMid[indxmid] = i_count;
 commandmid[indxmid] = command; 
 data2mid[indxmid] = Data2;
data3mid[indxmid] = Data3;
indxmid +=1;
plloop = i_count;
}
    }
    
 void play() {
   
  if(indxmid < 200 ){ 
  if(i_count >= TimerMid[indxmid]){
 Serial1.write(commandmid[indxmid]);
 Serial1.write(data2mid[indxmid]);
Serial1.write(data3mid[indxmid]);

if(i_count >= plloop){
i_count = 0;
indxmid = 0;
return;
}

indxmid +=1;
}
}  
  return;
 }  
  
 
