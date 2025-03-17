# EntropyVisualizer

### GIF


<img src="EntropyVisualization.gif" alt="gif of working Entropy Visualization" height="500">

### PNG

<img src="https://github.com/mklemmingen/mklemmingen/blob/main/EntropyVisualizer.png?raw=true" alt="png of working Entropy Visualization" height="500">

### Board Plan in text-form:

Components:

Arduino Uno
Adafruit SSD1306 OLED Display

DHT11 Temperature and Humidity Sensor

MQ Sensor
Adafruit NeoPixel LED Strip
KY-037 Microphone Sensor
Connecting Wires

Connections:

OLED Display:
VCC to 5V ,
GND to GND ,
SCL to A5 ,
SDA to A4 ,
RESET to GND (or any unused digital pin if needed)

DHT11 Sensor:
VCC to 5V ,
GND to GND ,
DATA to Digital Pin 2 

MQ Sensor:
AO (Analog Output) to A0 ,
DO (Digital Output) to Digital Pin 3 ,
VCC to 5V ,
GND to GND 

NeoPixel LED Strip:
DIN to Digital Pin 8 ,
VCC to 5V ,
GND to GND 

KY-037 Microphone Sensor:
AO (Analog Output) to A1 ,
DO (Digital Output) to Digital Pin 4 ,
VCC to 5V ,
GND to GND 

#### In my build, I connected the 5V cables of the LED Strip, the gas sensor and the temperature sensor into one point to plug into a 5V port. For the OLED, the 3.3V was used, and the last remaining easily includable 5V was used for the microphone to get a consistent reading. 

### Pseudocode

FUNCTION loop()
  DECLARE humidity, temperature, mqAnalogValue, micValue, mix

  FOR each LED from 0 to NUM_LEDS - 1 DO
    READ humidity from dht sensor
    READ temperature from dht sensor
    READ mqAnalogValue from MQ_AO_PIN
    READ micValue from MIC_AO_PIN

    IF humidity or temperature is NaN THEN
      RETURN
    END IF

    READ mqDigitalValue from MQ_DO_PIN
    READ micDigitalValue from MIC_DO_PIN

    CALCULATE tempFraction as fractional part of temperature
    CALCULATE mix using humidity, temperature, mqAnalogValue, micValue, mqDigitalValue, micDigitalValue, and tempFraction

    CALCULATE red, green, blue values using mix with modulo and minimum value adjustments

    SET pixel color on strip at current LED to (red, green, blue)
    PRINT mix to serial
  END FOR

  SHOW updated colors on strip

  CLEAR display
  SET text size to 1
  SET text color to white

  DISPLAY temperature on OLED
  DISPLAY humidity on OLED
  DISPLAY mqAnalogValue on OLED
  DISPLAY micValue on OLED
  DISPLAY mix on OLED

  UPDATE display

  DELAY for 500 milliseconds
END FUNCTION
