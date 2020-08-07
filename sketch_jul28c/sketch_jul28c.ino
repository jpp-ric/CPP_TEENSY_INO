#include <SD.h>
#include <SPI.h>
int chipSelect = BUILTIN_SDCARD;
void setup() {

if (!SD.begin()) Serial.println ("NO LUCK");
else Serial.println ("SD WORKS!");
}
void loop() {
// put your main code here, to run repeatedly:
}
