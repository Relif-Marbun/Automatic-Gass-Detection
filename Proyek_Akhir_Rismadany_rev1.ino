//HEADER
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "CTBot.h"
#include "SoftwareSerial.h"
#include "DFPlayer_Mini_Mp3.h"
int sensorgas = 2; int nilaisensor; int in1 = 18; int in2 = 19; const int buzzer = 5; int aktif = 0;

SoftwareSerial serialku(16, 17);
LiquidCrystal_I2C lcd(0x27, 16, 2);
CTBot myBot;

String ssid  = "Rismadany";
String pass  = "Dany1234";

String token = "5684048334:AAFDOPdEek4y_ifdWNxViJI5pEXlTUZ7NN4";
const int id = 1082693849;

void motor();
void setup() {
  Serial.begin(9600);
  lcd.init(); lcd.init(); lcd.backlight();
  lcd.setCursor(2, 0); lcd.print("WELCOME TO");
  lcd.setCursor(0, 1); lcd.print("PROYEKAKHIR DANY");
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT); pinMode(sensorgas, INPUT); pinMode(buzzer, OUTPUT);

  //  mulai df player dengan komunikasi serial
  serialku.begin(9600); mp3_set_serial (serialku); delay(1);
  mp3_set_volume (30); //set volume

  //Mulai koneksi internet
  myBot.wifiConnect(ssid, pass);

  //Mulai koneksi ke telegram
  myBot.setTelegramToken(token);

  //tes koneksi ke telegram
  if (myBot.testConnection()) {
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Koneksi Berhasil");
    Serial.println("Koneksi Berhasil");
  }
  else {
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("Koneksi Gagal");
    Serial.println("Koneksi Gagal");
  }
}

void loop() {
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Nilai Sensor: ");
  lcd.setCursor(15, 0); lcd.print(nilaisensor);
  lcd.setCursor(0, 1); lcd.print("LPG MASIH AMAN ");
  int nilai = digitalRead(sensorgas);
  if (nilai == 1) {
    nilaisensor = 0;
  }
  else if (nilai == 0) {
    nilaisensor = 1;
  }
  TBMessage msg;
  if (myBot.getNewMessage (msg)) {
    String pesan = msg.text;
    if (pesan == "on") {
      aktif = 1;
      myBot.sendMessage(msg.sender.id, "Sistem Aktif");
      lcd.clear();
      lcd.setCursor(1, 1); lcd.print("Sistem Aktif ");
      Serial.println ("Sistem Aktif");
    }
    else if (pesan == "off") {
      aktif = 0;
      myBot.sendMessage(msg.sender.id, "Sistem Nonaktif");
      lcd.clear();
      lcd.setCursor(0, 1); lcd.print("Sistem Nonaktif ");
      Serial.println ("Sistem Nonaktif");
    }
  }


  if (aktif == 1) {
    if (nilaisensor == 1 ) {
      lcd.clear();
      lcd.setCursor(2, 0); lcd.print("Nilai Sensor: ");
      lcd.setCursor(15, 0); lcd.print(nilaisensor);
      Serial.println("Peringatan!! Gas LPG Anda Bocor..."); Serial.println("Gas Terdeteksi");
      myBot.sendMessage(msg.sender.id, "Peringatan!Gas LPG Anda Bocor, Asap Terdeteksi");
      digitalWrite(buzzer, HIGH);
      lcd.setCursor(2, 1); lcd.print("LPG BOCOR");
      mp3_play (1); delay(3000); mp3_next();
      motor();
    }

    if (nilaisensor == 0) {
      digitalWrite(in1, LOW); digitalWrite(in2, LOW);
      Serial.println("Gas LPG aman");
      digitalWrite(buzzer, LOW);
    }
  }
}

void motor() {
  delay(3000);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  Serial.print(" "); Serial.print("Motor Maju"); Serial.print(" ");
  delay(3000);
  Serial.print(" "); Serial.print("Motor Mati"); Serial.print(" ");
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  delay(3000);
  Serial.print(" "); Serial.print("Motor Mundur"); Serial.print(" ");
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  delay(2000);
  Serial.print(" "); Serial.print("Motor Mati"); Serial.print(" ");
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
}
