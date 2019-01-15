#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Ticker.h>
#include <time.h>

const char* ssid = "VadimK";
const char* password = "afynjvfc";

ESP8266WebServer server(80);

bool isScheduleLoaded = false;

// Variables below have sense if (isScheduleLoaded == true).
int min_t  = -1;
int max_t  = -1;
int min_rh = -1;
int max_rh = -1;
int scheduleLoadedTime = -1;

const int DOSES_SIZE = 200;
int doses_len = 0;
int doseIds[DOSES_SIZE];    // dose_ids[i] is id of a dose in backend database.
int doseTimers[DOSES_SIZE]; // dose_timers[i] is how much time left before consuming.
bool isDoseConsumed[DOSES_SIZE];
bool isDoseAlerted[DOSES_SIZE];

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

void alertBlink(int localDelay, int repeats) {
  int currentState = digitalRead(wifiLedPin);
  unsigned long cur_millis;
  
  for (int i = 0; i < repeats; ++i) {
    currentState = currentState == HIGH ? LOW : HIGH;
    digitalWrite(wifiLedPin, currentState);
    cur_millis = millis();
    while (millis() < cur_millis + localDelay) {}
    currentState = currentState == HIGH ? LOW : HIGH;
    digitalWrite(wifiLedPin, currentState);
    cur_millis = millis();
    while (millis() < cur_millis + localDelay) {}
  }
}

Ticker measurer;

void measure() {
  noInterrupts();
  int cur_time = time(NULL);
  for (int i = 0; i < doses_len; ++i) {
    if (!isDoseAlerted[i] && cur_time - scheduleLoadedTime > doseTimers[i]) {
      alertBlink(300, 10);
      isDoseAlerted[i] = true;
      isDoseConsumed[i] = random(0, 5) > 0; // 20% of fail simulation
      break;
    }
  }
  
  int cur_temp = random(230, 255);
  int cur_rh = random(575, 626);
  
  if (isScheduleLoaded && 
      (cur_temp < min_t || cur_temp > max_t || cur_rh < min_rh || cur_rh > max_rh)
  ) {
    alertBlink(100, 30);
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

void handleSchedule() {
  noInterrupts();
  digitalWrite(wifiLedPin, wifiLedOn);

  Serial.println("handleSchedule");
  int startIndex = -1, endIndex = -1;
  String reqBody = server.arg("plain");

  Serial.println(reqBody);
  int cnt = 0;
  for (int i = 0; i < reqBody.length(); ++i) {
    if (reqBody[i] == '\"') {
      ++cnt;
    }
    if (startIndex == -1 && cnt == 3) {
      startIndex = i + 1;
    }
    else if (cnt == 4) {
      endIndex = i;
      break;
    }
  }
  Serial.println(cnt);
  Serial.println(startIndex);
  Serial.println(endIndex);

  int curIndex = startIndex, prevIndex = startIndex;
  while(reqBody[curIndex] != ' ') ++curIndex;
  doses_len = reqBody.substring(prevIndex, curIndex).toInt();
  Serial.println("doses_len: " + doses_len);

  for (int i = 0; i < doses_len; ++i) {
    ++curIndex; // skipping the last ' '
    prevIndex = curIndex;
    while(reqBody[curIndex] != ' ') ++curIndex;
    doseIds[i] = reqBody.substring(prevIndex, curIndex).toInt();
    Serial.print(i + ": " + doseIds[i]);
    ++curIndex; // skipping the last ' '
    prevIndex = curIndex;
    while(reqBody[curIndex] != ' ') ++curIndex;
    doseTimers[i] = reqBody.substring(prevIndex, curIndex).toInt();
    Serial.println(doseTimers[i]);
  }

  ++curIndex; // skipping the last ' '
  prevIndex = curIndex;
  while(reqBody[curIndex] != ' ') ++curIndex;
  min_t = reqBody.substring(prevIndex, curIndex).toInt();
  Serial.println("min_t: " + min_t);

  ++curIndex; // skipping the last ' '
  prevIndex = curIndex;
  while(reqBody[curIndex] != ' ') ++curIndex;
  max_t = reqBody.substring(prevIndex, curIndex).toInt();
  Serial.println("max_t: " + max_t);

  ++curIndex; // skipping the last ' '
  prevIndex = curIndex;
  while(reqBody[curIndex] != ' ') ++curIndex;
  min_rh = reqBody.substring(prevIndex, curIndex).toInt();
  Serial.println("min_rh: " + min_rh);

  ++curIndex; // skipping the last ' '
  prevIndex = curIndex;
  max_rh = reqBody.substring(prevIndex, endIndex).toInt();
  Serial.println("max_rh: " + max_rh);

  for (int i = 0; i < doses_len; ++i) {
    isDoseConsumed[i] = isDoseAlerted[i] = false;
  }

  isScheduleLoaded = true;
  scheduleLoadedTime = time(NULL);

  server.send(201, "text/plain", String(doses_len) + "," + String(min_t) + "," + String(max_t) + "," + String(min_rh) + "," + String(max_rh));
  
  digitalWrite(wifiLedPin, wifiLedOff);
  interrupts();
}

void handleConsumes() {
  noInterrupts();
  digitalWrite(wifiLedPin, wifiLedOn);
  String body = "{";
  body += "\"data\":\"";
  int cnt = 0;
  String tmp = "";
  for (int i = 0; i < doses_len; ++i) {
    if (isDoseAlerted[i]) {
      cnt++;
      tmp += String(doseIds[i]) + " " + (isDoseConsumed[i] ? "1" : "0") + " ";
    }
  }
  body += String(cnt) + " " + tmp + "\"";
  body += "}";
  server.send(200, "application/json", body);
  digitalWrite(wifiLedPin, wifiLedOff);
  interrupts();
}

void handleNotFound() {
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
}

void setup(void) {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  
  Serial.println("setup begin");
  addRecord(206, 608);
  addRecord(210, 610);
  addRecord(222, 600);
  addRecord(235, 595);
  Serial.println("test elems are added");
  
  pinMode(wifiLedPin, OUTPUT);
  digitalWrite(wifiLedPin, wifiLedOff);
  
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

  server.on("/schedule", handleSchedule);

  server.on("/consumes", handleConsumes);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  measurer.attach(60, measure);
  Serial.println("Measurer ticker started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
