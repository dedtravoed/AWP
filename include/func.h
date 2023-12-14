#pragma once
#include <Arduino.h>
#include "buttons.h"
#include "time.h"
#include <Adafruit_NeoPixel.h>
#include "time.h"
#include "timers.h"
#include <TFT_eSPI.h> 
#include <SPI.h>

#define NUM_ITEMS 3

#define PeriodId 2
#define SaveTimeId 10
#define waterCalibId 12
#define wateringHourId 3
#define wateringMinId 4
#define wateringAmountId 5
#define moistureArraySize 20

bool display_main_scr_fl, telegram_send_fl = 0;




// the number of the LED pin
const int ledPin = 19;  // 16 corresponds to GPIO16
const int freq = 1000;
const int ledChannel = 0;
const int resolution = 10;
 
void lcdPwmSetup(){
    ledcSetup(ledChannel, freq, resolution);
    ledcAttachPin(ledPin, ledChannel);
}
 
void lcdPwmWrite(int dutyCycle){
    if(dutyCycle > 980) dutyCycle = 980;
    ledcWrite(ledChannel, dutyCycle); 
}


//----------------------------------------ultrasonic sensor------------------------------------------start

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

struct tankVolumeUltrasonic
{
    public:
  long duration;
  float distanceCm;
  int trigPin;
  int echoPin;
  float maxVolcm;
  float minVolcm;
  float curVolcm;
  int curVolPercent;

  tankVolumeUltrasonic(int _trigPin, int _echoPin){
    trigPin = _trigPin;
    echoPin = _echoPin;
    ultrasonicSetup();
  }

void ultrasonicSetup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

int ultrasonicReadCm() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;


  
  return distanceCm;
}


void minVolmeasureCm(){
    minVolcm = ultrasonicReadCm();
}

void maxVolmeasureCm(){
    maxVolcm = ultrasonicReadCm();
}

void curVolmeasureCm(){
 for(int i = 5; i > 0; i--){
  float buff;
  buff = ultrasonicReadCm();
  curVolcm = curVolcm + buff;
 }
  curVolcm = curVolcm / 5;
  Serial.print("Distance (cm): ");
  Serial.println(curVolcm);
}

void curVolCmToPercent(){
  curVolPercent = map(curVolcm * 1000, minVolcm * 1000, maxVolcm * 1000, 0, 100000);
  curVolPercent = curVolPercent / 1000;
  Serial.print("curVolPercent: ");
  Serial.println(curVolPercent);
}

};

tankVolumeUltrasonic tankvolume(TRIGPIN, ECHOPIN);


int displayCodes, actionCodes = 0;

void WaterTankSetup(int buttonCodes){
  if(buttonCodes == 0){actionCodes = 0; displayCodes = 0; fucnFl = 0;}
  if(buttonCodes == 1){displayCodes++; actionCodes++;}
  if(actionCodes == 2)tankvolume.minVolmeasureCm();
  if(actionCodes == 3)tankvolume.maxVolmeasureCm();
  if(actionCodes == 4 ) {actionCodes = 0; displayCodes = 0; fucnFl = 0;}
}


//----------------------------------------ultrasonic sensor------------------------------------------end



Adafruit_NeoPixel pixels(1, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

void ledInit(){
  pixels.begin();
  pixels.setBrightness(50);
  pixels.show(); // Initialize all pixels to 'off'
}

void ledOn(){
   pixels.setPixelColor(0, pixels.Color(0, 150, 0));
   pixels.show();  
}

void ledOff(){
   pixels.setPixelColor(0, pixels.Color(0, 0, 0));
   pixels.show();  
}


int moistureData[moistureArraySize] = {};
int moistureWriteIteration = 0;
bool dayCount_fl = 0;
int pompOnInsec;
int secsTo100ml = 10;
int mlSec ;

void pomp_pin_init(){
    pinMode(pompPin, OUTPUT);
      digitalWrite(pompPin, HIGH);

}
void pomp_start(){
  digitalWrite(pompPin, HIGH);
}

void pomp_stop(){
  digitalWrite(pompPin, LOW);
}

struct soilMoisture
{
  private:
    static const int arr_size = 20;
    int port_pin; 

  public:
    int raw_value;
    int airValue = 4095;   
    int waterValue = 2399;  
    int percent_value;
    int raw_value_array[arr_size];
    int percent_value_array[arr_size];

  soilMoisture(int pin){
    port_pin = pin;
  }

  void raw_to_percent(){
   percent_value = map(raw_value, airValue, waterValue, 0, 100);
   if(raw_value > 4095) percent_value = 0;
   if(raw_value < 2399) percent_value = 100;
  }

  void percent_value_rotaion(){
    for(int i = arr_size; i > 0; i --) percent_value_array[i] = percent_value_array[i - 1];
    percent_value_array[0] = percent_value;
  }

void percent_value_array_SerialPrint(){
   for(int i = 0; i < 20; i++){
    Serial.print(percent_value_array[i]);
    Serial.print("% ");
 }
    Serial.print("MOISTURE_VAl %");  // @@Serial_DEBUG@@
    Serial.println(percent_value);   // @@Serial_DEBUG@@  
}


void read_raw(){
  analogReadResolution(12);
raw_value = analogRead(port_pin);
}

void raw_value_rotaion(){
 for(int i = arr_size; i > 0; i --) raw_value_array[i] = raw_value_array[i - 1];
 raw_value_array[0] = raw_value;
}

void raw_value_array_SerialPrint(){
   for(int i = 0; i < 20; i++){
    Serial.print(raw_value_array[i]);
    Serial.print(" ");
 }
    Serial.print("MOISTURE_VAl "); // @@Serial_DEBUG@@
    Serial.println(raw_value);     // @@Serial_DEBUG@@  
}

};


 
soilMoisture soil_moisture(MOISTURE_PIN);




void wateringLogicSerialDebug(){
  Serial.print("watering FL "); Serial.println(watering_fl); 
  Serial.print("pompOnInsec "); Serial.println(pompOnInsec);
  Serial.print("watering_cooldown_fl "); Serial.println(watering_cooldown_fl);
  Serial.print("watering_cooldown_time "); Serial.println(watering_cooldown_time);
  Serial.print("dayCount_fl "); Serial.println(dayCount_fl);
  Serial.print("tm.Hour "); Serial.println(tm.Hour);
  Serial.print("tm.Minute "); Serial.println(tm.Minute);
  Serial.print("actualPeriod "); Serial.println(actualPeriod);
  Serial.print("wateringDone_fl "); Serial.println(wateringDone_fl);
  Serial.println(" ");
}


void pompTimeCalc(){
mlSec = 100 / (secsTo100ml / 10);         // ml / sec 
pompOnInsec = menus[wateringAmountId].value * 10 / mlSec; // sec to our amount from menu
}

void pompTimeCalcStartStop(){
  waterinCoefCal_fl = !waterinCoefCal_fl;
  if(waterinCoefCal_fl == 1){secsTo100ml = 0; pomp_start(); ledOn();}
  else {pomp_stop(); pompTimeCalc(); ledOff();}
}

void pompTimeCalcInc(){
  if(waterinCoefCal_fl == 1){ 
    secsTo100ml ++; 
} 
}


void actualPeriodWrite(){
  actualPeriod = menus[PeriodId].value;
}

void wateringDayDecrement(){
 if(tm.Hour == 0 && tm.Minute == 0 && dayCount_fl == 0) {actualPeriod--; dayCount_fl = 1;}
}

void resetDayCount_fl(){
  if(dayCount_fl == 1 && tm.Hour == 0 && tm.Minute != 0) dayCount_fl = 0;
}

void wateringCheck(){
if(watering_fl == 0 && watering_cooldown_fl == 0){
 if(actualPeriod == 0 && menus[wateringHourId].value == tm.Hour && menus[wateringMinId].value == tm.Minute){
  pompTimeCalc();
  pomp_start();
  ledOn();
  Serial.println("watering check done");  // @@Serial_DEBUG@@
  watering_fl = 1;
  watering_cooldown_fl = 1;
  watering_cooldown_time = WATERINGCOOLDOWNTIME;
 }
 }
 if(wateringDone_fl == 1) {pomp_stop(); ledOff(); wateringDone_fl = 0; actualPeriodWrite();}
}

void wateringProcess(){
 if (watering_cooldown_fl == 1 && watering_cooldown_time > 0) watering_cooldown_time --;
  else watering_cooldown_fl = 0;
   if(watering_fl == 1 && pompOnInsec > 0){ 
   pompOnInsec --; 
   if(pompOnInsec == 0) {wateringDone_fl = 1; watering_fl = 0;}
   }
}

int timerScaler = 10;

void timer_handler(){            // this func 
if(timer_fl == 1){               // flag set every second from hardware timer interrupt !!!!AUTOMATICLY!!! TIMER ALWAYS WORK 1 SEC TICK  
 //moisureSensorRead();
  timerScaler--;
  if (timerScaler <= 0){
  /*tankvolume.curVolmeasureCm();
  tankvolume.curVolCmToPercent();*/
  timerScaler = 10;
  }
 pompTimeCalcInc();
 wateringProcess();
  soil_moisture.read_raw();
  soil_moisture.raw_to_percent();
  soil_moisture.percent_value_rotaion();
  Serial.print("raw_value: ");
  Serial.println(soil_moisture.raw_value);
  Serial.print("percent_value: ");
  Serial.println(soil_moisture.percent_value);
  display_main_scr_fl = 1;
  telegram_send_fl = 1;
  if(menus[actualIndex].id == 0){ 
   wateringLogicSerialDebug();  
  }
  analogReadResolution(10);
  int lightSensorVal = analogRead(33);
  Serial.print("lightSensorVal: ");
  Serial.println(lightSensorVal);
  lightSensorVal = map(lightSensorVal, 0, 1024, 1024, 0);
  Serial.print("mapped lightSensorVal: ");
  Serial.println(lightSensorVal);
  lcdPwmWrite(lightSensorVal);
}
  timer_fl = 0;
}

//func Handler, locate in main loop
void loopfuncHandler(){
   wateringDayDecrement();
   resetDayCount_fl();
   wateringCheck();
  
   if(actualIndex == PeriodId && editFl == 1) {
     actualPeriodWrite();
   }

   if(fucnFl == 1){
    if(actualIndex == SaveTimeId) {setTimeFromMenu(); fucnFl = 0;}
    //if(actualIndex == waterCalibId){pompTimeCalcStartStop(); calFuncPrint(); fucnFl = 0;}
    //if(actualIndex == 13){moistureGrafDraw(0); fucnFl = 0;}
   }
}

void do_shit(int buttonCode){
 if(fucnFl == 1){
  if(actualIndex == waterCalibId)pompTimeCalcStartStop();
  if(actualIndex == 13)WaterTankSetup(buttonCode);
}
}