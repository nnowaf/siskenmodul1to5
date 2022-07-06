int kiriMaju       = 6;  //MOTOR KIRI MAJU
int kananMaju      = 9;  //MOTOR KANAN MAJU

char buff[100];

//hitam 980
//putih 880
//peka = putih + ((hitam-putih)/2)
int peka = 900;

int
adc_sensor[8],
           sendat[8],
           x,
           sensorBit,

           t
           ;

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
  pinMode( kananMaju, OUTPUT);
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
  sprintf(buff, "%d%d%d%d%d%d%d%d %3d", sendat[7], sendat[6], sendat[5], sendat[4], sendat[3], sendat[2], sendat[1], sendat[0], peka);
  Serial.println(buff);
}

void loop()  //program dilakukan hanya 1x
{
  readSensor();
  if (adc_sensor[3] > 800 && adc_sensor[4] > 800)
  {
    digitalWrite(kiriMaju, HIGH);
    digitalWrite(kananMaju, HIGH);
  }
  if ((adc_sensor[0] > 800 || adc_sensor[1] > 800 || adc_sensor[2] > 800 || adc_sensor[3] > 800) && (adc_sensor[4] <= 800 || adc_sensor[5] <= 800 || adc_sensor[6] <= 800 || adc_sensor[7] <= 800))
  {
    digitalWrite(kiriMaju, HIGH);
    digitalWrite(kananMaju, LOW);
  }
   if ((adc_sensor[7] > 800 || adc_sensor[6] > 800 || adc_sensor[5] > 800 || adc_sensor[4] > 800) && (adc_sensor[3] <= 800 || adc_sensor[2] <= 800 || adc_sensor[1] <= 800 || adc_sensor[0] <= 800))
  {
    digitalWrite(kiriMaju, LOW);
    digitalWrite(kananMaju, HIGH);
  }
 if ((adc_sensor[0] <= 800 && adc_sensor[1] <= 800 && adc_sensor[2] <= 800 && adc_sensor[3] <= 800) && (adc_sensor[4] <= 800 && adc_sensor[5] <= 800 && adc_sensor[6] <= 800 && adc_sensor[7] <= 800))
  {
    digitalWrite(kiriMaju, LOW);
    digitalWrite(kananMaju, LOW);
  }

}

