#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// DHT11 settings
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// MQ Sensor settings
#define MQ_AO_PIN A0
#define MQ_DO_PIN 3
#define MQ_MAX_VALUE 1023

#define LED_PIN 8
#define NUM_LEDS 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// KY-037 Microphone settings
#define MIC_AO_PIN A1
#define MIC_DO_PIN 4

void setup() {
  // Initializes serial communication
  Serial.begin(9600);

  // Initializes DHT sensor
  dht.begin();

  // Initializes OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Stays here forever
  }
  display.display();
  delay(2000);
  display.clearDisplay();

  // Sets MQ sensor digital pin as input
  pinMode(MQ_DO_PIN, INPUT);

  // Initializes NeoPixel strip
  strip.begin();
  strip.setBrightness(15); // Set brightness (0-255)
  strip.show(); // Initialize all pixels to 'off'

  // Sets KY-037 microphone digital pin as input
  pinMode(MIC_DO_PIN, INPUT);
}

void loop() {
  float humidity = 0;
  float temperature = 0;
  int mqAnalogValue = 0;
  int micValue = 0;
  unsigned long mix = 0;

  // Iterates through Numbers of LEDS, check each component at each LED, mix and create colour output
  for (int i = 0; i < NUM_LEDS; i++) {
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    mqAnalogValue = analogRead(MQ_AO_PIN); // not converted to voltage for more leverage in mix calc
    micValue = analogRead(MIC_AO_PIN); // not converted to voltage for more leverage in mix calc

    // Checks if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature)) {
      return;
    }

    int mqDigitalValue = digitalRead(MQ_DO_PIN);
    int micDigitalValue = digitalRead(MIC_DO_PIN);

    // Introduce more variability without using random()
    float tempFraction = temperature - int(temperature); // Get fractional part of temperature
    mix = (humidity * (tempFraction + 1)) + (temperature * (mqAnalogValue % 10 + 1)) + (mqAnalogValue * (micValue % 10 + 1)) + (micValue * (mqDigitalValue + 1)) + (mqDigitalValue * (micDigitalValue + 1)) + (micDigitalValue * (humidity + 1));

    // Uses modulo to bring mixColour to 0-255 and ensure minimum value (for less white/off LEDs without affecting random mix value)
    int red = (mix % 255) + 50; // Ensure minimum value of 50
    int green = ((mix / 2) % 255) + 50; // Ensure minimum value of 50
    int blue = ((mix / 3) % 255) + 50; // Ensure minimum value of 50

    strip.setPixelColor(i, strip.Color(red, green, blue));

    // prints mix to serial
    Serial.println(mix);
  }

  strip.show();

  // Displays sensor values on OLED with graphics
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Displays temperature
  display.setCursor(0, 0);
  display.print(F("Temp: "));
  display.print(temperature);
  display.println(F(" C"));

  // Displays humidity
  display.setCursor(0, 10);
  display.print(F("Humidity: "));
  display.print(humidity);
  display.println(F(" %"));

  // Displays MQ sensor analog value as current/max
  display.setCursor(0, 20);
  display.print(F("Analog MQ: "));
  display.print(mqAnalogValue);

  // Displays Microphone Amount
  display.setCursor(0, 30);
  display.print(F("Analog Mic: "));
  display.print(micValue);

  // Displays Amount of Randomness between all values achieved from the LastLED
  display.setCursor(0, 40);
  display.print(F("LastLED Mix: "));
  display.print(mix);

  display.display();

  delay(500);
}