#pragma once 
#include <Arduino.h>

String CHAT_ID = "734376278";
String myString1 = "41389";
String myString2 = "lol123LOL";

const char *WIFI_SSID = myString1.c_str();
const char *WIFI_PASSWORD = myString2.c_str();


String stringOne = "Hello String";   
String processor(const String& var)
{

  String html4 = "<!DOCTYPE HTML>";

  if(var == "HELLO_FROM_TEMPLATE")
    return F("Hello world!");
  return String();
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="ru">
<head>
	<title>ESP Input Form</title>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8" name="viewport"  initial-scale=1">
	<link rel="stylesheet" type="text/css" href="style.css">
</head>

<body>
  <h2>Чтобы начать получать уведмления в телеграм</h2>
  <h2>Подключитесь к вашей сети через wifi </h2>
  <h2>И ведите id вашего телеграма </h2>
	<form action="/get">
		<div class="inputbox">
			<div class="buttons">
			<input id="input3" type="button" value="начать" class="button_active"onclick="location.href='http://10.10.10.1/page1';">
    </div>
	</form>
</body>

</html>)rawliteral";

// HTML web page to handle 3 input fields (input1, input2, input3)
const char index_html1[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
	<title>ESP Input Form</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>

<body>
	<h1>Wifi SSID</h1>
	<form action="/get">
		<div class="inputbox">
			<input id="inputfield" type="text" name="Wifi name">
			<div class="buttons">
				<input id="input1" type="button" value="back" class="button_active" onclick="location.href='http://10.10.10.1/';">
				<input id="input2" type="submit" value="next">
    </div>
	</form>
</body>
</html>)rawliteral";

const char index_html2[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
	<title>ESP Input Form</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>

<body>
	<h1>Wifi PASS</h1>
	<form action="/get">
		<div class="inputbox">
			<input id="inputfield" type="text" name="Wifi pass">
			<div class="buttons">
				<input id="input1" type="button" value="back" class="button_active"onclick="location.href='http://10.10.10.1/page1';"  >
				<input id="input2" type="submit" value="next">
    </div>
	</form>
</body>
</html>)rawliteral";

const char index_html3[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>

<head>
	<title>ESP Input Form</title>
	<meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>

<body>
	<h1>Telegram CHATID</h1>
	<form action="/get">
		<div class="inputbox">
			<input id="inputfield" type="text" name="tCHAT_ID">
			<div class="buttons">
				<input id="input1" type="button" value="back" class="button_active" onclick="location.href='http://10.10.10.1/page2';">
				<input id="input2" type="submit" value="next">
    </div>
			</div>
	</form>
</body>
</html>)rawliteral";

String  index_html4(String _myString1, String _myString2, String _CHAT_ID) {
String makestr =  "<!DOCTYPE HTML>\
<html>\
<head>\
	<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\" name=\"viewport\" initial-scale=1\">\
	<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\">\
</head>\
<body>\
	<h1>Confirm & connect </h1>\
	<div class=\"result\">\
		<div>SSID:"+ _myString1 +" </div>\
		<div>PASS:"+ _myString2 +"</div>\
		<div>tCHATID:"+ _CHAT_ID +" </div>\
		<form action=\"/get\">\
		<input id=\"input4\"  type=\"submit\" value=\"подключиться\" name=\"Connect\">\
		<input id=\"input4\" type=\"button\" value=\"на главную\" name=\"Connect\" onclick=\"location.href='http://10.10.10.1/';\">\
		</form>\
	</div>\
</body>\
</html>";
return makestr;
}




const char style_css[] PROGMEM = R"rawliteral(
* {
  margin: 4px;
  padding: 0;
  font-family: 'Times New Roman', Times, serif;

}
h1{
  font-size: 2em;
  padding: 0 .25em;
  color: #999;
}

h2{
  padding: 0 .25em;
  color: #999;
  align-items: center;
  font-size: 2em;
}

body{
  display: flex;
  justify-content: center;
  align-items: center;
  min-height: 10vh;
  flex-direction: column;
  gap: 1px;
  background: #1d2b3a;
}

.result{
margin: 7px;
font-size:  4em;
color: #999;
}

.inputbox input{
margin: 7px;
height: 40px;
margin-bottom: 0px;
padding-left: 10px;
border: solid 1px rgba(255, 255, 255, 0.25);
border-radius: 5px;
outline: none;
color: #fff;
background: #1d2b3a;
font-size:  1.3em;
}

#inputfield{
width: 322px;
}


#input1{
  width: 160px;
  margin: 4px;
  margin-top: 0px;
  background: #1d2b4a;
}

#input2{
  width: 160px;
  margin: 4px;
  margin-top: 0px;
  background: #1d2b4a;
}

#input3{
  width: 450px;
  height: 135px;
  padding-bottom: 6px;
  align-items: center;
  background: #1d2b4a;
  font-size:  6em;
}

#input4{
  width: 45%;
  height: 85px;
  background: #1d2b4a;
  border: solid 1px rgba(255, 255, 255, 0.25);
  border-radius: 5px;
  outline: none;
  font-size: 0.45em;
  color: rgb(255, 255, 255, 0.25);
  margin-top: 20px;
}

.result{
size: 100%;
flex-direction:row;
align-items: right;
font-size:  6em;
padding-left: 50px;
padding-right: 50px;
}
)rawliteral";