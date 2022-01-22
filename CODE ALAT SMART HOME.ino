#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int pingPin = 9;
int ambient=A0;
int pinTemp=A2;
float light;
int var=0;
void setup() {
  Serial.begin(9600);
  pinMode(ambient, INPUT);
  pinMode(pinTemp, INPUT);
  pinMode(13, OUTPUT);
  pinMode(A1, OUTPUT);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Selamat Datang");
  delay(1000);
  lcd.clear();
}

void loop() {
  long duration, inches, cm;
  int suhu = analogRead(pinTemp);
  float voltage = suhu*4.68;
  voltage /= 1024.0;

  float tempC = (voltage-0.5) * 100;
  
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm, ");
  Serial.print("Suhu Saat ini : ");
  Serial.print(tempC);
  Serial.print(" Degrees C");
  Serial.println();
  
  int light_value= analogRead(ambient);
  light = light_value * 0.0976;
  if(cm < 100){
    if(light_value < 300 && tempC > 38){
      var=1;
    }
    else if(light_value < 300 && tempC < 38){
      var=2;
    }
    else if(light_value > 300 && tempC > 38) {
      var=3;
    }
    else if(light_value > 300 && tempC < 38) {
      var=4;
    }
  }
  else if(cm > 100){
    var=0;
  }
  switch (var) {
    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Ruangan Gelap ");
      lcd.setCursor(0, 1);
      lcd.print("Temp : ");
      lcd.print(tempC); lcd.print("  ");
      digitalWrite(13,HIGH);
      analogWrite(A1, 255);
      break;
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Ruangan Gelap ");
      lcd.setCursor(0, 1);
      lcd.print("Temp : ");
      lcd.print(tempC); lcd.print("  ");
      digitalWrite(13,HIGH);
      analogWrite(A1, 0);
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("Ruangan Terang");
      lcd.setCursor(0, 1);
      lcd.print("Temp : ");
      lcd.print(tempC); lcd.print("  ");
      digitalWrite(13,LOW);
      analogWrite(A1, 255);
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Ruangan Terang");
      lcd.setCursor(0, 1);
      lcd.print("Temp : ");
      lcd.print(tempC); lcd.print("  ");
      digitalWrite(13,LOW);
      analogWrite(A1, 0);
      break;
    default:
      lcd.setCursor(0, 0);
      lcd.print("Ga ada Orang  ");
      lcd.setCursor(0, 1);
      lcd.print("            ");
      digitalWrite(13,LOW);
      analogWrite(A1, 0);
      break;
  }
}

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
