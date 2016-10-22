
/* 
 *  LCD    -> ESP8266 12-Q
 following pinmap http://arduino.esp8266.com/versions/1.6.5-1160-gef26c5f/doc/esp12.png
  --------------------------
  1|VSS| -> USB GND
  2|VDD| -> USB 5V +5v
  3|VO | -> USB GND pin + 1,6k ohm Resistor
  4|RS | -> ESP2866  pin 4
  5|RW | -> ESP2866 GND 
  6|E  | -> ESP2866 pin 5
 11|D4 | -> ESP2866 pin 13
 12|D5 | -> ESP2866 pin 12
 13|D6 | -> ESP2866 pin 14
 14|D7 | -> ESP2866 pin 16
 15|A  | -> GND + no or a below 10k Resistor  to dimm background light (Backlight power)
 16|K  | -> USB GND (Backlight ground)
*/

#include <ESP8266WiFi.h>
#include <LiquidCrystal.h>
#include <ESP8266WebServer.h>

const char* ssid = "your-ssid";
const char* password = "your-passwd"; 

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 13, 12, 14, 16);

ESP8266WebServer server(80);

String Line0 = "";
String Line1 = "";


void showText(String supposed, int line)
{
  if (line==0) {
    Line0 = supposed; }
  else { Line1 = supposed; }
  
  lcd.setCursor(0, line);
  lcd.print("                ");
  lcd.setCursor(0, line);
  lcd.print(supposed);
}


void set() {
  String data = server.arg("data");
  String line = server.arg("line");
  if(line=="1" ) {
    showText(data,1);  
  } else {
    showText(data,0);  
  }
  
  server.send(200, "text/plain", data);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleRoot() {
  server.send(200, "text/plain", Line0 + "\n" + Line1);
}

void setup() {
  lcd.display();
  lcd.begin(16, 2);

  showText("Connecting wifi",0);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    showText("waiting..",1);
  }
  showText("done.",1);

  
  showText("Starting Server",0);
  server.begin();

  server.on("/set", set); 
  server.on("/", handleRoot); 

  server.onNotFound(handleNotFound);
  server.begin();
  
  showText(WiFi.localIP().toString(),0);  
}



void loop() {
  server.handleClient();   
}
