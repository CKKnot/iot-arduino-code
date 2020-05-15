#include<Servo.h>
const int red = 12, yellow = 13, trig = 9, echo = 10, soil = A0, light = A1, button = 2;
unsigned long previousMillis = 0, previousMillis1 = 0;
int data[2];
Servo myservo;
void setup() {
  myservo.attach(3);
  myservo.write(0);
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (Serial.available() > 0) {
    //Get serial data send by the server
    String data = Serial.readStringUntil('\n');
    //Check for server response and operate the servo motor
    if(data == "water")
      myservo.write(174);
    //Check for server response and light up the correspond led depensing on result received
    if(data == "dry")
      digitalWrite(yellow, HIGH);
    //Check for server response, operate the servo motor and turn the led off
    else if (data == "wet"){
      digitalWrite(yellow, LOW);
      myservo.write(0);
    }
    //Check for server response and light up the correspond led
    if(data == "intruded")
      digitalWrite(red, HIGH);
    //Check for server response and turn off the correspond led
    else if (data == "safe")
      digitalWrite(red, LOW);
  }
  //Collect sensors data and send it to the server
  if (currentMillis - previousMillis >= 1000) {
    previousMillis = currentMillis;
    data[0] = analogRead (soil);
    data[1] = analogRead (light);
    Serial.println(String(data[0]) + "," + String(data[1]) + "," + String(readUltraSonic()));
  }
  delay(1);
}
//This function will manage the activation, receiving, and calculate the perceiving distance in cm with the ultrasonic sensor
long readUltraSonic(){
  long duration, cm;
  //Clears the trigPin
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  //Set the trigPin on HIGH state for 10us
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  //Reads the echoPin
  duration = pulseIn(echo, HIGH);
  //Calculate distance in centimeter
  cm = duration * 0.034 / 2;
  return cm;
}