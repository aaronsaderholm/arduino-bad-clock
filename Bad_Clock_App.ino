
#include "U8glib.h"
#include "RTClib.h"
#include <Time.h>

RTC_DS1307 RTC;

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
long last_millis = millis();
String sync_message = "nothing";
String inString = "";

void setup(void) {
  Serial.begin(115200);   
  RTC.begin();
  DateTime currTime = RTC.now();
  printTime(currTime);

}

void loop(void) {
  last_millis = millis();
  if(Serial.available()) {
    readSerial();
  }

  DateTime currTime = RTC.now();
  printTime(currTime);
  
  u8g.firstPage();  
  do {
    drawString(time_string(currTime), 20, 20);
    drawString(date_string(currTime), 20, 40);
  } while( u8g.nextPage() );

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
      char charBuf[20];
      memset(charBuf, 0, 20);
      inString.toCharArray(charBuf, 20);
      inString = "";
      long Long_Integer = atol(charBuf);
      Serial.println(inString);
      Long_Integer = Long_Integer - 14400;
      Serial.println(Long_Integer);
      DateTime FigureDate = DateTime(Long_Integer);
      RTC.adjust(FigureDate);
      printTime(FigureDate);
    }
  }
}

void printTime(DateTime Time) {
  char charBuf[25];
  snprintf(charBuf, 25, "%s %s", time_string(Time), date_string(Time)); 
  Serial.println(charBuf);
  memset(charBuf, 0, 25);
}
  
long elapsed_time() {
  long current_time = millis();
  long diff_time = current_time - last_millis;
  return diff_time;
}
  
char * time_string(DateTime time) {
   int hour = time.hour(); 
   char meridiem[] = "AM";
   if(hour > 12) {
      sprintf(meridiem, "%s", "PM");
      hour -= 12;
    }
   
    static char sendBuffer[12];
    snprintf(sendBuffer, 12, "%02d:%02d:%02d %s", hour, time.minute(), time.second(), meridiem); 
    return sendBuffer;
}

char * date_string(DateTime time) {
    static char sendBuffer[11];
    snprintf(sendBuffer, 11, "%02d-%02d-%04d", time.month(), time.day(), time.year()); 
    return sendBuffer;
}

void drawString(char* text, int x, int y) {

  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( x, y, text);
}
