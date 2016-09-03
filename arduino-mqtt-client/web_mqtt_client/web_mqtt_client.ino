// Project name: hjhome
// Copyright (C) 2016  hjltu@ya.ru
// https://launchpad.net/hjhome

// hjhome is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// hjhome is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <avr/wdt.h>	//disable for nano!
//#include <avr/pgmspace.h>	const PROGMEM 
#include <EEPROM.h>
////////////////////////////////
//	w5100
#include <SPI.h>
#include <Ethernet.h>

//	enc28j60
//#include <UIPEthernet.h>
/////////////////////////////////
#include <PubSubClient.h>

//	http
EthernetServer server(80);

byte ip[]={192,168,0,21};
byte mac[]={0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};

//	client name
String str_name="arduino";

//	your local mqtt server
//IP
byte mqtt_server[] = {192, 168, 0, 31 };
//DNS
//char mqtt_server[] = "mosquitto.org";

int port = 1883;
String str_topic;
String str_payload;
EthernetClient ethClient;
PubSubClient mqtt_client(mqtt_server, port, callback, ethClient);

bool update,blink;

void setup()
{
	wdt_disable();
	Serial.begin(9600);
	Serial.println(F("setup..."));
	memory_clear();
	Serial.println(F("Start in 5 network..."));
	Ethernet.begin(mac, ip);
	server.begin();
	Serial.print(F("IP Address: "));
	Serial.println(Ethernet.localIP());
	delay(1000);
	Serial.println(F("Start in 4 mqtt..."));
	Serial.print(F("name: "));
	Serial.println(str_name);
	Serial.print(F("server: "));
	//	IP
	Serial.print(String(mqtt_server[0]));
	Serial.print(F("."));
	Serial.print(String(mqtt_server[1]));
	Serial.print(F("."));
	Serial.print(String(mqtt_server[2]));
	Serial.print(F("."));
	Serial.println(String(mqtt_server[3]));
	//	DNS
	//Serial.println(String(mqtt_server));

	delay(1000);
	Serial.println(F("Start in 3 pin mode..."));
	my_pin_mode();
	delay(1000);
	Serial.println(F("Start in 2"));
	delay(1000);
	Serial.println(F("Start in 1"));
	delay(1000);
	Serial.println(F("Start"));
	wdt_enable(WDTO_8S);
}

void loop()
{
	Stage0();
	Stage1();
	Stage2();
	//delay(1000);
	wdt_reset();
}

void Stage0()
{
	my_web_server();
	my_mqtt_server();
}

void Stage1()
{
	my_automation();
	my_update();
}

void Stage2()
{

}
//	setup	//

void memory_clear()
{
	Serial.println(F("memory test..."));
	int shift=1000;
	char mydb[25];
	for(int i=0; i<25; i++)
	{
		mydb[i] = char(EEPROM.read(i+shift));
	}
	String check = String(mydb);
	Serial.println(check.length());
	check.trim();
	if(check=="mem-ok")	//if(check.indexOf("mem-o")>0)
		Serial.println(F("mem-ok"));
	else
	{
		Serial.println(F("clean mem..."));
		char ok[] = "mem-ok";
		for(int i=0; i<2048; i++)
		{
			EEPROM.write(i,0);
			delay(1);
		}
		for(int i=0; i<sizeof(ok); i++)
		{
			EEPROM.write(i+shift,ok[i]);
			delay(1);
			Serial.print(char(EEPROM.read(i+shift)));
		}
		Serial.println();
	}
}

void my_pin_mode()
{
	pinMode(7,OUTPUT);
}

//	stage0***********************************************************************8

//	my_web_server	//

void my_web_server()
{
	EthernetClient client = server.available();
	String readString;
	if(client)
	{
		Serial.println(F("-> New Connection"));
		while (client.connected()) 
		{
			if(client.available())
			{
				char c = client.read();
				if(readString.length()<100)
					readString += c;
				if(c == '\n')
				{
					readString.trim();
					Serial.println(readString);
					client.println(F("HTTP/1.1 200 OK"));
					client.println(F("Content-Type: text/html"));
					client.println();
client.println(F("	<!DOCTYPE html><html><head><title>arduino</title>"));
client.println(F("<style>body {padding: 3%}"));
//client.println(F(" * {font: 20px 'Lucida Grande', Helvetica, Arial, sans-serif}"));
client.println(F("	table {border-collapse: collapse;}"));
client.println(F("	td, th {text-align: right; padding: 5px;}"));
//client.println(F("	#top {min-width: 40%}"));
//client.println(F("	th {font-weight: normal}"));
client.println(F("	input[type=text] {text-align: right}</style>"));
client.println(F("<meta charset='UTF-8'>"));
client.println(F("</head><body><center><h1>Arduino MQTT client "));
client.println(F("<input type='button' onclick='reload()' value=' Reload &#x21bb; '></h1></center>"));
client.println(F("<script>function reload() {location.href='/';}</script>"));
client.println(F("<hr><table><tr><th>Name:</th><th>"));
client.print(str_name);
client.println(F("</th></tr><th>IP address:</th><th>"));
client.print(String(ip[0],DEC)+"."+String(ip[1],DEC)+"."+String(ip[2],DEC)+"."+String(ip[3],DEC));
client.println(F("</th></tr><th>MAC address:</th><th>"));
client.print(String(mac[0],HEX)+":"+String(mac[1],HEX)+":"+String(mac[2],HEX)+":");
client.print(String(mac[3],HEX)+":"+String(mac[4],HEX)+":"+String(mac[5],HEX));
client.println(F("</th></tr><th>MQTT Server:</th><th>"));
//IP
client.print(String(mqtt_server[0],DEC)+"."+String(mqtt_server[1],DEC)+".");
client.print(String(mqtt_server[2],DEC)+"."+String(mqtt_server[3],DEC));
//DNS
//client.print(String(mqtt_server));

client.println(F("</th></tr></table><hr>"));
client.println(F("You may send any message to mqtt server<br>"));
client.println(F("<table><tr><th>Topic</th><th><input type='text' id='top'></th>"));
client.println(F("<th></th></tr>"));
client.println(F("<tr><th>payload</th><th><input type='text' id='pay'></th><th>"));
client.println(F("<input type='button' onclick='send()' value='Send &#x2714;'></th></tr></table>"));
client.println(F("<br><br>"));
client.println(F("topic structure: /device_name/(in/out)/(pin/item_name)<br>"));
client.println(F("payload: 0,1 or 0-100<br>"));
client.print(F("For example: topic = /"));
client.print(str_name);
client.println(F("/in/test<br>"));
client.println(F("and payload = test<br>"));
client.println(F("list of available topics: <br>"));
client.println(F("test (max 9 letters)<br>"));
client.println(F("blink (0,1)<br>"));
//client.println(F("<br>"));
client.println(F("<br><br>"));

client.println(F("<hr><script>"));
client.print(F("document.getElementById('top').value = '/"));
client.print(str_name);
client.println(F("/';"));
client.println(F("function send() { "));
client.print(F("var myGet = 'send=|';"));
client.print(F("myGet += document.getElementById('top').value + '|';"));
client.print(F("myGet += document.getElementById('pay').value + '|';"));
client.println(F("location.href = myGet; myGet='/';}"));

client.println(F("function reload() {location.href='/';}"));
client.println(F("</script>"));

					if(readString.indexOf("send=")>0)
					{
						my_echo(readString);
						client.print(F("<script>location.href='/';</script>"));
					}
					client.print(F("<center>lp:hjhome</center>"));
					client.println(F("</body></html>"));
					readString="";
					Serial.println(F("end"));
					client.stop();
					delay(1);
				}
			}
		}
	}
}

void my_echo(String readString)
{
	String sub_string[3];
	int j=0;
	for(int i=0; i<readString.length(); i++)
	{
		if(String(readString[i]) == "|")
		{
			if(j==1)
				sub_string[1]=sub_string[0];
			if(j==2)
			{
				sub_string[2]=sub_string[0];
				break;
			}
			sub_string[0]="";
			j++;
		}
		else
			sub_string[0] += readString[i];
	}
	Serial.println(F("web_client echo: "));
	Serial.print(F("topic: "));
	Serial.println(sub_string[1]);
	Serial.print(F("payload: "));
	Serial.println(sub_string[2]);
	if(sub_string[1].length()>0 && sub_string[1].length()>0)
	{
		str_topic = sub_string[1];
		str_payload = sub_string[2];
		update=true;
	}
}

//	mqtt	//

void my_mqtt_server()
{
	if(!mqtt_client.connected())
	{
		update=false;
		reconnect();
	}
	if(mqtt_client.connected())
	{
		if(millis()%3600001==1)
			update=true;
		delay(1);
	}
	mqtt_client.loop();
}
void callback(char* topic, byte* payload, unsigned int length)
{
	payload[length]='\0';
	String inc_topic=String(topic);
	String inc_payload=String((char*)payload);
	str_topic = "";
	str_payload = "";
	Serial.println(F("mqtt incoming... "));
	Serial.print(F("topic: "));
	Serial.println(inc_topic);
	Serial.print(F("payload: "));
	Serial.println(inc_payload);
	if(inc_topic.indexOf("/in/")>0)
	{
		if(inc_topic.indexOf("/blink")>0)
		{
			if(inc_payload=="0" || inc_payload=="false")
				blink=false;
			if(inc_payload=="1" || inc_payload=="true")
				blink=true;
			str_topic = String("/" + str_name + "/out/blink");
			str_payload = String(blink);
		}
		if(inc_topic.indexOf("/test")>0)
		{
			str_topic += String("/" + str_name + "/out/test");
			str_payload = inc_payload;
		}
		update=true;
	}
}

void reconnect()
{
	if(millis()%5001==1)
	{
		delay(1);
		String str_subscribe = String("/" + str_name + "/#");
		Serial.print(F("mqtt reconnect..."));
		wdt_disable();
		if(mqtt_client.connect(NULL))
		{
			char subscr[str_subscribe.length()+1];
			str_subscribe.toCharArray(subscr,str_subscribe.length()+1);
			mqtt_client.subscribe(subscr);
			Serial.print(F("mqtt connected subscribe: "));
			Serial.println(String(subscr));
			update=true;
		}
		else
			Serial.println(F("Failed to connect to the MQTT Server"));
		wdt_reset();
		wdt_enable(WDTO_8S);
	}
}

void my_print()
{
	Serial.println(F("mqtt send message... "));
	char topicStr[str_topic.length()+1];
	char messageStr[str_payload.length()+1];
	str_topic.toCharArray(topicStr,str_topic.length()+1);
	str_payload.toCharArray(messageStr,str_payload.length()+1);
	mqtt_client.publish(topicStr,messageStr);
	Serial.print(F("topic: "));
	Serial.println(topicStr);
	Serial.print(F("payload: "));
	Serial.println(messageStr);
}

//	Stage1******************************************************************

void my_automation()
{
	if(millis()%3001==1 && blink==true)
	{
		delay(1);
		if(digitalRead(7)==LOW)
			digitalWrite(7,HIGH);
		else
			digitalWrite(7,LOW);
		update=true;
		str_topic = String("/" + str_name + "/out/D7");
		str_payload = String(digitalRead(7));
	}
}

void my_update()
{
	if(update==true)
	{
		if(str_topic.length()>0 && str_payload.length()>0)
			my_print();
		update=false;
	}
}

