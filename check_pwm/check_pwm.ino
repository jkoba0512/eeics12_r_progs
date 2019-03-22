// pin number
const int PIN_MOTOR_PWM_1 = 3;  // pwm output pin number

void setup() {
  // set pin mode
  pinMode(PIN_MOTOR_PWM_1, OUTPUT);

  // serial
  Serial.begin(9600);
}

void loop() {
  // pwm_com (0 ~ 255)
  static int pwm_com_1 = 0;
  
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      case 'a':
        pwm_com_1 = limitCOM(pwm_com_1 + 10);
        break;
      case 'z':
        pwm_com_1 = limitCOM(pwm_com_1 + 1);
        break;
      case 's':
        pwm_com_1 = 0;
        break;
      case 'd':
        pwm_com_1 = limitCOM(pwm_com_1 - 10);
        break;
      case 'c':
        pwm_com_1 = limitCOM(pwm_com_1 - 1);
        break;
      default:
        ;
    }

    // output pwm
    analogWrite(PIN_MOTOR_PWM_1, pwm_com_1);

    // print pwm_com_1
    Serial.println(pwm_com_1);
  }
}

int limitCOM(int _com) {
  if (_com >  255) return 255;
  if (_com < 0) return 0;
  return _com;
}
