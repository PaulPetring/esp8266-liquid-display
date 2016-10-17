/* 
 LCD    -> ESP8266 12-Q
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

const char* ssid = "your-ssid";
const char* password = "your-password";

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(4, 5, 13, 12, 14, 16);

WiFiServer server(80);

void showText(String supposed, int line)
{
  lcd.setCursor(0, line);
  lcd.print("                ");
  lcd.setCursor(0, line);
  lcd.print(supposed);
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
  
  
  showText(WiFi.localIP().toString(),0);  
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data

  while(!client.available()){
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  showText(req.substring(5),1);  
 
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n THX </html>";

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
  
}
