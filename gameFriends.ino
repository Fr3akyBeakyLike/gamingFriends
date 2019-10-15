// Libraries
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h> // Using version 5.13.5 

const String endpoint = "http://api.steampowered.com/ISteamUser/GetPlayerSummaries/v0002/?key=";  // Link to your API
const String key = "Your_Key_Here";  // Add your Steam Web API key here
const String steamID = "&steamids=Your_Steam_ID_Here"; // Add your Steam ID here (can be found by editing your profile)

// The data we want to extraxt
struct clientData {
  char personastate[8];
  char steamid[8];
  char personaname[8];
};

// Initialize Wifi connection to the router
char ssid[] = "Your_ID_Here";     // your network SSID (name)
char password[] = "Your_Password_Here"; // your network key

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Printing dots in the serial while waiting for the connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Confirming wifi connection 
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
}

void loop() {

  String url = endpoint + key + steamID;
  
  if (WiFi.status() == WL_CONNECTED) 
  {
    HTTPClient http; //Object of class HTTPClient
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) 
    {
      const size_t bufferSize = JSON_ARRAY_SIZE(1) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(17) + 735;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
 
      int id = root["id"]; 
      const char* personastate = root["response"]["players"]["personastate"]; // Your status (1-online, offline or busy)
      const char* steamid = root["response"]["players"]["steamid"]; // Your steam id
      const char* personaname = root["response"]["players"]["personaname"];  // Your steam username

      Serial.print("Status:");
      Serial.println(personastate);
      Serial.print("SteamID:");
      Serial.println(steamid);
      Serial.print("SteamName:");
      Serial.println(personaname);
    }
    http.end(); //Close connection
  }
  delay(60000);
}
