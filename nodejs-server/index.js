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


//change yourhost to your server (IP or DNS)
var websocketServer = "ws://yourhost:3002";

var mqtt = "mqtt:";
var color = '';
var lang = '';
var voice = "voice:"
var mobile = true;

//websocket
// Create a client instance

var socket = new WebSocket(websocketServer);

socket.onopen = function() {
	console.log("new connection");
	socket.send(mqtt + "/home/in/test:test");
	console.log("send " + mqtt + "/home/in/test:test");
};

socket.onclose = function(event) {
	if(event.wasClean) {
		console.log('close connection');
	}
	else {
		console.log('connection error');
	}
	console.log('code: ' + event.code + ' reason: ' + event.reason);
};

socket.onmessage = function(event) {
	console.log("incoming data " + event.data);
	var arr = (event.data).split(":");
	if(arr[0] === "mqtt") {
		$("#topics").html(" " + arr[1] + " : ");
		$("#payload").html(" " + arr[2]);
		if(arr[1] === "/home/out/dimm3") {
			$("#dimmer").html(arr[2]);
			$("#dimm3").val(arr[2]);
		}
		if(arr[1] === "/home/out/temp1")
			$("#temp1").html(arr[2] + "&#x2103");
		if(arr[1] === "/home/out/temp2")
			$("#temp2").html(arr[2] + "&#x2103");
		if(arr[1] === "/home/out/ir0") {
			if(arr[2] === "0")
				$("#ir0").html("OFF");
			if(arr[2] === "1")
				$("#ir0").html("ON");
		}
		if(arr[1] === "/home/out/temp1set") {
			$("#temp1set").val(arr[2]);
			$("#temp").html(arr[2] + "&#x2103");
		}
		if(arr[1] === "/home/out/heat13") {
			if(arr[2] === "0")
				$("#heat13").html("OFF");
			if(arr[2] === "1")
				$("#heat13").html("ON");
		}
		if(arr[1] === "/home/out/led3")
		{
			$("#led3").val(arr[2]);
			if(arr[2] === "0")
				document.getElementById("led3").style.background = '#f2f2f2';
			if(arr[2] === "1")
				document.getElementById("led3").style.background = color;
		}
		if(arr[1] === "/home/out/led6")
		{
			$("#led6").val(arr[2]);
			if(arr[2] === "0")
				document.getElementById("led6").style.background = '#f2f2f2';
			if(arr[2] === "1")
				document.getElementById("led6").style.background = color;
		}
	}
	if(arr[0] === "voice" && mobile === true) {
			speak(arr[1]);
	}
};

socket.onerror = function(error) {
	console.log("ERROR " + error.message);
};

//	start

$(document).ready(function() {
	letters = ['#6600cc','#0099ff','#009999','#cc6600','#cc0066'];
	color = letters[Math.floor(Math.random() * letters.length)];
	document.getElementsByClassName("head")[0].style.backgroundColor = color;

	$("#speak").click(function() {
		$("#myspeak").fadeToggle("slow");
		lang = document.getElementById("selectlang").value;
		$("#speaktxt").html("Language = " + lang);
	});

	//	check mobile or desktop

	myNavigator(mobile);
	console.log("user agent = " + mobile);
});

//	voice

function myMic() {
	if (!('webkitSpeechRecognition' in window))
		upgrade();
	else {
		var recognition = new webkitSpeechRecognition();
		if(lang === "English")
			recognition.lang = 'en';
		if(lang === "Русский")
			recognition.lang = 'ru';
		recognition.continuous = false;
		recognition.interimResults = false;
		var req = '';
	}

	recognition.onresult = function (event) {
		for (var i = event.resultIndex; i < event.results.length; ++i)
			if (event.results[i].isFinal)
				req=event.results[i][0].transcript;
	};

	recognition.onend = function() { 
		if (req.length>0) {
			$("#speaktxt").html("Out : " + req.substring(0, 25) + '...');
			speak(req);
			message = voice + req;
			socket.send(message);
			console.log("send " + message);
		}
	};
	recognition.start();
}

// say a message

function speak(text, callback) {
	var u = new SpeechSynthesisUtterance();
	u.text = text;
	if(lang === "English")
		u.lang = 'en-US';
	if(lang === "Русский")
		u.lang = 'ru-RU';
	u.onend = function () {
		if (callback) {
			callback();
		}
	};
	u.onerror = function (e) {
		if (callback) {
			callback(e);
		}
	};
	speechSynthesis.speak(u);
}

//	working

function myDimm3() {
	if(mobile === false) {
		message = mqtt + "/home/in/dimm3:" + $("#dimm3").val();
		socket.send(message);
		console.log("send " + message);
	}
}

$("#dimm3").on("touchend", function() {
	message = mqtt + "/home/in/dimm3:" + $("#dimm3").val();
	socket.send(message);
	console.log("send " + message);
});

function myON() {
	message = mqtt + $("#topic").val()+":1";
	socket.send(message);
	console.log("send " + message);
}

function myOFF() {
	message = mqtt + $("#topic").val() + ":0";
	socket.send(message);
	console.log("send " + message);
}

function myTemp() {
	if(mobile === false) {
		message = mqtt + "/home/in/temp1set:" + $("#temp1set").val();
		socket.send(message);
		console.log("send " + message);
	}
}

$("#temp1set").on("touchend", function() {
	message = mqtt + "/home/in/temp1set:" + $("#temp1set").val();
	socket.send(message);
	console.log("send " + message);
});

function myLed3() {
	message = mqtt + "/home/in/led3:" + $("#led3").val();
	socket.send(message);
	console.log("send " + message);
}

function myLed6() {
	message = mqtt + "/home/in/led6:" + $("#led6").val();
	socket.send(message);
	console.log("send " + message);
}

function myCam() {
	var data = "";
	if($("#cm").val() === "0") {
		data = "";
		document.getElementById("cm").style.background = '#f2f2f2';
	}
	if($("#cm").val() === "1") {
		data = "<img src=\"your_camera_ip\">";
		document.getElementById("cm").style.background = color;
	}
	$("#cam").html(data);
}


function myNavigator() {
	if( navigator.userAgent.match(/Android/i)
	|| navigator.userAgent.match(/webOS/i)
	|| navigator.userAgent.match(/iPhone/i)
	|| navigator.userAgent.match(/iPad/i)
	|| navigator.userAgent.match(/iPod/i)
	|| navigator.userAgent.match(/BlackBerry/i)
	|| navigator.userAgent.match(/Windows Phone/i))
		mobile = true;
	else
		mobile = false;
}

