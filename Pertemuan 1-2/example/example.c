#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// Buat server di port 80
ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<h1>Hello dari ESP8266!</h1>");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* AP_SSID = "ESP8266_AP";     // ganti nama WiFi
const char* AP_PASS = "passwordAP";     // min 8 karakter, kosongkan "" untuk WiFi open

ESP8266WebServer server(80);

// IP statis untuk AP (opsional tapi disarankan)
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway (192, 168, 4, 1);
IPAddress subnet  (255, 255, 255, 0);

void handleRoot() {
  String html = F(
    "<!doctype html><html><head><meta charset='utf-8'/>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'/>"
    "<title>ESP8266 AP</title></head><body>"
    "<h1>Halo dari ESP8266 (AP)</h1>"
    "<p>Akses via <b>http://192.168.4.1/</b></p>"
    "<p><a href='/led/on'>LED ON</a> | <a href='/led/off'>LED OFF</a></p>"
    "</body></html>"
  );
  server.send(200, "text/html", html);
}

void handleLedOn()  { digitalWrite(LED_BUILTIN, LOW);  server.send(200, "text/plain", "LED ON");  }
void handleLedOff() { digitalWrite(LED_BUILTIN, HIGH); server.send(200, "text/plain", "LED OFF"); }

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // Mode Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_ip, gateway, subnet);

  // Jika mau AP tanpa password (open), pakai: WiFi.softAP(AP_SSID);
  bool ap_ok = WiFi.softAP(AP_SSID, AP_PASS); // channel default 1, visible, max 4 client
  Serial.println(ap_ok ? "AP started" : "AP failed");
  Serial.print("AP SSID: "); Serial.println(AP_SSID);
  Serial.print("AP IP  : "); Serial.println(WiFi.softAPIP()); // biasanya 192.168.4.1

  // Routing
  server.on("/", handleRoot);
  server.on("/led/on", handleLedOn);
  server.on("/led/off", handleLedOff);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}

  Serial.println("WiFi terhubung!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // routing
  server.on("/", handleRoot);

  // mulai server
  server.begin();
  Serial.println("HTTP server dimulai");
}

void loop() {
  server.handleClient();
}