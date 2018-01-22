

int Leer_Sensor_Piso(int pin);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor_piso = Leer_Sensor_Piso(3);
  Serial.println(sensor_piso);
  delay(5);

}

int Leer_Sensor_Piso(int pin)
{
  int PinState;
  int cnt;
  int sensor;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(1);

  pinMode(pin, INPUT);

  cnt = 0;

  do{
    cnt ++;
    PinState = digitalRead(pin);
    }while(PinState == HIGH && cnt < 300);

 sensor = cnt;
 
 return sensor;
}
  
  
  
  
  
