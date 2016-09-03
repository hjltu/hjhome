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
#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
////////////////////////////////
//	w5100
#include <SPI.h>
#include <Ethernet.h>

//	enc28j60
//#include <UIPEthernet.h>
/////////////////////////////////

//oneWire
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

//	http
EthernetServer server(80);

byte ip[]={192,168,0,23};
byte mac[]={0xAA,0xBB,0xCC,0xA4,0xA9,0xB6};

//	client name
String str_name="ard23";

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

bool bt[4],pwm[2],ir;
int count[4],temp[2],an;
byte dimm[2],dimming[4]={100,100,100,100};


void setup()
{
	wdt_disable();
	Serial.begin(9600);
	Serial.println(F("setup..."));
	//memory_clear();
	Serial.print(F("5 net..."));
	Ethernet.begin(mac, ip);
	server.begin();
	Serial.print(F(" myIP: "));
	Serial.println(Ethernet.localIP());
	delay(1000);
	Serial.print(F("4 mqtt..."));
	Serial.print(F(" name: "));
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
	Serial.println(F("3 pin mode..."));
	my_pin_mode();
	delay(1000);
	Serial.println(F("2 sensors..."));
	sensors.begin();
	delay(1000);
	Serial.println(F("1 set var..."));
	delay(1000);
	Serial.println(F("Start"));
	wdt_enable(WDTO_8S);
}

void loop()
{
	Stage0();
	Stage1();
	//Stage2();
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
	my_temp();	//	temp
	my_button();	//	A0-A3
	my_sensor();	//	A4
	my_analog();	//	A5
}

void Stage2()
{
	my_automation();
}
//	setup	//

// void memory_clear()
// {
// 	Serial.println(F("memory test..."));
// 	int shift=1000;
// 	char mydb[25];
// 	for(int i=0; i<25; i++)
// 	{
// 		mydb[i] = char(EEPROM.read(i+shift));
// 	}
// 	String check = String(mydb);
// 	Serial.println(check.length());
// 	check.trim();
// 	if(check=="mem-ok")	//if(check.indexOf("mem-o")>0)
// 		Serial.println(F("mem-ok"));
// 	else
// 	{
// 		Serial.println(F("clean mem..."));
// 		char ok[] = "mem-ok";
// 		for(int i=0; i<2048; i++)
// 		{
// 			EEPROM.write(i,0);
// 			delay(1);
// 		}
// 		for(int i=0; i<sizeof(ok); i++)
// 		{
// 			EEPROM.write(i+shift,ok[i]);
// 			delay(1);
// 			Serial.print(char(EEPROM.read(i+shift)));
// 		}
// 		Serial.println();
// 	}
// }

void my_pin_mode()
{
	//	D2	temperature
	pinMode(3, OUTPUT);	//	rele
	pinMode(4, OUTPUT);	//	rele
	pinMode(5,OUTPUT);	//	rele
	pinMode(6,OUTPUT);	//	rele
	pinMode(7,OUTPUT);	//	rele
	pinMode(8,OUTPUT);	//pwm
	pinMode(9,OUTPUT);	//pwm
	pinMode(A0, INPUT_PULLUP);	//button
	pinMode(A1, INPUT_PULLUP);	//button
	pinMode(A2, INPUT_PULLUP);	//button
	pinMode(A3, INPUT_PULLUP);	//button
	pinMode(A4, INPUT);	//0-5volt
	//	analogRead(A5)	0-1023

}

//	stage0***********************************************************************8

//	my_web_server	//

void my_web_server()
{
	EthernetClient client = server.available();
	String readString;
	if(client)
	{
		Serial.println(F("-> web..."));
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
client.println(F("	table {border-collapse: collapse;}"));
client.println(F("	td, th {text-align: right; padding: 5px;}"));
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
client.println(F("You may send any message to mqtt server:<br>"));
client.println(F("<table><tr><th>Topic</th><th><input type='text' id='top'></th>"));
client.println(F("<th></th></tr>"));
client.println(F("<tr><th>payload</th><th><input type='text' id='pay'></th><th>"));
client.println(F("<input type='button' onclick='send()' value='Send &#x2714;'></th></tr></table>"));
client.println(F("<br><br>"));
client.print(F("For example: topic = /"));
client.print(str_name);
client.println(F("/in/echo<br>"));
client.println(F("and payload = Hello!<br>"));
client.println(F("<br><br>"));

client.println(F("<script>"));
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

client.println(F("<hr><table><tr><th>pin</th><th>I/O</th><th>topic</th>"));
client.print(F("<th>payload</th></tr><tr><th>D2</th><th>temp</th><th>/"));
client.print(str_name);
client.println(F("/out/tm(1,2)</th><th>&#x2103</th></tr>"));
for(int i=0; i<7; i++)
{
	client.print(F("<tr><th>D"));
	client.print(i+3);
	if(i<5)
	{
		client.print(F("</th><th>rele</th><th>/"));
		client.print(str_name);
		client.print(F("/out/D"));
		client.print(i+3);
		client.println(F("</th><th>0,1</th></tr>"));
	}
	if(i>4)
	{
		client.print(F("</th><th>pwm</th><th>/"));
		client.print(str_name);
		client.print(F("/out/D"));
		client.print(i+3);
		client.println(F("</th><th>0,1-255</th></tr>"));
	}
}
for(int i=0; i<6; i++)
{
	client.print(F("<tr><th>A"));
	client.print(i);
	if(i<4)
	{
		client.print(F("</th><th>button</th><th>/"));
		client.print(str_name);
		client.print(F("/out/(bt1,dm1)"));
		client.println(F("</th><th>0,1-255</th></tr>"));
	}
	if(i>3)
	{
		client.print(F("</th><th>sensor</th><th>/"));
		client.print(str_name);
		client.print(F("/out/"));
		if(i==4)
			client.print(F("sn4</th><th>0,1</th></tr><tr>"));
		if(i==5)
			client.println(F("an5</th><th>0-1023</th></tr>"));
	}
}
client.print(F("</table>"));

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
	Serial.print(F("web topic: "));
	Serial.print(sub_string[1]);
	Serial.print(F(" payload: "));
	Serial.println(sub_string[2]);
	if(sub_string[1].length()>0 && sub_string[2].length()>0)
		if(sub_string[1].length()<100 && sub_string[2].length()<10)
		{
			str_topic = sub_string[1];
			str_payload = sub_string[2];
			my_print();
		}
}

//	mqtt	//

void my_mqtt_server()
{
	if(!mqtt_client.connected())
	{
		reconnect();
	}
	if(mqtt_client.connected())
	{
		if(millis()%3600001==1)
		{
			delay(1);
		}
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
	Serial.print(F("incoming top: "));
	Serial.print(inc_topic);
	Serial.print(F(" pay: "));
	Serial.println(inc_payload);
	if(inc_topic.indexOf("/in/")>0)
	{
		if(inc_topic.indexOf("/echo")>0)
		{
			str_topic = String("/" + str_name + "/out/echo");
			str_payload = inc_payload;
			my_print();
		}
		if(inc_topic.indexOf("/test")>0)
		{
			for(int i=2; i<10; i++)
			{
				if(i==2)
				{
					str_topic = String("/" + str_name + "/out/temp" + 1);
					str_payload = String(temp[0]);
					my_print();
					str_topic = String("/" + str_name + "/out/temp" + 2);
					str_payload = String(temp[1]);
					my_print();
				}
				if(i>2 && i<8)
				{
					str_topic = String("/" + str_name + "/out/D" + i);
					str_payload = String(digitalRead(i));
					my_print();
				}
				if(i>7 && i<10)
				{
					str_topic = String("/" + str_name + "/out/D" + i);
					if(pwm[i-8]==false)
						str_payload = String("0");
					if(pwm[i-8]==true)
						str_payload = String(dimm[i-8]);
					my_print();
				}
			}
		}
		if(inc_topic.indexOf("/D")>0)
		{
			for(int i=3; i<10; i++)
			{
				if(inc_topic.indexOf("/D" + String(i))>0)
				{
					if(i>2 && i<8)
					{
						if(inc_payload=="0" || inc_payload=="false" || inc_payload=="OFF")
							if(digitalRead(i)==HIGH)
							{
								digitalWrite(i,LOW);
								str_topic = String("/" + str_name + "/out/D" + i);
								str_payload = inc_payload;
								my_print();
							}
		 				if(inc_payload=="1" || inc_payload=="true" || inc_payload=="ON")
							if(digitalRead(i)==LOW)
							{
								digitalWrite(i,HIGH);
								str_topic = String("/" + str_name + "/out/D" + i);
								str_payload = inc_payload;
								my_print();
							}
					}
					if(i>7 && i<10)
					{
						if(inc_payload.toInt()>0)
						{
							dimm[i-8]=inc_payload.toInt();
							pwm[i-8]=true;
							analogWrite(i,dimm[i-8]);
							str_topic = String("/" + str_name + "/out/D" + i);
							str_payload = String(dimm[i-8]);
							my_print();
						}
						if(inc_payload.toInt()==0)
						{
							pwm[i-8]=false;
							digitalWrite(i,LOW);
							str_topic = String("/" + str_name + "/out/D" + i);
							str_payload = "0";
							my_print();
						}
					}
				}
			}
		}
	}
}

void reconnect()
{
	if(millis()%25001==2)
	{
		delay(1);
		String str_subscribe = String("/" + str_name + "/#");
		Serial.println(F("mqtt reconnect..."));
		if(mqtt_client.connect(NULL))
		{
			char subscr[str_subscribe.length()+1];
			str_subscribe.toCharArray(subscr,str_subscribe.length()+1);
			mqtt_client.subscribe(subscr);
			Serial.print(F("ok! subscribe: "));
			Serial.println(String(subscr));
		}
	}
}

void my_print()
{
	if(str_topic.length()>0 && str_payload.length()>0)
		if(str_topic.length()<50 && str_payload.length()<10)
		{
			char topicStr[str_topic.length()+1];
			char messageStr[str_payload.length()+1];
			str_topic.toCharArray(topicStr,str_topic.length()+1);
			str_payload.toCharArray(messageStr,str_payload.length()+1);
			mqtt_client.publish(topicStr,messageStr);
			Serial.print(F("mqtt --> "));
			Serial.print(F(" topic: "));
			Serial.print(topicStr);
			Serial.print(F(" payload: "));
			Serial.println(messageStr);
		}
}

//	Stage1******************************************************************

void my_temp()
{
	if(millis()%5001==3)
	{
		delay(1);
		sensors.requestTemperatures();
		if(int(sensors.getTempCByIndex(0)) !=temp[0])
		{
			temp[0]=sensors.getTempCByIndex(0);
			str_topic = String("/" + str_name + "/out/temp" + 1);
			str_payload = String(temp[0]);
			my_print();
		}
		if(int(sensors.getTempCByIndex(1)) !=temp[1])
		{
			temp[1]=sensors.getTempCByIndex(1);
			str_topic = String("/" + str_name + "/out/temp" + 2);
			str_payload = String(temp[1]);
			my_print();
		}
	}
}

void my_button()
{
	for(int i=14; i<18; i++)
	{
		if(digitalRead(i)==LOW)
		{
			delay(1);
			count[i-14]++;
		}
		if(digitalRead(i)==HIGH && count[i-14]>50 && count[i-14]<500)
		{
			count[i-14]=0;
			if(bt[i-14]==false)
				bt[i-14]=true;
			else
				bt[i-14]=false;
			str_topic = String("/" + str_name + "/out/bt" + (i-14));
			str_payload = String(bt[i-14]);
			my_print();
		}
		if(count[i-14]>500 && millis()%50==4)
		{
			delay(1);
			dimming[i-14]=1+pow(dimming[i-14],1.0054);
			str_topic = String("/" + str_name + "/out/dm" + (i-14));
			str_payload = String(dimming[i-14]);
			my_print();
			if(dimming[i-14]>250)
				count[i-14]=0;
		}
		if(digitalRead(i)==HIGH && count[i-14]>0)
			count[i-14]=0;
	}
}

void my_sensor()
{
	if(ir != digitalRead(A4))
	{
		ir=digitalRead(A4);
		str_topic = String("/" + str_name + "/out/ir" + 4);
		str_payload = String(ir);
		my_print();
	}
}

void my_analog()
{
	if(an != analogRead(A5) && millis()%60001==5)
	{
		delay(1);
		an=analogRead(A5);
		str_topic = String("/" + str_name + "/out/an" + 5);
		str_payload = String(an);
		my_print();
	}
}

//	Stage 2	////////////////////////////////////////////////

void my_automation()
{
	//	node-red
}