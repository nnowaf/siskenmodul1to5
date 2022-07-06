#include <EEPROM.h>
#define swLeftUp    digitalRead(pinSwLeftUp)
#define swLeftDown  digitalRead(pinSwLeftDown)
#define swRightUp   digitalRead(pinSwRightUp)
#define swRightDown digitalRead(pinSwRightDown)

int kiriMaju       = 6;  //MOTOR KIRI MAJU
int kananMaju      = 9;  //MOTOR KANAN MAJU
int kiriMundur     = 5;
int kananMundur    = 10;
int pinSwLeftUp    = 3;
int pinSwLeftDown  = 2;
int pinSwRightUp = 11;
int pinSwRightDown = 12;
int ledLeft = 7;
int ledMid = 13;
int ledRight = 8;
String start = "Stop";
String kalibrasi = "Stop";
char buff[100];

unsigned long waktu = millis();

//hitam 980
//putih 880
//peka = putih + ((hitam-putih)/2)
int peka = 500;
int adc_sensor[8], pekax[8],
    sensorMax[8] = {0, 0, 0, 0, 0, 0, 0, 0},
                   sensorMin[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023},
                                  sendat[8], robotSpeedRight, robotSpeedLeft,
                                  robotSpeed = 255,
                                  kp = 25,
                                  kd = 20,
                                  /*                 detik
                                     kp 25, kd 20 -> 37,41
                                     kp 28, kd 16 -> 37,85
                                     kp 28, kd 18 ->
                                  */
                                  lastError = 0,
                                  error = 0,
                                  x, rate, sensorBit,
                                  maxpwm = 250,
                                  t;


void setup() //program dilakukan hanya 1x
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  pinMode( kiriMaju, OUTPUT);
  pinMode( kiriMundur, OUTPUT);
  pinMode( kananMaju, OUTPUT);
  pinMode( kananMundur, OUTPUT);
  pinMode(pinSwLeftUp, INPUT_PULLUP);
  pinMode(pinSwLeftDown, INPUT_PULLUP);
  pinMode(pinSwRightUp, INPUT_PULLUP);
  pinMode(pinSwRightDown, INPUT_PULLUP);
  pinMode( ledLeft, OUTPUT);
  pinMode( ledMid, OUTPUT);
  pinMode( ledRight, OUTPUT);
  delay(100);
  Serial.println("Kelompok nowaf dan alrio");
}

void loop()
{
  if (waktu == 10000){
     digitalWrite(swLeftUp, LOW);
     digitalWrite(swLeftDown, LOW);
     digitalWrite(swRightUp, LOW);
     digitalWrite(swRightDown, LOW);
  }
  if (!swLeftUp)
  {
    start = "high";
    kalibrasi = "stop";
  }
  if (!swLeftDown)
  {
    kalibrasi = "mulai";
    start = "stop";
  }
  if (start == "high")
  {
    follow_line();
  }

  if (kalibrasi == "mulai")
  {
    digitalWrite(ledLeft, HIGH);
    digitalWrite(ledRight, HIGH);
    kalibrasistart(); //menjadikan ledleft mengeluarkan logika 1 atau tegangan 5volt
    digitalWrite(ledLeft, LOW);
    digitalWrite(ledRight, LOW);
    delay(10);
  }
}
void kalibrasistart() {
  adc_sensor[0] = analogRead(A3);
  adc_sensor[1] = analogRead(A2);
  adc_sensor[2] = analogRead(A1);
  adc_sensor[3] = analogRead(A0);
  adc_sensor[4] = analogRead(A7);
  adc_sensor[5] = analogRead(A6);
  adc_sensor[6] = analogRead(A5);
  adc_sensor[7] = analogRead(A4);
  delay(10);
  for (x = 7; x >= 0; x--)
  {
    if (adc_sensor[x] > sensorMax[x])
    {
      sensorMax[x] = adc_sensor[x];
    }

    if (adc_sensor[x] < sensorMax[x])
    {
      sensorMin[x] = adc_sensor[x];
    }

    pekax[x] = (sensorMax[x] + sensorMin[x]) / 2;
    EEPROM.write(x, pekax[x]);

  }
  sprintf(buff, "Sensor Max : %3d %3d %3d %3d %3d %3d %3d %3d ", sensorMax[7], sensorMax[6], sensorMax[5], sensorMax[4], sensorMax[3], sensorMax[2], sensorMax[1], sensorMax[0]);
  Serial.print(buff);
  sprintf(buff, "xx Nilai Tengah : %3d %3d %3d %3d %3d %3d %3d %3d xx", pekax[7], pekax[6], pekax[5], pekax[4], pekax[3], pekax[2], pekax[1], pekax[0]);
  Serial.print(buff);
  sprintf(buff, "Sensor Min : %d%d%d%d%d%d%d%d %3d", sensorMin[7], sensorMin[6], sensorMin[5], sensorMin[4], sensorMin[3], sensorMin[2], sensorMin[1], sensorMin[0]);
  Serial.println(buff);
}
void readSensor() {
  adc_sensor[0] = analogRead(A3) ;
  adc_sensor[1] = analogRead(A2) ;
  adc_sensor[2] = analogRead(A1) ;
  adc_sensor[3] = analogRead(A0) ;
  adc_sensor[4] = analogRead(A7) ;
  adc_sensor[5] = analogRead(A6) ;
  adc_sensor[6] = analogRead(A5) ;
  adc_sensor[7] = analogRead(A4) ;
  delay(50);
  for (x = 7; x >= 0; x--)
  {
    pekax[x] = EEPROM.read(x);
    if (adc_sensor[x] > peka) {
      sendat[x] = 1;
    }
    else                        {
      sendat[x] = 0;
    }
  }

  sensorBit = 0;
  for (x = 0; x <= 7; x++) {
    sensorBit += sendat[x] * (1 << x);
  }
  //tampil data

  sprintf(buff, "%3d %3d %3d %3d %3d %3d %3d %3d ", adc_sensor[7], adc_sensor[6], adc_sensor[5], adc_sensor[4], adc_sensor[3], adc_sensor[2], adc_sensor[1], adc_sensor[0]);
  Serial.print(buff);
  sprintf(buff, "xx %d%d%d%d%d%d%d%d Peka %3d", sendat[7], sendat[6], sendat[5], sendat[4], sendat[3], sendat[2], sendat[1], sendat[0], peka);
  Serial.println(buff);
}

void pv() {
  switch (sensorBit) {

    case 0b10000000: error = -7; break;
    case 0b11000000: error = -6; break;
    case 0b01000000: error = -5; break;
    case 0b01100000: error = -4; break;
    case 0b00100000: error = -3; break;
    case 0b00110000: error = -2; break;
    case 0b00010000: error = -1; break;

    case 0b00011000: error = 0; break;

    case 0b00001000: error = 1; break;
    case 0b00001100: error = 2; break;
    case 0b00000100: error = 3; break;
    case 0b00000110: error = 4; break;
    case 0b00000010: error = 5; break;
    case 0b00000011: error = 6; break;
    case 0b00000001: error = 7; break;

    default: error = lastError; break;
  }
}

void follow_line() {
  readSensor();
  pv();

  if (error <  -2)
  {
    digitalWrite(ledLeft, 1);
    digitalWrite(ledMid, 0);
    digitalWrite(ledRight, 0);
  }
  else if (error >  2)
  {
    digitalWrite(ledLeft, 0);
    digitalWrite(ledMid, 0);
    digitalWrite(ledRight, 1);
  }
  else
  {
    digitalWrite(ledLeft, 0);
    digitalWrite(ledMid, 1);
    digitalWrite(ledRight, 0);
  }
  rate = error - lastError;
  lastError = error;

  int moveControl = (kp * error) + (rate * kd);

  robotSpeedRight = robotSpeed - moveControl;
  robotSpeedLeft  = robotSpeed + moveControl;

  setMotor(robotSpeedLeft, robotSpeedRight);
}

void setMotor(int pwmLeft, int pwmRight)
{
  if (pwmLeft > maxpwm) {
    pwmLeft = maxpwm;
  }
  else if (pwmLeft < -maxpwm) {
    pwmLeft = -maxpwm;
  }
  if (pwmLeft < 0) {
    pwmLeft *= -1;
    analogWrite(kiriMundur, pwmLeft);
    analogWrite(kiriMaju, 0);
  }
  else
  {
    analogWrite(kiriMundur, 0);
    analogWrite(kiriMaju, pwmLeft);
  }

  if (pwmRight > maxpwm) {
    pwmRight = maxpwm;
  }
  else if (pwmRight < -maxpwm) {
    pwmRight = -maxpwm;
  }
  if (pwmRight < 0) {
    pwmRight *= -1;
    analogWrite(kananMundur, pwmRight);
    analogWrite(kananMaju, 0);
  }
  else {
    analogWrite(kananMundur, 0);
    analogWrite(kananMaju, pwmRight);
  }

}
