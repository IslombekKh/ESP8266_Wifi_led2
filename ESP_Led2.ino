#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// LED o'rnatilgan pin
const int ledPin = 2; // D4 pin (ESP8266 uchun ichki LED)

ESP8266WebServer server(80); // HTTP serverni 80-portda ishga tushirish

// WiFi tarmoq sozlamalari
const char* ssid = "WiFi_SSID"; // WiFi tarmoq nomi
const char* password = "WiFi_password"; // WiFi paroli

void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>LED Boshqaruv</title>";
  html += "<style>";
  html += "body { text-align: center; font-family: Arial; background-color: #f7f7f7;}";
  html += "h1 { color: #333; }";
  html += ".button { background-color: #4CAF50; color: white; padding: 15px 30px; text-align: center; text-decoration: none; display: inline-block; font-size: 20px; margin: 10px; border-radius: 10px; }";
  html += ".button-off { background-color: #f44336; }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<h1>LED Boshqaruv Paneli</h1>";
  html += "<a href=\"/on\"><button class=\"button\">LED ON</button></a>";
  html += "<a href=\"/off\"><button class=\"button button-off\">LED OFF</button></a>";
  html += "</body>";
  html += "</html>";
  
  server.send(200, "text/html", html);
}

void handleLEDOn() {
  digitalWrite(ledPin, LOW); // LEDni yoqish
  handleRoot(); // To'g'ridan-to'g'ri boshqaruv interfeysiga qaytish
}

void handleLEDOff() {
  digitalWrite(ledPin, HIGH); // LEDni o'chirish
  handleRoot(); // To'g'ridan-to'g'ri boshqaruv interfeysiga qaytish
}

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LEDni dastlab o'chirish

  // WiFi tarmog'iga ulanish
  WiFi.begin(ssid, password);
  Serial.println("WiFi tarmog'iga ulanish...");
  
  // WiFi tarmog'iga ulanish tugaguncha kutish
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi tarmog'iga ulandi!");
  Serial.print("IP Manzil: ");
  Serial.println(WiFi.localIP());
  
  // HTTP server so'rovlarini sozlash
  server.on("/", handleRoot);      // Asosiy sahifa
  server.on("/on", handleLEDOn);   // LEDni yoqish
  server.on("/off", handleLEDOff); // LEDni o'chirish
  
  server.begin(); // HTTP serverni ishga tushirish
  Serial.println("HTTP server ishga tushirildi.");
}

void loop() {
  server.handleClient(); // Mijoz so'rovlarini qayta ishlash
}
