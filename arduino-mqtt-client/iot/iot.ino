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

//13/05/16
#include <avr/wdt.h>
#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//mqtt
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

//network
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED};

//mqtt server
byte localserver[] = {192, 168, 0, 25};

//my ip
byte ip[] = {192, 168, 0, 20 };

char clientName[] = "iot20";

EthernetClient ethClient;
PubSubClient client(ethClient);

char topicStr[30];
char messageStr[10];

//oneWire
OneWire oneWire(2);
DallasTemperature sensors(&oneWire);

//buttons
int countA1=0,ir0last=0;

int temp1=0,temp2=0,dimming3=0;
bool update=false;
bool ir0=false,dimm3=false;
bool led3=true,led6=true,heat13=false;

void setup()
{
	wdt_disable();	////////////////////////////////watchdog
	Serial.begin(9600);

	client.setServer(localserver, 1883);
	client.setCallback(callback);
	Ethernet.begin(mac, ip);

	sensors.begin();

	pinMode(3, OUTPUT);  	//led3pwm
	pinMode(6, OUTPUT);  	//led6
	pinMode(13, OUTPUT);	//heat13
	pinMode(A0, INPUT);	//pir sensor, pin A0
	pinMode(A1, INPUT_PULLUP);	//push LOW

	Serial.println("Start in 5");
	delay(1000);
	Serial.println("Start in 4");
	delay(1000);
	Serial.println("Start in 3");
	delay(1000);
	Serial.println("Start in 2");
	delay(1000);
	Serial.println("Start in 1");
	delay(1000);
	Serial.print("Start ");
	Serial.println(clientName);
	Serial.print("mac=");
	Serial.print(mac[0],HEX);
	Serial.print(":");
	Serial.print(mac[1],HEX);
	Serial.print(":");
	Serial.print(mac[2],HEX);
	Serial.print(":");
	Serial.print(mac[3],HEX);
	Serial.print(":");
	Serial.print(mac[4],HEX);
	Serial.print(":");
	Serial.println(mac[5],HEX);
	Serial.print("ip=");
	Serial.print(ip[0]);
	Serial.print(".");
	Serial.print(ip[1]);
	Serial.print(".");
	Serial.print(ip[2]);
	Serial.print(".");
	Serial.println(ip[3]);
	Serial.print("server=");
	Serial.print(localserver[0]);
	Serial.print(".");
	Serial.print(localserver[1]);
	Serial.print(".");
	Serial.print(localserver[2]);
	Serial.print(".");
	Serial.println(localserver[3]);
	wdt_enable(WDTO_8S);		////////////////////watchdog
}

void loop()
{
	Stage0();
	Stage1();
	Stage2();

	wdt_reset();    ////////////////////////////////watchdog
}

void Stage0()
{
	if(!client.connected())
	{
		update=false;
		reconnect();
	}
	if(client.connected())
	{
		if(millis()%3600001==1)
			update=true;
		delay(1);
	}
	client.loop();
}

void Stage1()
{
	myTemp();
	myA0(); //pir-sensor
	myA1(); //buttonA1 Dimm out-3
}

void Stage2()
{
	if(update==true)
		myUpdate();
	myOutput();
}

/////////////////////	1	///////////////////////////////////////////////////////

void reconnect()
{
	if(millis()%20001==1)
	{
		delay(1);
		Serial.println("reconnect...");
		if(client.connect(clientName))
		{
			client.subscribe("/home/in/#");
			Serial.println("ok");
			update=true;
		}
	}
}

void callback(char* topic, byte* payload, unsigned int length)
{
	payload[length]='\0';
	String strTopic=String(topic);
	String strPayload=String((char*)payload);

	Serial.print("debug ");
	Serial.print(strTopic);
	Serial.print(" : ");
	Serial.println(strPayload);

	if(strTopic=="/home/in/led3")
	{
		if(strPayload.toInt()==0 || strPayload.toInt()==1)
		{
			led3=true;
			if(EEPROM.read(3) != strPayload.toInt())
				EEPROM.write(3,strPayload.toInt());
		}
	}
	if(strTopic=="/home/in/dimm3")
	{
		if(strPayload.toInt()>0 && strPayload.toInt()<256)
		{
			dimm3=true;
			if(strPayload.toInt() != EEPROM.read(203))
				EEPROM.write(203,strPayload.toInt());
			String("/home/out/dimm3").toCharArray(topicStr,30);
			String(EEPROM.read(203)).toCharArray(messageStr,10);
			myPrint();
		}
	}
	if(strTopic=="/home/in/led6")
	{
		if(strPayload.toInt()==0 || strPayload.toInt()==1)
		{
			led6=true;
			if(EEPROM.read(6) != strPayload.toInt())
				EEPROM.write(6,strPayload.toInt());
		}
	}
	if(strTopic=="/home/in/temp1set")
	{
		if(strPayload.toInt()>9 && strPayload.toInt()<36)
		{
			if(strPayload.toInt() != EEPROM.read(101))
				EEPROM.write(101,strPayload.toInt());
			String("/home/out/temp1set").toCharArray(topicStr,30);
			String(EEPROM.read(101)).toCharArray(messageStr,10);
			myPrint();
		}
	}
	if(strPayload=="test")
	{
		update=true;
		Serial.println("test");
	}
}

void myUpdate()
{
	Serial.println("update");
	update=false;
	String("/home/out/led3").toCharArray(topicStr,30);
	String(EEPROM.read(3)).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/dimm3").toCharArray(topicStr,30);
	String(EEPROM.read(203)).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/led6").toCharArray(topicStr,30);
	String(digitalRead(6)).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/heat13").toCharArray(topicStr,30);
	String(digitalRead(13)).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/temp1").toCharArray(topicStr,30);
	String(temp1).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/temp2").toCharArray(topicStr,30);
	String(temp2).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/temp1set").toCharArray(topicStr,30);
	String(EEPROM.read(101)).toCharArray(messageStr,10);
	myPrint();
	String("/home/out/ir0").toCharArray(topicStr,30);
	String(ir0last).toCharArray(messageStr,10);
	myPrint();
}

void myTemp()
{
	if(millis()%5001==1)
	{
		delay(1);
		sensors.requestTemperatures();
		if(int(sensors.getTempCByIndex(0)) !=temp1)
		{
			temp1=sensors.getTempCByIndex(0);
			String("/home/out/temp1").toCharArray(topicStr,30);
			String(temp1).toCharArray(messageStr,10);
			myPrint();
		}
		if(int(sensors.getTempCByIndex(1)) !=temp2)
		{
			temp2=sensors.getTempCByIndex(1);
			String("/home/out/temp2").toCharArray(topicStr,30);
			String(temp2).toCharArray(messageStr,10);
			myPrint();
		}
		if(temp1<EEPROM.read(101) && digitalRead(13)==LOW)
			heat13=true;
		if(temp1>EEPROM.read(101) && digitalRead(13)==HIGH)
			heat13=true;
	}
}

void myA0()
{
	if(ir0last != digitalRead(A0))
	{
		ir0=true;
		ir0last=digitalRead(A0);
		String("/home/out/ir0").toCharArray(topicStr,30);
		String(digitalRead(A0)).toCharArray(messageStr,10);
		myPrint();
	}
}

void myA1()
{
	if(digitalRead(A1)==LOW)
	{
		countA1++;
		delay(1);
	}
	if(countA1>10 && countA1<400 && digitalRead(A1)==HIGH)
	{
		led3=true;
		if(EEPROM.read(3)==LOW)
			EEPROM.write(3,HIGH);
		else
			EEPROM.write(3,LOW);
	}
	if(countA1>500)
	{
		dimm3=true;
		delay(150);
		dimming3=EEPROM.read(203);
		dimming3++;
		if(dimming3>10)
			dimming3++;
		if(dimming3>50)
			dimming3++;
		if(dimming3>100)
			dimming3++;
		if(dimming3>255)
			dimming3=1;
			EEPROM.write(203,dimming3);
		String("/home/out/dimm3").toCharArray(topicStr,30);
		String(EEPROM.read(203)).toCharArray(messageStr,10);
		myPrint();
	}
	if(digitalRead(A1)==HIGH && countA1>0)
	{
		delay(200);
		countA1=0;
	}
}

/////////////////	2	///////////////////////////////////

void myOutput()
{

//	led3

	if(led3==true)
	{
		led3=false;
		if(EEPROM.read(3)==HIGH)
			analogWrite(3,EEPROM.read(203));
		else
			analogWrite(3,0);
		String("/home/out/led3").toCharArray(topicStr,30);
		String(EEPROM.read(3)).toCharArray(messageStr,10);
		myPrint();
	}

	if(dimm3==true)
	{
		dimm3=false;
		if(EEPROM.read(3)==HIGH)
			analogWrite(3,EEPROM.read(203));
		String("/home/out/led3").toCharArray(topicStr,30);
		String(EEPROM.read(3)).toCharArray(messageStr,10);
		myPrint();

	}

//	led6

	if(led6==true)
	{
		led6=false;
		if(EEPROM.read(6)==HIGH)
			digitalWrite(6,HIGH);
		else
			digitalWrite(6,LOW);
		String("/home/out/led6").toCharArray(topicStr,30);
		String(digitalRead(6)).toCharArray(messageStr,10);
		myPrint();
	}

	if(ir0==true)
	{
		ir0=false;
		if(EEPROM.read(6)==LOW)
		{
			if(digitalRead(A0)==HIGH)
				digitalWrite(6,HIGH);
			else
				digitalWrite(6,LOW);
			String("/home/out/led6").toCharArray(topicStr,30);
			String(digitalRead(6)).toCharArray(messageStr,10);
			myPrint();
		}
	}

//	heat13

	if(heat13==true)
	{
		heat13=false;
		if(temp1<EEPROM.read(101))
			digitalWrite(13,HIGH);
		else
			digitalWrite(13,LOW);
		String("/home/out/heat13").toCharArray(topicStr,30);
		String(digitalRead(13)).toCharArray(messageStr,10);
		myPrint();
	}
}

void myPrint()
{
	client.publish(topicStr,messageStr);
	Serial.print(topicStr);
	Serial.print(" : ");
	Serial.println(messageStr);
}