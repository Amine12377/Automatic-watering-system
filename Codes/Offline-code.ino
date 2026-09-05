#include <Wire.h>                  // I2C communication library
#include <Adafruit_GFX.h>          // Graphics library for OLED display
#include <Adafruit_SSD1306.h>      // OLED display driver

// Pin definitions
const int trig_pin = 9;            // Ultrasonic sensor trigger pin
const int relay_pin = 8;           // Relay controlling the water pump
const int echo_pin = 10;           // Ultrasonic sensor echo pin

// Variables used for water level calculation
float duration, distance, total;
int validcount, percentage;

// Variables used for soil moisture
int moisture, per, moistureRaw;

// Create a 128x64 OLED display object using I2C
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// ----------------------------------------------------
// Function: waterlevel()
// Measures the distance between the ultrasonic sensor
// and the water surface.
// Returns the distance and calculates the tank percentage.
// ----------------------------------------------------
float waterlevel() { // function that calculates the water level using the ultrasonic sensor, returning percentage level

    validcount = 0;
    total = 0;

    // Take 50 measurements for better accuracy
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

        delay(10);
    }

    // Calculate average distance
    distance = total / validcount;

    // Convert distance into water level percentage, the tank is full at 2.5cm and empty at 8cm

    percentage = (8 - distance) * 100.0 / (8 - 2.5);

    // Limit percentage between 0 and 100
    if (percentage < 0)
        percentage = 0;

    if (percentage > 100)
        percentage = 100;

    return distance;
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

    // Configure pins
    pinMode(trig_pin, OUTPUT);
    pinMode(echo_pin, INPUT);
    pinMode(relay_pin, OUTPUT);

    // Start Serial Monitor
    Serial.begin(9600);

    // Initialise OLED display
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}


void loop() {

    // Read sensor values
    per = round(waterlevel());
    moisture = moisturelevel();



    // If water tank is almost empty
    if (per < 20) {

        // Display warning message
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 0);

        display.println("Water level too low!");

        display.print(moisture);
        display.print(" %");

        display.display();

        delay(100);

        // Skip the rest of the loop
        return;
    }



    // Display sensor information
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.print("Water level: ");
    display.print(per);
    display.println(" %");

    display.setCursor(0, 20);
    display.print("Moisture: ");
    display.print(moisture);
    display.println(" %");

    display.display();

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
