#include <MotorDriver.h>
#include <stdint.h>
#include <Multiservo.h>

// Создаём объект для работы с сервомоторами
Multiservo multiservo;

#include "PriUint64.h"

#define LEFT_MOTOR MOTORB
#define RIGHT_MOTOR MOTORA

#define PIN_ENCODER_LEFT 2
#define PIN_ENCODER_RIGHT 3
//1,2 - левый
//3,4 - правый


volatile int64_t ticksLeft = 0;
volatile int64_t ticksRight = 0;
volatile bool isFwdLeft = true;
volatile bool isFwdRight = true;
int roww;
volatile void tickLeft(void) {
  ticksLeft += isFwdLeft ? 1 : -1;
}

volatile void tickRight(void) {
  ticksRight += isFwdRight ? 1 : -1;
}

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

    
      roww = analogRead(A2);
  //Serial.println(analogRead(A2));
  if (roww < 350 && roww > 330)
  {
//    Serial.print(roww);
//    Serial.println(" 470 Om");
  multiservo.write(0);
    motordriver.setSpeed(0, LEFT_MOTOR);
  motordriver.setSpeed(0, RIGHT_MOTOR);
  delay(500);
    motordriver.setSpeed(spd, LEFT_MOTOR);
  motordriver.setSpeed(spd, RIGHT_MOTOR);
   multiservo.write(110);
  } else if (roww < 850 && roww > 830)
  { 
//    Serial.print(roww);
//    Serial.println("  4.7 kOm");
  } else if (roww < 530 && roww > 510)
  { 
//    Serial.print(roww);
//    Serial.println(" 1 kOm");
  }else
   multiservo.write(110);

  
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

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  
multiservo.attach(0);
  multiservo.write(110);
  
  delay(2000);

    go_mm(-1000, -1000);
  
    go_mm(WHEELS_DISTANCE_MM * 3 / 2, 0);
    go_mm(93*100, 93*100);
    motordriver.setSpeed(spd/2.5, LEFT_MOTOR);
    motordriver.setSpeed(spd, RIGHT_MOTOR);
    go_mm(500, 1000);
    motordriver.setSpeed(spd, LEFT_MOTOR);
    motordriver.setSpeed(spd, RIGHT_MOTOR);
    go_mm(0, 300);
    go_mm(20*100, 20*100);
    motordriver.setSpeed(spd, LEFT_MOTOR);
    motordriver.setSpeed(spd*0.8, RIGHT_MOTOR);
    go_mm(-200, 0);
    go_mm(-1000, -1000*0.8);
  
    delay(1000);
    motordriver.setSpeed(spd, LEFT_MOTOR);
    motordriver.setSpeed(spd, RIGHT_MOTOR);
    go_mm(140*100, 140*100);
    go_mm(-140*100, -140*100);






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
