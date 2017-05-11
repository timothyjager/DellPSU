# DellPSU
Arduino library for reading values from a Dell power supply via 1 wire protocol

Reads the data from a dell charger using OneWire and parses the wattage, voltage, and current

Connect the center pin of the Dell power supply to the desired Arduino I/O pin

Add a 4.7K pull-up resistor between the tiny center pin of the Dell power supply and the Arduino +5V rail

Connect the ground pin of the Dell power supply to the Arduino GND

Do not connect the 19.5V pin of the Dell power supply to the Arduino!!!! 

Credits:
Inspired by this blog post: https://hclxing.wordpress.com/2014/02/06/hacking-the-dell-laptop-power-adapter/
Guillermo Lovato's one wire sample code was used as a starting point for the code base.

