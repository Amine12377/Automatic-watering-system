This is my first personal project showcasing a automatic watering system for plants.
the project relies on sensors that capture the water level of a water tank and the moisture of the soil. The information is then collected to make decisions about watering the plant.
Hardware used:
- Arduino nano esp32
- Relay module
- 16x2 LCD display with i2c module attached to it.
- TP 4056 type c charger module for lithium batteries.
- Resistive soil moisture sensor.
- HC-SR04 ultrasonic sensor.
- MT3608 DC to DC boost converter module, this module is used to convert the 3.7V coming from the battery to a stable 5V.
- Solar panel (5V, 10Watt, 210x130x1.5mm)
- A solar management board that provides a regulated output from the solar panel.
- a water pump and water tank
