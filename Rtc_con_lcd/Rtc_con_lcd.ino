#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>  

Servo ioe;
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;  // Crea un objeto RTC

int trig = 2;
int echo = 3;
int tiempo;
int distancia;

void setup() {
  // Configura los pines del sensor ultrasónico
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  // Configura el servomotor
  ioe.attach(5);
  
  // Configura la LCD
  lcd.init();
  lcd.backlight();
  
  // Inicia el RTC
  if (!rtc.begin()) {
    lcd.setCursor(0, 0);
    lcd.print("RTC no encontrado");
    while (1);
  }

  if (rtc.lostPower()) {
    // Si el RTC ha perdido la hora, configúrala a una hora predeterminada
    rtc.adjust(DateTime(22, 5, 2024, 10, 45, 00)); // Fecha y hora actual
  }
}

void loop() {
  // Obtiene la hora actual del RTC
  DateTime now = rtc.now();

  // Envía el pulso del trig
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  // Calcula el tiempo del pulso del echo
  tiempo = pulseIn(echo, HIGH);
  distancia = tiempo / 58.2;
  delay(500);

  if (distancia <= 20) {
    ioe.write(120);  // Levanta la plumilla
    lcd.setCursor(0, 1);
    lcd.print("Adelante        ");  // Muestra "Adelante" en la LCD
    delay(3000);
  } else {
    ioe.write(45);  // Baja la plumilla
    lcd.setCursor(0, 1);
    lcd.print("Esperando      ");  // Muestra "Esperando" en la LCD
  }

  // Muestra la fecha en la LCD
  lcd.setCursor(0, 0);
  lcd.print(now.year(), DEC);
  lcd.print('/');
  lcd.print(now.month(), DEC);
  lcd.print('/');
  lcd.print(now.day(), DEC);
  lcd.print("     ");

  // Muestra la hora en la LCD
  lcd.setCursor(0, 1);
  lcd.print(now.hour(), DEC);
  lcd.print(':');
  lcd.print(now.minute(), DEC);
  lcd.print(':');
  lcd.print(now.second(), DEC);
  lcd.print("     ");
}
