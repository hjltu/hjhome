# hjhome
**new version** ☺️
<br> see branch: testing

# hjmqtt
https://github.com/hjltu/hjmqtt
<br> MQTT client, bridge between my mqtt devices and other systems, like homekit2mqtt, Homeassistant, OpenHUB etc.

**more documents and instructions**
https://github.com/OSA83/hjhome-doc

# hjconnect
https://github.com/hjltu/hjconnect
<br> Remote telemetry system  for raspberry pi

# sima
https://github.com/hjltu/sima
<br>Yandex voice assistant for home automation control
<br>https://youtu.be/XiIFVnCSt-M

# my home automation devices
<br>
<br>1. wifi controller(wemos)</b>
<br>https://github.com/hjltu/wemos-wifi-controller
<br>https://github.com/hjltu/hjmqtt/tree/master/arduino
<br>https://github.com/hjltu/hjmqtt/tree/master/arduino/doc
<br>https://youtu.be/PPkzPMNiBQY
<br>https://www.ebay.com/itm/254534654473
<br><b>I/O port reference:</b>
<br>bt1,2   input for buttons (short ang long press) with impulse counter.
<br>ds      input for ds18b20 sensor (temperature sensor).
<br>DHT     input for DHT22 (temperature, humidity sensor).
<br>A0      analog input for any analog devices(CO,CO2,luminosity).
<br>p1,2    pwm outputs, for dimmers and motorizer curtains control(Somfy) 
<br>L1,2    triac outputs, AC current (light, jalousie).
<br><img src="img/wemos.jpg" width="500">
<br>
<b>2. lan controller(arduino mega)</b>
<br>https://github.com/hjltu/hjmqtt/tree/master/arduino
<br>https://github.com/hjltu/hjmqtt/tree/master/arduino/doc
<br>https://www.ebay.com/itm/254015934717
<br><b>I/O port reference:</b>
<br>A0-A16           input for buttons (between bt and GND)
<br>D14-D21          input for ds18b20 sensor (temperature sensor).
<br>D14-D21          input for DHT22 (temperature, humidity sensor).
<br>A0-A6            analog input for any analog devices.
<br>D2-D9,D11-D13    pwm outputs.
<br>D22-D49          triac outputs (220 volt).
<br>D10,D50-D53      lan network W5100
<br><img src="img/mega.jpg" width="500">
test: 
<br>mosquitto_sub -h test.mosquitto.org -t /mega11/# -v
<br>mosquitto_pub -h test.mosquitto.org -t /mega11/in/echo -m hello!
<br>
<br><b>3. Triac dimmer, PWM input 3.3v/5v, 1kHz min.</b>
<br>https://youtu.be/Dj-iw9Rz5CE
<br>https://www.ebay.com/itm/254046916040
<br>https://github.com/hjltu/hjmqtt/tree/master/arduino
<br><img src="img/pwm-dimmer.jpg" width="500">
<br><br>
<b>4. 28 channel solid-state relay (triac BT-139)</b>
<br><img src="img/28-channel.jpg" width="500">
