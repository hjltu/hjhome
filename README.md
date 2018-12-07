# hjmqtt
https://github.com/hjltu/hjmqtt

# hjconnect
remote telemetry system  for raspberry pi

# my home automation devices
<br>
<b>1. wifi controller(wemos)</b>
<br>https://github.com/hjltu/hjhome/blob/master/docs/http-wemos.html
<br>https://www.ebay.com/itm/253763472195
<br><img src="img/wemos.jpg" width="500">
<br>
<b>2. lan controller(arduino mega)</b>
<br>https://github.com/hjltu/hjhome/blob/master/docs/http-mega.html
<br>https://www.ebay.com/itm/254015934717
<br><img src="img/mega.jpg" width="500">
<br>test: 
<br>mosquitto_sub -h 79.110.246.131 -p 18830 -t /# -v
<br>mosquitto_pub -h 79.110.246.131 -p 18830 -t /mega11/in/echo -m hello!
<br>
<b>3. pwm dimmer 3.3v/5v, 1kHz min (arduino pro mini)</b>
<br><img src="img/pwm-dimmer.jpg" width="500">
<br>
<b>4. 28 channel solid-state relay (BT-139)</b>
<br><img src="img/28-channel.jpg" width="500">
