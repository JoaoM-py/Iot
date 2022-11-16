#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

const char* ssid = "VIRUS-2.4GHz";
const char* password = "mateira13";

String serverName = "http://postman-echo.com/";

char* ntpServer = "pool.ntp.org";
long gmtOffset_sec = 0;
int daylightOffset_sec = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

struct tm timeInfo;

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi with IP Address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    if(!getLocalTime(&timeInfo)){
    Serial.println("Acesso ao ntp falhou");
    return;
  }
 
}

void loop() {
  if ((millis() - lastTime) > timerDelay){
      if(WiFi.status() == WL_CONNECTED && timeInfo.tm_min % 5 == 0) {   
        if(!getLocalTime(&timeInfo)){
          Serial.println("Acesso ao ntp falhou");
        }
      
    // GET
    HTTPClient http;
    String tmp = serverName + "get?temp=21.0&minutes=" + timeInfo.tm_min;
    http.begin(tmp.c_str());
    int httpCode = http.GET();

    if(httpCode > 0) {
      Serial.print("Status code: ");
      Serial.println(httpCode);
      String payload = http.getString();
      Serial.print("Response: ");
      Serial.println(payload);
    } else {
      Serial.println("Erro! Sem conexão...");  
    }
  }
  lastTime = millis();
 }
}
