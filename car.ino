/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool forward;
  bool backward;
  bool left;
  bool right;
} struct_message;

// Create a struct_message called myData
struct_message myData;

#define ENA 11
#define IN1 10
#define IN2 9
#define IN3 8
#define IN4 7
#define ENB 6

unsigned long lastRecv = 0;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  lastRecv = millis();
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("forward: ");
  Serial.println(myData.forward);
  Serial.print("backward: ");
  Serial.println(myData.backward);
  Serial.print("left: ");
  Serial.println(myData.left);
  Serial.print("right: ");
  Serial.println(myData.right);
  Serial.println();
}

void setup() {
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  Serial.begin(9600);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  stop();
  center();
}
void loop() {
  if (millis() - lastRecv > 500) {
    stop();
    center();
  } else {
    if (myData.forward) {
      forward();
    } else if (myData.backward) {
      back();
    } else {
      stop();
    }
    if (myData.left) {
      left();
    } else if (myData.right) {
      right();
    } else {
      center();
    }
  }
  delay(20);
}

void test() {
  Serial.println("starting test");
  forward();
  delay(1000);
  stop();
  delay(500);
  back();
  delay(1000);
  stop();
  right();
  delay(500);
  left();
  delay(500);
  center();
  Serial.println("ending test");
}

void forward() {
  digitalWrite(ENB, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("forward");
}
void back() {
  digitalWrite(ENB, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("back");
}
void stop() {
  digitalWrite(ENB, LOW);
  Serial.println("stop");
}
void right() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  Serial.println("right");
}
void left() {
  digitalWrite(ENA, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  Serial.println("left");
}
void center() {
  digitalWrite(ENA, LOW);
  Serial.println("center");
}