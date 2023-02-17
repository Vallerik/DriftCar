/*
   Remote control program for drift cars
*/

#include <avr/wdt.h>

//Settings
#define MUSIC_OFF_ON_COMMAND 1
#define MUSIC_NEXT_COMMAND 2
#define ON_OFF_BACKLIGHT 3
#define NEXT_ILLUMINATION 4

//Steering wheel
#define CORNER_CORRECTION -11 //Correction of the central position of the wheels (- right, + left)
#define MIN_ANGLE 44 //Minimum steering angle
#define CENTER_ANGLE 90 CORNER_CORRECTION // Servo wheel center position + correction
#define MAX_ANGLE 124 //Max steering angle

#define WHEEL_MINIMUM_RESISTOR_VALUE 377 //Minimum variable resistor value
#define WHEEL_AVERAGE_RESISTOR_VALUE 509 //Average value of the variable resistor
#define WHEEL_MAXIMUM_RESISTOR_VALUE 623 //Maximum value of the variable resistor
#define WHEEL_DEADBAND 5 //Deadband


//Throttle lever
#define SPEED_AVERAGE_RESISTOR_VALUE 543 //Middle speed position
#define SPEED_MAXIMUM_RESISTOR_VALUE 632 // Maximum throttle position
#define SPEED_MINIMUM_RESISTOR_VALUE 470 //Minimum throttle position
#define SPEED_DEADBAND 5 //Deadband

//Battery overdischarge protection settings
#define OVER_DISCHARGE_VOLTAGE 3.1 //Battery over-discharge protection voltage
#define BOARD_SUPPLY_VOLTAGE 5.0 //Board supply voltage (needed to read the voltage)

//Pins
#define VARIABLE_RESISTOR_WHEEL A1
#define VARIABLE_RESISTOR_SPEED A0
#define VOLTMETER A2
#define MUSIC_BUTTON 2
#define LIGHT_BUTTON 3
#define LED_GREEN 4
#define LED_RED 5

int carSpeed;
int rotationAngle;
int rotationAngle2;

float volt = 0;

boolean overdischargeProtection = false;
boolean mb_flag = 0;
boolean mb_hold_flag = 0;
boolean lb_flag = 0;
boolean lb_hold_flag = 0;

unsigned long mb_last_press = 0;
unsigned long lb_last_press = 0;
unsigned long startTime = 0;

struct Str {
  uint8_t direction;
  int16_t speed;
  uint8_t angle;
  uint8_t command;
};
Str buf;


void setup() {
  Serial.begin(9600);

  pinMode(VARIABLE_RESISTOR_WHEEL, INPUT);
  pinMode(VARIABLE_RESISTOR_SPEED, INPUT);
  pinMode(VOLTMETER, INPUT);
  pinMode(MUSIC_BUTTON, INPUT_PULLUP);
  pinMode(LIGHT_BUTTON, INPUT_PULLUP);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  startTime = millis();

  digitalWrite(LED_GREEN, HIGH);
  wdt_enable(WDTO_1S);
}

void sendingData() {
  Serial.write((byte*)&buf, sizeof(buf)); //Sending data
  if (buf.command != 0) {
    buf.command = 0;
  }
  startTime = millis();
}


void resistorProcessing() {
  carSpeed = analogRead(VARIABLE_RESISTOR_WHEEL);
  rotationAngle = analogRead(VARIABLE_RESISTOR_SPEED);

  if (carSpeed > SPEED_MAXIMUM_RESISTOR_VALUE) {
    carSpeed = SPEED_MAXIMUM_RESISTOR_VALUE;
  }

  if (carSpeed < SPEED_MINIMUM_RESISTOR_VALUE) {
    carSpeed = SPEED_MINIMUM_RESISTOR_VALUE;
  }

  if ((rotationAngle < WHEEL_AVERAGE_RESISTOR_VALUE + WHEEL_DEADBAND) || (rotationAngle > WHEEL_AVERAGE_RESISTOR_VALUE - WHEEL_DEADBAND)) {
    buf.angle = CENTER_ANGLE;
  }

  if (rotationAngle > WHEEL_AVERAGE_RESISTOR_VALUE + WHEEL_DEADBAND) {
    buf.angle = map(rotationAngle, WHEEL_AVERAGE_RESISTOR_VALUE + WHEEL_DEADBAND, WHEEL_MAXIMUM_RESISTOR_VALUE, CENTER_ANGLE, MIN_ANGLE);
  }

  if (rotationAngle < WHEEL_AVERAGE_RESISTOR_VALUE - WHEEL_DEADBAND) {
    buf.angle = map(rotationAngle, WHEEL_AVERAGE_RESISTOR_VALUE - WHEEL_DEADBAND, WHEEL_MINIMUM_RESISTOR_VALUE, CENTER_ANGLE, MAX_ANGLE);
  }

  if ((carSpeed < SPEED_AVERAGE_RESISTOR_VALUE + SPEED_DEADBAND) || (carSpeed > SPEED_AVERAGE_RESISTOR_VALUE - SPEED_DEADBAND)) {
    buf.speed = 1500;
    buf.direction = 0;
  }

  if (carSpeed > SPEED_AVERAGE_RESISTOR_VALUE + SPEED_DEADBAND) {
    buf.speed = map(carSpeed, SPEED_AVERAGE_RESISTOR_VALUE + SPEED_DEADBAND, SPEED_MAXIMUM_RESISTOR_VALUE, 1500, 700);
    buf.direction = 1;
  }

  if (carSpeed < SPEED_AVERAGE_RESISTOR_VALUE - SPEED_DEADBAND) {
    buf.speed = map(carSpeed, SPEED_AVERAGE_RESISTOR_VALUE - SPEED_DEADBAND, SPEED_MINIMUM_RESISTOR_VALUE, 1500, 2200);
    buf.direction = 2;
  }
}


void buttons() {
  //Backlight change button
  if (digitalRead(LIGHT_BUTTON) == 0 && lb_flag == 0) {
    lb_flag = 1;
    lb_last_press = millis();
  }

  if (digitalRead(LIGHT_BUTTON) == 0 && lb_hold_flag == 0 && (millis() - lb_last_press) > 1000) {
    lb_hold_flag = 1;
    buf.command = ON_OFF_BACKLIGHT;

  }

  if (digitalRead(LIGHT_BUTTON) == 1 && lb_flag == 1) {
    if (lb_hold_flag == 0 && (millis() - lb_last_press) > 100) {
      buf.command = NEXT_ILLUMINATION;
    }

    lb_flag = 0;
    lb_hold_flag = 0;
  }


  //Button to change and turn on music
  if (digitalRead(MUSIC_BUTTON) == 0 && mb_flag == 0) {
    mb_flag = 1;
    mb_last_press = millis();
  }

  if (digitalRead(MUSIC_BUTTON) == 0 && mb_hold_flag == 0 && (millis() - mb_last_press) > 1000) {
    mb_hold_flag = 1;
    buf.command = MUSIC_OFF_ON_COMMAND;
  }

  if (digitalRead(MUSIC_BUTTON) == 1 && mb_flag == 1) {
    if (mb_hold_flag == 0 && (millis() - mb_last_press) > 80) {
      //Код нажатия
      buf.command = MUSIC_NEXT_COMMAND;
    }

    mb_flag = 0;
    mb_hold_flag = 0;
  }
}

void loop() {
  volt = analogRead(VOLTMETER);
  if (((volt * BOARD_SUPPLY_VOLTAGE) / 1024) <= OVER_DISCHARGE_VOLTAGE) {
    overdischargeProtection = true;
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
  }

  if (overdischargeProtection == false) {
    resistorProcessing();
    buttons();

    if ((millis() - startTime) > 50) {
      sendingData();
    }
  }
  wdt_reset();
}
