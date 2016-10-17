
#include "RTClib.h"
#include <Time.h>

RTC_DS1307 RTC;

long last_millis = millis();
String sync_message = "nothing";
String inString = "";

void setup(void) {
  Serial.begin(115200);   
  RTC.begin();
}

void loop(void) {
  last_millis = millis();
  if(Serial.available()) {
    readSerial();
  }
 
  long diff = elapsed_time();
  //Serial.println(elapsed_time());
  delay(1000 - diff);  
}

void readSerial() {
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char
      // and add it to the string:
      inString += (char)inChar;
    }
    if (inChar == '\n') {
      char charBuf[50];
      inString.toCharArray(charBuf, 50);
      long Long_Integer = atol(charBuf);
      DateTime FigureDate = DateTime(Long_Integer);
      RTC.adjust(FigureDate);
      Serial.println(date_string(FigureDate)); 
    }
  }
}
  
long elapsed_time() {
  long current_time = millis();
  long diff_time = current_time - last_millis;
  return diff_time;
}
  
char * time_string(DateTime time) {
    static char sendBuffer[9];
    snprintf(sendBuffer, 9, "%02d:%02d:%02d", time.hour(), time.minute(), time.second()); 
    return sendBuffer;
}

char * date_string(DateTime time) {
    static char sendBuffer[11];
    snprintf(sendBuffer, 11, "%02d-%02d-%04d", time.month(), time.day(), time.year()); 
    return sendBuffer;
}

void drawString(char* text, int x, int y) {

  return;
  
}
