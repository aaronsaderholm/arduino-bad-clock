
#include "U8glib.h"
#include "RTClib.h"
#include <Wire.h>
#include <Time.h>
//#include <Timezone.h>

#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

RTC_DS1307 rtc;
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 

void draw(String text, int x, int y) {
  int str_len = text.length() + 1; 
  char char_array[str_len];
  text.toCharArray(char_array, str_len);
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( x, y, char_array);
}

String time(RTC_DS1307 rtc) {
    DateTime now = rtc.now();
    String string = "";
    string += charToString(now.hour());
    string += ':';
    string += charToString(now.minute());
    string += ':';
    string += charToString(now.second());
    return string;
}

String date(RTC_DS1307 rtc) {
    DateTime now = rtc.now();
    String string = "";
    string += now.year();
    string += '/';
    string += now.month();
    string += '/';
    string += now.day();
    return string;
}

String charToString(int integer) {
    char str[2];
    sprintf(str, "%02d", integer);
    String randomString(str);
    return randomString;
}


void setup(void) {
  Wire.begin();
  pinMode(8, OUTPUT);
  //upinMode(buttonPin, INPUT_PULLUP);
  Wire.beginTransmission(0x68); // address DS3231
  Wire.write(0x0E); // select register
  Wire.write(0b00011100); // write register bitmap, bit 7 is /EOSC
  Wire.endTransmission();
}

void loop(void) {

    if(Serial.available() ) 
  {
    processSyncMessage();
  }
  
  // picture loop
  u8g.firstPage();  
  do {
    draw(time(rtc), 4, 20);
    draw(date(rtc), 4, 40);
  } while( u8g.nextPage() );

  delay(1000);
  
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while(Serial.available() >=  TIME_MSG_LEN ){  // time message consists of header & 10 ASCII digits
    char c = Serial.read() ; 
    Serial.print(c);  
    if( c == TIME_HEADER ) {       
      time_t pctime = 0;
      for(int i=0; i < TIME_MSG_LEN -1; i++){   
        c = Serial.read();          
        if( c >= '0' && c <= '9'){   
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number    
        }
      }   
      set(pctime);   // Sync Arduino clock to the time received on the serial port
    }  
  }
}

