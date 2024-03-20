#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Zmienna
bool p1 = false;              //sygnał ukończenia 1 etapu ruchu
bool p2 = false;              //sygnał ukończenia 2 etapu ruchu
bool p3 = false;              //sygnał ukończenia 3 etapu ruchu
int offset=10;                //wartość do zmiany wysokości podnoszenia nóg hexapoda
int status=0   ;              //Zmienna do przechowywania sygnałów z aplikacji
int FM1 = 0;                  //Zmienna pomocnicza przy wykonywaniu ruchu nogą
int FM2 = 0;                  //Zmienna pomocnicza przy wykonywaniu ruchu nogą
int FM3 = 0;                  //Zmienna pomocnicza przy wykonywaniu ruchu nogą
int i = 75;                   //Zmienna wykorzystywania do pozycjonowania nóg w trybie MECANUM
int test1=20;
int test2=15;

////////////////////PINY DO ŚILNIKÓW DC////////////////////
int m1_1=3;
int m1_2=4;

int m2_1=5;
int m2_2=6;

int m3_1=8;
int m3_2=9;

int m4_1=10;
int m4_2=11;


////////////////////POZYCJE DOMYŚLNE///////////////////////
int hl1s1=97;
int hl1s2=93-test1;
int hl1s3=122-test2;

int hl2s1=82;
int hl2s2=100-test1;
int hl2s3=128-test2;

int hl3s1=90;
int hl3s2=87-test1;
int hl3s3=130-test2;

int hl4s1=77;
int hl4s2=75+test1;
int hl4s3=55+test2;

int hl5s1=81;
int hl5s2=82+test1;
int hl5s3=45+test2;

int hl6s1=81;
int hl6s2=79+test1;
int hl6s3=46+test2;

Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

void setup() {
  Serial.begin(9600);
  pwm1.begin();
  pwm1.setPWMFreq(60);  // Hz do serw
  pwm2.begin();
  pwm2.setPWMFreq(60);  // Hz do serw

  pinMode(m1_1, OUTPUT);
  pinMode(m1_2, OUTPUT);
  pinMode(m2_1, OUTPUT);
  pinMode(m2_1, OUTPUT);
  pinMode(m3_1, OUTPUT);
  pinMode(m3_2, OUTPUT);
  pinMode(m4_1, OUTPUT);
  pinMode(m4_1, OUTPUT);
  start();
}

void loop() {
  if(Serial.available()>0){   //Sprawdź czy otrzymano dane z modułu HC-05
    status=Serial.read();     //Przypisanie informacji z modułu HC-05 do zmiennej 'status'
  }
  //Odczyt zmiennej "status" i wykonanie odpowiedniej funkcji
  if (status=='1'){
    get_up();
  }
  if (status=='2'){
    get_down();
  }
  if (status=='3'){
    move_frwd();
  }
  if (status=='4'){
    turn_right();
  }
  if (status=='5'){
    move_bckwd();
  }
  if (status=='6'){
    turn_left();
  }
  if (status=='7'){
    home();
  }
  if (status=='8'){
    ride_frwd();
  }
  if (status=='9'){
    ride_FR();
  }
  if (status==':'){
    ride_right();
  }
  if (status==';'){
    ride_BR();
  }
  if (status=='<'){
    ride_bckwd();
  }
  if (status=='='){
    ride_BL();
  }
  if (status=='>'){
    ride_left();
  }
  if (status=='?'){
    ride_FL();
  }
  if (status=='@'){
    stop();
  }
  if (status=='A'){
    MEC_turn_right();
  }
  if (status=='B'){
    MEC_turn_left();
  }
  delay(25);
}

//////////////////////////////////MECANUM MOVEMENT//////////////////////////////////
//Kontrola ruchu robota z kołami mecanum odbywa się poprzez kontrolę kierunku obrotu poszczególnych napędów
void stop(){                //Funkcja zatrzymująca robota
  digitalWrite(m1_1, LOW);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1, LOW);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, LOW);
}//Stop Wheel Movement

void ride_bckwd(){          //Funkcja jazdy do tyłu
  digitalWrite(m1_1,  LOW);
  digitalWrite(m1_2, HIGH);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);

  digitalWrite(m3_1,  HIGH);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, HIGH);
  digitalWrite(m4_2, LOW);
}//End Ride Backward

void ride_frwd(){           //Funkcja jazdy do przodu
  digitalWrite(m1_1,  HIGH);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1,  LOW);
  digitalWrite(m3_2, HIGH);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, HIGH);
}//End Ride Forward

void ride_right(){          //Funkcja jazdy w prawo
  digitalWrite(m1_1,  HIGH);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);

  digitalWrite(m3_1,  LOW);
  digitalWrite(m3_2, HIGH);

  digitalWrite(m4_1, HIGH);
  digitalWrite(m4_2, LOW);
}//End Ride Right

void ride_left(){           //Funkcja jazdy w lewo
  digitalWrite(m1_1,  LOW);
  digitalWrite(m1_2, HIGH);

  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1,  HIGH);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, HIGH);
}//End Ride Left

void ride_FR(){             //Funkcja jazdy po skosie do przodu w prawo
  digitalWrite(m1_1,  HIGH);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1,  LOW);
  digitalWrite(m3_2, HIGH);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, LOW);
}//End Ride Forward Right

void ride_FL(){             //Funkcja jazdy po skosie do przodu w lewo
  digitalWrite(m1_1,  LOW);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1,  LOW);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, HIGH);
}//End Ride Forward Right

void ride_BL(){             //Funkcja jazdy po skosie do tyłu w lewo
  digitalWrite(m1_1,  LOW);
  digitalWrite(m1_2, HIGH);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1,  HIGH);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, LOW);
}//End Ride Backward Left

void ride_BR(){             //Funkcja jazdy po skosie do tyłu w prawo
  digitalWrite(m1_1,  LOW);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);

  digitalWrite(m3_1,  LOW);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, HIGH);
  digitalWrite(m4_2, LOW);
}//End Ride Backward Right

void MEC_turn_right(){      //Obrót w prawo
  digitalWrite(m1_1,  HIGH);
  digitalWrite(m1_2, LOW);

  digitalWrite(m2_1, LOW);
  digitalWrite(m2_2, HIGH);

  digitalWrite(m3_1,  HIGH);
  digitalWrite(m3_2, LOW);

  digitalWrite(m4_1, LOW);
  digitalWrite(m4_2, HIGH);
}//End Turn Right

void MEC_turn_left(){       //Obrót w lewo
  digitalWrite(m1_1,  LOW);
  digitalWrite(m1_2, HIGH);

  digitalWrite(m2_1, HIGH);
  digitalWrite(m2_2, LOW);

  digitalWrite(m3_1,  LOW);
  digitalWrite(m3_2, HIGH);

  digitalWrite(m4_1, HIGH);
  digitalWrite(m4_2, LOW);
}//End Turn left

//////////////////////////////////HEXAPOD MOVEMENT//////////////////////////////////
void home(){                                  //Domyślna pozycja stojąca robota
  pwm1.setPWM(0, 0, angleToPulse(hl1s1) );
  pwm1.setPWM(1, 0, angleToPulse(hl1s2) );
  pwm1.setPWM(2, 0, angleToPulse(hl1s3) );

  pwm1.setPWM(4, 0, angleToPulse(hl2s1) );
  pwm1.setPWM(5, 0, angleToPulse(hl2s2) );
  pwm1.setPWM(6, 0, angleToPulse(hl2s3) );

  pwm1.setPWM(8, 0, angleToPulse(hl3s1) );
  pwm1.setPWM(9, 0, angleToPulse(hl3s2) );
  pwm1.setPWM(10, 0, angleToPulse(hl3s3) );

  pwm2.setPWM(0, 0, angleToPulse(hl4s1) );
  pwm2.setPWM(1, 0, angleToPulse(hl4s2) );
  pwm2.setPWM(2, 0, angleToPulse(hl4s3) );

  pwm2.setPWM(4, 0, angleToPulse(hl5s1) );
  pwm2.setPWM(5, 0, angleToPulse(hl5s2) );
  pwm2.setPWM(6, 0, angleToPulse(hl5s3) );

  pwm2.setPWM(8, 0, angleToPulse(hl6s1) );
  pwm2.setPWM(9, 0, angleToPulse(hl6s2) );
  pwm2.setPWM(10, 0, angleToPulse(hl6s3) );
}//Base Standing Position

void start(){                                     //Funkcja uruchamiana przy starci robota podnosząca i centrująca jego nogi
    pwm1.setPWM(1, 0, angleToPulse(hl1s2 + 75) );                //podniesienie nogi 1
    pwm1.setPWM(5, 0, angleToPulse(hl2s2 + 75) );                //podniesienie nogi 2
    pwm1.setPWM(9, 0, angleToPulse(hl3s2 + 75) );                //podniesienie nogi 3
    
    pwm2.setPWM(1, 0, angleToPulse(hl4s2 - 75) );                //podniesienie nogi 4
    pwm2.setPWM(5, 0, angleToPulse(hl5s2 - 75) );                //podniesienie nogi 5
    pwm2.setPWM(9, 0, angleToPulse(hl6s2 - 75) );                //podniesienie nogi 6
    pwm1.setPWM(0, 0, angleToPulse(hl1s1) );
    pwm1.setPWM(2, 0, angleToPulse(hl1s3) );
    pwm1.setPWM(4, 0, angleToPulse(hl2s1) );
    pwm1.setPWM(6, 0, angleToPulse(hl2s3) );
    pwm1.setPWM(8, 0, angleToPulse(hl3s1) );
    pwm1.setPWM(10, 0, angleToPulse(hl3s3) );
    pwm2.setPWM(0, 0, angleToPulse(hl4s1) );
    pwm2.setPWM(2, 0, angleToPulse(hl4s3) );
    pwm2.setPWM(4, 0, angleToPulse(hl5s1) );
    pwm2.setPWM(6, 0, angleToPulse(hl5s3) );
    pwm2.setPWM(8, 0, angleToPulse(hl6s1) );
    pwm2.setPWM(10, 0, angleToPulse(hl6s3) );
  }
//Lifting legs on startup

void get_up(){                                    //Funkcja zmiany z trybu jazdy w tryb chodzenia
  while (i<75){
    pwm1.setPWM(1, 0, angleToPulse(hl1s2 + i) ); //opuszczenie nogi 1
    pwm1.setPWM(5, 0, angleToPulse(hl2s2 + i) ); //opuszczenie nogi 2
    pwm1.setPWM(9, 0, angleToPulse(hl3s2 + i) ); //opuszczenie nogi 3
    
    pwm2.setPWM(1, 0, angleToPulse(hl4s2 - i) ); //opuszczenie nogi 4
    pwm2.setPWM(5, 0, angleToPulse(hl5s2 - i) ); //opuszczenie nogi 5
    pwm2.setPWM(9, 0, angleToPulse(hl6s2 - i) ); //opuszczenie nogi 6
    i++;
    //Wyśrodkowanie nóg
    pwm1.setPWM(0, 0, angleToPulse(hl1s1) );
    pwm1.setPWM(2, 0, angleToPulse(hl1s3) );
    pwm1.setPWM(4, 0, angleToPulse(hl2s1) );
    pwm1.setPWM(6, 0, angleToPulse(hl2s3) );
    pwm1.setPWM(8, 0, angleToPulse(hl3s1) );
    pwm1.setPWM(10, 0, angleToPulse(hl3s3) );
    pwm2.setPWM(0, 0, angleToPulse(hl4s1) );
    pwm2.setPWM(2, 0, angleToPulse(hl4s3) );
    pwm2.setPWM(4, 0, angleToPulse(hl5s1) );
    pwm2.setPWM(6, 0, angleToPulse(hl5s3) );
    pwm2.setPWM(8, 0, angleToPulse(hl6s1) );
    pwm2.setPWM(10, 0, angleToPulse(hl6s3) );
    delay(60);
  }
}//

void get_down(){                                  //Funkcja zmiany z trybu chodzenia w tryb jazdy
  while (i>0){
    pwm1.setPWM(1, 0, angleToPulse(hl1s2 + i) ); //podniesienie nogi 1
    pwm1.setPWM(5, 0, angleToPulse(hl2s2 + i) ); //podniesienie nogi 2
    pwm1.setPWM(9, 0, angleToPulse(hl3s2 + i) ); //podniesienie nogi 3
    
    pwm2.setPWM(1, 0, angleToPulse(hl4s2 - i) ); //podniesienie nogi 4
    pwm2.setPWM(5, 0, angleToPulse(hl5s2 - i) ); //podniesienie nogi 5
    pwm2.setPWM(9, 0, angleToPulse(hl6s2 - i) ); //podniesienie nogi 6
    i--;
    pwm1.setPWM(0, 0, angleToPulse(hl1s1) );
    pwm1.setPWM(2, 0, angleToPulse(hl1s3) );
    pwm1.setPWM(4, 0, angleToPulse(hl2s1) );
    pwm1.setPWM(6, 0, angleToPulse(hl2s3) );
    pwm1.setPWM(8, 0, angleToPulse(hl3s1) );
    pwm1.setPWM(10, 0, angleToPulse(hl3s3) );
    pwm2.setPWM(0, 0, angleToPulse(hl4s1) );
    pwm2.setPWM(2, 0, angleToPulse(hl4s3) );
    pwm2.setPWM(4, 0, angleToPulse(hl5s1) );
    pwm2.setPWM(6, 0, angleToPulse(hl5s3) );
    pwm2.setPWM(8, 0, angleToPulse(hl6s1) );
    pwm2.setPWM(10, 0, angleToPulse(hl6s3) );
    delay(60);
  }
}//Lifting Legs for MECANUM Mode

void move_frwd(){                           //Funkcja realizująca ruch do przodu
    ////////////////////////////////////////////////////////////////ETAP 1  //////////////////////////////////////////////////////////////
  if (!p1 && !p2 && !p3 ){  
    if (FM1<=10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 + FM1) );                  //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + 2*FM1+offset) );         //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - 2*FM1) );                //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 + FM1) );                  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + 2*FM1+offset) );         //podniesienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - 2*FM1) );               //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 - FM1) );                  //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - 2*FM1-offset) );         //podniesienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + 2*FM1) );                //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 - FM1) );                  //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 + FM1) );                  //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 + FM1) );                  //ruch nogi 6 do tyłu
      
      FM1++;
      FM1++;
    }
    if (FM1<=20 && FM1>10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 + FM1) );                  //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - ((2*FM1)-20)) );    //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + ((2*FM1)-20)) );    //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 + FM1) );                  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - ((2*FM1)-20)) );    //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + ((2*FM1)-20)) );   //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 - FM1) );                  //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + ((2*FM1)-20)) );    //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - ((2*FM1)-20)) );    //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 - FM1) );                  //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 + FM1) );                  //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 + FM1) );                  //ruch nogi 6 do tyłu
      
      FM1++;
      FM1++;
    }
    if (FM1==20){
      p1=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 2  ////////////////////////////////////////////////////////////////
  if (p1 && !p2 && !p3){  
  if (FM2<=20){
      pwm1.setPWM(4, 0, angleToPulse(hl2s1-20 + FM2) ); //ruch nogi 2 do przodu
      pwm1.setPWM(5, 0, angleToPulse(hl2s2 + FM2 + offset) ); //podniesienie nogi 2
      pwm1.setPWM(6, 0, angleToPulse(hl2s3 - FM2) ); //podniesienie nogi  2

      pwm2.setPWM(0, 0, angleToPulse(hl4s1+20 - FM2) ); //ruch nogi 4 do przodu
      pwm2.setPWM(1, 0, angleToPulse(hl4s2 - FM2 - offset) ); //podniesienie nogi 4
      pwm2.setPWM(2, 0, angleToPulse(hl4s3 + FM2) ); //podniesienie nogi  4
        
      pwm2.setPWM(8, 0, angleToPulse(hl6s1+20 - FM2) ); //ruch nogi 6 do przodu
      pwm2.setPWM(9, 0, angleToPulse(hl6s2 - FM2 - offset) ); //podniesienie nogi 6
      pwm2.setPWM(10, 0, angleToPulse(hl6s3 + FM2) ); //podniesienie nogi  6

      pwm1.setPWM(0, 0, angleToPulse((hl1s1+FM1) - FM2) ); //ruch nogi 1 do tyłu
      pwm1.setPWM(8, 0, angleToPulse((hl3s1+FM1) - FM2) ); //ruch nogi 3 do tyłu
      pwm2.setPWM(4, 0, angleToPulse((hl5s1-FM1) + FM2) ); //ruch nogi 5 do tyłu
      FM2++;
      FM2++;
    }
  if (FM2<=40 && FM2>20){
      pwm1.setPWM(4, 0, angleToPulse(hl2s1-20 + FM2) ); //ruch nogi 2 do przodu
      pwm1.setPWM(5, 0, angleToPulse((hl2s2+20) - (FM2-20)) ); //postawienie nogi 2
      pwm1.setPWM(6, 0, angleToPulse((hl2s3-20) + (FM2-20)) ); //postawienie nogi 2

      pwm2.setPWM(0, 0, angleToPulse(hl4s1+20 - FM2) ); //ruch nogi 4 do przodu
      pwm2.setPWM(1, 0, angleToPulse((hl4s2-20) + (FM2-20)) ); //postawienie nogi 4
      pwm2.setPWM(2, 0, angleToPulse((hl4s3+20) - (FM2-20)) ); //postawienie nogi 4

      pwm2.setPWM(8, 0, angleToPulse(hl6s1+20 - FM2) ); //ruch nogi 6 do przodu
      pwm2.setPWM(9, 0, angleToPulse((hl6s2-20) + (FM2-20)) ); //postawienie nogi 6
      pwm2.setPWM(10, 0, angleToPulse((hl6s3+20) - (FM2-20)) ); //postawienie nogi 6

      pwm1.setPWM(0, 0, angleToPulse((hl1s1+FM1) - FM2) ); //ruch nogi 1 do tyłu
      pwm1.setPWM(8, 0, angleToPulse((hl3s1+FM1) - FM2) ); //ruch nogi 3 do tyłu
      pwm2.setPWM(4, 0, angleToPulse((hl5s1-FM1) + FM2) ); //ruch nogi 5 do tyłu

      FM2++;
      FM2++;
}
    if (FM2==40){
      p2=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 3  ////////////////////////////////////////////////////////////////
  if (p1 && p2 && !p3){ 
    if (FM3<=20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1-20 + FM3) ); //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + FM3 + offset) ); //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - FM3) ); //podniesienie nogi 1 

      pwm1.setPWM(8, 0, angleToPulse(hl3s1-20 + FM3) ); //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + FM3 + offset) ); //podniesienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - FM3) ); //podniesienie nogi 3 

      pwm2.setPWM(4, 0, angleToPulse(hl5s1+20 - FM3) ); //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - FM3 - offset) ); //podniesienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + FM3) ); //podniesienie nogi 5 

      pwm1.setPWM(4, 0, angleToPulse((hl2s1+20) - FM3) ); //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1-20) + FM3) ); //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1-20) + FM3) ); //ruch nogi 6 do tyłu

      FM3++;
      FM3++;
    }
    if (FM3<=40 && FM3>20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1-20 + FM3) ); //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - (FM3-20)) ); //postawienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + (FM3-20)) ); //postawienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1-20 + FM3) );  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - (FM3-20)) ); //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + (FM3-20)) ); //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1+20 - FM3) ); //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + (FM3-20)) ); //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - (FM3-20)) ); //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse((hl2s1+20) - FM3) ); //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1-20) + FM3) ); //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1-20) + FM3) ); //ruch nogi 6 do tyłu

      FM3++;
      FM3++;
      if (FM3==40){
      p3=true;
    }
    }
    if (p1 && p2 && p3){
      FM2=0;
      FM3=0;
      p2=false;
      p3=false;
    }
  }
}//End Move Forward

void move_bckwd(){                    //Funkcja realizująca ruch do tyłu
    ////////////////////////////////////////////////////////////////ETAP 1  //////////////////////////////////////////////////////////////
  if (!p1 && !p2 && !p3 ){  
    if (FM1<=10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 - FM1) );                  //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + 2*FM1+offset) );          //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - 2*FM1) );                //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 - FM1) );                  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + 2*FM1+offset) );         //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - 2*FM1) );               //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 + FM1) );                  //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - 2*FM1-offset) );         //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + 2*FM1) );                //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 + FM1) );                  //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 - FM1) );                  //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 - FM1) );                  //ruch nogi 6 do tyłu
      
      FM1++;
      FM1++;
    }
    if (FM1<=20 && FM1>10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 - FM1) );                  //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - ((2*FM1)-20)) );    //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + ((2*FM1)-20)) );    //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 - FM1) );                  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - ((2*FM1)-20)) );    //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + ((2*FM1)-20)) );   //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 + FM1) );                  //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + ((2*FM1)-20)) );    //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - ((2*FM1)-20)) );    //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 + FM1) );                  //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 - FM1) );                  //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 - FM1) );                  //ruch nogi 6 do tyłu
      
      FM1++;
      FM1++;
    }
    if (FM1==20){
      p1=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 2  ////////////////////////////////////////////////////////////////
  if (p1 && !p2 && !p3){  
    if (FM2<=20){
        pwm1.setPWM(4, 0, angleToPulse(hl2s1+20 - FM2) );               //ruch nogi 2 do przodu
        pwm1.setPWM(5, 0, angleToPulse(hl2s2 + FM2 + offset) );                  //podniesienie nogi 2
        pwm1.setPWM(6, 0, angleToPulse(hl2s3 - FM2) );                  //podniesienie nogi  2

        pwm2.setPWM(0, 0, angleToPulse(hl4s1-20 + FM2) );               //ruch nogi 4 do przodu
        pwm2.setPWM(1, 0, angleToPulse(hl4s2 - FM2 - offset) );                  //podniesienie nogi 4
        pwm2.setPWM(2, 0, angleToPulse(hl4s3 + FM2) );                  //podniesienie nogi  4
         
        pwm2.setPWM(8, 0, angleToPulse(hl6s1-20 + FM2) );               //ruch nogi 6 do przodu
        pwm2.setPWM(9, 0, angleToPulse(hl6s2 - FM2 - offset) );                  //podniesienie nogi 6
        pwm2.setPWM(10, 0, angleToPulse(hl6s3 + FM2) );                 //podniesienie nogi  6

        pwm1.setPWM(0, 0, angleToPulse((hl1s1-FM1) + FM2) );            //ruch nogi 1 do tyłu
        pwm1.setPWM(8, 0, angleToPulse((hl3s1-FM1) + FM2) );            //ruch nogi 3 do tyłu
        pwm2.setPWM(4, 0, angleToPulse((hl5s1+FM1) - FM2) );            //ruch nogi 5 do tyłu
        FM2++;
        FM2++;
      }
    if (FM2<=40 && FM2>20){
        pwm1.setPWM(4, 0, angleToPulse(hl2s1+20 - FM2) );               //ruch nogi 2 do przodu
        pwm1.setPWM(5, 0, angleToPulse((hl2s2+20) - (FM2-20)) );        //postawienie nogi 2
        pwm1.setPWM(6, 0, angleToPulse((hl2s3-20) + (FM2-20)) );        //postawienie nogi 2

        pwm2.setPWM(0, 0, angleToPulse(hl4s1-20 + FM2) );               //ruch nogi 4 do przodu
        pwm2.setPWM(1, 0, angleToPulse((hl4s2-20) + (FM2-20)) );        //postawienie nogi 4
        pwm2.setPWM(2, 0, angleToPulse((hl4s3+20) - (FM2-20)) );        //postawienie nogi 4

        pwm2.setPWM(8, 0, angleToPulse(hl6s1-20 + FM2) );               //ruch nogi 6 do przodu
        pwm2.setPWM(9, 0, angleToPulse((hl6s2-20) + (FM2-20)) );        //postawienie nogi 6
        pwm2.setPWM(10, 0, angleToPulse((hl6s3+20) - (FM2-20)) );       //postawienie nogi 6

        pwm1.setPWM(0, 0, angleToPulse((hl1s1-FM1) + FM2) );            //ruch nogi 1 do tyłu
        pwm1.setPWM(8, 0, angleToPulse((hl3s1-FM1) + FM2) );            //ruch nogi 3 do tyłu
        pwm2.setPWM(4, 0, angleToPulse((hl5s1+FM1) - FM2) );            //ruch nogi 5 do tyłu

        FM2++;
        FM2++;
  }
    if (FM2==40){
      p2=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 3  ////////////////////////////////////////////////////////////////
  if (p1 && p2 && !p3){ 
    if (FM3<=20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1+20 - FM3) );               //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + FM3 + offset) );                  //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - FM3) );                  //podniesienie nogi 1 

      pwm1.setPWM(8, 0, angleToPulse(hl3s1+20 - FM3) );               //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + FM3 + offset) );                  //podniesienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - FM3) );                 //podniesienie nogi 3 

      pwm2.setPWM(4, 0, angleToPulse(hl5s1-20 + FM3) );               //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - FM3 - offset) );                  //podniesienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + FM3) );                  //podniesienie nogi 5 

      pwm1.setPWM(4, 0, angleToPulse((hl2s1-20) + FM3) );            //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1+20) - FM3) );            //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1+20) - FM3) );            //ruch nogi 6 do tyłu

      FM3++;
      FM3++;
    }
    if (FM3<=40 && FM3>20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1+20 - FM3) );               //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - (FM3-20)) );        //postawienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + (FM3-20)) );        //postawienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1+20 - FM3) );               //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - (FM3-20)) );        //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + (FM3-20)) );        //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1-20 + FM3) );               //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + (FM3-20)) );        //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - (FM3-20)) );        //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse((hl2s1-20) + FM3) );            //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1+20) - FM3) );            //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1+20) - FM3) );            //ruch nogi 6 do tyłu

      FM3++;
      FM3++;
      if (FM3==40){
      p3=true;
    }
    }
    if (p1 && p2 && p3){
      FM2=0;
      FM3=0;
      p2=false;
      p3=false;
    }
  }
}//End Move Backward

void turn_right(){                        //Funkcja realizująca obrót w prawo
    ////////////////////////////////////////////////////////////////ETAP 1  //////////////////////////////////////////////////////////////
  if (!p1 && !p2 && !p3 ){  
    if (FM1<=10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 + FM1) );                  //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + 2*FM1+offset) );         //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - 2*FM1) );                //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 + FM1) );                  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + 2*FM1+offset) );         //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - 2*FM1) );               //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 + FM1) );                  //ruch nogi 5 do tyłu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - 2*FM1-offset) );          //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + 2*FM1) );                //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 - FM1) );                  //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 - FM1) );                  //ruch nogi 4 do przodu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 - FM1) );                  //ruch nogi 6 do przodu
      
      FM1++;
      FM1++;
    }
    if (FM1<=20 && FM1>10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 + FM1) );                  //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - ((2*FM1)-20)) );    //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + ((2*FM1)-20)) );    //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 + FM1) );                  //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - ((2*FM1)-20)) );    //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + ((2*FM1)-20)) );   //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 + FM1) );                  //ruch nogi 5 do tyłu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + ((2*FM1)-20)) );    //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - ((2*FM1)-20)) );    //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 - FM1) );                  //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 - FM1) );                  //ruch nogi 4 do przodu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 - FM1) );                  //ruch nogi 6 do przodu
      
      FM1++;
      FM1++;
    }
    if (FM1==20){
      p1=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 2  ////////////////////////////////////////////////////////////////
  if (p1 && !p2 && !p3){  
    if (FM2<=20){
        pwm1.setPWM(4, 0, angleToPulse(hl2s1-20 + FM2) );               //ruch nogi 2 do przodu
        pwm1.setPWM(5, 0, angleToPulse(hl2s2 + FM2+offset) );                  //podniesienie nogi 2
        pwm1.setPWM(6, 0, angleToPulse(hl2s3 - FM2) );                  //podniesienie nogi  2

        pwm2.setPWM(0, 0, angleToPulse(hl4s1-20 + FM2) );               //ruch nogi 4 do tyłu
        pwm2.setPWM(1, 0, angleToPulse(hl4s2 - FM2-offset) );                  //podniesienie nogi 4
        pwm2.setPWM(2, 0, angleToPulse(hl4s3 + FM2) );                  //podniesienie nogi  4
         
        pwm2.setPWM(8, 0, angleToPulse(hl6s1-20 + FM2) );               //ruch nogi 6 do przodu
        pwm2.setPWM(9, 0, angleToPulse(hl6s2 - FM2-offset) );                  //podniesienie nogi 6
        pwm2.setPWM(10, 0, angleToPulse(hl6s3 + FM2) );                 //podniesienie nogi  6

        pwm1.setPWM(0, 0, angleToPulse((hl1s1+FM1) - FM2) );            //ruch nogi 1 do tyłu
        pwm1.setPWM(8, 0, angleToPulse((hl3s1+FM1) - FM2) );            //ruch nogi 3 do tyłu
        pwm2.setPWM(4, 0, angleToPulse((hl5s1+FM1) - FM2) );            //ruch nogi 5 do przodu
        FM2++;
        FM2++;
      }
    if (FM2<=40 && FM2>20){
        pwm1.setPWM(4, 0, angleToPulse(hl2s1-20 + FM2) );               //ruch nogi 2 do przodu
        pwm1.setPWM(5, 0, angleToPulse((hl2s2+20) - (FM2-20)) );        //postawienie nogi 2
        pwm1.setPWM(6, 0, angleToPulse((hl2s3-20) + (FM2-20)) );        //postawienie nogi 2

        pwm2.setPWM(0, 0, angleToPulse(hl4s1-20 + FM2) );               //ruch nogi 4 do tyłu
        pwm2.setPWM(1, 0, angleToPulse((hl4s2-20) + (FM2-20)) );        //postawienie nogi 4
        pwm2.setPWM(2, 0, angleToPulse((hl4s3+20) - (FM2-20)) );        //postawienie nogi 4

        pwm2.setPWM(8, 0, angleToPulse(hl6s1-20 + FM2) );               //ruch nogi 6 do tyłu
        pwm2.setPWM(9, 0, angleToPulse((hl6s2-20) + (FM2-20)) );        //postawienie nogi 6
        pwm2.setPWM(10, 0, angleToPulse((hl6s3+20) - (FM2-20)) );       //postawienie nogi 6

        pwm1.setPWM(0, 0, angleToPulse((hl1s1+FM1) - FM2) );            //ruch nogi 1 do tyłu
        pwm1.setPWM(8, 0, angleToPulse((hl3s1+FM1) - FM2) );            //ruch nogi 3 do tyłu
        pwm2.setPWM(4, 0, angleToPulse((hl5s1+FM1) - FM2) );            //ruch nogi 5 do przodu

        FM2++;
        FM2++;
  }
    if (FM2==40){
      p2=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 3  ////////////////////////////////////////////////////////////////
  if (p1 && p2 && !p3){ 
    if (FM3<=20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1-20 + FM3) );               //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + FM3+offset) );                  //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - FM3) );                  //podniesienie nogi 1 

      pwm1.setPWM(8, 0, angleToPulse(hl3s1-20 + FM3) );               //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + FM3+offset) );                  //podniesienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - FM3) );                 //podniesienie nogi 3 

      pwm2.setPWM(4, 0, angleToPulse(hl5s1-20 + FM3) );               //ruch nogi 5 do tyłu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - FM3-offset) );                  //podniesienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + FM3) );                  //podniesienie nogi 5 

      pwm1.setPWM(4, 0, angleToPulse((hl2s1+20) - FM3) );            //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1+20) - FM3) );            //ruch nogi 4 do przodu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1+20) - FM3) );            //ruch nogi 6 do przodu

      FM3++;
      FM3++;
    }
    if (FM3<=40 && FM3>20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1-20 + FM3) );               //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - (FM3-20)) );        //postawienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + (FM3-20)) );        //postawienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1-20 + FM3) );               //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - (FM3-20)) );        //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + (FM3-20)) );        //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1-20 + FM3) );               //ruch nogi 5 do tyłu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + (FM3-20)) );        //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - (FM3-20)) );        //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse((hl2s1+20) - FM3) );            //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1+20) - FM3) );            //ruch nogi 4 do przodu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1+20) - FM3) );            //ruch nogi 6 do przodu

      FM3++;
      FM3++;
      if (FM3==40){
      p3=true;
    }
    }
    if (p1 && p2 && p3){
      FM2=0;
      FM3=0;
      p2=false;
      p3=false;
    }
  }
}//End Turn Right

void turn_left(){                                 //Funkcja realizująca obrót w lewo
    ////////////////////////////////////////////////////////////////ETAP 1  //////////////////////////////////////////////////////////////
  if (!p1 && !p2 && !p3 ){  
    if (FM1<=10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 - FM1) );                  //ruch nogi 1 do tyłu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + 2*FM1+offset) );         //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - 2*FM1) );                //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 - FM1) );                  //ruch nogi 3 do tyłu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + 2*FM1+offset) );         //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - 2*FM1) );               //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 - FM1) );                  //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - 2*FM1-offset) );         //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + 2*FM1) );                //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 + FM1) );                  //ruch nogi 2 do przodu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 + FM1) );                  //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 + FM1) );                  //ruch nogi 6 do tyłu
      
      FM1++;
      FM1++;
    }
    if (FM1<=20 && FM1>10){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1 - FM1) );                  //ruch nogi 1 do tyłu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - ((2*FM1)-20)) );    //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + ((2*FM1)-20)) );    //podniesienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1 - FM1) );                  //ruch nogi 3 do tyłu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - ((2*FM1)-20)) );    //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + ((2*FM1)-20)) );   //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1 - FM1) );                  //ruch nogi 5 do przodu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + ((2*FM1)-20)) );    //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - ((2*FM1)-20)) );    //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse(hl2s1 + FM1) );                  //ruch nogi 2 do przodu
      pwm2.setPWM(0, 0, angleToPulse(hl4s1 + FM1) );                  //ruch nogi 4 do tyłu
      pwm2.setPWM(8, 0, angleToPulse(hl6s1 + FM1) );                  //ruch nogi 6 do tyłu
      
      FM1++;
      FM1++;
    }
    if (FM1==20){
      p1=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 2  ////////////////////////////////////////////////////////////////
  if (p1 && !p2 && !p3){  
    if (FM2<=20){
        pwm1.setPWM(4, 0, angleToPulse(hl2s1+20 - FM2) );               //ruch nogi 2 do tyłu
        pwm1.setPWM(5, 0, angleToPulse(hl2s2 + FM2+offset) );                  //podniesienie nogi 2
        pwm1.setPWM(6, 0, angleToPulse(hl2s3 - FM2) );                  //podniesienie nogi  2

        pwm2.setPWM(0, 0, angleToPulse(hl4s1+20 - FM2) );               //ruch nogi 4 do przodu
        pwm2.setPWM(1, 0, angleToPulse(hl4s2 - FM2-offset) );                  //podniesienie nogi 4
        pwm2.setPWM(2, 0, angleToPulse(hl4s3 + FM2) );                  //podniesienie nogi  4
         
        pwm2.setPWM(8, 0, angleToPulse(hl6s1+20 - FM2) );               //ruch nogi 6 do przodu
        pwm2.setPWM(9, 0, angleToPulse(hl6s2 - FM2-offset) );                  //podniesienie nogi 6
        pwm2.setPWM(10, 0, angleToPulse(hl6s3 + FM2) );                 //podniesienie nogi  6

        pwm1.setPWM(0, 0, angleToPulse((hl1s1-FM1) + FM2) );            //ruch nogi 1 do przodu
        pwm1.setPWM(8, 0, angleToPulse((hl3s1-FM1) + FM2) );            //ruch nogi 3 do przodu
        pwm2.setPWM(4, 0, angleToPulse((hl5s1-FM1) + FM2) );            //ruch nogi 5 do tyłu
        FM2++;
        FM2++;
      }
    if (FM2<=40 && FM2>20){
        pwm1.setPWM(4, 0, angleToPulse(hl2s1+20 - FM2) );               //ruch nogi 2 do tyłu
        pwm1.setPWM(5, 0, angleToPulse((hl2s2+20) - (FM2-20)) );        //postawienie nogi 2
        pwm1.setPWM(6, 0, angleToPulse((hl2s3-20) + (FM2-20)) );        //postawienie nogi 2

        pwm2.setPWM(0, 0, angleToPulse(hl4s1+20 - FM2) );               //ruch nogi 4 do przodu
        pwm2.setPWM(1, 0, angleToPulse((hl4s2-20) + (FM2-20)) );        //postawienie nogi 4
        pwm2.setPWM(2, 0, angleToPulse((hl4s3+20) - (FM2-20)) );        //postawienie nogi 4

        pwm2.setPWM(8, 0, angleToPulse(hl6s1+20 - FM2) );               //ruch nogi 6 do przodu
        pwm2.setPWM(9, 0, angleToPulse((hl6s2-20) + (FM2-20)) );        //postawienie nogi 6
        pwm2.setPWM(10, 0, angleToPulse((hl6s3+20) - (FM2-20)) );       //postawienie nogi 6

        pwm1.setPWM(0, 0, angleToPulse((hl1s1-FM1) + FM2) );            //ruch nogi 1 do przodu
        pwm1.setPWM(8, 0, angleToPulse((hl3s1-FM1) + FM2) );            //ruch nogi 3 do przodu
        pwm2.setPWM(4, 0, angleToPulse((hl5s1-FM1) + FM2) );            //ruch nogi 5 do tyłu

        FM2++;
        FM2++;
  }
    if (FM2==40){
      p2=true;
    }
  }
  ////////////////////////////////////////////////////////////////ETAP 3  ////////////////////////////////////////////////////////////////
  if (p1 && p2 && !p3){ 
    if (FM3<=20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1+20 - FM3) );               //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse(hl1s2 + FM3+offset) );                  //podniesienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse(hl1s3 - FM3) );                  //podniesienie nogi 1 

      pwm1.setPWM(8, 0, angleToPulse(hl3s1+20 - FM3) );               //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse(hl3s2 + FM3+offset) );                  //podniesienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse(hl3s3 - FM3) );                 //podniesienie nogi 3 

      pwm2.setPWM(4, 0, angleToPulse(hl5s1+20 - FM3) );               //ruch nogi 5 do tyłu
      pwm2.setPWM(5, 0, angleToPulse(hl5s2 - FM3-offset) );                  //podniesienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse(hl5s3 + FM3) );                  //podniesienie nogi 5 

      pwm1.setPWM(4, 0, angleToPulse((hl2s1-20) + FM3) );            //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1-20) + FM3) );            //ruch nogi 4 do przodu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1-20) + FM3) );            //ruch nogi 6 do przodu

      FM3++;
      FM3++;
    }
    if (FM3<=40 && FM3>20){
      pwm1.setPWM(0, 0, angleToPulse(hl1s1+20 - FM3) );               //ruch nogi 1 do przodu
      pwm1.setPWM(1, 0, angleToPulse((hl1s2+20) - (FM3-20)) );        //postawienie nogi 1
      pwm1.setPWM(2, 0, angleToPulse((hl1s3-20) + (FM3-20)) );        //postawienie nogi 1

      pwm1.setPWM(8, 0, angleToPulse(hl3s1+20 - FM3) );               //ruch nogi 3 do przodu
      pwm1.setPWM(9, 0, angleToPulse((hl3s2+20) - (FM3-20)) );        //postawienie nogi 3
      pwm1.setPWM(10, 0, angleToPulse((hl3s3-20) + (FM3-20)) );        //postawienie nogi 3

      pwm2.setPWM(4, 0, angleToPulse(hl5s1+20 - FM3) );               //ruch nogi 5 do tyłu
      pwm2.setPWM(5, 0, angleToPulse((hl5s2-20) + (FM3-20)) );        //postawienie nogi 5
      pwm2.setPWM(6, 0, angleToPulse((hl5s3+20) - (FM3-20)) );        //postawienie nogi 5

      pwm1.setPWM(4, 0, angleToPulse((hl2s1-20) + FM3) );            //ruch nogi 2 do tyłu
      pwm2.setPWM(0, 0, angleToPulse((hl4s1-20) + FM3) );            //ruch nogi 4 do przodu
      pwm2.setPWM(8, 0, angleToPulse((hl6s1-20) + FM3) );            //ruch nogi 6 do przodu

      FM3++;
      FM3++;
      if (FM3==40){
      p3=true;
    }
    }
    if (p1 && p2 && p3){
      FM2=0;
      FM3=0;
      p2=false;
      p3=false;
    }
  }
}//End Turn Left

int angleToPulse(int ang){                //Funkcja do przeliczania podanego kąta w impuls rozumiany przez serwomechanizm
   int puls = map(ang,0, 180, SERVOMIN,SERVOMAX); // zmapowanie kąta w stosunku do impulsu serwa
   return puls;
}