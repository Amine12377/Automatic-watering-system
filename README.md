# 🌱 Smart Automatic Watering System

An Arduino-based automatic watering system with real-time soil moisture and water tank level monitoring. The system automatically waters plants when the soil moisture falls below a predefined threshold. A second version of the project adds Wi-Fi connectivity for remote monitoring and control.

## Repository Structure

This repository contains two versions of the project:

* **farmtrue** – Standalone version without Wi-Fi functionality.
* **Wi-Fi Version** – Extends the project with remote monitoring and control features.

## Features

* Automatic plant watering using a relay-controlled water pump
* Real-time soil moisture monitoring
* Water tank level monitoring
* OLED display showing sensor readings
* Low water level warning
* Wi-Fi remote monitoring and control (Wi-Fi version only)

## Hardware Used

* Arduino-compatible microcontroller
* REES52 Soil Moisture Sensor
* HC-SR04 Ultrasonic Distance Sensor
* SSD1306 OLED Display
* Relay Module
* Water Pump
* Power Supply

## How It Works

### Soil Moisture Measurement

The **REES52 soil moisture sensor** measures the moisture content of the soil by providing an analogue reading. The `moisturelevel()` function converts the raw sensor value into a percentage, making it easier to determine whether the soil is dry enough to require watering.

### Water Tank Monitoring

The **HC-SR04 ultrasonic sensor** measures the distance between the sensor and the surface of the water inside the tank. The `waterlevel()` function averages multiple measurements to reduce noise and converts the measured distance into a water level percentage. If the water level drops below a predefined threshold, the system displays a warning and prevents the pump from running.

### Automatic Irrigation

If the soil moisture falls below the configured threshold, the relay activates the water pump for a short period before checking the moisture level again.

## Software

* Arduino IDE
* C++
* Adafruit SSD1306 Library
* Adafruit GFX Library
* Wire Library

## Future Improvements

* Mobile application for remote control
* Water usage statistics
* Weather-aware watering schedules
* Push notifications for low water levels
* Historical sensor data logging
