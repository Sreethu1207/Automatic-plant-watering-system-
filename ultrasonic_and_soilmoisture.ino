#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensor_pin = A0;       
int relay_pin = 7;         
int trig_pin = 9;         
int echo_pin = 10;         
long duration;             
int distance;              

void setup() {
  Serial.begin(9600);
  lcd.init();                     
  lcd.backlight();

  pinMode(sensor_pin, INPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
}

void loop() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  duration = pulseIn(echo_pin, HIGH);
  distance = duration * 0.034 / 2; 

  Serial.print("Water Level Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  int sensor_data = analogRead(sensor_pin);
  Serial.print("Sensor_data: ");
  Serial.println(sensor_data);

  lcd.clear();

 
  lcd.setCursor(0, 0);
  lcd.print("Water: ");
  lcd.print(distance);
  lcd.print("cm  ");  

  if (sensor_data > 950) { 
    if (distance>=3) {
      Serial.println("Soil is dry but water level is low, turning OFF motor");
      digitalWrite(relay_pin, LOW); 
      lcd.setCursor(0, 1);
      lcd.print("Dry&Low OFF  ");
    } else {
      Serial.println("Soil is dry, water level sufficient, turning ON motor");
      digitalWrite(relay_pin, HIGH); 
      lcd.setCursor(0, 1);
      lcd.print("Dry&Motor ON ");
    }
  } else if (sensor_data >= 500 && sensor_data <= 950) {
    Serial.println("Soil is medium, motor OFF");
    digitalWrite(relay_pin, LOW); 
    lcd.setCursor(0, 1);
    lcd.print("Medium&MotorOFF");
  } else if (sensor_data < 500) {
    Serial.println("Soil is wet, motor OFF");
    digitalWrite(relay_pin, LOW); 
    lcd.setCursor(0, 1);
    lcd.print("Wet&Motor OFF ");
  }

  delay(1000); 
}
