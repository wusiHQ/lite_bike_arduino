/*
 lite bike arduino source codes
 Language: Wiring/Arduino

 */

#define LOCK 1
#define UNLOCK 0
#define MOVE_MAX 75

int firstSensor = 0;    // first analog sensor
int secondSensor = 0;   // second analog sensor
int thirdSensor = 0;    // digital sensor
int inByte = 0;         // incoming serial byte
int led = 13;
int ledStatus=0;

int lockStatus = LOCK;
int stolenStatus = 0;

int pinS = 12;
int pinX = A4;
int pinY = A3;
int pinZ = A2;
int pinBuzzer = 8;

void Buzzer_Di(int times)
{
  for (int i = 0; i < times; ++i) {
    digitalWrite(pinBuzzer,HIGH); //蜂鸣器响
    delay(200);     //延时
    digitalWrite(pinBuzzer,LOW); //蜂鸣器关闭
    delay(200);     //延时
  }
}

void Buzzer_alarm() {
  for(int i=0;i<20;i++)//输出一个频率的声音 
    { 
      digitalWrite(pinBuzzer,HIGH);//发声音 
      delay(i);//延时1ms 
      digitalWrite(pinBuzzer,LOW);//不发声音 
      delay(1);//延时ms 
    } 
}
void setup()
{
  // start serial port at 115200 bps:
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(pinS, OUTPUT);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinZ, INPUT);
  pinMode(pinBuzzer, OUTPUT);

}
int cx=0,cy=0,cz=0;
int lastX=0, lastY=0, lastZ=0;
int deltaX, deltaY, deltaZ;
void loop()
{
  // if we get a valid byte, read analog ins:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    if (inByte == 'L' || inByte == 'l') {
      lockStatus = LOCK;
    }
    else if (inByte == 'U' || inByte == 'u'){
      lockStatus = UNLOCK;
      stolenStatus = 0;
       Buzzer_Di(2);
    } else if (inByte == 'F' || inByte == 'f') {
      stolenStatus = 0;
    }

    Serial.println(inByte);
  }    //ledStatus = ~ledStatus;

  if (lockStatus == LOCK) {
    cx = analogRead(pinX);
    cy = analogRead(pinY);
    cz = analogRead(pinZ);

    deltaX = abs(cx - lastX);
    deltaY = abs(cy - lastY);
    deltaZ = abs(cz - lastZ);
    if (deltaX + deltaY + deltaZ > MOVE_MAX && lastX > 0) {
      //lockStatus = UNLOCK;
      if (stolenStatus == 0) {
        Serial.print("S");
      }
      stolenStatus = 1;
    }
    /*Serial.println('start');
     Serial.println(lockStatus);
     Serial.println(deltaX);
     Serial.println(deltaY);
     Serial.println(deltaZ);
     */
    lastX = cx;
    lastY = cy;
    lastZ = cz;
    //delay(1000);
  }

  digitalWrite(led, lockStatus);   // turn the LED on (HIGH is the voltage level)
  if (stolenStatus == 1) {
    Buzzer_alarm();
  }
  digitalWrite(pinS, stolenStatus);   // turn the LED on (HIGH is the voltage level)

  delay(200);               // wait for a second
}
