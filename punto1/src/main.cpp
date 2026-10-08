#include <Arduino.h>


#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";

const int ledPin = 2; 

AsyncWebServer server(80);

int display=0;

int leds[7] = {2, 7, 8, 9, 10, 11, 12};

int d7s_c_estados[13][7]={
    {0,0,0,0,1,1,0},
    {1,0,1,1,0,1,1},
    {1,0,0,1,1,1,1},
    {1,1,0,0,1,1,0},
    {0,1,0,1,1,0,1},
    {0,1,1,1,1,1,1},
    {0,0,0,0,1,1,1},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1},
};

String txt="black";
String txt1="XXXX"
int ven = 11;  // an1

 
int ventilador=0;





// pagina
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Control LEDS ESP32</title>
  <style>
    body{ background-color: #ecf0f1;}
    html { font-family: Arial Black; text-align: center; color: #2c3e50 }
    .boton { display: inline-block; background-color: #4CAF50; border: 1px solid; border-color: black; color: white;
           height:60px; width: 180px;t:180px; text-decoration: none; font-size: 24px; margin: 2px; cursor: pointer; border-radius: 15px; }
    .off{ background-color: #c0392b; }
    .low{ background-color: #2980b9; }
    .medium{ background-color: #27ae60; }
    .high{ background-color: #f39c12; }
  </style>
</head>
<body>
  <h1>CONTROL DE VENTILADOR</h1>
  <span>Velocidad Actual: XXXX</span>
  <p>
    <a href="/1"><button class="btn-acceso">permitir</button></a>
  </p>
   <p>
    <a href="/2"><button class="b">Low</button></a>
  </p>
 <p>
    <a href="/3"><button class="boton medium">Medium</button></a>
  </p>
   <p>
    <a href="/4"><button class="boton high">High</button></a>
  </p>
</body>
</html>
)rawliteral";






void setup() {

  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  // conexion al wifi como antes
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 20; // 20 intentos de 500ms = 10 segundos
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    
    String pagina = pagina_template;
    pagina.replace(" __COLOR__",txt);

    request->send(200, "text/html", pagina);
  });


  server.on("/1",HTTP_GET, [](AsyncWebServerRequest *request){
    ventilador=1;
    txt="green";
    
    request->redirect("/");
  });
  server.on("/2",HTTP_GET, [](AsyncWebServerRequest *request){
    ventilador=2;
    txt="yellow";
   request->redirect("/");
  });
    server.on("/3",HTTP_GET, [](AsyncWebServerRequest *request){
    ventilador=3;
    txt="red";
   request->redirect("/");
  });



  server.begin();
}

void loop() {
  switch(ventilador){
    case 1:
      analogWrite(ven,0);
      
      break;
    case 2:
      analogWrite(ven,63);
      
      break;
          case 3:
      analogWrite(ven,127);
      break;
          case 4:
      analogWrite(ven,255);
  }
  
}