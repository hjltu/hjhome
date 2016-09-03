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

// Change yourhost to Server address(IP or DNS)
var mqttServer = 'mqtt://yourhost';

var http = require('http');
var fs = require('fs');
var mqtt = require('mqtt');
var WS = new require('ws');
var auth = require('http-auth');
var basic = auth.basic( {realm: "Welcome frend."}, function (username, password, callback) {
	callback(username === "user" && password === "password");
});

//	no authentication

http.createServer(function(req, res) {
	if(req.url == "/") {
		var file = new fs.ReadStream("index.html", {encoding: 'utf-8'});
		sendFile(file,res);
	}
	if(req.url == "/jquery-1.8.2.min.js") {
		var file = new fs.ReadStream("jquery-1.8.2.min.js");
		sendFile(file,res);
	}
	if(req.url == "/index.js") {
		var file = new fs.ReadStream("index.js");
		sendFile(file,res);
	}
	if(req.url == "/index.css") {
		var file = new fs.ReadStream("index.css");
		sendFile(file,res);
	}
}).listen(3000);
console.log('Server running localhost:3000');

//	basic authentication

var httpServer = http.createServer(basic, function (req, res) {
	console.log(req.method, req.url);//, req.headers);
	switch(req.method) {
		case "GET":
			if(req.url == "/") {
				var file = new fs.ReadStream("index.html", {encoding: 'utf-8'});
				sendFile(file,res);
			}
			else if(req.url == "/jquery-1.8.2.min.js") {
				var file = new fs.ReadStream("jquery-1.8.2.min.js");
				sendFile(file,res);
			}
			else if(req.url == "/index.js") {
				var file = new fs.ReadStream("index.js");
				sendFile(file,res);
			}
			else if(req.url == "/index.css") {
				var file = new fs.ReadStream("index.css");
				sendFile(file,res);
			}
			else
				get404(req, res);
			break;
		case "POST":
			break;
		default:
			get405(req, res);
			break;
	}
}).listen(3001);

console.log('Server running localhost:3001');

function sendFile(file,res) {
	file.pipe(res);
	file.on('error', function(err) {
		get500(req,res);
	});
	res.on('close', function() {
		file.destroy();
	});
}

function get404(req, res) {
	res.writeHead(404, "Resourse not found",{'Content-Type': 'text/plain'});
	res.write("404 Resourse not found : " + req.url);
	res.end();
}

function get405(req, res) {
	res.writeHead(405, "Something happend",{'Content-Type': 'text/plain'});
	res.write("405 Something happend");
	res.end();
}

function get500(req, res) {
	res.writeHead(500, "Server ERROR",{'Content-Type': 'text/plain'});
	res.write("405 Server ERROR");
	res.end();
}

//	mqtt

var client = mqtt.connect(mqttServer);
var topics, messages;
client.on('connect', function () {
	client.subscribe('/home/out/#');
	client.publish('/home/in/test', 'test');
});

//	websocket

var clients = {};
var webSocketServer = new WS.Server({port: 3002});

webSocketServer.on('connection', function(ws) {

	var id = Math.random();
	clients[id] = ws;
	console.log("new connection " + id);

	ws.on('message', function(message) {
		console.log('incoming ' + message);
		var arr = message.split(":");
		if(arr[0] === "mqtt")
			client.publish(arr[1], arr[2]);
		if(arr[0] === "voice")
			myVoice(arr[1]);
	});

	ws.on('close', function() {
		console.log('close connection ' + id);
		delete clients[id];
	});

	client.on('message', function (topic, message) {
		topics = topic.toString();
		messages = message.toString();
		console.log(topics + " : " + messages);
		for (var key in clients) {
			clients[key].send("mqtt:" + topics + ":" + messages);
			// if(topics === "/home/out/heat13") {
			// 	if(messages === "0")
			// 		clients[key].send("voice:" + "heat is off");
			// 	if(messages === "1")
			// 		clients[key].send("voice:" + "heat is on");
			// }
		}
	});
});

function myVoice(voice) {
	if(voice === "test")
		client.publish("/home/in/test", "test");
	if(voice === "включить")
		client.publish("/home/in/led6", "1");
	if(voice === "выключить")
		client.publish("/home/in/led6", "0");
	console.log("voicecommand is: " + voice);
}

