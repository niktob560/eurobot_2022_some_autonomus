/*
 * Before using this scketch you can read https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogInput
 * Use 300-400Ohm resistor in the scheme. 
 */


int checkResistor(int sensorPin) {
  const int constRes = 330;
  int sensormV = 4.887 * analogRead(sensorPin);
  if (sensormV < 50) return -1;
  
  int ohm = 5000 / sensormV * constRes - constRes;
  
  return ohm;
}


int checkResNum(int sensorPin) {
  /*
   * Gets:
   *   Analog pin with 0..5V voltage interval.
   * Returns:
   *   -1 if resistor not connected;
   *   0 if no resistance or resistor unidentified;
   *   1 for 470Ohm resistor;
   *   2 for 1kOhm;
   *   3 for 4.7kOhm.
   */
  const int interval = 200;
  const int firstRes = 470;
  const int secondRes = 1000;
  const int thirdRes = 4700;
  
  int ohm = checkResistor(sensorPin);
  if (ohm == -1) return -1;
  else if (ohm > firstRes - interval && ohm < firstRes + interval) {
    return 1;
  }
  else if (ohm > secondRes - interval && ohm < secondRes + interval) {
    return 2;
  }
  else if (ohm > thirdRes - interval && ohm < thirdRes + interval) {
    return 3;
  }
  else return 0;
}


void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println(checkResNum(A0));
  delay(100);
}
