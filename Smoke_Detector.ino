#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

#define MQ7_PIN A0
#define MQ135_PIN A1

int mq7Base = 0;
int mq135Base = 0;

void setup() {

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // ===================
  // KALIBRASI
  // ===================

  display.setCursor(0,20);
  display.println("KALIBRASI...");
  display.println("TUNGGU 10 DETIK");
  display.display();

  long sum7 = 0;
  long sum135 = 0;

  for(int i=0;i<100;i++) {

    sum7 += analogRead(MQ7_PIN);
    sum135 += analogRead(MQ135_PIN);

    delay(100);
  }

  mq7Base = sum7 / 100;
  mq135Base = sum135 / 100;

  Serial.println("=== BASELINE ===");
  Serial.print("MQ7 Base = ");
  Serial.println(mq7Base);

  Serial.print("MQ135 Base = ");
  Serial.println(mq135Base);

  display.clearDisplay();

  display.setCursor(0,0);
  display.println("BASELINE");

  display.print("MQ7:");
  display.println(mq7Base);

  display.print("MQ135:");
  display.println(mq135Base);

  display.display();

  delay(3000);
}

void loop() {

  // ===================
  // MODE SIAP
  // ===================

  display.clearDisplay();

  display.setCursor(0,0);
  display.println("SMART SPIROMETER");

  display.setCursor(0,20);
  display.println("SILAKAN TIUP");

  display.display();

  delay(2000);

  // ===================
  // PENGUKURAN 5 DETIK
  // ===================

  int min7 = 1023;
  int min135 = 1023;

  display.clearDisplay();
  display.setCursor(0,20);
  display.println("MENGUKUR...");
  display.println("TIUP 5 DETIK");
  display.display();

  unsigned long startTime = millis();

  while(millis() - startTime < 5000) {

    int mq7 = analogRead(MQ7_PIN);
    int mq135 = analogRead(MQ135_PIN);

    if(mq7 < min7)
      min7 = mq7;

    if(mq135 < min135)
      min135 = mq135;

    delay(50);
  }

  // ===================
  // HITUNG SELISIH
  // ===================

  int d7 = mq7Base - min7;
  int d135 = mq135Base - min135;

  Serial.print("MIN7=");
  Serial.print(min7);

  Serial.print(" MIN135=");
  Serial.print(min135);

  Serial.print(" D7=");
  Serial.print(d7);

  Serial.print(" D135=");
  Serial.println(d135);

  // ===================
  // HASIL
  // ===================

  display.clearDisplay();

  display.setCursor(0,0);
  display.print("D7:");
  display.println(d7);

  display.print("D135:");
  display.println(d135);

  display.println();

  if(d135 < 100) {

    display.println("NAPAS");
    display.println("TIDAK TERDETEKSI");

  }
  else {

    if(d7 < 18) {

      display.println("NON-PEROKOK");

    }
    else if(d7 < 24) {

      display.println("VAPER");

    }
    else {

      display.println("PEROKOK/IQOS");

    }
  }

  display.display();

  delay(5000);
}