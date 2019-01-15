#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>

const char* ssid = "VadimK";
const char* password = "afynjvfc";

ESP8266WebServer server(80);

const int wifiLedPin = 2;
// Declared explicitly because only for wifiLedPin LOW is HIGH and vice versa.
const int wifiLedOn  = 0;
const int wifiLedOff = 1;

const int RECORDS_SIZE = 120; // The last 2 hours.
int records_len = 0; // Current number of records.
int t[RECORDS_SIZE], rh[RECORDS_SIZE]; // To get the real measure, must be "float(t[i])/10.0".

void addRecord(int t_val, int rh_val) {
  if (records_len == RECORDS_SIZE) {
    for (int i = 0; i < records_len - 1; ++i) {
      t[i] = t[i + 1];
      rh[i] = rh[i + 1];
    }
  }
  else {
    ++records_len;
  }
  t[records_len - 1] = t_val;
  rh[records_len - 1] = rh_val;
}

String arrToStr(String label, int n, int *arr) {
  String res = "\"" + label + "\": \"";
  res += String(n) + " ";
  for (int i = 0 ; i < n - 1; ++i) {
    res += String(arr[i]) + " ";
  }
  res += String(arr[n - 1]) + "\"";
  return res;
}

void dangerousBlink() {
  int currentState = digitalRead(wifiLedPin);
  unsigned long cur_millis;
  unsigned long local_delay = 300;
  
  for (int i = 0; i < 10; ++i) {
    currentState = currentState == HIGH ? LOW : HIGH;
    digitalWrite(wifiLedPin, currentState);
    cur_millis = millis();
    while (millis() < cur_millis + local_delay) {}
    currentState = currentState == HIGH ? LOW : HIGH;
    digitalWrite(wifiLedPin, currentState);
    cur_millis = millis();
    while (millis() < cur_millis + local_delay) {}
  }
}

Ticker measurer;

void measure() {
  noInterrupts();
  int cur_temp = random(230, 255);
  int cur_rh = random(550, 675);
  
  if (cur_temp > 250 || cur_rh > 650) {
    dangerousBlink();
  }
  
  addRecord(cur_temp, cur_rh);
  interrupts();
}

void handleRoot() {
  noInterrupts();
  digitalWrite(wifiLedPin, wifiLedOn);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(wifiLedPin, wifiLedOff);
  interrupts();
}

void handleSensors() {
  noInterrupts();
  digitalWrite(wifiLedPin, wifiLedOn);
  String body = "{";
  body += arrToStr("t", records_len, t);
  body += ",";
  body += arrToStr("rh", records_len, rh);
  body += "}";
  server.send(200, "application/json", body);
  digitalWrite(wifiLedPin, wifiLedOff);
  interrupts();
}

void handleNotFound() {
//  while (mutex) {
//    // wait
//  }
//  ++mutex;
  noInterrupts();
  digitalWrite(wifiLedPin, wifiLedOn);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  message += "Received request body:\n";
  if (server.hasArg("plain") == false) {
    message += "Nothing!\n";
  }
  else {
    message += server.arg("plain");
    message += "\n";
  }
  message += "Temperatures:\n";
  message += "\n";
  
  server.send(404, "text/plain", message);
  digitalWrite(wifiLedPin, wifiLedOff);
  interrupts();
  //--mutex;
}

void setup(void) {
  randomSeed(analogRead(0));
  
  Serial.println("setup begin");
  addRecord(206, 608);
  addRecord(210, 610);
  addRecord(222, 600);
  addRecord(235, 595);
  Serial.println("test elems are added");
  
  pinMode(wifiLedPin, OUTPUT);
  digitalWrite(wifiLedPin, wifiLedOff);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/sensors", handleSensors);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  measurer.attach(15, measure);
  Serial.println("Measurer ticker started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
