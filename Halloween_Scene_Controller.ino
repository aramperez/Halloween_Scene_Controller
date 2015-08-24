// The MIT License (MIT)
//
// Copyright (c) 2015 Aram Perez
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <DigitalIO.h> //Download from https://github.com/aramperez/digital-io
#include <Servo.h>
#include "pitches.h"

#define SERVO_MIN 5
#define SERVO_MAX 155

//
// Pin Definitions
//
#define AC1_PIN 2
#define AC2_PIN 3
#define PIR_PIN 4
#define OWL_PIN 5
#define SERVO_PIN 6
#define SPEAKER_PIN 7

DigitalInput pir(PIR_PIN);
DigitalOutputAL owl(OWL_PIN);
DigitalOutputAH ac1(AC1_PIN);
DigitalOutputAH ac2(AC2_PIN);
Servo servo;

void setup()
{
  Serial.begin(115200);
  randomSeed(analogRead(0));
  servo.attach(SERVO_PIN);
  Serial.println("Setup completed.");
}

void loop()
{
  static bool motionDetected = false;
  static unsigned long  owlStart, owlDuration,
         ac1Start, ac1Duration,
         ac2Start, ac2Duration,
         noteStart, noteDuration,
         servoStart, servoDuration;
  if ( pir.Read() ) {
    delay(200);
    if ( pir.Read() && !motionDetected ) {
      Serial.println("Motion detected!");
      motionDetected = true;
      owlStart = ac1Start = ac2Start = noteStart = servoStart = millis();
      owl.On();
      owlDuration = random(200, 2000);
      ac1.On();
      ac1Duration = random(1000, 2000);
      ac2.On();
      ac2Duration = random(1000, 2000);
      noteDuration = (unsigned long) random(500, 1500);
      tone(SPEAKER_PIN, random(NOTE_B0, NOTE_DS8 + 1));
      servoDuration = random(SERVO_MIN, SERVO_MAX);
      servo.write(random(5, 170));
    }
  } else {
    delay(200);
    if ( !pir.Read() && motionDetected ) {
      Serial.println("Motion ended!");
      motionDetected = false;
      noTone(SPEAKER_PIN);
      owl.Off();
      ac1.Off();
      ac2.Off();
    }
  }
  if ( motionDetected ) {
    int currentMillis = millis();
    if ( currentMillis - owlStart > owlDuration ) {
      owlStart = currentMillis;
      owlDuration = random(100, 500);
      owl.Toggle();
    }
    if ( currentMillis - ac1Start > ac1Duration ) {
      ac1Start = currentMillis;
      ac1Duration = random(500, 1000);
      ac1.Toggle();
    }
    if ( currentMillis - ac2Start > ac2Duration ) {
      ac2Start = currentMillis;
      ac2Duration = random(500, 1000);
      ac2.Toggle();
    }
    if ( currentMillis - noteStart > noteDuration ) {
      noteStart = currentMillis;
      noteDuration = (unsigned long) random(300, 1200);
      tone(SPEAKER_PIN, random(NOTE_B0, NOTE_DS8 + 1));
    }
    if ( currentMillis - servoStart > servoDuration ) {
      servoStart = currentMillis;
      servoDuration = random(300, 500);
      servo.write(random(SERVO_MIN, SERVO_MAX));
    }
  }
}

