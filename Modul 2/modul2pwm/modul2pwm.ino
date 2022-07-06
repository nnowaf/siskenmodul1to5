int kiriMaju       = 6;  //MOTOR KIRI MAJU
int kananMaju      = 9;  //MOTOR KANAN MAJU

char buff[100];

//hitam 980
//putih 880
//peka = putih + ((hitam-putih)/2)
int peka = 900;
int lastError = 0;


String stringKondisi = "00000000";
String stringKondisiAkhir = "00000000";

int
adc_sensor[8],sendat[8],x,sensorBit,t;

void setup() //program dilakukan hanya 1x
{
  Serial.begin(9750);
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
  //100000
  if (adc_sensor[0] > 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
   
    stringKondisi = "10000000";
    lastError = -6;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] > 750 && adc_sensor[1] > 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
   
    stringKondisi = "11000000";
    lastError = -5;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] > 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "01000000";
    lastError = -4;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] > 750 && adc_sensor[2] > 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "01100000";
    lastError = -3;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] > 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
   
    stringKondisi = "00100000";
    lastError = -2;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "00110000";
    lastError = -1;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] > 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {

    stringKondisi = "00010000";
    lastError = 0;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }
  
  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] > 750 && adc_sensor[4] > 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "00011000";
    lastError = 0;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] > 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    analogWrite(kiriMaju, 255 * 30 / 100);
    analogWrite(kananMaju, 255 * 50 / 100);
    stringKondisi = "00001000";
    lastError = 0;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] > 750 && adc_sensor[5] > 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "00001100";
    lastError = 1;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] > 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    analogWrite(kiriMaju, 255 * 20 / 100);
    
    stringKondisi = "00000100";
    lastError = 2;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] > 750 &&  adc_sensor[6] > 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "00000110";
    lastError = 3;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] > 750 && adc_sensor[7] <= 750)
  {
    
    stringKondisi = "00000010";
    lastError = 4;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] > 750 && adc_sensor[7] > 750)
  {
    
    stringKondisi = "00000011";
    lastError = 5;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] > 750)
  {
    stringKondisi = "00000001";
    lastError = 6;
    Serial.print ("Error = ");
    Serial.print (lastError);
  }

  if (adc_sensor[0] <= 750 && adc_sensor[1] <= 750 && adc_sensor[2] <= 750 && adc_sensor[3] <= 750 && adc_sensor[4] <= 750 && adc_sensor[5] <= 750 &&  adc_sensor[6] <= 750 && adc_sensor[7] <= 750)
  {
    analogWrite(kiriMaju, 255 * 5 / 100);
    analogWrite(kananMaju, 255 * 100 / 50);
    Serial.print ("Garis Hitam Tidak Terdeteksi");
  }

  //TES TES TES

  if (lastError == -6) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 0 / 100);
    }

    if (lastError == -5) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 5 / 100);
    }

    if (lastError == -4) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 10 / 100);
    }

    if (lastError == -3) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 15 / 100);
    }

    if (lastError == -2) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 20 / 100);
    }

    if (lastError == -1) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 25 / 100);
    }

    if (lastError == 0) {
      analogWrite(kiriMaju, 255 * 50 / 100);
      analogWrite(kananMaju, 255 * 30 / 100);
    }

    if (lastError == 0) {
      analogWrite(kiriMaju, 255 * 60 / 100);
      analogWrite(kananMaju, 255 * 60 / 100);
    }

    if (lastError == 0) {
      analogWrite(kiriMaju, 255 * 30 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }

    if (lastError == 1) {
      analogWrite(kiriMaju, 255 * 25 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }

    if (lastError == 2) {
      analogWrite(kiriMaju, 255 * 20 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }

    if (lastError == 3) {
      analogWrite(kiriMaju, 255 * 15 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }

    if (lastError == 4) {
      analogWrite(kiriMaju, 255 * 10 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }

    if (lastError == 5) {
      analogWrite(kiriMaju, 255 * 5 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }

    if (lastError == 6) {
      analogWrite(kiriMaju, 255 * 0 / 100);
      analogWrite(kananMaju, 255 * 50 / 100);
    }
  
}
