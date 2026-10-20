#include <ThreeWire.h>
#include <RtcDS1302.h>

// DS1302 RTC connections
#define RTC_DAT 4
#define RTC_CLK 5
#define RTC_RST 6

ThreeWire myWire(RTC_DAT, RTC_CLK, RTC_RST);
RtcDS1302<ThreeWire> rtc(myWire);


// Defining the different states of clock
#define TAKE_MEDICATION 0
#define DRINK_WATER      1
#define REST             2

int current_state = REST;


int red = 8;
int yellow = 9;
int green = 10;
int buzzer = 11;

//For the green light
unsigned long previousMillis = 0;
const unsigned long interval = 500;
//For buzzer
unsigned long previousMilliss = 0; //for medications
bool beepState = LOW;

unsigned long previousMillisss = 0;  //for water

unsigned long previousMillissss = 0;  //for melodies

bool greenState = LOW;


void setup() {

  Serial.begin(9600);

  // Set LED pins as outputs
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buzzer, OUTPUT);

  rtc.Begin();

  Serial.println("System started.");

  // Check if RTC time is valid
  if (!rtc.IsDateTimeValid()) {
    Serial.println("RTC time is not valid!");
  }
}


void loop() {
 unsigned long currentMillis = millis();

  // Get time from DS1302
  RtcDateTime now = rtc.GetDateTime();

  int hour = now.Hour();
  int minute = now.Minute();

  Serial.print(now.Hour());
  Serial.print(":");
  Serial.print(now.Minute());
  Serial.print(":");
  Serial.println(now.Second());


  if ((hour == 9 && minute < 10) || (hour == 15 && minute < 10) || (hour == 18 && minute < 41) ) {
    if (current_state != TAKE_MEDICATION) {
      current_state = TAKE_MEDICATION;
    }
  }
  else if ((hour == 7 && minute >= 30 && minute < 40) || (hour == 12 && minute >= 30 && minute < 40) || (hour == 18 && minute < 45)) {
    if (current_state != DRINK_WATER) {
      current_state = DRINK_WATER;
    }
  }
  else {
    if (current_state != REST) {
      current_state = REST;
    }
  }


  if (current_state == TAKE_MEDICATION) {

    digitalWrite(red, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(green, LOW);
    tone(buzzer, 3000);

    if (currentMillis - previousMilliss >= 200) {

  previousMilliss = currentMillis;

  beepState = !beepState;

  if (beepState) {
    tone(buzzer, 3000);   // 3000 Hz
  } 
  else {
    noTone(buzzer);
  }
    }

    Serial.println("TIME FOR MEDICINE");

  }

  else if (current_state == DRINK_WATER) {

    digitalWrite(red, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(green, LOW);
    
    if (currentMillis - previousMillisss >= interval) {

  previousMillisss = currentMillis;

  beepState = !beepState;

  if (beepState) {
    tone(buzzer, 1000);   // 1000 Hz
  } 
  else {
    noTone(buzzer);
  }
}

    Serial.println("TIME TO DRINK WATER");

  }

  else if (current_state == REST) {

    digitalWrite(red, LOW);
    digitalWrite(yellow, LOW);
    noTone(buzzer);


    if (currentMillis - previousMillis >= interval) {

      previousMillis = currentMillis;

      greenState = !greenState;

      digitalWrite(green, greenState);
    }

    int melody[] = {2000, 3000, 3500};
int note = 0;

if (currentMillis - previousMillissss >= 400) {

  previousMillissss = currentMillis;

  beepState = !beepState;

  if (beepState) {
    tone(buzzer, melody[note]);

    note++;

    if (note >= 3) {
      note = 0;
    }
  } 
  else {
    noTone(buzzer);
  }
}

    Serial.println("REST");
  }
}
