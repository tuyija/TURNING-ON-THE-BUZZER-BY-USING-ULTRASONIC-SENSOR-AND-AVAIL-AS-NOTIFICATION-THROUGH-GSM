#define echo 7 // connect echo pin to arduino D7 (ultra sonic sensor)
#define trig 8 // connect trig pin to arduino D8 (ultra sonic sensor)
#define led 13 // optional
#define thresholdDis 20
long duration; // store time duration of sound travel
int dis; // store actual distance to object
#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11); // RX, TX
bool  flag = 0;
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  pinMode(echo, INPUT); // echo pin work as input
  pinMode(trig, OUTPUT);
  //sendSMS("0760154020", "Hi, Hello.this is me :)"); // send msg one time
}
void loop() { // run over and over
  float disInCm = checkDistance();
  Serial.println(disInCm);
  if ((disInCm > thresholdDis) && (flag == 0)) {
    flag = 1; // prevent sms sending continusly
    sendSMS("0717424602", "Max distance detected. now distance is " + String(disInCm) + "Cm"); // send msg one time
  }
  if (disInCm < thresholdDis) {
    flag = 0; // clear flag for next event
  }
  delay(100);
}
int checkDistance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH); // triger signal as datasheet
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  dis = duration * (0.034 / 2.00); // speed of sound
  return dis;
}
void sendSMS(String mobileNUmber, String msgBody) {
  mySerial.println("AT"); // check connection
  readResponse(); // read modem response
    delay(1000);
    mySerial.println("AT+CMGF=1");// SET MODULE TO TEXT MODE
    readResponse(); // read modem response
    delay(1000);
  mySerial.println("AT+CMGS=\"" + mobileNUmber + "\""); // sender mob number
  readResponse(); // read modem response
  delay(1000);
  mySerial.println(msgBody); // enter msg body
  delay(1000);
  mySerial.print((char)26); // send msg
  delay(1000);
}
void readResponse() {
  //simple function for read modem response. we will check work or not
  delay(1000);
  String response = mySerial.readStringUntil("OK");
  Serial.println(response);
}
