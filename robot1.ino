#include <Servo.h>
// Pins of motor 1
#define mpin00 5
#define mpin01 6
// Pins of motor 2
#define mpin10 3
#define mpin11 11
Servo srv; 
String s;
const int trigPin = 12;
const int echoPin = 13;
long duration;
void setup() {
  // put your setup code here, to run once:
  //Serial.begin(115200);
  Serial.begin(115200);

  Serial.setTimeout(1000000);

  digitalWrite(mpin00, 0);
  digitalWrite(mpin01, 0);
  digitalWrite(mpin10, 0);
  digitalWrite(mpin11, 0);

  
  pinMode (mpin00, OUTPUT);
  pinMode (mpin01, OUTPUT);
  pinMode (mpin10, OUTPUT);
  pinMode (mpin11, OUTPUT); 


  srv.attach(8);
  srv.write(90);

  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  //Serial.print("1");
  Serial.print("AT+CWSAP_DEF=\"robotel1\",\"UnjEm3hc\",6,3\r\n");
  while(Serial.readStringUntil('\n') != "OK\r");
  Serial.print("AT+CIPMUX=1\r\n");
  while(Serial.readStringUntil('\n') != "OK\r");
  Serial.print("AT+CIPSERVER=1,4444\r\n");
  while(Serial.readStringUntil('\n') != "OK\r");
  //Serial.print("2");
 
}
void StartMotor (int m1, int m2, int forward, int speed)
{

 if (speed==0){ // stop
    digitalWrite(m1, 0); 
    digitalWrite(m2, 0);
 }else{
    if (forward){
        digitalWrite(m2, 0);
        analogWrite (m1, speed); // use PWM
    }
    else{
      digitalWrite(m1, 0);
      analogWrite(m2, speed);
    }
 }
}
String forward(int s ){
  StartMotor (mpin00, mpin01, 1, s);
  StartMotor (mpin10, mpin11, 1, s); 
  return "kk";
}
String  backward(int s){
  StartMotor (mpin00, mpin01, 0, s);
  StartMotor (mpin10, mpin11, 0, s); 
  return "kk";
}
String left(int s){
  StartMotor (mpin00, mpin01, 1, s);
  StartMotor (mpin10, mpin11, 0, s); 
  return "kk";
}
String right(int s){
  StartMotor (mpin00, mpin01, 0, s);
  StartMotor (mpin10, mpin11, 1, s); 
  return "kk";
}
String  stopMotor(){
   StartMotor (mpin00, mpin01, 0, 0);//motor 1 stop
   StartMotor (mpin10, mpin11, 0, 0); //motor 2 stop
   return "bineBoss";
}
int   distance(){
   digitalWrite(trigPin, LOW);
   delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
   // Reads the echoPin, returns the sound wave travel time in microseconds
   duration = pulseIn(echoPin, HIGH);
   // Calculating the distance
   int distance= duration*0.034/2;
   // Prints the distance on the Serial Monitor
   return distance;
}



String playWithServo(unsigned char angle)
{

 srv.write(angle);
 return "kBoss";
} 

void loop(){
  String str1 = Serial.readStringUntil('\n');
  if(str1.startsWith("0,")){
    //Serial.print("Stop Motors");
    String s=stopMotor();
  }
  else if(str1.startsWith("+IPD,0,")){
    String str2 = str1.substring(str1.indexOf(':') + 1, str1.length() - 1);
    //Serial.print(str2);
    char b=str2[0];
    String response;
    unsigned char n = 0;
    unsigned int d = 0;
     switch(b){
            case'f':
              response=forward(255);
              //Serial.print("fw");
              Serial.print("AT+CIPSEND=0,4\r\n");
              Serial.readStringUntil('\n');
              Serial.print(response + "\r\n");
              Serial.readStringUntil('\n');
              break;
            case'b':
              response=backward(255);
              //Serial.print("bw");
              Serial.print("AT+CIPSEND=0,4\r\n");
              Serial.readStringUntil('\n');
              Serial.print(response + "\r\n");
              Serial.readStringUntil('\n');
              break;
            case'r':
              response=right(255);
              Serial.print("AT+CIPSEND=0,4\r\n");
              Serial.readStringUntil('\n');
              Serial.print(response + "\r\n");
              Serial.readStringUntil('\n');
              break;
            case'l':
              response=left(255);
              Serial.print("AT+CIPSEND=0,4\r\n");
              Serial.readStringUntil('\n');
              Serial.print(response + "\r\n");
              Serial.readStringUntil('\n');
              break;
            case's':
              response=stopMotor();
              Serial.print("AT+CIPSEND=0,10\r\n");
              Serial.readStringUntil('\n');
              Serial.print(response + "\r\n");
              Serial.readStringUntil('\n');
              break;
            case 'm':
              n=str2[1];
              //Serial.print(n);
              response=playWithServo(n);
              Serial.print("AT+CIPSEND=0,7\r\n");
              Serial.readStringUntil('\n');
              Serial.print(response + "\r\n");
              Serial.readStringUntil('\n');
              break;
            case 'd':
              d=distance();
              //Serial.print(d);
              Serial.print("AT+CIPSEND=0,4\r\n");
              Serial.readStringUntil('\n');
              Serial.write(d);
              Serial.write(d >> 8);
              Serial.print("\r\n");
              Serial.readStringUntil('\n');
              break;
  }
}
}
