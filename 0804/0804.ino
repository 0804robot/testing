#include <Servo.h>
#define TRIG 8
#define ECHO 9

long duration;
int distance;

Servo right; //Right motor
Servo left; //Left motor
Servo radar;
//To control the motor - Servo.write(90); is a stop. As you write down to zero, the speed increases in one direction
//As you write up to 180, the speed ramps up in the opposite direction

void setup () {
  right.attach(5); //On hardware this is the motor on the left [On the driver, M1]
  left.attach(6); //On hardware this is the motor on the right [On the driver, M2]
  radar.attach(3);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(115200);
}

void loop () {
    drive_F(100);
    obstacle();
  
}

void right_turn(int turn_time) { //Right turn at maximum speed - 0 is maximum speed, 90 is no speed
    right.write(80); //Both below 90 or below 1500 for microseconds
    left.write(80);
    delay(turn_time);
    right.write(90);
    left.write(90);
    drive_F(50);
}

//To stop, call drive_F(0);
void drive_F(int drive_speed) { //Keep drive speed between 0 and 500
    right.writeMicroseconds(1500 - drive_speed);
    left.writeMicroseconds(1500 + drive_speed);
    delay(20);
}

void drive_R(int drive_speed) { //Keep drive speed between 0 and 500
    right.writeMicroseconds(1500 + drive_speed);
    left.writeMicroseconds(1500 - drive_speed);
    delay(20);
}

void left_turn(int turn_time) { //Left turn at maximum speed - 0 is maximum speed, 90 is no speed
    right.write(110); //Both above 90 or above 1500 for microseconds
    left.write(110);
    delay(20);
    delay(turn_time);
    right.write(90);
    left.write(90);
    delay(20);
    drive_F(80);
}

void obstacle () {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = duration * 0.034 / 2;
    Serial.println(distance);

    if(distance < 5 && distance > 0){
        check_LR();
        drive_F(0);
    }
}

int read_Sonic () {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);
    duration = pulseIn(ECHO, HIGH);
    distance = duration * 0.034 / 2;
    return distance;
}

void check_LR() {
    radar.write(0);
    if (read_Sonic() < 5) {
        radar.write(180);
        if (read_Sonic() < 5) stop_pause();
        }
    else {
        left_turn(300);
    } 

}

void stop_pause() {
    drive_F(0);
    delay(3000);
    drive_R(200);
    left_turn(200);
}