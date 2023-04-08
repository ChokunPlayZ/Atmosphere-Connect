# Atmosphere Connect
Home Assistant Intregration for `Atmosphere 101076TH` Air Purifier (last 2 charector should't matter)  
it will work with this model, for other model, your milage may vary.  
## How To Replace the Board
1. First Pop out the Front Cover of your Air Purifier,   
<Picture Here\>  
You only need to take out the top 5 screws and nothing else  
after you take it off it should just pop off  
2. Disconect the Old Motherboard
unplug the cable and remove the old mainboard  
<Picture Here\>  
in this version, since the air sensor is not working yet, you only need the 4 pin connector, ignore the other one  
3. Connect The Pico W
Based On This Picure, it should give you an idea on what to do  
<Picture Here\>  

|Color|Info|
|--|--|
|Black|GND|
|Orange|Fan Control|
|White|*Not Figured Out Yet*|
|Yellow|5V|

Connect 5V to Pin 39 (VSYS)  
Connect GND to GND  
and Connect Orange Pin 1 (GPIO0)  

## Dependencies
- [Raspberry Pi Pico W](https://www.raspberrypi.com/products/raspberry-pi-pico/?variant=raspberry-pi-pico-w), a $6 microcontroller board with WiFi
- [PlatformIO](https://platformio.org/platformio-ide), an extension for [VSCode](https://code.visualstudio.com) with the most robust and reliable Arduino developer experience
- [`arduino-pico`](https://github.com/earlephilhower/arduino-pico): a community-developed Arduino core for Pico/Pico W and other RP2040 boards with constant updates and excellent support for existing Arduino code
- [`arduino-home-assistant`](https://github.com/dawidchyrzynski/arduino-home-assistant): a thorough Arduino/Home Assistant integration using MQTT

# Pull requests

PRs are welcome on these points:

- Enhance documentation (especially any troubleshooting steps you found helpful)
- Refine project structure
- Additional code paths to accommodate common variations (unsecured MQTT broker, different WiFi authentication, etc)
- Fix my bad C++
- Fix My Terrible Documentation
---
Questions? Comments? Gripes? Open an issue!
