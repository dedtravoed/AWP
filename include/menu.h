#pragma once
#include <Arduino.h>

struct menu // Структура описывающая менюk
{
  int id;        // Идентификационный уникальный индекс ID
  int parentid;  // ID родителя
  int lvlid;
  bool isParam;  // Является ли пункт изменяемым параметром
  bool isFunc;   // Является ли пункт функцией
  String _name;  // Название
  int value; // Актуальное значение
  int _min;  // Минимально возможное значение параметра
  int _max;  // Максимально возможное значение параметра
};

uint8_t menuArraySize = 20; // Задаем размер массива

menu menus[] = {
    // Задаем пункты меню
    {0, -1, 0, false, false, "Main", 0, 0, 0},
     {1, 0, 0, false, false, "Plant setup", 0, 0, 0},
       {2, 1, 0, true, false, "Period  ", 1, 1, 99},
       {3, 1, 1, true, false, "Hour  ", 0, 0, 23},
       {4, 1, 2, true, false, "Minute  ", 0, 0, 59},
       {5, 1, 3, true, false, "Water amount  ", 15, 0, 100}, // ml ratio * 10
     {6, 0, 1, false, false, "Time set", 0, 0, 0},
       {7, 6, 0, true, false, "Hour  ", 23, 0, 23},
       {8, 6, 1, true, false, "Min  ", 59, 0, 59},
       {9, 6, 2, true, false, "Sec  ", 45, 0, 59},
       {10, 6, 3, false, true, "Save time", 0, 0, 0},
     {11, 0, 2, false, false, "Settings", 0, 0, 0},
       {12, 11, 0, false, true, "Water calibration ", 0, 0, 0},
       {13, 11, 1, false, true, "Water tank setup ", 0, 0, 0},
       {14, 11, 2, false, true, "Debug inf ", 0, 0, 0},
       {15, 11, 3, false, false, "Network ", 0, 0, 0},
          {16, 15, 0, false, true, "WiFi info ", 0, 0, 0},
          {17, 15, 1, true, false, "Change to AP mode ", 0, 0, 1},
          {18, 15, 2, false, false, "Telegram Settings ", 0, 0, 0},
           {19, 18, 0, true, false, "Use telegram bot ", 0, 0, 1},
           {20, 18, 1, true, false, "Notifications time ", 0, 0, 23}
};