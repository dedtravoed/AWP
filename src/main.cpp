
#include "func.h"
#include "display.h"
#include "network.h"


void timeInit() {
  bool parse=false;
  bool config=false;

  // get the date and time the compiler was run
  if (getDate(__DATE__) && getTime(__TIME__)) {
    parse = true;
    // and configure the RTC with this info
    if (RTC.write(tm)) {
      config = true;
    }
  }
  while (!Serial) ; // wait for Arduino Serial Monitor
  delay(200);
  if (parse && config) {
    Serial.print("DS1307 configured Time=");
    Serial.print(__TIME__);
    Serial.print(", Date=");
    Serial.println(__DATE__);
  } else if (parse) {
   Serial.println("DS1307 Communication Error :-{");
   Serial.println("Please check your circuitry");
  } else {
    Serial.print("Could not parse info from the compiler, Time=\"");
    Serial.print(__TIME__);
    Serial.print("\", Date=\"");
    Serial.print(__DATE__);
    Serial.println("\"");
  }
}

const int potPin = 34;
int potValue = 0;


void setup(void) {
  Serial.begin(115200);
  butt_init();
  pomp_pin_init();
  pomp_stop();
  display_init();
  display_menu_lvl0();
  timerSetup();
  timeInit();
  pompTimeCalc();
  actualPeriodWrite();
  lcdPwmSetup();
  ledInit();
  //setTimeFromMenu();
  //Serial.println("ota_update_complite!!(main)");
  //setup_telegram_bot();
  lcdPwmWrite(255);
 
}

void loop() {

  if (int i = butt_read() != 0){  // buttons handler
  do_shit(i);              // -
  display_shit();         // -
  delay(100);             // buttons handler 
  }
  loopfuncHandler();  // 
  timer_handler();    // stuff on timer interrupt fl 
  display_handler();  // display something if necessary
  network_handler(); 
  loop_telegram_bot_handler();

  //Serial.println("ota_update_complite!!(loop)");
}