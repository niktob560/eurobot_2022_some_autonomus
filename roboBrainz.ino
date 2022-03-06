#include "MotorDriver.h"
#include <stdint.h>

#include "PriUint64.h"

#define LEFT_MOTOR MOTORB
#define RIGHT_MOTOR MOTORA

#define PIN_ENCODER_LEFT 3
#define PIN_ENCODER_RIGHT 2

volatile int64_t ticksLeft = 0;
volatile int64_t ticksRight = 0;
volatile bool isFwdLeft = true;
volatile bool isFwdRight = true;

volatile void tickLeft(void) { ticksLeft += isFwdLeft ? 1 : -1; }

volatile void tickRight(void) { ticksRight += isFwdRight ? 1 : -1; }

const uint8_t spd = 255;

const long double CLOCKS_PER_MM = 8.928571428571429;
const long int WHEELS_DISTANCE_MM = 1200;

void go(const int64_t deltaTicksLeft, const int64_t deltaTicksRight) {
  ticksLeft = 0;
  ticksRight = 0;
  isFwdRight = deltaTicksRight > 0;
  isFwdLeft = deltaTicksLeft > 0;

  int64_t tgtTicksRight = ticksRight + deltaTicksRight;
  int64_t tgtTicksLeft = ticksLeft + deltaTicksLeft;

  motordriver.rotate(isFwdLeft, LEFT_MOTOR);
  motordriver.rotate(isFwdRight, RIGHT_MOTOR);

  bool stoppedRight = false, stoppedLeft = false;
  while (!(stoppedLeft && stoppedRight)) {
    if ((isFwdRight ? (tgtTicksRight <= ticksRight)
                   : (tgtTicksRight >= ticksRight)) && !stoppedRight) {
      motordriver.stop(RIGHT_MOTOR);
      Serial.println("STOP right");
      stoppedRight = true;
    }
    if ((isFwdLeft ? (tgtTicksLeft <= ticksLeft)
                  : (tgtTicksLeft >= ticksLeft)) && !stoppedLeft) {
      motordriver.stop(LEFT_MOTOR);
      Serial.println("STOP left");
      stoppedLeft = true;
    }
  }
}

void go_mm(const long double deltaMmLeft, const long double deltaMmRight) {
  const int64_t deltaTicksLeft = deltaMmLeft * CLOCKS_PER_MM;
  const int64_t deltaTicksRight = deltaMmRight * CLOCKS_PER_MM;
  go(deltaTicksLeft, deltaTicksRight);
}

// void rotate_placed(const long double degreeCW) {
//   if (degreeCW < 0) {

//   }
// }

void setup() {
  /*C
  onfigure the motor A to control the wheel at the left side.*/
  /*Configure the motor B to control the wheel at the right side.*/
  motordriver.init();
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT), tickLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT), tickRight, FALLING);
  Serial.begin(115200);
  motordriver.setSpeed(spd, LEFT_MOTOR);
  motordriver.setSpeed(spd, RIGHT_MOTOR);
  delay(2000);
  // go(10000, 0);
  // delay(2000);
  // go(0, 10000);
  // delay(2000);
  // go(-10000, -10000);
  // delay(2000);
  // go(-10000, 10000);
  // go_mm(200, 200);

  go_mm(-1000, -1000);

  go_mm(WHEELS_DISTANCE_MM * 3 / 2, 0);
  go_mm(90*100, 90*100);
  motordriver.setSpeed(spd/2, LEFT_MOTOR);
  motordriver.setSpeed(spd, RIGHT_MOTOR);
  go_mm(500, 1000);
  motordriver.setSpeed(spd, LEFT_MOTOR);
  motordriver.setSpeed(spd, RIGHT_MOTOR);
  go_mm(0, 300);
  go_mm(200*100, 200*100);



  // while (1) {
  //   go_mm((-(WHEELS_DISTANCE_MM * 3.1415 / 2) / 149) * 180,
  //         ((WHEELS_DISTANCE_MM * 3.1415 / 2) / 149) * 180);
  //   delay(1000);
  //   go_mm(((WHEELS_DISTANCE_MM * 3.1415 / 2) / 149) * 180,
  //         (-(WHEELS_DISTANCE_MM * 3.1415 / 2) / 149) * 180);
  //   delay(1000);
  // }
  //  delay(1000);
  //  go_mm(290, 0);
  //  delay(1000);
  //  go_mm(-110, -110);
  //  delay(1000);
  //  go_mm(-110, -110);
  //  delay(1000);
  //
}

void loop() {
  //  motordriver.goForward();
  //  delay(2000);
  //  motordriver.stop();
  //  delay(1000);
  //  motordriver.goBackward();
  //  delay(2000);
  //  motordriver.stop();
  //  delay(1000);
  //  motordriver.goLeft();
  //  delay(2000);
  //  motordriver.stop();
  //  delay(1000);
  //  motordriver.goRight();
  //  delay(2000);
  //  motordriver.stop();
  delay(1000);
}
