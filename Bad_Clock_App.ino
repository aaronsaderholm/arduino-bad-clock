
#include "U8glib.h"
#include "RTClib.h"
#include <Time.h>



#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


RTC_DS1307 RTC;
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C / TWI 
long last_millis = millis();

String sync_message = "nothing";

void setup(void) {
  Serial.begin(115200);   
  RTC.begin();
   
  // Check if the RTC is running.
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running");
  }
 
  // This section grabs the current datetime and compares it to 
  // the compilation time.  If necessary, the RTC is updated.
  DateTime now = RTC.now();
  DateTime compiled = DateTime(__DATE__, __TIME__);
  if (now.unixtime() < compiled.unixtime()) {
    Serial.println("RTC is older than compile time! Updating");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
   
  Serial.println("Setup complete.");
}

void loop(void) {
  last_millis = millis();
  processSyncMessage(); 
  u8g.firstPage();  
  do {
    drawString(time(RTC), 4, 20);
    drawString(date(RTC), 4, 40);
    drawString(sync_message, 4, 60);   
  } while( u8g.nextPage() );

  long diff = elapsed_time();
  Serial.println(elapsed_time());
  delay(1000 - diff);  
}

void processSyncMessage() {

  while(Serial.available()) {
    char c = Serial.read() ; 
    Serial.print(c);  
  }

  
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

      Serial.println(pctime);
      

      sync_message = String(pctime);
    }    
  }
} 
  
long elapsed_time() {
  long current_time = millis();
  long diff_time = current_time - last_millis;
  return diff_time;
}
  
String time(RTC_DS1307 RTC) {
    DateTime now = RTC.now();
    String string = "";
    string += charToString(now.hour());
    string += ':';
    string += charToString(now.minute());
    string += ':';
    string += charToString(now.second());
    return string;
}

String date(RTC_DS1307 RTC) {
    DateTime now = RTC.now();
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

void drawString(String text, int x, int y) {
  int str_len = text.length() + 1; 
  char char_array[str_len];
  text.toCharArray(char_array, str_len);
  // graphic commands to redraw the complete screen should be placed here  
  u8g.setFont(u8g_font_unifont);
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( x, y, char_array);
}

