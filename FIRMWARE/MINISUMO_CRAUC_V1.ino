//INCLUDES

//DEFINICIONES

//MOTOR IZQUIERDA POR FUNCION
#define MOTOR_PWMB 6
#define MOTOR_BIN2 8
#define MOTOR_BIN1 7

//MOTOR DERECHA POR FUNCION
#define MOTOR_AIN1 11
#define MOTOR_AIN2 10
#define MOTOR_PWMA 5

#define SENSOR_IN_P1 12
#define SENSOR_IN_P2 2

#define LED1 3
#define LED2 4

#define SENSOR_IN_FI A0
#define SENSOR_IN_LI 13  //CAMBIO A DIGITAL POR SENSOR DE FRENTE
#define SENSOR_IN_FD A4
#define SENSOR_IN_LD A3

#define SENSOR_IN_C  A1 //CAMBIO POR ANALOGO


#define DIPSW2       A7
#define DIPSW3       A6
#define DIPSW4       A5

#define MODULOSTART  A2

#define VELOCIDAD_ATAQUE        220
#define VELOCIDAD_BUSQUEDA      170

//VARIABLES

//Sensores
int cal_sp_l,cal_sp_d;
int sp_l,sp_d;
int SensorDistaciaCentral;
int detecto_oponente;

//estrategia
int dsw;
int opnente_det;
int bandera_inicio;
int timeout_str;

//Ataque
int a_ataque;
int a_ubicacion;
int control_md, control_mi;


void setup() {
  
  //CONFIGURACION DE PINES
  pinMode(SENSOR_IN_LI,INPUT);
  
  //PWM
  pinMode(MOTOR_PWMA,OUTPUT);
  pinMode(MOTOR_PWMB,OUTPUT);
  pinMode(MOTOR_AIN2,OUTPUT);
  pinMode(MOTOR_AIN1,OUTPUT);
  pinMode(MOTOR_BIN1,OUTPUT);
  pinMode(MOTOR_BIN2,OUTPUT);
  
  
  Control_Motores(0, 0);
  //TEST FUNCION DE MOTORES
    //Control_Motores(-250, 0);
    //delay(10000);

  
  pinMode(SENSOR_IN_P1,INPUT);
  pinMode(SENSOR_IN_P2,INPUT);

  pinMode(LED1,OUTPUT);
  pinMode(LED2,OUTPUT);

  pinMode(DIPSW2,INPUT);
  pinMode(DIPSW3,INPUT);
  pinMode(DIPSW4,INPUT);
  
  Serial.begin(9600);
  
  //CALIBRAR SENSORES DE PISO
   cal_sp_l = 0;
   cal_sp_d = 0;
   cal_sp_l = Leer_Sensor_Piso(SENSOR_IN_P1,cal_sp_l );
   cal_sp_d = Leer_Sensor_Piso(SENSOR_IN_P2, cal_sp_d );
   
   //LEER ESTRATEGIA
   dsw = leer_dipsw();
   bandera_inicio = 1;
   switch(dsw)
   {
      case 0:
            {//ESPERAR DE FRENTE --2 3 4 OFF
               control_md =  30;
               control_mi =  30;
               timeout_str = 2000;
               opnente_det = 110; //OPONENETE A IZQUIERDA
               break;
             }
      case 1:
            { //OPOENTE DE DERECHA DIAGONAL --2 ON 3 4 OFF
              control_md =  VELOCIDAD_ATAQUE/4;
              control_mi =  VELOCIDAD_ATAQUE;
              timeout_str = 2000;
              opnente_det =  10; //OPONENETE A DERECHA
              break;
             }         
       case 2:
            { //OPOENTE DE ATRAS CENTRO DE PISTA - GIRO DE UNA --2 OFF 3ON 4 OFF
              control_md = -VELOCIDAD_ATAQUE;
              control_mi =  VELOCIDAD_ATAQUE;
              timeout_str = 2000;
              opnente_det = 1000; //OPONENETE A IZQUIERDA
              break;
             }         
       case 3:
            { //OPOENTE DE DERECHA GIRO COMPLETO --2 ON 3ON 4 OFF
              control_md = -VELOCIDAD_ATAQUE;
              control_mi =  VELOCIDAD_ATAQUE;
              timeout_str = 2000;
              opnente_det = 1; //OPONENETE A IZQUIERDA
              break;
             }         
       case 4:
            {
              //OPOENTE DE IZQUIERDA DIAGONAL --2 OFF 3 OFF 4 ON
              control_md = VELOCIDAD_ATAQUE;
              control_mi = VELOCIDAD_ATAQUE/4;
              timeout_str = 2000;
              opnente_det = 100; //OPONENETE A IZQUIERDA
              break;
             }         
       case 5:
            { //OPOENTE DE FRENTE EN CENTRO DE PISTA --2 ON 3 OFF 4 ON
              control_md =  VELOCIDAD_ATAQUE;
              control_mi =  VELOCIDAD_ATAQUE;
              timeout_str = 2000;
              opnente_det = 110; //OPONENETE A IZQUIERDA
              break;
             }         
       case 6:
            { //OPOENTE DE IZQUIERDA GIRO TOTAL --2 OFF 3 ON 4 ON
              control_md = -VELOCIDAD_ATAQUE;
              control_mi =  VELOCIDAD_ATAQUE;
              timeout_str = 2000;
              opnente_det = 1000; //OPONENETE A IZQUIERDA
              break;
             }         
       case 7:
            { //OPONENTE ATRAS EN BORDE DE PISTA MINISUMO--2 3 4 ON
                control_md = -VELOCIDAD_ATAQUE;
                control_mi = -VELOCIDAD_ATAQUE;
                timeout_str = 2000;
                opnente_det = 1000; //OPONENETE A IZQUIERDA
              break;
             }
         default:
        {
          opnente_det == 1000; //OPONENETE A IZQUIERDA
          break;
        }
             
   
   }
  
  delay(1000);

   int Analogpw = analogRead(MODULOSTART);
   Analogpw = analogRead(MODULOSTART);
   while(Analogpw < 200)
   {
      Serial.print(" POWER : ");
      Serial.println(Analogpw);
      Analogpw = analogRead(MODULOSTART);
   }
   
   
  
}


void loop() {
    
   int Analogpw= analogRead(MODULOSTART);
   Serial.print(" POWER : ");
   Serial.println(Analogpw);
   if(Analogpw < 200)
   {
      Control_Motores(0, 0);
      while(1); //SE APAGA
   }
  
    //LECTURA DE SENSORES
  
  
    //Serial.print(" Oponete : ");
    //Serial.print(opnente_det);
    
    //Serial.print("  OP: ");
    //Serial.print(detecto_oponente);
    
    a_ubicacion = Detectar_Ubicacion();
    //Serial.print("  a_ubicacion : ");
    //Serial.print(a_ubicacion);
    
    
      
   int Analogain = analogRead(SENSOR_IN_C);
   if(Analogain > 200 && Analogain < 700)
   {
      SensorDistaciaCentral = 1;
   }
   else
   {
      SensorDistaciaCentral = 0;
   }
   
    //Serial.print("  CENTRO : ");
    //Serial.print(SensorDistaciaCentral);
    //Serial.print("  DISTANCIA : ");
    //Serial.print(Analogain);
    
    
    sp_l = Leer_Sensor_Piso(SENSOR_IN_P1,cal_sp_l );
    //Serial.print(" -- SPI : ");
    //Serial.print(sp_l);
  
    sp_d = Leer_Sensor_Piso(SENSOR_IN_P2,cal_sp_d );
    //Serial.print("  SPD : ");
    //Serial.print(sp_d);
  
  
  //ESTARTEGIA DE INICIO
  if(bandera_inicio == 1)
  {
    if(timeout_str) //HAY TIEMPO
    {
      timeout_str --;
    }
  }
  
  if(timeout_str <= 0 )
  {
    bandera_inicio = 0;
  }
  
  ///////////////INICIO DE ESTRATEGIA DE BATALLA
  //PRIMERA PRIORIDAD ATAQUE CON SENSORES CERCANOS
  if(a_ubicacion != -1)
  {
    detecto_oponente = 1; // PARA DEBUGER
    
    bandera_inicio = 0; //SE ACABA ESTRATEGIA
    timeout_str = 0;
    
    if(a_ubicacion > 100)
    {
      //OPONENTE A DERECHA
      control_md = -VELOCIDAD_ATAQUE;
      control_mi = VELOCIDAD_ATAQUE;
      opnente_det = 1; //0001 --SENSORES - NUMERO PARA RECORDAR ULTIMA UBICACION
    }
    else if(a_ubicacion > 0)
    {// entre 100 y 0 -- oponente centro derecha
       control_md = -VELOCIDAD_ATAQUE/4;
       control_mi =  VELOCIDAD_ATAQUE;
       opnente_det = 10; //0010 --SENSORES
    }
    else if(a_ubicacion < -100)
    {
      //OPONENTE A IZQUIERDA
       control_md = VELOCIDAD_ATAQUE;
       control_mi = -VELOCIDAD_ATAQUE;
       opnente_det = 1000; //1000 --SENSORES 
    }
    else if(a_ubicacion < 0)
    {// entre -100 y 0 -- oponente centro izquierda
       control_md = VELOCIDAD_ATAQUE;
       control_mi = -VELOCIDAD_ATAQUE/4;
       opnente_det = 100; //0100 --SENSORES
    }
    else
    { //OPONENTE AL CENTRO
        control_md = 250;
        control_mi = 250;
        opnente_det = 110; //0110
    }   
        
  }
  
  //SEGUNDA SENSORES DE PISO
  /*
  //SENSOR PISO
  else if(abs(sp_l) > 20 || abs(sp_d) > 20)
  {
    detecto_oponente = 9;
    
    bandera_inicio = 0; //SE ACABA ESTRATEGIA
    timeout_str = 0;
    
     if(abs(sp_l) > 20 && abs(sp_d) > 20)
     {
          control_md = -VELOCIDAD_BUSQUEDA;
          control_mi = -VELOCIDAD_BUSQUEDA;
     }
     else if(abs(sp_l) > 20)
     {
        control_md = -VELOCIDAD_BUSQUEDA;
        control_mi = -VELOCIDAD_BUSQUEDA;
     }
     else if(abs(sp_d) > 20)
     {
        control_md = -VELOCIDAD_BUSQUEDA;
        control_mi = -VELOCIDAD_BUSQUEDA;
     }
     
  }
  */
  //TERCERA PRIORIDAD SENSOR DE DISTANCIA CENTRAL DETECTADO y BUSQUEDA
 
  else if(SensorDistaciaCentral == 1)
  { 
    
    detecto_oponente = 6;
    
    bandera_inicio = 0; //SE ACABA ESTRATEGIA
    timeout_str = 0;
    
    
    
    // SE REVISA EN QUE SENTIDO GIRO PARA CORREGIR
    if(opnente_det == 1 || opnente_det == 10)
    {
      //OPONENTE DETECTADO GIRANDO A DERECHA - MOTORES A IZQUIERDA CAMBIO DE GIRO
      control_md = VELOCIDAD_BUSQUEDA;
      control_mi = VELOCIDAD_BUSQUEDA/2;
      opnente_det = 100;
    }
    else if(opnente_det == 1000 || opnente_det == 100 )
    {
      //OPONENTE DETECTADO GIRANDO A IZQUIERDA - MOTORES A DERECHA CAMBIO DE GIRO
       control_md = VELOCIDAD_BUSQUEDA/2;
       control_mi = VELOCIDAD_BUSQUEDA;
       opnente_det = 1;
    }
    else
    { //OPONENTE AL CENTRO --  LO PERDI CERCANO-- ES EXTRAÑO -- BUSCO AL LADO -- NO DE FRENTE PUEDO SALIRME AL BORDE
         control_md = VELOCIDAD_BUSQUEDA;
         control_mi = -VELOCIDAD_BUSQUEDA;
         opnente_det = 100;
    }  
  }

  ///////////////////////////////////////////////////////////////////////

  else //NO OPONENTE - NO PISO
  {
    detecto_oponente = 0;
    
    //bandera_inicio = 0; // AQI NOP, NO HA ENCONTRADO ENEMIGO
    
    if(opnente_det == 1 || opnente_det == 10)
    {
      //OPONENTE A DERECHA
      control_md =  -VELOCIDAD_BUSQUEDA;
      control_mi =   VELOCIDAD_BUSQUEDA;
    }
    else if(opnente_det == 1000 || opnente_det == 100)
    {
      //OPONENTE A IZQUIERDA
       control_md =  VELOCIDAD_BUSQUEDA;
       control_mi = -VELOCIDAD_BUSQUEDA;
    }
    else
    { //OPONENTE AL CENTRO --  LO PERDI -- BUSCO AL LADO -- NO DE FRENTE
        control_md = VELOCIDAD_BUSQUEDA;
        control_mi = -VELOCIDAD_BUSQUEDA;
        
        opnente_det = 100;
     
    }   
  }
  
    Control_Motores(control_mi, control_md);
    
    //PARA DEBUGER
    //Serial.print("  control_mi : ");
    //Serial.print(control_mi);
    
    //Serial.print("  control_md : ");
    //Serial.println(control_md);
    
  delay(1);
}


/////////////////////////////////////////////////////////////////
///FUNCIONES LOCALES
/////////////////////////////////////////////////////////////////

int Leer_Sensor_Piso(int pin, int cal)
{
  int PinState;
  int cnt;
  int sensor;
  
   pinMode(pin,OUTPUT);
   digitalWrite(pin, HIGH);
   delay(1);
  
   pinMode(pin,INPUT);
   
   
   cnt = 0;
   do{
     cnt ++;
     PinState = digitalRead(pin);
   }while(PinState == HIGH && cnt < 300);
     
   sensor = cnt - cal;
   
   return sensor;
   //if(sensor > 0)
   //    return 0;
   //else
   //    return 1;
  
}

void Control_Motores(int mi, int md)
{
 if(md > 255)
  {
    md = 255;
  }
  else if (md < -255)
  {
     md = -255;
  }
  
  if(md > 0)
  {
    digitalWrite(MOTOR_AIN1, LOW);
    digitalWrite(MOTOR_AIN2, HIGH);
    analogWrite(MOTOR_PWMA, abs(md));
  }
  else if(md < 0)
   {
    digitalWrite(MOTOR_AIN1, HIGH);
    digitalWrite(MOTOR_AIN2, LOW);
    analogWrite(MOTOR_PWMA, abs(md));
  }
  else
  {  //BREAK
     digitalWrite(MOTOR_AIN1, HIGH);
     digitalWrite(MOTOR_AIN2, HIGH);
  }
  
  //MOTOR IZQUIERDO
  if(mi > 255)
  {
    mi = 255;
  }
  else if (mi < -255)
  {
     mi = -255;
  }
  
  if(mi > 0)
  {
    digitalWrite(MOTOR_BIN1, LOW);
    digitalWrite(MOTOR_BIN2, HIGH);
    analogWrite(MOTOR_PWMB, abs(mi));
  }
  else if(mi < 0)
   {
    digitalWrite(MOTOR_BIN1, HIGH);
    digitalWrite(MOTOR_BIN2, LOW);
    analogWrite(MOTOR_PWMB, abs(mi));
  }
  else
  { //BREAK
     digitalWrite(MOTOR_BIN1, HIGH);
     digitalWrite(MOTOR_BIN2, HIGH);
  }
  
}

int leer_dipsw(void)
{
  int value;
  int buttonState = analogRead(DIPSW2);
  Serial.print("  SW2 : ");
  Serial.print(buttonState);
  if (buttonState > 0) 
    {
    digitalWrite(LED1, LOW);
    value = 0;
    }
    else 
    {
    digitalWrite(LED1, HIGH);
    value = 1;
    }
   
  buttonState = analogRead(DIPSW3);
  Serial.print("  SW3 : ");
  Serial.print(buttonState);
  if (buttonState > 0) 
    {
    digitalWrite(LED2, LOW);
    }
    else 
    {
    digitalWrite(LED2, HIGH);
     value = value + 2;
    }
  
  buttonState = analogRead(DIPSW4);
  Serial.print("  SW4 : ");
  Serial.print(buttonState);
   if (buttonState > 0) 
    {
    digitalWrite(LED2, HIGH);
    }
    else 
    {
    digitalWrite(LED2, HIGH);
    value = value + 4;
    }

  return value; 
}



int Detectar_Ubicacion(void)
{
 int SensorState;
 int Oponente = 0;
 int num_sensores = 0;
 //SENSORES
  digitalWrite(SENSOR_IN_LI, HIGH);
  SensorState = digitalRead(SENSOR_IN_LI); //CAMBIO A DIGITAL
  if(SensorState == LOW )
  {
     Oponente = -200;
     num_sensores++;
  }
  
  
  SensorState = analogRead(SENSOR_IN_FI);
   if(SensorState < 500 )
  {
     Oponente = Oponente -100;
     num_sensores++;
  }
  
  
  SensorState = analogRead(SENSOR_IN_FD);
   if(SensorState < 500 )
  {
     Oponente = Oponente + 100;
     num_sensores++;
  }
  
  SensorState = analogRead(SENSOR_IN_LD);
   if(SensorState < 500 )
  {
     Oponente = Oponente + 200;
     num_sensores++;
  }
  
    //Serial.print(" num_sensores : ");
    //Serial.print(num_sensores);
    
    //Serial.print(" Oponente : ");
    //Serial.print(Oponente);
    
  if(num_sensores)
  {
     return Oponente/num_sensores;
  }
  else
  {
     return -1;
  }
}
  
