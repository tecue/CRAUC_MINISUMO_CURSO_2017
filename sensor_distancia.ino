#define MODULOSTART  A2

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
   int Analogpw = analogRead(MODULOSTART);
   Serial.print(" POWER : ");
   Serial.println(Analogpw);
}
