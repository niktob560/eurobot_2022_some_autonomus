// #include <Multiservo.h>
#include <stdint.h>

// Создаём объект для работы с сервомоторами
// Multiservo multiservo;

// #include "PriUint64.h"

// #define LEFT_MOTOR MOTORB
// #define RIGHT_MOTOR MOTORA

#define PIN_ENCODER_LEFT 2
#define PIN_ENCODER_RIGHT 3
// 1,2 - левый
// 3,4 - правый

#define MOTOR_CH1 5
#define MOTOR_CH2 6
#define MOTOR_CH3 10
#define MOTOR_CH4 11

volatile int64_t ticksLeft = 0;
volatile int64_t ticksRight = 0;
volatile bool isFwdLeft = true;
volatile bool isFwdRight = true;
int roww;
volatile void tickLeft(void) { ticksLeft += isFwdLeft ? 1 : -1; }

volatile void tickRight(void) { ticksRight += isFwdRight ? 1 : -1; }

const uint8_t spd = 128;

//Setup zone
const long double TICKS_PER_DEGREE = 17.271;
const long int WHEEL_D_MM = 63;


const long double WHEEL_L_MM = M_PI * WHEEL_D_MM;
const long double MM_PER_DEGREE = WHEEL_L_MM / 360;
const long double TICKS_PER_MM = TICKS_PER_DEGREE / MM_PER_DEGREE;
const long int WHEELS_DISTANCE_MM = 120;

int leftSpeed = 255;
int rightSpeed = 255;




void do_busywait(void) {
  //do smth
}



void setLeftSpd(int newSpd) {
  leftSpeed = newSpd;
  rotateLeft(isFwdLeft);
}

void setRightSpd(int newSpd) {
  rightSpeed = newSpd;
  rotateRight(isFwdRight);
}

void rotateLeft(bool isFwd) {
  isFwdLeft = isFwd;
  if (!isFwd) {
    analogWrite(MOTOR_CH1, leftSpeed);
    analogWrite(MOTOR_CH2, 0);
  } else {
    analogWrite(MOTOR_CH1, 0);
    analogWrite(MOTOR_CH2, leftSpeed);
  }
}

void rotateRight(bool isFwd) {
  isFwdRight = isFwd;
  if (!isFwd) {
    analogWrite(MOTOR_CH3, rightSpeed);
    analogWrite(MOTOR_CH4, 0);
  } else {
    analogWrite(MOTOR_CH3, 0);
    analogWrite(MOTOR_CH4, rightSpeed);
  }
}

void stopRight() {
  analogWrite(MOTOR_CH4, 255);
  analogWrite(MOTOR_CH3, 255);
}

void stopLeft() {
  analogWrite(MOTOR_CH1, 255);
  analogWrite(MOTOR_CH2, 255);
}

void go(const int64_t deltaTicksLeft, const int64_t deltaTicksRight) {
  ticksLeft = 0;
  ticksRight = 0;
  isFwdRight = deltaTicksRight > 0;
  isFwdLeft = deltaTicksLeft > 0;

  int64_t tgtTicksRight = ticksRight + deltaTicksRight;
  int64_t tgtTicksLeft = ticksLeft + deltaTicksLeft;

  rotateLeft(isFwdLeft);
  rotateRight(isFwdRight);

  bool stoppedRight = false, stoppedLeft = false;
  while (!(stoppedLeft && stoppedRight)) {
    if ((isFwdRight ? (tgtTicksRight <= ticksRight)
                    : (tgtTicksRight >= ticksRight)) &&
        !stoppedRight) {
      stopRight();
      Serial.println("STOP right");
      stoppedRight = true;
    }
    if ((isFwdLeft ? (tgtTicksLeft <= ticksLeft)
                   : (tgtTicksLeft >= ticksLeft)) &&
        !stoppedLeft) {
      stopLeft();
      Serial.println("STOP left");
      stoppedLeft = true;
    }
    do_busywait();
  }
}

void go_mm(const long double deltaMmLeft, const long double deltaMmRight) {
  const int64_t deltaTicksLeft = deltaMmLeft * TICKS_PER_MM;
  const int64_t deltaTicksRight = deltaMmRight * TICKS_PER_MM;
  go(deltaTicksLeft, deltaTicksRight);
}


void setup() {
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_LEFT), tickLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_RIGHT), tickRight, FALLING);
  Serial.begin(115200);
  setLeftSpd(spd);
  setRightSpd(spd);
  stopLeft();
  stopRight();

  // pinMode(A0, OUTPUT);
  // pinMode(A1, OUTPUT);
  // digitalWrite(A0, HIGH);
  // digitalWrite(A1, LOW);

  // multiservo.attach(0);
  // multiservo.write(110);

  delay(2000);

  Serial.println("Waiting for start");
  while (!Serial.available());

  // Serial.println("START");
  // go_mm(0, 198);
  // go(0, TICKS_PER_DEGREE * 360);
  // Serial.println("STOP");

  // Serial.println("ZEROING");

  // go_mm(-100, -100);
  Serial.println("GOING");
  delay(1000);


  go_mm(WHEELS_DISTANCE_MM * 1.25, 0);
  Serial.println("WFD");
  go_mm(93 * 100, 93 * 100);
  Serial.println("ASYMMETRIC");
  setLeftSpd(spd / 2);
  setRightSpd(spd);
  go_mm(500, 1000);
  setLeftSpd(spd);
  setRightSpd(spd);
  go_mm(0, 300);
  go_mm(20 * 100, 20 * 100);
  setLeftSpd(spd);
  setRightSpd(spd * 0.8);
  go_mm(-200, 0);
  go_mm(-1000, -1000 * 0.8);

  delay(1000);
  setLeftSpd(spd);
  setRightSpd(spd);
  go_mm(140 * 100, 140 * 100);
  go_mm(-140 * 100, -140 * 100);

  // delay(2000);
  //
  //  //go_mm(1000, 1000);
  //
  //  go_mm(-WHEELS_DISTANCE_MM * 3 / 2,0);
  //  go_mm(-93*100, -93*100);
  //  motordriver.setSpeed(spd/2.5, RIGHT_MOTOR);
  //  motordriver.setSpeed(spd,LEFT_MOTOR );
  //  go_mm( -500,-1000);
  //  motordriver.setSpeed(spd, LEFT_MOTOR);
  //  motordriver.setSpeed(spd, RIGHT_MOTOR);
  //  go_mm(-300, 0);
  //  go_mm(-20*120, -20*120);
  //
  //  go_mm(200, 0);
  //
  //  motordriver.setSpeed(spd, RIGHT_MOTOR);
  //  motordriver.setSpeed(spd*0.8, LEFT_MOTOR );
  //  go_mm(1000*0.8, 1000);
  //
  //  delay(1000);
  //  motordriver.setSpeed(spd, LEFT_MOTOR);
  //  motordriver.setSpeed(spd, RIGHT_MOTOR);
  //  go_mm(-200*100, -200*100);
  //
}

void loop() {
  //  int roww = analogRead(A2);
  //  //Serial.println(analogRead(A2));
  //  if (roww < 350 && roww > 330)
  //  {
  //    Serial.print(roww);
  //    Serial.println(" 470 Om");
  //
  //  } else if (roww < 850 && roww > 830)
  //  { Serial.print(roww);
  //    Serial.println("  4.7 kOm");
  //  } else if (roww < 530 && roww > 510)
  //  { Serial.print(roww);
  //    Serial.println(" 1 kOm");
  //  }
  //  //myservo.write(180);
  //  delay(1000);
  //  myservo.write(0);
  //  delay(1000);
}
