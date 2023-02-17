//Settings
#define MUSIC_OFF_ON_COMMAND 1
#define MUSIC_NEXT_COMMAND 2
#define ON_OFF_BACKLIGHT 3
#define NEXT_ILLUMINATION 4
#define NUMBER_OF_BACKLIGHT_MODES 7 //Number of backlight modes
#define CORNER_CORRECTION -11 //(-право)
#define CENTER_ANGLE 90 CORNER_CORRECTION //Center wheel position
#define MAX_ANGLE 124 //Maximum steering angle
#define MIN_ANGLE 44 //Minimum steering angle

//Libraries
#include <avr/wdt.h>
#include <Arduino.h>
#include "DYPlayerArduino.h"
#include <SoftwareSerial.h>
#include <Servo.h>

#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(9, 8); // RX, TX
DY::Player player(&mySerial);
Servo servo;
Servo motor;

#define LEDPIN 2
#define NUMLEDS 8     //Number of LEDs
#include <microLED.h>
#include <FastLEDsupport.h>
microLED<NUMLEDS, LEDPIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

//Pins
#define M_REGULATOR 3
#define RUDDER_SERVO 6
#define REVERSE_LEDS 7


boolean playerStatus = 0;
boolean triggerFlag = 0;
boolean musicStatus = 0;
boolean backlightStatus = 0;
boolean noData = 0;
boolean emergencyStop = 0;

int backlightMode = 0;

unsigned long ledTimer = 0;
unsigned long noDataTimer = 0;

struct Str {
  uint8_t direction;
  int16_t speed;
  uint8_t angle;
  uint8_t command;
};

Str buf;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  servo.attach(RUDDER_SERVO);
  motor.attach(M_REGULATOR);

  pinMode(REVERSE_LEDS, OUTPUT);

  player.begin();
  player.setVolume(95); //Set the volume to 95%
  player.setCycleMode(DY::PlayMode::Random);
  player.stop();

  strip.setBrightness(64);
  buf.angle = CENTER_ANGLE;

  wdt_enable(WDTO_1S);
}

void ReceiveData() {
  Serial.readBytes((byte*)&buf, sizeof(buf));
}

void PlayerControl(int command) {
  servo.detach();
  motor.detach();

  if (command == 0) {
    musicStatus = 0;  //Turn off the music
    player.stop();
  }

  if (command == 1) {
    musicStatus = 1;  //Turn on the music
    player.play();
  }

  if (command == 2 && musicStatus == 1) {
    player.next();  //Next music
  }

  motor.attach(M_REGULATOR);
  servo.attach(RUDDER_SERVO);
}

void loop() {
  ControlCar();

  if (Serial.available() > 0) {
    ReceiveData();
    noData = 0;
    emergencyStop = 0;
  } else {
    if (noData == 0) {
      noDataTimer = millis();
      noData = 1;
    }
    if (noData == 1 && emergencyStop == 0 && (millis() - noDataTimer) > 500) {
      buf.direction = 0;
      buf.speed = 1500;
      player.stop();
      offLeds(); //Turn off backlight
      backlightMode = 0;
      emergencyStop = 1;
    }
  }

  carBacklight();
  wdt_reset();
}

void ControlCar() {
  if (buf.direction == 0) {
    motor.writeMicroseconds(1500);
    digitalWrite(REVERSE_LEDS, LOW);
  }

  if (buf.direction == 1) {
    motor.writeMicroseconds(buf.speed);
    digitalWrite(REVERSE_LEDS, LOW);
  }

  if (buf.direction == 2) {
    motor.writeMicroseconds(buf.speed);
    digitalWrite(REVERSE_LEDS, HIGH);
  }

  if (buf.angle >= MAX_ANGLE) {
    buf.angle = MAX_ANGLE;
  }

  if (buf.angle <= MIN_ANGLE) {
    buf.angle = MIN_ANGLE;
  }

  servo.write(buf.angle); //Turning the servo

  if (buf.command == MUSIC_OFF_ON_COMMAND && triggerFlag == 0) { //Turning music on and off
    triggerFlag = 1;
    playerStatus = 1 - playerStatus;
    PlayerControl(playerStatus);
  }

  if (buf.command == MUSIC_NEXT_COMMAND && triggerFlag == 0) { //Switching music
    triggerFlag = 1;
    PlayerControl(2);
  }

  if (buf.command == 3 && triggerFlag == 0) { //Turning the backlight on and off
    triggerFlag = 1;
    backlightStatus = backlightStatus - 1;
    backlightMode = 1;

    ledTimer = millis();

    if (backlightStatus == 0) {
      offLeds(); //Turn off the backlight
      backlightMode = 0;
    }
  }

  if (buf.command == NEXT_ILLUMINATION && triggerFlag == 0 && backlightStatus == 1) { //Switching backlight mode
    triggerFlag = 1;
    backlightMode++;
    if (backlightMode > NUMBER_OF_BACKLIGHT_MODES) {
      backlightMode = 1;
    }

  }

  if (buf.command == 0 && triggerFlag == 1) {
    triggerFlag = 0;
  }

}

void offLeds() {
  strip.leds[0] = mBlack;
  strip.leds[1] = mBlack;
  strip.leds[2] = mBlack;
  strip.leds[3] = mBlack;
  strip.leds[4] = mBlack;
  strip.leds[5] = mBlack;
  strip.leds[6] = mBlack;
  strip.leds[7] = mBlack;
  strip.show();
}

void carBacklight() {
  if ( ((millis() - ledTimer) > 40) && backlightStatus == 1 ) {
    motor.detach();
    static byte count = 0;
    static int count2 = 0;
    count = count + 4;
    count2 += 10;
    strip.begin();
    for (int i = 0; i < NUMLEDS; i++) {
      switch (backlightMode) {
        case 1: //Green
          strip.leds[0] = mGreen;
          strip.leds[1] = mGreen;
          strip.leds[2] = mGreen;
          strip.leds[3] = mGreen;
          strip.leds[4] = mGreen;
          strip.leds[5] = mGreen;
          strip.leds[6] = mGreen;
          strip.leds[7] = mGreen;
          strip.show();
          break;

        case 2: //Blue
          strip.leds[0] = mBlue;
          strip.leds[1] = mBlue;
          strip.leds[2] = mBlue;
          strip.leds[3] = mBlue;
          strip.leds[4] = mBlue;
          strip.leds[5] = mBlue;
          strip.leds[6] = mBlue;
          strip.leds[7] = mBlue;
          strip.show();
          break;

        case 3: //Red
          strip.leds[0] = mRed;
          strip.leds[1] = mRed;
          strip.leds[2] = mRed;
          strip.leds[3] = mRed;
          strip.leds[4] = mRed;
          strip.leds[5] = mRed;
          strip.leds[6] = mRed;
          strip.leds[7] = mRed;
          strip.show();
          break;

        case 4:
          strip.send(CRGBtoData(ColorFromPalette(RainbowColors_p, count + i * 3, 255, LINEARBLEND)));
          break;

        case 5:
          strip.send(CRGBtoData(ColorFromPalette(RainbowStripeColors_p, count + i * 3, 255, NOBLEND)));
          break;

        case 6:
          strip.send(CRGBtoData(ColorFromPalette(RainbowColors_p, count + i * 3, 255, LINEARBLEND)));
          break;

        case 7:
          strip.send(CRGBtoData(ColorFromPalette(myRedWhiteBluePalette_p, count + i * 3, 255, NOBLEND)));
          break;

      }
    }
    strip.end();
    motor.attach(M_REGULATOR);
    ledTimer = millis();
  }
}


const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};
