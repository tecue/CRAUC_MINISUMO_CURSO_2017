
//MOTOR IZQUIERDA POR FUNCION
#define MOTOR_PWMB 6
#define MOTOR_BIN2 8
#define MOTOR_BIN1 7

//MOTOR DERECHA POR FUNCION
#define MOTOR_PWMA 5
#define MOTOR_AIN1 11
#define MOTOR_AIN2 10



void setup() {
  //PWM
  pinMode(MOTOR_PWMA,OUTPUT);
  pinMode(MOTOR_PWMB,OUTPUT);
  
  pinMode(MOTOR_AIN2,OUTPUT);
  pinMode(MOTOR_AIN1,OUTPUT);
  
  pinMode(MOTOR_BIN1,OUTPUT);
  pinMode(MOTOR_BIN2,OUTPUT);

}

void loop() {
  
    digitalWrite(MOTOR_AIN1, HIGH);
    digitalWrite(MOTOR_AIN2, LOW);
    analogWrite(MOTOR_PWMA, 230);//0-255 

    digitalWrite(MOTOR_BIN1, LOW);
    digitalWrite(MOTOR_BIN2, HIGH);
    analogWrite(MOTOR_PWMB, 100);

}
