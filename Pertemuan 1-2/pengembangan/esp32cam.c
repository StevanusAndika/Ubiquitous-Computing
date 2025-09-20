#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* AP_SSID = "ESP8266_AP";     // ganti nama WiFi
const char* AP_PASS = "passwordAP";     // min 8 karakter, kosongkan "" untuk WiFi open

ESP8266WebServer server(80);

// IP statis untuk AP (opsional tapi disarankan)
IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

void handleRoot() {
  String html = F(
    "<!DOCTYPE html>"
    "<html lang='id'>"
    "<head>"
      "<meta charset='UTF-8'>"
      "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
      "<title>Kontrol ESP8266 AP</title>"
      "<style>"
        "*{margin:0;padding:0;box-sizing:border-box;font-family:'Segoe UI',Tahoma,Geneva,Verdana,sans-serif;}"
        "body{background:linear-gradient(135deg,#667eea 0%,#764ba2 100%);min-height:100vh;display:flex;justify-content:center;align-items:center;padding:20px;}"
        ".container{background-color:rgba(255,255,255,0.9);border-radius:15px;box-shadow:0 10px 30px rgba(0,0,0,0.2);padding:30px;width:100%;max-width:500px;text-align:center;}"
        "h1{color:#333;margin-bottom:20px;font-weight:600;}"
        ".info-box{background-color:#f8f9fa;border-left:4px solid #667eea;padding:15px;margin:20px 0;text-align:left;border-radius:4px;}"
        ".info-box p{margin:5px 0;color:#555;}"
        ".btn-container{display:flex;justify-content:center;gap:15px;margin-top:25px;}"
        ".btn{padding:12px 25px;border:none;border-radius:50px;font-size:16px;font-weight:600;cursor:pointer;transition:all 0.3s ease;box-shadow:0 4px 8px rgba(0,0,0,0.1);text-decoration:none;display:inline-block;}"
        ".btn-on{background-color:#4CAF50;color:white;}"
        ".btn-on:hover{background-color:#45a049;transform:translateY(-2px);box-shadow:0 6px 12px rgba(0,0,0,0.15);}"
        ".btn-off{background-color:#f44336;color:white;}"
        ".btn-off:hover{background-color:#d32f2f;transform:translateY(-2px);box-shadow:0 6px 12px rgba(0,0,0,0.15);}"
        ".status{margin-top:25px;padding:10px;border-radius:8px;font-weight:500;}"
        ".status-on{background-color:rgba(76,175,80,0.2);color:#2e7d32;}"
        ".status-off{background-color:rgba(244,67,54,0.2);color:#c62828;}"
        ".footer{margin-top:30px;color:#777;font-size:14px;}"
      "</style>"
    "</head>"
    "<body>"
      "<div class='container'>"
        "<h1>Kontrol ESP8266 (Access Point)</h1>"
        "<div class='info-box'>"
          "<p><strong>SSID:</strong> ESP8266_AP</p>"
          "<p><strong>Alamat IP:</strong> 192.168.4.1</p>"
          "<p><strong>Status:</strong> Terhubung</p>"
        "</div>"
        "<p>Kontrol LED onboard ESP8266 dengan tombol di bawah:</p>"
        "<div class='btn-container'>"
          "<a href='/led/on' class='btn btn-on'>LED ON</a>"
          "<a href='/led/off' class='btn btn-off'>LED OFF</a>"
        "</div>"
        "<div id='status' class='status status-off'>Status: LED MATI</div>"
        "<div class='footer'>"
          "<p>Dibuat dengan ESP8266 | &copy; 2023</p>"
        "</div>"
      "</div>"
      "<script>"
        "function updateStatus(){"
          "const statusEl=document.getElementById('status');"
          "if(window.location.href.indexOf('led/on')>-1){"
            "statusEl.textContent='Status: LED HIDUP';"
            "statusEl.className='status status-on';"
          "}else if(window.location.href.indexOf('led/off')>-1){"
            "statusEl.textContent='Status: LED MATI';"
            "statusEl.className='status status-off';"
          "}"
        "}"
        "document.querySelectorAll('.btn').forEach(button=>{"
          "button.addEventListener('click',function(e){"
            "e.preventDefault();"
            "const url=this.getAttribute('href');"
            "fetch(url)"
              ".then(response=>response.text())"
              ".then(data=>{"
                "console.log(data);"
                "updateStatus();"
              "})"
              ".catch(error=>console.error('Error:',error));"
          "});"
        "});"
        "document.addEventListener('DOMContentLoaded',updateStatus);"
      "</script>"
    "</body>"
    "</html>"
  );
  server.send(200, "text/html", html);
}

void handleLedOn()  { 
  digitalWrite(LED_BUILTIN, LOW);  
  server.send(200, "text/plain", "LED ON");  
}

void handleLedOff() { 
  digitalWrite(LED_BUILTIN, HIGH); 
  server.send(200, "text/plain", "LED OFF"); 
}

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