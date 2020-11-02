#include <PID_v1.h>
#include <LiquidCrystal.h>
#include <SoftPWM.h>
#include <SoftPWM_timer.h>
LiquidCrystal lcd(2,3,4,5,6,7);
#define PIN_INPUT    A1 //lm35
#define PIN_OUTPUT  2 // lcd screen 
#define PIN_OUTPUT 11 // dc motor


double Input,  Output;
double DESIRED; // will be desired valur
double K_P=2, K_I=10, K_D=1; // PID parameters
float Temp_diff; // difference between received temperature and desired value
float Temp_read; // received temperature form lm35 sensor
float elapsed_Time , Time;
int PID_p=0;
int PID_i=0;
int PID_d=0;
const int motor=11; // pin where motor is
int speed;
PID myPID (&Input, &Output, &DESIRED, K_P, K_I, K_D, DIRECT);


void setup() {
  Input = analogRead(PIN_INPUT);
  DESIRED=20;
  pinMode(Temp_read,INPUT); // input from lm35
  pinMode(2, OUTPUT); // output from lcd screen
  pinMode(11,OUTPUT); // output from dc motor
  lcd.begin(16,2); // using 2x16 lcd screen
  Serial.begin(9600);
  myPID.SetMode(AUTOMATIC);
  
}

void loop() {
  Time = millis();
  elapsed_Time = (Time) /1000;
  Input= analogRead(A1); 
  myPID.Compute();
  Temp_read=analogRead(A1); // reading received temperature
  Temp_diff= Temp_read-DESIRED; // diff between to received temp and desired
  PID_p=K_P * Temp_diff; // Calculate P value
  if (-5<Temp_diff<5){
 
    PID_i = PID_i + (K_I * Temp_diff); // Calculate I value
    
   PID_d = K_D*(Temp_diff/elapsed_Time); // Calculate D value
  speed= PID_p + PID_i + PID_d; // formula of speed
  Temp_diff=digitalRead(Temp_diff);
  }
  
  if (Temp_diff<=0){
    speed=0;
    digitalWrite(motor,LOW);
  }
    else {
    speed=PID_p + PID_i + PID_d ;
    digitalWrite(11,HIGH);
  }
  
  
  lcd.clear();
  lcd.setCursor(0,0); // location 
  lcd.print(" PID Cooling Control");
  lcd.setCursor(0,1); 
  lcd.print("Ds.="); // desired value
  lcd.println(DESIRED); // display desired value
  lcd.setCursor(7,1);
  lcd.print("A.S= "); // actual speed
  lcd.println(speed); // display speed
  delay(500);
}





  
