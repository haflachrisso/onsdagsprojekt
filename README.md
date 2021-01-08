# onsdagsprojekt

A small embedded controller project by Christian Nick Rasmussen.
Project started on 04/04/2021

The premise is an embedded controller for some kind of machine, where the power can be controlled and temperature monitored. 

## Parts List

<ul>
<li>STM32 F7 Discovery Board</li>
<li>USB A to mini B cable</li>
<li>Grove Button</li>
<li>Grove Temperature Sensor V1.2</li>
<li>Grove Rotary Angle Sensor</li>
<li>2 x Grove LED Sockets</li>
<li>Grove Red LED</li>
<li>Grove Green LED</li>
<li>5 x 26AWG Grove Cables</li>
</ul>

## Connections
<ul>
<li>Grove Temperature Sensor V1.2 => A0</li>
<li>Grove Rotary Angle Sensor => A1</li>
<li>2 x Grove LED Sockets => D2 and D3</li>
<li>Grove Button => D4</li>
</ul>

## Features
### Header
Shows name of the project, an on timer and a checked counter. 
The timer is in seconds.
The check counter symbolises every time a security check has been made. To increase, press the external button, to reset, touch the display on the counter.

### Bottom Buttons
Used to cycle through different screens, by touch.

### Power Screen
Shows power in percentage and is controlled with the Rotary Angle Sensor. As the percentage increases, the percentage on the display changes color. If power is above 90%, peripheral LED will start flashing.

### Temperature Screen
Shows the temperature measured by the Temperature Sensor, in celcius, fahrenheit and kelvin. 
