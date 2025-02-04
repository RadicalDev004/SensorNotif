#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include<string>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // I2C address for most 128x64 OLED displays

// Wi-Fi credentials
const char* ssid = "UPCE134F3E";
const char* password = "cv7zv3SUfhxa";

const char* serverURL = "http://192.168.0.136:5000/send";

#define IR_SENSOR_PIN 4
HTTPClient http;
String client_id = "17346827";

// Create a web server on port 80
// WebServer server(80);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void updateDisplay() {
  display.clearDisplay();   // Clear previous content
  display.setCursor(0, 0);  // Reset to the top-left
  display.setTextSize(1);   // Text size
}

void PostIrInfo(int info)
{
  http.addHeader("Content-Type", "application/json");
  String payload = "{\"id\":\"" + client_id + "\"," +
                      "\"message\": \"sensor-" + String(info) + "\""
                      + "}";
    Serial.println(payload);
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: " + response);

      display.println("Data sent succesfully with code\n" + String(httpResponseCode));
      display.display();
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));

      display.println("Error sending message\n" + String(httpResponseCode));
      display.display();
    }
}


void setup() {
  Serial.begin(921600);
  
  if (!display.begin(0x3C, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Clear the buffers
  display.clearDisplay();

  // // Display text
  display.setTextSize(1);      // Text size
  display.setTextColor(SSD1306_WHITE); // Text color
  display.setCursor(0, 10);    // Position (x, y)
  
  
  
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  String data_output = "Connected " + WiFi.localIP().toString();
  display.println(data_output);
  display.display();   
  
  //Define a handler for the root URL
  // server.on("/", []() {
  //   server.send(200, "text/plain", "Hello from ESP32!");
  // });

  // // Start the server
  //server.begin();
  //Serial.println("Web server started");
  
  //pinMode(PIR_SENSOR_PIN, INPUT);
  pinMode(IR_SENSOR_PIN, INPUT);  // Set the IR sensor pin as input
  //Serial.println("Obstacle Avoidance Test!");

  delay(2000);

  updateDisplay();
  display.println("Attemtping connection to http server...");
  display.display();
  delay(2000);

  if (WiFi.status() == WL_CONNECTED) {
    

    http.begin(serverURL);
    updateDisplay();
    display.println("Connection succeeded for " + String(serverURL));
    display.display();
    
    delay(2000);

    updateDisplay();
    
    http.addHeader("Content-Type", "application/json");
    // Prepare data to send
    //String my_ip = WiFi.localIP().toString();
    String payload = "{\"id\":\"" + client_id + "\"," +
                      "\"message\": \"test \"" 
                      + "}";
    Serial.println(payload);
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Server Response: " + response);

      display.println("Data sent succesfully with code\n" + String(httpResponseCode));
      display.display();
    } else {
      Serial.println("Error on sending POST: " + String(httpResponseCode));

      display.println("Error sending message\n" + String(httpResponseCode));
      display.display();
    }

  http.end();
  } else {
  Serial.println("WiFi Disconnected");
  }
  

}

void loop() {

  int IR_STATUS = digitalRead(IR_SENSOR_PIN);
  String status;
  // if (WiFi.status() == WL_CONNECTED)
  // {


    // Prepare data to send
    if(IR_STATUS == LOW){
      status = "Object detected";
    } else{
      status = "Object not detected";
    }
    Serial.println(status);

    PostIrInfo(IR_STATUS == LOW ? 1 : 0);
    //   String payload = "{\"IR_SENSOR\":\"" + status + "\"}";
    //   int httpResponseCode = http.POST(payload);

    //   if (httpResponseCode > 0) {
    //     String response = http.getString();
    //     Serial.println("Server Response: " + response);

    //     display.println("Data sent succesfully with code\n" + String(httpResponseCode));
    //     display.display();
    //   } else {
    //     Serial.println("Error on sending POST: " + String(httpResponseCode));

    //     display.println("Error sending message\n" + String(httpResponseCode));
    //     display.display();
    //   }

    // http.end();
    // } else {
    // Serial.println("WiFi Disconnected");
    // }
    delay(5000); // Delay before the next request*/
}