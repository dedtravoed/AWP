#pragma once
#include <Arduino.h>
#include "bitmap.h"
#include "func.h"





TFT_eSPI tft = TFT_eSPI();



void display_init(){

  Serial.print("Hello! ST7735 TFT Test");
  // Use this initializer if you're using a 1.8" TFT
  
  //tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.init(INITR_BLACKTAB);   // initialize a ST7735S chip
  tft.fillScreen(ST7735_WHITE);
  delay(100);
  tft.fillScreen(ST7735_BLUE);
  delay(100);
  tft.fillScreen(ST7735_GREEN);
  delay(100);
  tft.fillScreen(ST7735_RED);
  delay(100);
  Serial.println("Initialized");
  uint16_t time = millis();
  tft.fillScreen(ST7735_WHITE);
  tft.setRotation(1);  
}

void print2digits_tft(int number) {
  if (number >= 0 && number < 10) {
    tft.print('0');
  }
  tft.print(number);
}

void display_grid(){
  int space = 20; //space 20 px
  int yres = 128; //px
  int xres = 160; //px
  int numYlines =  yres / space;
  int numXlines =  xres / space;
  tft.setTextSize(0);

  for(int i = space; i < xres;){
    tft.drawFastVLine(i, 0, yres, ST7735_BLUE);
    tft.setCursor(i + 2, 0);
    tft.print(i);
    i = i + space; 
  }

  
  for(int i = space; i < yres;){
    tft.drawFastHLine(0, i, xres, ST7735_BLUE);
    tft.setCursor(0, i + 2);
    tft.print(i);
    i = i + space; 
  }
}
/*-------------------------------------------------display menu-----------------------------------------------------------start*/
struct tftDisplay
{
  int refresh_conter, previos_moisture_val, previos_tank_volume;
  const int refresh_rate = 1000; 

void main_base(){

tft.drawBitmap(0, 12, epd_bitmap__main_screen_plant, 88, 114, ST7735_BLACK);
tft.drawBitmap(110, 65, epd_bitmap__main_screen_leica, 48, 56, ST7735_BLACK);
tft.drawLine(100, 100, 120, 110, TFT_RED);
tft.drawWideLine(60, 100, 80, 110, 0, TFT_BLACK);
tft.fillSmoothRoundRect(70, 110, 31, 11, 4, TFT_BLACK);
//display_grid();
}

void main_refresh(){

tft.setTextColor(ST7735_BLACK, ST7735_WHITE);
tft.setTextSize(2); tft.setCursor(100, 6);
RTC.read(tm);
print2digits_tft(tm.Hour); tft.print(":"); print2digits_tft(tm.Minute);
tft.setTextSize(1); tft.setCursor(80, 26);
tft.print("next watering");
tft.setCursor(90, 34);
tft.printf("in %d days", actualPeriod);
tft.drawWideLine(60, 100, 80, 110, 0, TFT_BLACK);

if(soil_moisture.percent_value > 9 && previos_moisture_val < 10) tft.fillSmoothRoundRect(70, 110, 31, 11, 4, TFT_BLACK);
if(soil_moisture.percent_value < 10 && previos_moisture_val > 9) tft.fillSmoothRoundRect(70, 110, 31, 11, 4, TFT_BLACK);
tft.setCursor(74, 112);
tft.setTextColor(TFT_WHITE, TFT_BLACK);
tft.printf("%d %%", soil_moisture.percent_value);

previos_moisture_val = soil_moisture.percent_value;
}

};

tftDisplay display;



const unsigned char* icon_bitmap_Array[NUM_ITEMS] = {
  epd_bitmap_plant_icon,
	epd_bitmap_clock_icon,
	epd_bitmap_settings_icon
  //epd_bitmap__info_icon
};

char menu_lvl0_items[NUM_ITEMS] [20] = {
	{"Plant setup"},
	{"Time set   "},
	{"Settings   "},
  //{"Info       "},
};

void display_menu_lvl0(){
  int item1, item2, item3;
  item2 = menus[actualIndex].lvlid;
  item1 = item2 - 1;
  if (item1 < 0) item1 = NUM_ITEMS - 1;
  item3 = item2 + 1;
  if (item3 > NUM_ITEMS - 1) item3 = 0;  

  Serial.print("menus[actualIndex].lvlid ");
  Serial.println(menus[actualIndex].lvlid);
  Serial.print("item2 ");
  Serial.println(item2);
  Serial.print("actualIndex ");
  Serial.println(actualIndex);

  tft.setTextColor(ST7735_BLACK, ST7735_WHITE);
  tft.setTextSize(1);
  tft.drawBitmap(8, 6,  icon_bitmap_Array[item1], 26, 26, ST7735_BLACK, ST7735_WHITE);
  tft.drawBitmap(8, 51, icon_bitmap_Array[item2], 26, 26, ST7735_BLACK, ST7735_WHITE);
  tft.drawBitmap(8, 95, icon_bitmap_Array[item3], 26, 26, ST7735_BLACK, ST7735_WHITE);

  tft.setCursor(40, 17);
  tft.print(menu_lvl0_items[item1]);
  tft.setCursor(40, 60);
  tft.print(menu_lvl0_items[item2]);
  tft.setCursor(40, 105);
  tft.print(menu_lvl0_items[item3]);
  tft.drawBitmap(2, 46, epd_bitmap_select, 152, 38, ST7735_BLACK);
}

int previos_cur = 0; // for clear previos displayed cursor 
bool edit_screen_fl = 0;  

const char* edit_sreen_lvl1_items_up[][4] = {
  {"Watering period", "   Watering", "   Watering", "Amount of water"},
  {"  Clock time", "  Clock time", "  Clock time"},
  {"  Clock time", "  Clock time", "  Clock time"},
  {"  Clock time", "  Clock time", "  Clock time"},
  {"Use telegram bot ", "Hour to send", "yx1"}};

const char* edit_sreen_lvl1_items_down[][4] = {
  {"    In days", "     Hour", "    Minute", " In milliliters"},
  {"     Hour", "     Minute", "    Second"},
  {"     Hour", "     Minute", "    Second"},
  {"      as", "as", "as"},
  {"to send notification", "telegram notifications", "yx1"}};



void edit_sreen_lvl1_print(uint8_t itemSwitch){
   if (edit_screen_fl == 0) tft.fillScreen(ST7735_WHITE);
   tft.setTextSize(1);
   tft.setCursor(37, 20);

   tft.print(edit_sreen_lvl1_items_up[itemSwitch][menus[actualIndex].lvlid]);
   tft.setCursor(37, 90);
   tft.print(edit_sreen_lvl1_items_down[itemSwitch][menus[actualIndex].lvlid]);
   tft.drawBitmap(73, 43,  epd_bitmap_inc_dec_arrows, 15, 42, ST7735_BLACK);
      
   if(menus[actualIndex].id != 5){
    if(menus[actualIndex].value < 10) tft.setCursor(76, 57);
     else tft.setCursor(69, 57);
     tft.setTextSize(2);
     tft.setTextColor(ST7735_BLACK);
     tft.fillRect(69, 57, 22, 14, ST7735_WHITE);
     tft.print(menus[actualIndex].value);
     if(menus[actualIndex]._max == 1){
      tft.setTextSize(2);
      tft.fillRect(65, 55, 37, 18, ST7735_WHITE);
      if(menus[actualIndex].value == 1) {tft.setCursor(65, 57); tft.print("yes");}
      if(menus[actualIndex].value == 0) {tft.setCursor(69, 57);tft.print("no");}
     }
  }else{
    tft.setCursor(64, 57);
    tft.setTextSize(2);
    tft.setTextColor(ST7735_BLACK);
    tft.fillRect(64, 57, 36, 14, ST7735_WHITE);
    tft.print(menus[actualIndex].value * 10);
  }
   edit_screen_fl = 1;
}

uint8_t itemSwitch;

void display_menu_lvl1(){
 tft.setTextColor(ST7735_BLACK, ST7735_WHITE);
 uint8_t posY [4] ={15, 37, 59, 81};
 uint8_t posX = 14;


if (editFl == 0){

// clear display if need
if(edit_screen_fl == 1){tft.fillScreen(ST7735_WHITE);  edit_screen_fl = 0;} 

//draw cursor
if (previos_cur == 0){previos_cur = posY[menus[actualIndex].lvlid];} 
 tft.drawBitmap(posX - 12, previos_cur - 9,  epd_bitmap_choose_lvl1, 156, 26, ST7735_WHITE); //delete previos cursor
 tft.drawBitmap(posX - 12, posY[menus[actualIndex].lvlid] - 9,  epd_bitmap_choose_lvl1, 156, 26, ST7735_BLACK); // draw current cursor

//print menu item & value
 if (menus[actualIndex].parentid == 1) itemSwitch = 0;
 if (menus[actualIndex].parentid == 6) itemSwitch = 1;
 if (menus[actualIndex].parentid == 11) itemSwitch = 2;
 if (menus[actualIndex].parentid == 14) itemSwitch = 3;
 if (menus[actualIndex].parentid == 17) itemSwitch = 4;

  int position = 0;
  int indxBuff = actualIndex; 
 
  while(menus[indxBuff].parentid == menus[actualParent].id) indxBuff--;

  if(menus[indxBuff + 1].parentid == actualParent){
  indxBuff++;
  while (menus[indxBuff].parentid == actualParent)
  {
    //Serial.printf("indxBuff = %d \r \n", indxBuff);
    tft.setTextSize(1);
    tft.setCursor(posX, posY[position]);
    tft.print(menus[indxBuff]._name);
    if(menus[indxBuff].isParam == 1){
     tft.print(menus[indxBuff].value);
     //if(menus[actualIndex].id == 5) tft.print(menus[indxBuff].value * 10);
    }
    indxBuff++;
    position++;
  }
  }

}
 previos_cur = posY[menus[actualIndex].lvlid];

 //print edit screen
if (editFl == 1){
edit_sreen_lvl1_print(itemSwitch);
}
}

/*-------------------------------------------------display menu-------------------------------------------------------------end*/

/*-------------------------------------------------display func-----------------------------------------------------------start*/
void calFuncPrint(){
  if(waterinCoefCal_fl == 1){
  tft.fillScreen(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0,10);
  tft.print("     Calibration start,");
  tft.setCursor(0,18);
  tft.print(" when water amount reach");
  tft.setCursor(0,26);
  tft.print("  100ml press ok button");
  
  edit_screen_fl = 1;
} else {
  tft.fillScreen(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(0,10);
  tft.print("    Calibration done,");
  tft.setCursor(0,18);
  tft.setTextColor(ST7735_DARKGREEN);
  tft.print(" Congratulations!");
}
}


void moistureGrafDraw(int size){
tft.fillScreen(ST7735_WHITE);
tft.drawFastHLine(20, 106, 135, ST7735_BLACK);
tft.drawFastVLine(20, 20, 88,  ST7735_BLACK);
tft.setTextSize(1);
tft.setCursor(0, 2);
tft.println("hum");
tft.println(" %");
tft.print("100");
tft.setCursor(2, 60);
tft.println("50");
tft.setCursor(2, 100);
tft.println("0");
tft.setCursor(110, 120);
tft.print("days ago");

int maxMoisture;
int minMoisture;

int nuberOfpoints = 14;
//if(size == 1) nuberOfpoints = 16;
//nuberOfpoints = size;

int xPos [nuberOfpoints] = {25}; 
int ypos [nuberOfpoints] = {26, 29, 33, 40, 41, 45, 49, 54, 66, 78, 83, 95, 98, 90, 100, 26, 52, 66};
//int day = nuberOfpoints;

for(int i = 0; i < nuberOfpoints; i++){
   ypos[i] = map(soil_moisture.percent_value_array[i] , 100, 0, 25, 100);
   //ypos[i] = soil_moisture.percent_value_array[i];
}

int posXspace = (160 - 32) / (nuberOfpoints - 1);

for(int i = 1; i < nuberOfpoints; i++){
  xPos[i] = xPos[i - 1] + posXspace;
}

for(int i = 0; i < nuberOfpoints; i++){
  tft.setCursor(xPos[i] + 2, 108);
  if(i%2 == 1) {
    if (i > 9) tft.setCursor(xPos[i] - 2, 108);
    tft.print(i + 1);
    }
  else tft.fillCircle(xPos[i] + 3, 110, 1, ST7735_BLACK);
  tft.drawFastVLine(xPos[i] + 3, 20, 84, ST7735_LIGHTGREY);
  tft.fillCircle(xPos[i] + 3, ypos[i], 2, ST7735_BLACK);
  //day--;  
 if(i < nuberOfpoints - 1) {
  tft.drawLine(xPos[i] + 2, ypos[i], xPos[i + 1] + 2, ypos[i + 1], ST7735_BLACK);
}
}

}


void waterTankSetupDisplay(){
if(displayCodes == 1){
  tft.fillScreen(ST7735_WHITE);
  tft.print("to setup ultrasonic sensor place your sensor on empty water tank, and press ok");
}
if(displayCodes == 2){
  tft.fillScreen(ST7735_WHITE);
  tft.print("now fill tank with water, left  ~2cm free space distance to the sensor, and press ok");
}
if(displayCodes == 3){
  tft.fillScreen(ST7735_WHITE);
  tft.print("setup done for exit pess back button");
}

}
/*-------------------------------------------------display func-------------------------------------------------------------end*/

int previosParentId; // for clear previos displayed menu 
int previos_funcFL;

void display_shit(){ 
    if (previosParentId != menus[actualIndex].parentid) tft.fillScreen(ST7735_WHITE); // Display clear functions 
    previosParentId = menus[actualIndex].parentid;                                    // Display clear functions      
    if (previos_funcFL != fucnFl) tft.fillScreen(ST7735_WHITE);                       // Display clear functions   
    previos_funcFL = fucnFl;                                                          // Display clear functions 

    switch (menus[actualIndex].parentid)                                              // Menu draw  functions
    {                                                                                 //
    case 0:                                                                           //  
       display_menu_lvl0();                                                           //
       //display_grid();                                                              //
       break;                                                                         //
                                                                                      //
    case 1:                                                                           //
       display_menu_lvl1();                                                           //
       //display_grid();                                                              //
       break;                                                                         //
    case  6:                                                                          //
       display_menu_lvl1();                                                           //
       //display_grid();                                                              //
       break;                                                                         //
    case  11:                                                                         //
       display_menu_lvl1();                                                           //
       break;                                                                         //
    case  14:                                                                         //
       display_menu_lvl1();                                                           //
       break;
    case  15:                                                                         //
       display_menu_lvl1();                                                           //
       break;
    case  18:                                                                         //
       display_menu_lvl1();                                                           //
       break;  
    default:                                                                          //
        break;                                                                        //
    }                                                                                 // Menu draw  functions

    if(fucnFl == 1){                                                                  // Func draw  functions
    if(actualIndex == 14) moistureGrafDraw(10);                                       // Func draw  functions
    if(actualIndex == 12)  calFuncPrint();                                            // Func draw  functions
    if(actualIndex == 13) waterTankSetupDisplay();
     }
}

void display_handler(){ 

    if(menus[actualIndex].id != 0) display.refresh_conter = 0;
    if(menus[actualIndex].id == 0){ 
     if (display_main_scr_fl == 1){
      if( display.refresh_conter == 0) display.main_base();
      display.main_refresh();
      display.refresh_conter ++;
      if(display.refresh_conter >= display.refresh_rate)  display.refresh_conter = 0;
    }
    display_main_scr_fl = 0;
}
}