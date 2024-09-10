#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi tarmoq ma'lumotlari
const char* ssid = "WiFi_ismi";       // Wi-Fi ismi (SSID)
const char* password = "WiFi_parol";  // Wi-Fi paroli

ESP8266WebServer server(80);          // Web server 80-portda ishlaydi

const int ledPin = 2;                 // LED ulangan pin

void setup() {
  Serial.begin(115200);               // Serial monitor uchun baud rate

  // LED pinni chiqish pin sifatida o'rnatish
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);          // Dastlab LED o'chirilgan bo'ladi

  // Wi-Fi ga ulanish
  WiFi.begin(ssid, password);         
  Serial.print("Wi-Fi ga ulanish...");
  
  // Wi-Fi ulanishini kutish
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  // Wi-Fi ulandi
  Serial.println("");
  Serial.println("Wi-Fi ga ulandi");
  Serial.println("IP manzil: ");
  Serial.println(WiFi.localIP());     // ESP8266 ning IP manzilini chop etish

  // Veb sahifa uchun marshrutlar o'rnatish
  server.on("/", handleRoot);         // Asosiy sahifa
  server.on("/LED_ON", handleLEDOn);  // LEDni yoqish
  server.on("/LED_OFF", handleLEDOff);// LEDni o'chirish

  server.begin();                     // Web serverni ishga tushirish
  Serial.println("Web server boshlandi.");
}

void loop() {
  server.handleClient();              // Kiruvchi mijozlarni boshqarish
}

// Veb sahifa
void handleRoot() {
  String html = "<html><body><h1>LEDni boshqarish</h1><p><a href=\"/LED_ON\">Yoqish</a></p><p><a href=\"/LED_OFF\">O'chirish</a></p></body></html>";
  server.send(200, "text/html", html); // Veb sahifani ko'rsatish
}

// LEDni yoqish
void handleLEDOn() {
  digitalWrite(ledPin, HIGH);         // LED yoqish
  server.send(200, "text/html", "<h1>LED Yoqildi</h1><p><a href=\"/\">Orqaga</a></p>"); // Javob
}

// LEDni o'chirish
void handleLEDOff() {
  digitalWrite(ledPin, LOW);          // LED o'chirish
  server.send(200, "text/html", "<h1>LED O'chirildi</h1><p><a href=\"/\">Orqaga</a></p>"); // Javob
}
