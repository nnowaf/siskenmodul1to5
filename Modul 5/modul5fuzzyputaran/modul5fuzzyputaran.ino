#include <EEPROM.h>
int address = 0;
byte value;
String kondisiSensor;
unsigned long now, selisih, lasttime;
int LastError; int state, state2, state3,setting=0;
int simpan;
int x; int kd; int i;
int s[8];
int error;
int moveControl, kecepatanMotorKanan, kecepatanMotorKiri, rate;
int kp;
int sp;int nkp,nkd;
int maxs[8];
int mins[8];int isi=0;
int t[8];
#define ledr 8
#define ledl 7
void setup() {

  Serial.begin(9600);
  for (i = 0; i <= 7; i++) {
    maxs[i] = 0;
    mins[i] = 1023;
    isi=EEPROM.read(i);
    t[i]=isi*4;
  }
  kp=EEPROM.read(8);
  kd=EEPROM.read(9);
  nkp=kp;
  nkd=kd;

    for(i=0;i<=7;i++){
Serial.print("nilai tengah sensor "); Serial.print(i); Serial.print(" : "); Serial.print(t[i]); Serial.println(" ");}
Serial.print("kp: ");
Serial.print(EEPROM.read(8));
Serial.println("");
Serial.print("kd: ");
Serial.print(EEPROM.read(9));
Serial.println("");

  pinMode(ledl, OUTPUT);
  pinMode(ledr, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A7, INPUT);
  pinMode(A6, INPUT);
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);
  analogWrite(9, LOW);
  analogWrite(5, LOW);
  analogWrite(6, LOW);
  analogWrite(10, LOW);
  delay(3000);
}

void loop() {

  if ((digitalRead(2) == LOW)&&(setting==0)) {
    state = 4;
    setting = 0;
    state3 = 0;
  }

  if (digitalRead(3) == LOW) {
    state = 0;
    state2 = 3;
    state3 = 0;
  }

  if ((digitalRead(2) == LOW)&&(setting==0)) {
    state = 4;
    state2 = 0;
    state3 = 0;
  }
  if (digitalRead(3) == LOW) {
    state = 0;
    state2 = 3;
    state3 = 0;
    setting = 1;

  }

  if ((state == 4)&&(setting==0)) {

    if (digitalRead(3) == LOW) {
      state = 0;
      state2 = 3;
      state3 = 0;
    }
    Serial.print(state);
    Serial.println("");
    digitalWrite(ledr, HIGH);
    digitalWrite(ledl, HIGH);
    s[0] = analogRead(A4);
    s[1] = analogRead(A5);
    s[2] = analogRead(A6);
    s[3] = analogRead(A7);

    s[4] = analogRead(A0);
    s[5] = analogRead(A1);
    s[6] = analogRead(A2);
    s[7] = analogRead(A3);
    for (i = 0; i <= 7; i++) {
      if (s[i] > maxs[i]) {
        maxs[i] = s[i];
      }
      if (s[i] < mins[i]) {
        mins[i] = s[i];
      }

      t[i] = (maxs[i] + mins[i]) / 2;
      address=i;
      value=t[i]/4;
      EEPROM.write(address, value);
      Serial.print("nilai tengah sensor "); Serial.print(i); Serial.print(" : "); Serial.print(t[i]); Serial.println(" ");
      Serial.print("kp: ");
Serial.print(EEPROM.read(8));
Serial.println("");
Serial.print("kd: ");
Serial.print(EEPROM.read(9));
Serial.println("");
    }
    digitalWrite(ledr, LOW);
    digitalWrite(ledl, LOW);
    if (digitalRead(3) == LOW) {
      state = 0;
      state2 = 3;
    }

  }
  Serial.print(state);
  Serial.println("");

  if (state2 == 3) {
  for (i = 0; i <= 7; i++) {
    isi=EEPROM.read(i);
    t[i]=isi*4;
  }
    Serial.print(state);
    Serial.println("");
    s[0] = analogRead(A4);
    s[1] = analogRead(A5);
    s[2] = analogRead(A6);
    s[3] = analogRead(A7);
    s[4] = analogRead(A0);
    s[5] = analogRead(A1);
    s[6] = analogRead(A2);
    s[7] = analogRead(A3);
Serial.print("kp: ");
Serial.print(EEPROM.read(8));
Serial.println("");
Serial.print("kd: ");
Serial.print(EEPROM.read(9));
Serial.println("");

digitalWrite(3,HIGH);
  if (digitalRead(2) == LOW) {
kp=kp-1;
nkp =kp;
EEPROM.write(8,kp);
digitalWrite(ledl,HIGH);delay(1000);digitalWrite(ledl,LOW);
  }

  if (digitalRead(3) == LOW) {
kp=kp+1;
nkp =kp;
EEPROM.write(8,kp);
digitalWrite(ledr,LOW);digitalWrite(ledl,LOW);delay(10);
digitalWrite(ledl,HIGH);delay(500);digitalWrite(ledl,LOW);
  }
  if (digitalRead(2) == LOW) {
kp=kp-1;
nkp =kp;
EEPROM.write(8,kp);
digitalWrite(ledr,LOW);digitalWrite(ledl,LOW);delay(10);
digitalWrite(ledl,HIGH);delay(500);digitalWrite(ledl,LOW);
  }
  
  if (digitalRead(12) == LOW) {
kd=kd-1;
nkd=kd;
EEPROM.write(9,kd);
digitalWrite(ledr,LOW);digitalWrite(ledl,LOW);delay(10);
digitalWrite(ledr,HIGH);delay(500);digitalWrite(ledr,LOW);
  }

  if (digitalRead(11) == LOW) {
kd=kd+1;
nkd=kd;
EEPROM.write(9,kd);
digitalWrite(ledr,LOW);digitalWrite(ledl,LOW);delay(10);
digitalWrite(ledr,HIGH);delay(500);digitalWrite(ledr,LOW);
  }



    if ((s[0] >= t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      
      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, LOW);
      error = -7;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "10000000";

      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, 200);
      analogWrite(6, 0);

    }
    if ((s[0] >= t[0]) && (s[1] >= t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {

      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, LOW);
      error = -6;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "11000000";

      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] >= t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, LOW);
      error = -5;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "01000000";

      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] >= t[1]) && (s[2] >= t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, LOW);
      error = -4;
      Serial.print(" error = "); Serial.print(error);

      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);

    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] >= t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, LOW);
      error = -3;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00100000";
      Serial.print("set point ="); Serial.print(190);
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] >= t[2]) && (s[3] >= t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, LOW);
      error = -2;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00110000";
      Serial.print("set point ="); Serial.print(190);
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);

    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] >= t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      error = -1;
      Serial.print(" error = "); Serial.print(error);
      Serial.print("set point ="); Serial.print(190);
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, 200);
      analogWrite(6, 180);
    }

    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] >= t[3]) && (s[4] >= t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, HIGH);
      digitalWrite(ledl, HIGH);
      error = 0;
      now = millis();
      selisih=now-lasttime;
      if(selisih>=800){
        sp=255;
      }
      else{
        sp=190;
      }
      Serial.print(" error = "); Serial.print(error);
      Serial.print(" selisih = "); Serial.print(selisih);
      Serial.print(" NOW = "); Serial.print(now);
      Serial.print("set point ="); Serial.print(sp);
      kondisiSensor == "00011000";
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = sp - (moveControl);   
      kecepatanMotorKiri = sp + (moveControl); 
      analogWrite(9, 250);
      analogWrite(6, 250);

    }

    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] >= t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {

      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);
      error = 1;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00001000";
      Serial.print("set point ="); Serial.print(190);
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, 180);
      analogWrite(6, 200);

    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] >= t[4]) && (s[5] >= t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);
      error = 2;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00001100";
      Serial.print("set point ="); Serial.print(190);
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] >= t[5]) && (s[6] < t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);
      error = 3;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00000100";
      Serial.print("set point ="); Serial.print(190);
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] >= t[5]) && (s[6] >= t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);
      error = 4;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00000110";
  
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] >= t[6]) && (s[7] < t[7])) {
      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);
      error = 5;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00000010";

      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] >= t[6]) && (s[7] >= t[7])) {
      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);
      error = 6;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00000011";
   
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, kecepatanMotorKanan);
      analogWrite(6, kecepatanMotorKiri);
    }
    if ((s[0] < t[0]) && (s[1] < t[1]) && (s[2] < t[2]) && (s[3] < t[3]) && (s[4] < t[4]) && (s[5] < t[5]) && (s[6] < t[6]) && (s[7] >= t[7])) {

      digitalWrite(ledr, LOW);
      digitalWrite(ledl, HIGH);

      error = 7;
      Serial.print(" error = "); Serial.print(error);
      kondisiSensor == "00000001";
 
      rate = (LastError) - (error);
      moveControl = (nkp * error) + (nkd * rate);    
      kecepatanMotorKanan = 190 - (moveControl);   
      kecepatanMotorKiri = 190 + (moveControl); 
      analogWrite(9, 0);
      analogWrite(6, 200);

    }

    LastError = error;
    Serial.print(" LastError = "); Serial.print(LastError);
    Serial.println(" ");

  }



}
