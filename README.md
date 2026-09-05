This is my first personal project showcasing an automatic watering system for plants. The main goal of this project is to monitor the soil moisture  while I am away.
The project relies on sensors that capture the water level of a water tank and the moisture of the soil. The information is then collected to make decisions about watering the plant.


Hardware used:
- Arduino nano esp32
- Relay module that controls the pump.
- 16x2 LCD display with i2c module attached to it.
- TP 4056 type c charger module for lithium batteries.
- Resistive soil moisture sensor.
- HC-SR04 ultrasonic sensor.
- MT3608 DC to DC boost converter module, this module is used to convert the 3.7V coming from the battery to a stable 5V.
- Solar panel (5V, 10Watt, 210x130x1.5mm)
- A solar management board that provides a regulated output from the solar panel.
- a water pump and water tank

How the project works:
- The ultrasonic sensor calculates the distance between the surface of the water and the sensor, the sensor sends an ultrasonic wave and waits for it to bounce back from the water, using the time it took for the signal to go and bounce back, and the speed of sound, the distance is then calculated.
- The resistive soil sensor is used to obtain the soil moisture level, the Arduino receives a raw ADC signal ranging from 0 to 4095 depending on how moist the soil is. 
- When the soil becomes dry, "in my case, I set the thresholds to 50%" the system activates the pump.
- When the water level becomes too low "I set it to 20%", the system stops the watering and provides a warning.
- The lcd display displays the moisture level and the water level.
- the system is powered by a lithium battery and the pump is powered by an external source.

Power usage estimation (theoretical estimation):
- The entire system without the pump draws approximately 150mA of current with a 5V source, so that means 0.75W.
- The battery has a capacity of 2600mAH and works in 3.7V, that means it has a capacity of 9.62W.h
- The solar panels provide at best 10W, so for average, 5W.
- That means that the system could last on a full charge 12 hours without sunlight.
- When we include the solar panels, the system would have a surplus of 4.25W for charging the battery, so theoretically the system could run 24/7 if there is enough sunlight.
    
The first iteration of this project works without the online features.
The second iteration adds online features for remote control

Online features:
- I have used a telegram bot to control and monitor my plant.
- The bots responds to three commands
- /stop : the system stops
- /start: the system starts
- /getinfo: getting the soil moisture and the water level of the tank

Problems encountered and future improvements:
- The ultrasonic sensor gave multiple invalid readings, the problem was that the water didn't have a smooth surface causing the signal to bounce back in random directions. To overcome the problem, I have created a loop that takes an average of 50 readings and skips past the invalid ones.
- The original system was supposed to run on a 3.7V lithium battery, but the LCD display required 5V. This problem was fixed using the MT3608 module.
- In the future, a better signal processing system needs to be implemented to make have better readings.
- For a better design, A PCB needs to be implemented including a 3d printed housing.
- A weather input can also be used, for instance, using a weather API, the system could automatically stop on a rainy day.
