#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TimeLib.h>
#include "func.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include "webpage.h"


// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6592402942:AAGrdhfDTdIc9P-ccxM8u8CAG2vVFjBctfk"

// Wifi network station credentials
int MAX_SSID_LENGTH, MAX_PASS_LENGTH = 30;


bool AP_mode_fl = 1;
bool STA_mode_fl = 0;
bool telegram_handler_fl = 0;


WebServer server(80);


const unsigned long BOT_MTBS = 1000; // mean time between scan messages

WiFiClientSecure secured_client;

UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          // last time messages' scan has been done
bool Start = false;

void handleNewMessages(int numNewMessages)
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    if (text == "/send_test_action")
    {
      bot.sendChatAction(chat_id, "typing");
      delay(4000);
      bot.sendMessage(chat_id, "Did you see the action message?");
    }

    if (text == "/start")
    {
      String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
      welcome += "This is Chat Action Bot example.\n\n";
      welcome += "/send_test_action : to send test chat action message\n";
      bot.sendMessage(chat_id, welcome);
    }
  }
}

void setup_telegram_bot()
{
  Serial.println();
  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

}




AsyncWebServer server2(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "AWP";
const char* password = "12345678";

const char* PARAM_INPUT_1 = "Wifi name";
const char* PARAM_INPUT_2 = "Wifi pass";
const char* PARAM_INPUT_3 = "tCHAT_ID";
const char* PARAM_INPUT_4 = "Connect";


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void wifiAPsetup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_AP);  
  WiFi.softAP(ssid, password);
  delay(1000);
  IPAddress IP = IPAddress (10, 10, 10, 1);
  IPAddress NMask = IPAddress (255, 255, 255, 0);
  WiFi.softAPConfig(IP, IP, NMask);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Send web page with input fields to client
  server2.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

    // Send web page with input fields to client
  server2.on("/page1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html1);
  });

    server2.on("/page2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html2);
  });

    server2.on("/page3", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html3);
  });

    server2.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", style_css);
  });


  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server2.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      myString1 = inputMessage;
      Serial.println(WIFI_SSID);
      request->send_P(200, "text/html", index_html2);
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      myString2 = inputMessage;
      Serial.println(WIFI_PASSWORD);
      request->send_P(200, "text/html", index_html3);
    }
    else if (request->hasParam(PARAM_INPUT_3)) {
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      CHAT_ID = inputMessage;
      Serial.println(CHAT_ID);
     request->send(200, "text/html", index_html4(myString1, myString2, CHAT_ID));
   
    }



    else if (request->hasParam(PARAM_INPUT_4)) {
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
      server2.end();
      WiFi.disconnect();
      STA_mode_fl = 1;
      Serial.println("server 2 end (AP server)"); 
      return;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
  });
  server2.onNotFound(notFound);
  server2.begin();
  Serial.println("server 2 begin (AP server)"); 
}



bool notification_send_fl = 0;

void loop_telegram_bot_handler()
{
    //int numNewMessages;

    if(menus[18].value == 1 and menus[19].value == tm.Hour and notification_send_fl == 0){  // check if telegram notifications on and if its time
      String t_message ("влажность почвы ");
      t_message = t_message + soil_moisture.percent_value + " %"; 
      Serial.println(t_message);
      bot.sendMessage(CHAT_ID, t_message , "");
      notification_send_fl = 1;
    }

    if(menus[19].value > tm.Hour) notification_send_fl = 0;
    /*if(telegram_check_fl == 1){ 
    numNewMessages = bot.getUpdates(bot.last_message_received + 1); 
    telegram_check_fl = 0; 
    Serial.println("@@@@telegram_check_fl=1!!!!");}

    while (numNewMessages)
    {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }*/
 /*if(telegram_send_fl== 1){
   String t_message ("влажность почвы ");
   t_message = t_message + soil_moisture.percent_value; 
   Serial.println(t_message);
   bot.sendMessage(CHAT_ID, t_message , "");
 }*/
}

void network_handler(){

  if(AP_mode_fl) wifiAPsetup(); AP_mode_fl = 0;
  if(STA_mode_fl) setup_telegram_bot(); STA_mode_fl = 0; telegram_handler_fl = 1;
  if(telegram_handler_fl) loop_telegram_bot_handler();

}