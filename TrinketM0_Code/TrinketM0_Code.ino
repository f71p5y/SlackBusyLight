#include <Adafruit_DotStar.h>

#define NUMPIXELS 1
#define DATAPIN   7  // Pin where the DotStar data line is connected (Trinket M0 has a built-in DotStar on pin 1)
#define CLOCKPIN  8  // Pin where the DotStar clock line is connected (Trinket M0 has a built-in DotStar on pin 2)

#define d_LED_RED_PIN      (0)
#define d_LED_BLUE_PIN     (2)
#define d_LED_GREEN_PIN    (1)

#define d_STATUS_AWAY      ("AWAY")
#define d_STATUS_BUSY      ("BUSY")
#define d_STATUS_FLOW      ("FLOW")
#define d_STATUS_AVAILABLE ("AVAILABLE")
#define d_STATUS_LUNCH     ("LUNCH")



Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
String status ="EMPTY";

// Function to set the color of the onboard DotStar LED
void setColor(uint8_t red, uint8_t green, uint8_t blue) {
  strip.setPixelColor(0, strip.Color(red, green, blue));
  strip.show();
}

void setup() {
  Serial.begin(115200);
  Serial.write("CrudeBusyLight V1.0.0");
  strip.begin();  // Initialize DotStar
  setColor(0, 0, 0); // RGB values for yellow
  strip.show();   // Initialize all pixels to 'off'
  pinMode(d_LED_RED_PIN, OUTPUT);
  pinMode(d_LED_BLUE_PIN, OUTPUT);
  pinMode(d_LED_GREEN_PIN, OUTPUT);

}

void loop() {
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n');
    if (receivedData == "?") {
      Serial.println("CrudeBusyLight");
    }
    if (receivedData == "AWAY") {
      digitalWrite(d_LED_BLUE_PIN,LOW);
      digitalWrite(d_LED_RED_PIN,LOW);
      digitalWrite(d_LED_GREEN_PIN,LOW);
      status = d_STATUS_AWAY;
    }
    if (receivedData == "BUSY") {
      digitalWrite(d_LED_BLUE_PIN,LOW);
      digitalWrite(d_LED_RED_PIN,HIGH);
      digitalWrite(d_LED_GREEN_PIN,LOW);
      status = d_STATUS_BUSY;
    }
        if (receivedData == "FLOW") {
      digitalWrite(d_LED_BLUE_PIN,HIGH);
      digitalWrite(d_LED_RED_PIN,HIGH);
      digitalWrite(d_LED_GREEN_PIN,LOW);
      status = d_STATUS_FLOW;
    }
      if (receivedData == "AVAILABLE") {
      digitalWrite(d_LED_BLUE_PIN,LOW);
      digitalWrite(d_LED_RED_PIN,LOW);
      digitalWrite(d_LED_GREEN_PIN,HIGH);
      status = d_STATUS_AVAILABLE;
    }
    if (receivedData == "LUNCH") {
      digitalWrite(d_LED_BLUE_PIN,LOW);
      digitalWrite(d_LED_RED_PIN,HIGH);
      digitalWrite(d_LED_GREEN_PIN,HIGH);
      status = d_STATUS_LUNCH;
    }
      if (receivedData == "STATUS") {
      Serial.println(status);
    }
  }
}