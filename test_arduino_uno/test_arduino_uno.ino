#include <ESP8266WiFi.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

// Wi-Fi credentials
const char* ssid = "APNA LAGWA LE";
const char* password = "bidibehen1";

// Telegram credentials
const char* botToken = "7226364345:AAHnXKhEQfD7Bt-Sd276gSJgFG2pCckOP1M";
const char* chatID = "8144369628";

// Components
const int ldrPin = A0;     // LDR connected to A0
const int buzzerPin = D1;  // Buzzer connected to D1
const int buttonPin = D2;  // Button connected to D2
int threshold = 700;       // Adjust based on your setup
bool buzzerActive = false; // Track buzzer state

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  pinMode(buttonPin, INPUT_PULLUP); // Button with internal pull-up resistor
}

void loop() {
  int ldrValue = analogRead(ldrPin);
  Serial.println(ldrValue);

  // Check LDR for beam interruption
  if (ldrValue < threshold && !buzzerActive) { // Laser beam broken
    digitalWrite(buzzerPin, HIGH); // Activate buzzer
    buzzerActive = true;
    bot.sendMessage(chatID, "Alert! Laser beam broken!", "");
    delay(2000); // Prevent spamming messages
  }

  // Check button press to stop buzzer
  if (buzzerActive && digitalRead(buttonPin) == LOW) { // Button pressed
    digitalWrite(buzzerPin, LOW); // Deactivate buzzer
    buzzerActive = false;
    Serial.println("Buzzer deactivated by button press");
  }

  delay(100); // Adjust for responsiveness
}
