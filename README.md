# hjmqtt
https://github.com/hjltu/hjmqtt
<br> MQTT client, bridge between my mqtt devices and other systems, like homekit2mqtt, Homeassistant, OpenHUB etc.

# hjconnect
https://github.com/hjltu/hjconnect
<br> Remote telemetry system  for raspberry pi

# my home automation devices
<br>
<b>1. wifi controller(wemos)</b>
<br>https://github.com/hjltu/hjhome/blob/master/docs/http-wemos.html
<br>https://github.com/hjltu/hjhome/blob/master/docs/wifi-controller-instructions.txt
<br>https://youtu.be/PPkzPMNiBQY
<br>https://www.ebay.com/itm/253763472195
<br><b>I/O port reference:</b>
bt1,2   input for buttons (short ang long press) with impulse counter.
<br>ds      input for ds18b20 sensor (temperature sensor).
<br>DHT     input for DHT22 (temperature, humidity sensor).
<br>A0      analog input for any analog devices(CO,CO2,luminosity).
<br>p1,2    pwm outputs, for dimmers and motorizer curtains control(Somfy) 
<br>L1,2    triac outputs, AC current (light, jalousie).
<br><img src="img/wemos.jpg" width="500">
<br>
<b>2. lan controller(arduino mega)</b>
<br>https://github.com/hjltu/hjhome/blob/master/docs/http-mega.html
<br>https://github.com/hjltu/hjhome/blob/master/docs/mega-controller-instructions.txt
<br>https://www.ebay.com/itm/254015934717
<br><img src="img/mega.jpg" width="500">
test: 
<br>mosquitto_sub -h test.mosquitto.org -t /mega11/# -v
<br>mosquitto_pub -h test.mosquitto.org -t /mega11/in/echo -m hello!
<br>
<br><b>3. Triac dimmer ~220v, PWM 3.3v/5v, 1kHz min (arduino pro mini)</b>
<br>https://youtu.be/Dj-iw9Rz5CE
<br>https://www.ebay.com/itm/254046916040
<br>https://github.com/hjltu/hjhome/blob/master/prog/pwm_to_dimm.ino
<br><img src="img/pwm-dimmer.jpg" width="500">
<br><br>
<b>4. 28 channel solid-state relay (BT-139)</b>
<br><img src="img/28-channel.jpg" width="500">
