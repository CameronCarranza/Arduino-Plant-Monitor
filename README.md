# Arduino Plant Monitor

### Note: Work in Progress

This repository contains the Arduino Config and Node.js API. Frontend in progress (will be React or plain HTML with JS to consume the API).

## Hardware
* [Adafruit 2821 ESP8266](https://www.adafruit.com/product/2821) or other WiFi compatible Arduino.
* A [battery](https://www.amazon.com/gp/product/B00J2QET64/) may be desired depending on your use-case.
* Moisture Sensor(s) such as the [Phantom YoYo](https://www.amazon.com/gp/product/B00AFCNR3U/).
* If using more than 1 sensor on the Adafruit or other Arduino with only one analog input, a multiplexer will be needed. I am using a [4051](https://www.amazon.com/gp/product/B071GKXHBS/) currently.

## Arduino Configurations

### Direct Analog Input
If you are using an Arduino device with one or many analog inputs, use the `plant_monitor_analog_only` package in the Arduino folder.

* Set up WiFI under the Network Configuration section.
* Update the Sensors array with your pin(s) (if using the Adafruit 2821 no change is needed here).

![Image of Adafruit 2821 with Sensor and Battery](https://i.imgur.com/XE8tham.jpg)

### Analog Input with Multiplexer
If you are using a multiplexer with a single analog input, use the `plant_monitor_multiplexer` package in the Arduino folder.

* Set up WiFI under the Network Configuration section.
* Set `amount_of_mux_inputs` to match how many devices you will run off of the multiplexer (if 2, set 2).
* Connect your sensors to the Multiplexer. It is important to start from `y0` and count upwards as there is no way to detect if a sensor is plugged in or not.
* Edit `s0`, `s1`, and `s2` to match your digital selection pins.
* Update your AnalogSensor Pin if not using the Adafruit 2821.

![Image of Adafruit 2821 with Two Sensors, Battery, and Multiplexer](https://i.imgur.com/yQgRmrL.jpg)

