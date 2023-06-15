int sensor = 2;
int nilai; int nilaisensor;
void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  nilai = digitalRead(sensor);
  if (nilai == 1) {
    nilaisensor = 0;
  }
  else if (nilai == 0) {
    nilaisensor = 1;
  }
  if (nilaisensor == 0) {
    Serial.print("Sensor Nonktif "); Serial.print(" ");
  }
  else if (nilaisensor == 1) {
    Serial.print("Sensor Aktif "); Serial.print(" ");
  }

  Serial.print("Nilai Sensor: ");
  Serial.println(nilaisensor);
  delay(300);
}
