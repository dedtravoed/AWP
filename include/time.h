#pragma once
#include <Wire.h>
#include "menu.h"
#include <DS1307RTC.h>
#include <TimeLib.h>
#include <Arduino.h>

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

tmElements_t tm;



bool getTime(const char *str)
{
  int Hour, Min, Sec;

  if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;
  tm.Hour = Hour;
  tm.Minute = Min;
  tm.Second = Sec;
  return true;
}

bool getDate(const char *str)
{
  char Month[12];
  int Day, Year;
  uint8_t monthIndex;

  if (sscanf(str, "%s %d %d", Month, &Day, &Year) != 3) return false;
  for (monthIndex = 0; monthIndex < 12; monthIndex++) {
    if (strcmp(Month, monthName[monthIndex]) == 0) break;
  }
  if (monthIndex >= 12) return false;
  tm.Day = Day;
  tm.Month = monthIndex + 1;
  tm.Year = CalendarYrToTm(Year);
  return true;
  /*Serial.println(tm.Day);
  Serial.println(tm.Month);
  Serial.println(tm.Year);*/
}

void setDate()
{
  char Month[12];
  int Day, Year;
  tm.Day = 15;
  tm.Month = 2;
  tm.Year = 2004;
  RTC.write(tm);

  /*Serial.println(tm.Day);
  Serial.println(tm.Month);
  Serial.println(tm.Year);*/
}

bool setTime(uint8_t hour, uint8_t min, uint8_t sec){
  tm.Hour = hour;
  tm.Minute = min;
  tm.Second = sec;
  if (RTC.write(tm)) return true;
}

bool setTimeFromMenu(){
  return setTime(menus[7].value, menus[8].value, menus[9].value);
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    //Serial.write('0');
  }
   // Serial.print(number);
}



void readDate(){

  if (RTC.read(tm)) {
   /* Serial.print("Ok, Time = ");
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print(", Date (D/M/Y) = ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();*/
  } else {
    if (RTC.chipPresent()) {
      /*Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();*/
    } else {
     /* Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();*/
    }
  }
}


