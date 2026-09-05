#include <Wire.h>                  // I2C communication library
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

const char* ssid = "...";
const char* password = "...";

#define BOT_TOKEN "..."
#define CHAT_ID "..."

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
//variables representing state.
bool warning = false;
bool start = true;
String msg; 

// Pin definitions
const int trig_pin = 9;            // Ultrasonic sensor trigger pin
const int relay_pin = 8;           // Relay controlling the water pump
const int echo_pin = 10;           // Ultrasonic sensor echo pin

// Variables used for water level calculation
float duration, distance, total;
int validcount, percentage, n;

// Variables used for soil moisture
int moisture, per, moistureRaw;


LiquidCrystal_I2C lcd(0x27, 20, 4);

// Function: waterlevel()
// Measures the distance between the ultrasonic sensor
// and the water surface.
// Returns the distance and calculates the tank percentage.

float waterlevel() { 

    validcount = 0;
    total = 0;

    // Take 50 measurements for more precision
    for (int i = 0; i < 50; i++) {

        // Send ultrasonic pulse
        digitalWrite(trig_pin, LOW);
        delayMicroseconds(2);

        digitalWrite(trig_pin, HIGH);
        delayMicroseconds(10);

        digitalWrite(trig_pin, LOW);

        // Measure echo duration
        duration = pulseIn(echo_pin, HIGH, 30000);

        // Convert time into distance (cm)
        distance = duration * 0.0343 / 2;

        // Ignore invalid readings above 10 cm
        if (distance > 10) {
            continue;
        }

        validcount++;
        total += distance;

        delay(20);
    }

    // Calculate average distance
    distance = total / validcount;

    // Convert distance into water level percentage, the tank is full at 2.5cm and empty at 8cm.
    percentage = (8 - distance) * 100.0 / (8 - 2.5);

    // Limit percentage between 0 and 100
    if (percentage < 0)
        percentage = 0;

    if (percentage > 100)
        percentage = 100;

    return percentage;
}


int moisturelevel() { //function that reads the moisture level

    // Read analogue value from sensor
    moisture = analogRead(A0);

    // Convert raw ADC value into percentage
    moisture = map(moisture, 0, 4095, 0, 100);

    // Reverse percentage as higher reading means lower moisture 
    moisture = 100 - moisture;

    return moisture;
}


void setup() {
    // Start Serial Monitor
    Serial.begin(115200);
    // Configure pins
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    pinMode(relay_pin, OUTPUT);

    


    // Initialise LCD display
    lcd.init();
    lcd.backlight();

    WiFi.begin(ssid, password); //connecting the arduino to wifi 

    while (WiFi.status() != WL_CONNECTED) { //sending a waiting message until the arduino connects to wifi, this is for testing purposes
        Serial.print(".");
        delay(400);
    } 
//displaying message when connected to wifi
Serial.println(); 
  Serial.println("WiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  client.setInsecure(); //skipping certificate validation for easier and faster connection.
  Serial.println("TLS configured");


}


void loop() {
    n = bot.getUpdates(bot.last_message_received+ 1);
    if (n>0){
        for (int i = 0; i<n; i++){
            msg = bot.messages[i].text;
            if(msg == "/stop" && start){
                bot.sendMessage(CHAT_ID, "The system is off");
                start = false;
            }
            if(msg == "/start" && !start){
                bot.sendMessage(CHAT_ID, "The system is on");
                start = true;
            }
            if(msg == "/getinfo"){
                int per = round(waterlevel());
                int moisture = moisturelevel();
                bot.sendMessage(CHAT_ID,  "The water level is " + String(per) + "%" +
                    " and the moisture level is " + String(moisture) + "%");
            }
            
        }
    }


    
    if (start){

    int per = round(waterlevel());
    int moisture = moisturelevel();
    // If water tank is almost empty
    if (per < 20) {

        // Display warning message
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Water too low!");

        lcd.setCursor(0, 1);
        lcd.print(moisture);
        lcd.print(" %");
        
        if (warning == false){
            bot.sendMessage(CHAT_ID, "WATER LEVEL TOO LOW!!!", "");
            warning = true;
        }
        delay(100);

        // Skip the rest of the loop
        return;
    }
    else {
        warning = false;
    }    
    
       

    // Display sensor information
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Water: ");
    lcd.print(per);
    lcd.print(" %");

    lcd.setCursor(0, 1);
    lcd.print("Moisture: ");
    lcd.print(moisture);
    lcd.print(" %");
    
  
    // If soil is dry (below 50%)
    if (moisture < 50) {

        // Turn pump ON
        digitalWrite(relay_pin, HIGH);

        // Water plants for 3 seconds
        delay(3000);

        // Turn pump OFF
        digitalWrite(relay_pin, LOW);
    }

    // Small delay before next measurement
    delay(100);
    }
}
