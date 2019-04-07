#include <string.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//dht11
#include <DHT.h>

//lcd
#define DHTTYPE DHT11   // DHT 11
const int sda = A4;
const int scl = A5;
const int w = 4;
const int r = A1;
const int g = A2;
const int b = A3;
const int DHTPIN = A0;
const int light = 13;
//init lib
DHT dht(DHTPIN, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD I2C address

#define BACKLIGHT_PIN     13

//cores
//http://www.perbang.dk/rgbgradient/
unsigned int RED[]   = {0xff, 0xff, 0xff, 0xaa, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned int GREEN[] = {0x00, 0x71, 0xe2, 0xff, 0xff, 0xff, 0xff, 0xe2, 0x71, 0x00};
unsigned int BLUE[]  = {0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0xa9, 0xff, 0xff, 0xff};

void setup() {
  pinMode(w, OUTPUT);
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(light, INPUT);
  pinMode(DHTPIN, INPUT);

  dht.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Carregando");
  lcd.setCursor(0, 1);
  lcd.print("...");

  digitalWrite(r, LOW);
  digitalWrite(g, LOW);
  digitalWrite(b, LOW);
}

unsigned int val(unsigned int input) {
  unsigned int v = 0xff - input;
  return v;
}

bool getDhtOk(float &t, float &h) {
  h = dht.readHumidity();
  t = dht.readTemperature();

  return !isnan(t) || !isnan(h);
}

void loop() {
  float h = 0;
  float t = 0;

  int l = digitalRead(light);
  if (l == HIGH) {
    digitalWrite(w, LOW);
  } else {
    digitalWrite(w, HIGH);
  }

  if (!getDhtOk(t, h)) {
    String msg = "Failed to read from DHT";

  } else {
    // Print a message to the LCD.
    String text = "Humidity: " + String(h) + "%";

    String text2 = "Temperature: " + String(t) + "*C";

    lcd.setCursor(0, 0);
    lcd.print(text);
    lcd.setCursor(0, 1);
    lcd.print(text2);
  }

  int color = h / 10;
  analogWrite(r, val(RED[color]));
  analogWrite(g, val(GREEN[color]));
  analogWrite(b, val(BLUE[color]));

  delay(12510);
}
