#include <EEPROM.h>
unsigned long awal, nilai, selisih, waktu = 100, sekarang, er0 = 2000, erx = 200, selor = 0, awalor = 0, nilor = 0;
int value;
int address = 0,
    kondisisensor, kondisi[8],
    lasterror, state,
    state2,
    state3,
    setting = 0,
    setting2 = 0,
    simpan,
    sp = 180,
    x, kd, i,
    s[8],
    error,
    moveControl,
    kecepatanMotorKanan,
    kecepatanMotorKiri,
    rate, kp, nkp, nkd, maxs[8], mins[8], isi = 0, peka[8], setpoint = 150;
int mp[8];
#define ledr 8
#define ledl 7
void setup() {
        EEPROM.write(8, EEPROM.read(8));
  EEPROM.write(9, EEPROM.read(9));
  Serial.begin(9600);
  for (i = 0; i <= 7; i++) {
    maxs[i] = 0;
    mins[i] = 1023;
    isi = EEPROM.read(i);
    peka[i] = isi * 4;
  }
  kp = EEPROM.read(8);
  kd = EEPROM.read(9);
  for (i = 0; i <= 7; i++) {
    //Serial.print("nilai tengah sensor "); Serial.print(i); Serial.print(" : "); Serial.print(peka[i]); Serial.println(" ");
  }
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
  pinMode(13, OUTPUT);
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
  analogWrite(10, LOW); digitalWrite(13, HIGH);
  delay(100);
  awal = millis(); awalor = millis();
}
void loop() {
  cektombol();
  cekstate();
}

void cektombol() {
  if (!(digitalRead(2)) && (setting == 0)) {
    state = 4;
    state2 = 0;
    setting = 1;
    setting2 = 0;
  }
  if (!digitalRead(3)) {
    state = 0;
    setting = 1;
    state2 = 3;
    setting2 = 1;
  }
}
void cekstate() {
  if ((state == 4) && (setting == 1)) { // Tombol 2 kiri bawah ditekan
    Serial.println("Kalibrasi");
    kalibrasi();
  }
  if ((state2 == 3) && (setting2 == 1)) { // Tombol 3 kanan atas ditekan
    //Serial.println("Robot Jalan");
    jalankan();
  }
}
void kalibrasi() {
  nyalabareng();
  bacasensor();
  for (i = 0; i <= 7; i++) {
    if (s[i] > maxs[i]) {
      maxs[i] = s[i];
    }
    if (s[i] < mins[i]) {
      mins[i] = s[i];
    }
    peka[i] = (maxs[i] + mins[i]) / 2;
    address = i;
    value = peka[i] / 4;
    EEPROM.write(address, value);

  }
  ledmati();
  tampilkandata();
}
void tampilkandata() {
  for (i = 0; i <= 7; i++) {
    Serial.print(s[i]); Serial.print(" ");
  } Serial.println("");
  for (i = 0; i <= 7; i++) {
    Serial.print("nilai tengah sensor ");
    Serial.print(i); Serial.print(" : ");
    Serial.print(peka[i]); Serial.println(" ");
  }
  
  Serial.print("kp: ");
  Serial.print(EEPROM.read(8));
  Serial.println("");
  Serial.print("kd: ");
  Serial.print(EEPROM.read(9));
  Serial.println("");
}
void bacasensor() {
  s[0] = analogRead(A4);
  s[1] = analogRead(A5);
  s[2] = analogRead(A6);
  s[3] = analogRead(A7);

  s[4] = analogRead(A0);
  s[5] = analogRead(A1);
  s[6] = analogRead(A2);
  s[7] = analogRead(A3);
}

void jalankan() {
  //bacaerror();
  bacapekaPD();
  bacastate();
  jalankanwaktu();
  bacakondisisensor();
  cekerror();
  putarkanmotor();
  ceklampu();
  lasterror = error;
  printdata();
  Serial.println(jalankanwaktu());
  if (jalankanwaktu() > 2000) {
    Serial.print("Timer : ");
    Serial.println(jalankanwaktu());
    Serial.print("Setpoint : ");
    Serial.print(sp);
    Serial.println("");
    Serial.print(" error = "); Serial.print(error); Serial.println("");
  }
  
}
void ceklampu() {
  if (error <= -1) {
    ledkanan();
    selain();
  } else if (error >= 1) {
    ledkiri();
    selain();
  } else {
    nyalabareng();
    lederror0();
  }
}
void bacakondisisensor() {
  s[0] = analogRead(A3);
  s[1] = analogRead(A2);
  s[2] = analogRead(A1);
  s[3] = analogRead(A0);
  s[4] = analogRead(A7);
  s[5] = analogRead(A6);
  s[6] = analogRead(A5);
  s[7] = analogRead(A4);

  for (i = 0; i <= 7; i++)
  {
    if (s[i] >= peka[i]) {
      kondisi[i] = 1;
    }
    else {
      kondisi[i] = 0;
    }
  }
  kondisisensor = 0;
  for (i = 0; i <= 7; i++) {
    kondisisensor += kondisi[i] * (1 << i);
  }

}

void printdata() {
  Serial.print(" lasterror = "); Serial.print(lasterror);
  Serial.println(" ");
  Serial.print(" kanan = "); Serial.print(kecepatanMotorKanan);
  Serial.println(" ");
  Serial.print(" kiri = "); Serial.print(kecepatanMotorKiri);
  Serial.println(" ");
  Serial.print(" waktu awal = "); Serial.print(awal);
  Serial.println(" ");
  Serial.print(" waktu cek error = "); Serial.print(nilai);
  Serial.println(" ");
  Serial.print(" selisih waktu = "); Serial.print(selisih);
  Serial.println(" ");
Serial.print(" Kp = "); 
Serial.print(kp);
Serial.println(" ");
Serial.print(" Kd = "); 
Serial.print(kd);
Serial.println(" ");
}

void putarkanmotor() {


  if ((selisih > waktu) && (error == 0)) {
    sp += 20; awal = sekarang;
  } else {
    sp = 150;
    selisih = 0;
  }
  //Serial.print(" error = "); Serial.print(error); Serial.println("");
  //Serial.print(kondisisensor); Serial.println("");
  rate = (lasterror) - (error);
  moveControl = (kp * error) + (kd * rate);
  kecepatanMotorKanan = sp - (moveControl);
  kecepatanMotorKiri = sp + (moveControl);

  if (kecepatanMotorKanan > 255) {
    kecepatanMotorKanan = 255;
  } else if (kecepatanMotorKanan < -255) {
    kecepatanMotorKanan = -255;
  }
  if (kecepatanMotorKanan < 0) {
    kecepatanMotorKanan *= -1;
    analogWrite(10, kecepatanMotorKanan);
    analogWrite(9, 0);
  } else {
    kecepatanMotorKanan -= 15;
    analogWrite(10, 0);
    analogWrite(9, kecepatanMotorKanan);
  }
  if (kecepatanMotorKiri > 255) {
    kecepatanMotorKiri = 255;
  } else if (kecepatanMotorKiri < -255) {
    kecepatanMotorKiri = -255;
  }
  if (kecepatanMotorKiri < 0) {
    kecepatanMotorKiri *= -1;
    analogWrite(5, kecepatanMotorKiri);
    analogWrite(6, 0);
  } else {
    analogWrite(5, 0);
    analogWrite(6, kecepatanMotorKiri);
  }


}
unsigned long jalankanwaktu() {
  sekarang = millis();
  selisih = sekarang - awal;
  selor = sekarang - awalor;

  return selor;

}
void bacastate() { // kondisi robot jalan
  if (!digitalRead(3)) { 
    Serial.println("Kp +");
    kp = kp + 1;
    EEPROM.write(8, kp);
    ledmati(); delay(500);
    ledkiri(); delay(500); ledmati();
  }
  if (!digitalRead(2)) {
     Serial.println("Kp -");
    kp = kp - 1;
    EEPROM.write(8, kp);
    ledmati(); delay(500);
    ledkiri(); delay(500); ledmati();
  }

  if (!digitalRead(12)) {
    Serial.println("Kd +");
    kd = kd - 1;
    EEPROM.write(9, kd);
    ledmati(); delay(500);
    ledkanan(); delay(500); ledmati();
  }

  if (!digitalRead(11)) {
    Serial.println("Kd -");
    kd = kd + 1;
    EEPROM.write(9, kd);
    ledmati(); delay(500);
    ledkanan(); delay(500); ledmati();
  }
}
void ledkanan() {
  digitalWrite(ledr, HIGH); digitalWrite(ledl, LOW);
}
void ledkiri() {
  digitalWrite(ledl, HIGH); digitalWrite(ledr, LOW);
}
void ledmati() {
  digitalWrite(ledr, LOW); digitalWrite(ledl, LOW);
}
void nyalabareng() {
  digitalWrite(ledr, HIGH); digitalWrite(ledl, HIGH);
}
void cekerror() {
  switch (kondisisensor) {
    case  0b10000000: error = -6; break;
    case  0b11000000: error = -5; break;
    case  0b01000000: error = -4; break;
    case  0b01100000: error = -3; break;
    case  0b00100000: error = -2; break;
    case  0b00110000: error = -1; break;
    case  0b00010000: error = 0; break;
    case  0b00011000: error = 0; break;
    case  0b00001000: error = 0; break;
    case  0b00001100: error = 1; break;
    case  0b00000100: error = 2; break;
    case  0b00000110: error = 3; break;
    case  0b00000010: error = 4; break;
    case  0b00000011: error = 5; break;
    case  0b00000001: error = 6; break;
  }
}
void selain() {
  if (sekarang - awalor >= erx) {
    awalor = sekarang;
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
void lederror0() {
  if (sekarang - awalor >= er0) {
    awalor = sekarang;
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }
}
void bacapekaPD() {
  kp = EEPROM.read(8);
  kd = EEPROM.read(9);

}
