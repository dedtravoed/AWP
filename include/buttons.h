#pragma once
#include <Arduino.h>
#include "menu.h"
#include "pins.h"



int actualIndex = 1;
int actualParent = 0;
bool editFl, fucnFl = 0;


#define WATERMOISTURE 260 // reading from arduino uno,
#define DRYMOISTURE 722   // 5v regulator

void butt_init()
{
    pinMode(up_butt, INPUT_PULLUP);
    pinMode(down_butt, INPUT_PULLUP);
    pinMode(ok_butt, INPUT_PULLUP);
    pinMode(back_butt, INPUT_PULLUP);
}



bool ok_butt_read() // ok
{
    if (digitalRead(ok_butt) == LOW)
    {
        delay(200);

        if (menus[actualIndex].isParam == 0 && menus[actualIndex].isFunc == 0 && menus[actualIndex].id == menus[actualIndex + 1].parentid)
        {
            actualParent = actualIndex;
            actualIndex++;
        }
        else if (menus[actualIndex].isParam == 1)
        {
            editFl = 1;
            //lcd_print_pointer();
        }
        else if (menus[actualIndex].isFunc == 1)
        {
            fucnFl = 1;
            //funcSwitch();
        }
        //debug_print();
        //Serial.printf("edit_fl %d ", editFl);
        return 1;
    }
    return 0;
}

bool up_butt_read() //вверх
{
    if (digitalRead(up_butt) == LOW)
    {
        delay(100);
        if(fucnFl == 0){
        if (actualIndex != 0)
        {

            if (editFl == 0)
            {
                if (actualParent == menus[actualIndex - 1].parentid)
                    actualIndex--;
                else
                {
                    int i;
                    i = actualIndex;
                    do
                    {
                        i--;
                        if (i == 0)
                            i = menuArraySize;
                    } while (actualParent != menus[i].parentid);
                    actualIndex = i;
                }
            }
            else
            {
                if (menus[actualIndex].value > menus[actualIndex]._min)
                {
                    menus[actualIndex].value--;
                }
            }

        }
        //Serial.println("up_butt_read");
        return 1;
    }
    }
    return 0;
}

bool down_butt_read() //вниз
{
    if (digitalRead(down_butt) == LOW)
    {
        delay(100);
        if(fucnFl == 0){
        if (actualIndex != 0)
        {
            if (editFl == 0)
            {
                if (actualParent == menus[actualIndex + 1].parentid)
                    actualIndex++;
                else
                {
                    int i;
                    i = actualIndex;
                    do
                    {
                        i++;
                        if (i >= menuArraySize)
                            i = 0;
                    } while (actualParent != menus[i].parentid);
                    actualIndex = i;
                }
            }
            else
            {
                if (menus[actualIndex].value < menus[actualIndex]._max)
                {
                    menus[actualIndex].value++;
                }
            }
        }
        //Serial.println("down_butt_read");
        return 1;
    }
    }
    return 0;
}

bool back_butt_read() //назад
{
    if (digitalRead(back_butt) == LOW)
    {
        delay(300);
        if(fucnFl == 0){
        if (actualParent > -1)
        {
            if (menus[actualIndex].isParam == false)
            {
                actualIndex = actualParent;
                actualParent = menus[actualIndex].parentid;
                if(actualParent >= 0){
                }
                if (actualIndex == 0){};
            }
            else
            {
                if (editFl == 0)
                {
                    actualIndex = actualParent;
                    actualParent = menus[actualIndex].parentid;
                }
                if (editFl == 1)
                {
                    editFl = 0;
                }
            }
        }
        } else fucnFl = 0;
        return 1;
    }
    return 0;
}

int butt_read (){
if  (ok_butt_read()) return 1;
if (up_butt_read()) return 2;
if (down_butt_read()) return 3;
if (back_butt_read()) return 4;
return 0;
}