#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid =  "your-ssid";
const char *pass =  "password";
String client_name = "esp-name";

WiFiClient wclient;
IPAddress mqtt_server(192, 168, 0, 31);	//ip address mqtt server
PubSubClient client(wclient, mqtt_server);

WiFiServer web_server(80);

String str_topic;
String str_payload;

void setup()
{
	Serial.begin(115200);
	Serial.println("");
	pinMode(2, OUTPUT);
	digitalWrite(2, 0);
	WiFi.begin(ssid, pass);
	web_server.begin();
	Serial.println(F("Web Server started"));

}

void loop()
{
	if(millis()%5001==1)
		my_connect();
	my_web();
	my_automation();

}

void my_connect()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(F("Connecting to "));
		Serial.print(ssid);
		Serial.println(F("..."));
		WiFi.begin(ssid, pass);

		if(WiFi.waitForConnectResult() != WL_CONNECTED)
			return;
		Serial.println(F("WiFi connected"));
	}
	if(WiFi.status() == WL_CONNECTED)
	{
		if(!client.connected())
		{
			Serial.println(F("Connecting to MQTT server"));
			if(client.connect(client_name))
			{
				Serial.println(F("Connected to MQTT server"));
				client.set_callback(callback);
				client.subscribe("/" + client_name + "/#");
				Serial.println("Subscribe to : /" + client_name + "/#");
			}
			else
				Serial.println(F("Could not connect to MQTT server"));
		}
		if(client.connected())
			client.loop();
	}
}

void callback(const MQTT::Publish& pub)
{
	Serial.print(pub.topic());
	Serial.print(F(" => "));
	Serial.println(pub.payload_string());
	
	String topic = pub.topic();
	String payload = pub.payload_string();
	
	if(topic.indexOf(String("/in/"))>0)
	{
		if(topic.indexOf("/gpio2")>0)
		{
			if(payload.toInt()==0 || payload=="false" || payload=="OFF")
				digitalWrite(2,LOW);
			if(payload.toInt()==1 || payload=="true" || payload=="ON")
				digitalWrite(2,HIGH);
			str_topic = String("/" + client_name + "/out/gpio2");
			str_payload = String(digitalRead(2));
		}
		if(topic.indexOf("/test")>0 || topic.indexOf("/echo")>0)
		{
			str_topic = String("/" + client_name + "/out/echo");
			str_payload = payload;
		}
		my_print();
	}
}

void my_web()
{
	WiFiClient client = web_server.available();
	if(!client)
		return;
	Serial.println(F("new client"));
	while(!client.available())
		delay(1);
	String req = client.readStringUntil('\n'); //r
	Serial.println(req);
	client.flush();
	Serial.println(req.length());
// String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n";
// s += "It works!";
// client.print(s);
if(req.length()<100)
{
client.println(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"));
client.println(F("<!DOCTYPE html><html><head><title>arduino</title>"));
client.println(F("<style>body {padding: 3%}"));
client.println(F("table {border-collapse: collapse;}"));
client.println(F("td, th {text-align: right; padding: 5px;}"));
client.println(F("input[type=text] {text-align: right}</style>"));
client.println(F("<meta charset='UTF-8'>"));
client.println(F("</head><body><center><h1>ESP8266 MQTT client "));
client.println(F("<input type='button' onclick='reload()' value=' Reload &#x21bb; '>"));
client.println(F("</h1></center>"));
client.println(F("<script>function reload() {location.href='/';}</script>"));
client.println(F("<hr><table><tr><th>Name:</th><th>"));
client.print(client_name);
client.println(F("</th></tr><th>IP address:</th><th>"));
client.print(WiFi.localIP());
client.println(F("</th></tr><th>MAC address:</th><th>"));
client.print(WiFi.macAddress());
client.println(F("</th></tr><th>MQTT Server:</th><th>"));
client.print(String(mqtt_server[0],DEC)+"."+String(mqtt_server[1],DEC)+".");
client.print(String(mqtt_server[2],DEC)+"."+String(mqtt_server[3],DEC));
client.println(F("</th></tr></table><hr>"));
client.println(F("You may send any message to mqtt server<br>"));
client.println(F("<table><tr><th>Topic</th><th><input type='text' id='top'></th>"));
client.println(F("<th></th></tr>"));
client.println(F("<tr><th>payload</th><th><input type='text' id='pay'></th><th>"));
client.println(F("<input type='button' onclick='send()' value='Send &#x2714;'>"));
client.println(F("</th></tr></table>"));
client.println(F("<br><br>"));
client.println(F("topic structure: /device_name/(in/out)/(item_name)<br>"));
client.println(F("payload: 0,1 or 1-255<br>"));
client.print(F("For example: topic = /"));
client.print(client_name);
client.println(F("/in/test<br>"));
client.println(F("and payload = echo<br>"));
client.println(F("list of available topics: <br>"));
client.println(F("test (echo)<br>"));
client.println(F("gpio2 (0,1)<br>"));
client.println(F("<br><br>"));

client.println(F("<hr><script>"));
client.print(F("document.getElementById('top').value = '/"));
client.print(client_name);
client.println(F("/';"));
client.println(F("function send() { "));
client.print(F("var myGet = 'send=|';"));
client.print(F("myGet += document.getElementById('top').value + '|';"));
client.print(F("myGet += document.getElementById('pay').value + '|';"));
client.println(F("location.href = myGet; myGet='/';}"));
client.println(F("function reload() {location.href='/';}"));
client.println(F("</script>"));

if(req.indexOf("send=") != -1)
{
	my_echo(req);
	client.print(F("<script>location.href='/';</script>"));
}
client.print(F("<center>lp:hjhome</center>"));
client.println(F("</body></html>"));
req="";
Serial.println(F("http end"));
}
client.flush();
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

void my_print()
{
	if(str_topic.length()>0 && str_payload.length()>0)
		if(str_topic.length()<50 && str_payload.length()<10)
		{
			client.publish(str_topic,str_payload);
			Serial.print(F("send ... "));
			Serial.print(F(" topic: "));
			Serial.print(str_topic);
			Serial.print(F(" payload: "));
			Serial.println(str_payload);
		}
}

void my_automation()
{

}
