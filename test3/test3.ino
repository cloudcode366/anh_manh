#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"

//#define BLYNK_TEMPLATE_ID "TMPLfMVeDNKk"
//#define BLYNK_DEVICE_NAME "HE THONG TUOI TIEU TU DONG"
//#define BLYNK_AUTH_TOKEN "F9D4hjfXf8DBYz_LhsrJFbLfaY2MQwXj"
#define BLYNK_TEMPLATE_ID "TMPL1TNyDFqx"
//#define BLYNK_DEVICE_NAME "HE THONG TUOI TIEU TU DONG"
//#define BLYNK_AUTH_TOKEN "r2ZO_oyKHHEuzob3Y0S2B__VNcsuhKPf"

#include <ESP8266WiFi.h>
#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>
SoftwareSerial esp(4, 5);
BlynkTimer timer;
// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "Manhbtpp";
const char* pass = "12341234";

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "r2ZO_oyKHHEuzob3Y0S2B__VNcsuhKPf";

WiFiClient wifiClient;

// This function tries to connect to the cloud using TCP
bool connectBlynk()
{
  wifiClient.stop();
  return wifiClient.connect(BLYNK_DEFAULT_DOMAIN, BLYNK_DEFAULT_PORT);
}

// This function tries to connect to your WiFi network
void connectWiFi()
{
  Serial.print("Connecting to ");
  Serial.println(ssid);

  if (pass && strlen(pass)) {
    WiFi.begin((char*)ssid, (char*)pass);
  } else {
    WiFi.begin((char*)ssid);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
int data[2];
void myTimerEvent()
{
  Blynk.virtualWrite(V2, data[0]);
  Blynk.virtualWrite(V3, data[1]);
  
}
BLYNK_CONNECTED() {
    Blynk.syncAll();
}
int buttonMode=0;
int buttonPump=0;
BLYNK_WRITE(V1){
   buttonMode = param.asInt();
//  Serial.print("button v1:");
//  Serial.println(i);
 Serial.print("v1:");
  Serial.println(buttonMode);
  if(buttonMode==1){
    Blynk.setProperty(V0, "isDisabled", false);
  }
  else {
    Blynk.setProperty(V0, "isDisabled", true);
  }
}
BLYNK_WRITE(V0){
  buttonPump = param.asInt();
  Serial.print("v0:");
  Serial.println(buttonPump);
}
void getData(int data[2]) {
  String dataRaw="";
  String tmp = "";
    while(esp.available()>0){
      char c;
      c=esp.read();
      dataRaw+=c;
      delay(5);
    }
  for (int i = 0 ; i < dataRaw.length(); i++) {
    if ((dataRaw.charAt(i) != ',')) {
      tmp += dataRaw.charAt(i);
    }
    else {
      data[0] = tmp.toInt();
      tmp = "";
      for (int j = i + 1; j < dataRaw.length(); j++) {
        tmp += dataRaw.charAt(j);
      }
      data[1] = tmp.toInt();
      break;
    }
  }
}
void setup()
{
  // Debug console
  Serial.begin(9600);
  esp.begin(9600);
  connectWiFi();

  connectBlynk();

  Blynk.begin(wifiClient, auth);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  // Reconnect WiFi
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
    return;
  }
  String dataRaw = String(buttonMode)+","+String(buttonPump);
//  Serial.println(dataRaw);
  char buff[4];
  dataRaw.toCharArray(buff,sizeof(buff));
  esp.write(buff,sizeof(buff));
  // Reconnect to Blynk Cloud     
  if (!wifiClient.connected()) {
    connectBlynk();
    return;
  }
  if(esp.available()){
  getData(data);
  }
  Blynk.run();
  timer.run();
  delay(100);
}
