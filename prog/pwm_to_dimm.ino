// Copyright (c) 2016  hjltu@ya.ru

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files
// (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of the Software,
// and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

//pwm2triac dimmer
// 5v, 16MHz, 168-328
// A0 - cond 220mkF 16-25v
//10-nov-18 pwm 3.3v,5v 1kHz work 0-990-1023
//21-nov-18 5v sw=false, 3v3 sw=true an*1.47 & an<680
//23-nov-18 pin 9 pullup

//unsigned int an,count,dimm,ac_dimm=9,sync,wait;
unsigned int an,shift,dimm,count;
bool sw;

void setup()
{
	Serial.begin(9600);
	Serial.println("Setup...");
	pinMode(3,OUTPUT);                        // Set AC Dimmer
	pinMode(8,OUTPUT);                        // jumper 
	pinMode(13,OUTPUT);                        // Set AC Dimmer
	pinMode(9,INPUT_PULLUP);                         // 3.3-5v
	delay(100);
	//attachInterrupt(0, my_dimm, RISING); //pin 2
	attachInterrupt(0, my_dimm, RISING); //pin 2
	Serial.println("start");
}

void loop()
{
	my_analog();
    my_sw();
    //if(count>50)
    //    my_delay();
	if(millis()%2999==0)
	    my_print();

}

void my_analog()
{
    if(analogRead(A0)>an)
    {
        if(sw==false)
            if(an<1022)
                an++;
        if(sw==true)
            if(an<680)
                an++;
    }
    if(analogRead(A0)<an)
        if(an>0)
            an--;
}

void my_sw()
{
    if(digitalRead(9)==LOW && count<15)
        count++;
    if(digitalRead(9)==HIGH && count>0)
        count--;
    if(count>10 && sw==false)
    {
        sw=true;                //3v3
        digitalWrite(13,HIGH);
        delay(999);
        digitalWrite(13,LOW);
    }
    if(count<5 && sw==true)
    {
        sw=false;               // 5v
        digitalWrite(13,HIGH);
        delay(299);
        digitalWrite(13,LOW);
    }
    if(sw==true)
        shift=an*1.47;
    if(sw==false)
        shift=an;
}

void my_dimm()
{
    if(an>0)
    {
	    delayMicroseconds(499);     //499
	    //delayMicroseconds(8500-an*8);    // 45
	    delayMicroseconds(9000-shift*8);    // 45
	    digitalWrite(3, HIGH);
	    delayMicroseconds(10);
	    digitalWrite(3, LOW);
    }
}

void my_blink(int pin)
{
    digitalWrite(pin,HIGH);
    delay(9);
    digitalWrite(pin,LOW);
}

void my_print()
{
    Serial.print("read:an:sw:shift = ");
	Serial.print(analogRead(A0));
    Serial.print(":");
	Serial.print(an);
    Serial.print(":");
	Serial.print(sw);
    Serial.print(":");
	Serial.println(shift);
    my_blink(13);
}
