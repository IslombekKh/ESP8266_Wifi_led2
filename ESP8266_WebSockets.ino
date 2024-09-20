#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);
WebSocketsServer webSocket(81);

const char html_template[] PROGMEM = R"=====( 
<html lang="en">
   <head>
      <meta charset="utf-8">
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>Potentiometer Value</title>
      <script>
        var socket;
        window.onload = function() {
            socket = new WebSocket("ws://" + window.location.hostname + ":81/");
            socket.onmessage = function(e) {  
              var values = e.data.split(",");
              document.getElementById("sensor_value").innerHTML = values[0];
              document.getElementById("sensor_value2").innerHTML = values[1];
            };
        };
      </script>
   </head>
   <body style="text-align:center; font-family:Arial;">
      <h1>Potentiometer Values</h1>
      <p id="sensor_value" style="font-size:50px;">0</p>
      <p id="sensor_value2" style="font-size:50px;">0</p>
   </body>
</html>
)=====";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    webSocket.sendTXT(num, "0,0");
  }
}

void setup() {
  Serial.begin(115200);

  // Access Point rejimida ishlash
  WiFi.softAP("Potentiometer_AP", "12345678"); // AP nomi va parol
  IPAddress myIP = WiFi.softAPIP();
  Serial.println("Access Point mode started");
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // WebSocket va HTTP serverlarni ishga tushirish
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  server.on("/", []() {
    server.send_P(200, "text/html", html_template);
  });
  server.begin();
}

void loop() {
  webSocket.loop();
  server.handleClient();

  // Ikkita potentiometr qiymatini o'qish (agar ulangan bo'lsa)
  String value1 = String(analogRead(A0));   // 1-potentiometr
  String value2 = String(digitalRead(D3));  // 2-potentiometr yoki boshqa sensor qiymati
  String values = value1 + "," + value2;

  // Klientga qiymatlarni jo'natish
  webSocket.broadcastTXT(values);
  
  delay(100);
}
