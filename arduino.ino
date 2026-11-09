//SD Card
#include <SD.h>

File myFile;
const int CSpin = 10;

//Line Sensor
#include <QTRSensors.h>

QTRSensors qtrD;
const uint8_t SensorCount = 4;
uint16_t sensorValuesD[SensorCount];

//Colour Sensor
#define S2 A2
#define S3 A3
#define sensorOut A4

int green[] = {340, 320, 340};
bool greenFlag[] = {false, false, false};

int purple[] = {360, 490, 250};
bool purpleFlag[] = {false, false, false};

int yellow[] = {120, 190, 240};
bool yellowFlag[] = {false, false, false};

int frequency = 0;

//Ultrasonic Sensor
const int pingPin = A0;

//Motors
const int In1 = A1;
const int In2 = A5;
const int In3 = 4;
const int In4 = 7;
const int EnA = 5;
const int EnB = 6;



void setup() {

  //SD Card
  pinMode(CSpin, OUTPUT);

  //Line Array Sensor
  qtrD.setTypeRC();
  qtrD.setSensorPins((const uint8_t[]){2, 3, 8, 9}, SensorCount);

  //Colour Sensor
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  //motors
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);

  pinMode(EnA, OUTPUT);
  pinMode(EnB, OUTPUT);

  delay(500);

  //Line Array Sensor Calibration
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  for (uint16_t i = 0; i < 400; i++) {

    qtrD.calibrate();

  }

  digitalWrite(LED_BUILTIN, LOW);

  delay(1000);

}



void loop() {

  if(SD.begin(CSpin)) {

    myFile = SD.open("test.txt");

    if(myFile){

      while(myFile.available()) {

        char move = myFile.read();

        switch(move) {

          case 'L':

            analogWrite(EnB, 100);
            analogWrite(EnA, 100);

            digitalWrite(In1, HIGH);
            digitalWrite(In2, LOW);  
            digitalWrite(In3, LOW);
            digitalWrite(In4, LOW);

            delay(500);

            break;

          case 'R':

            analogWrite(EnB, 100);
            analogWrite(EnA, 100);

            digitalWrite(In1, LOW);
            digitalWrite(In2, LOW);  
            digitalWrite(In3, HIGH);
            digitalWrite(In4, LOW);

            delay(500);

            break;

          case 'F':

            analogWrite(EnB, 100);
            analogWrite(EnA, 100);

            digitalWrite(In1, HIGH);
            digitalWrite(In2, LOW);  
            digitalWrite(In3, HIGH);
            digitalWrite(In4, LOW);

            delay(500);

            break;

          case 'B':

            analogWrite(EnB, 100);
            analogWrite(EnA, 100);

            digitalWrite(In1, LOW);
            digitalWrite(In2, HIGH);  
            digitalWrite(In3, LOW);
            digitalWrite(In4, HIGH);

            delay(500);

            break;

        }

      }

      myFile.close();

    } else {

      normalMode();

    }

  } else {

    normalMode();

  }
  
}



void normalMode() {

  detectColour();

  if(greenFlag[0] && greenFlag[1] && greenFlag[2]) {

    analogWrite(EnB, 100);
    analogWrite(EnA, 100);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  
    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(3000);

    Safety();

  } else if(purpleFlag[0] && purpleFlag[1] && purpleFlag[2]) {

    Left();

  } else if(yellowFlag[0] && yellowFlag[1] && yellowFlag[2]) {

    Right();

  } else {

    lineFollow();

  }

}



void goStraight() {

  analogWrite(EnB, 100);
  analogWrite(EnA, 100);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);  
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);

  delay(50);

}



void Stop() {

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);

  delay(50);

}



void Left() {

  analogWrite(EnA, 100);
  analogWrite(EnB, 100);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);  

  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);

  delay(200);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);  
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);

  delay(200);

}



void Right() {

  analogWrite(EnA, 100);
  analogWrite(EnB, 100);

  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);  

  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);

  delay(200);

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);  
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW);

  delay(200);

}



void detectObject() {

  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);  

  analogWrite(EnA, 100);

  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);

  analogWrite(EnB, 100);

  delay(50);

}



void Push() {

  detectColour();

  if(greenFlag[0] && greenFlag[1] && greenFlag[2]) {

    goStraight();

  } else {

    Stop();

  }

  delay(50);

}



void Safety() {

  while(true) {
    long duration, cm;
  
    pinMode(pingPin, OUTPUT);

    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);

    pinMode(pingPin, INPUT);

    duration = pulseIn(pingPin, HIGH);

    cm = microsecondsToCentimeters(duration);

    if(cm < 10) {
    
      Push();

    } else {
    
      detectObject();

    }

  }

}



long microsecondsToCentimeters(long microseconds) {
  
  return microseconds / 29 / 2;

}



void lineFollow() {

  uint16_t position = qtrD.readLineBlack(sensorValuesD);

  if(position > 1500 && position <= 2000) {

    analogWrite(EnA, 100);
    analogWrite(EnB, 70);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  

    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(50);

  } else if(position > 2000 && position <= 2500) {

    analogWrite(EnA, 150);
    analogWrite(EnB, 70);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  

    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(50);

  } else if(position > 2500 && position <= 3000) {

    analogWrite(EnA, 200);
    analogWrite(EnB, 70);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  

    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(50);

  } else if(position >= 1000 && position < 1500) {

    analogWrite(EnA, 70);
    analogWrite(EnB, 100);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  

    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(50);

  } else if(position >= 500 && position < 1000) {

    analogWrite(EnA, 70);
    analogWrite(EnB, 150);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  

    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(50);

  } else if(position >= 0 && position < 500) {

    analogWrite(EnA, 70);
    analogWrite(EnB, 200);

    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);  

    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);

    delay(50);

  } else {

    goStraight();

  }

  delay(50);

}



void detectColour() {

  greenFlag[0] = false; purpleFlag[0] = false; yellowFlag[0] = false;
  greenFlag[1] = false; purpleFlag[1] = false; yellowFlag[1] = false;
  greenFlag[2] = false; purpleFlag[2] = false; yellowFlag[2] = false;

  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);

  frequency = pulseIn(sensorOut, LOW);

  if(abs(frequency - green[0])<40) {

    greenFlag[0] = true;

  } else if(abs(frequency - purple[0])<40) {

    purpleFlag[0] = true;

  } else if(abs(frequency - yellow[0])<30) {

    yellowFlag[0] = true;

  }

  delay(50);

  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);

  frequency = pulseIn(sensorOut, LOW);

  if(abs(frequency - green[1])<30) {

    greenFlag[1] = true;

  } else if(abs(frequency - purple[1])<30) {

    purpleFlag[1] = true;

  } else if(abs(frequency - yellow[1])<30) {

    yellowFlag[1] = true;

  }

  delay(50);

  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);

  frequency = pulseIn(sensorOut, LOW);

  if(abs(frequency - green[2])<40) {

    greenFlag[2] = true;

  } else if(abs(frequency - purple[2])<30) {

    purpleFlag[2] = true;

  } else if(abs(frequency - yellow[2])<30) {

    yellowFlag[2] = true;

  } 

  delay(50);

}








