# 957-s-RoboLight
A new way to control Adafruit NeoPixels/ws2812b LEDs for First Robotics.

This repo has Arduino and Java code for controlling ws2812b LEDs intended for use in the First Robotics Challenge.
The Arduino code implements Adafruit's wonderful Arduino library for controlling NeoPixels (here: https://github.com/adafruit/Adafruit_NeoPixel) by acting as a byte reciever.
The Java code communicates over sockets to the Arduino on port 5810 (which can be changed).
