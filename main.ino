#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
int ADCV=A0; // Analog input connected to TMP36
#include "pitches.h"

const float minTempC = 10.0;
const float maxTempC = 25.0;

const int BUZZER_PIN = 5;
const int red_led = 3;

int melody[] = { //Notes in pitches.h
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
};

int durations[] = { //Durations to play each note
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
};


void setup()
{
  lcd.init(); // initialize the lcd
  pinMode(red_led, OUTPUT); // red led pin to alert
  pinMode(BUZZER_PIN,OUTPUT); // buzzer pin to alert

}
void loop()
{
  int VoltageRead=analogRead(ADCV); //Read ADC value (0-1023)
  float Vo=(VoltageRead/1023.0)*5; //Convert ADC value to voltage
  float TempC=(Vo-0.5)*100 - 1; //Convert TMP36 voltage to temp C
  lcd.backlight(); //backlight on
  lcd.setCursor(0,0); //initialize the cursor
  lcd.print("T:"); // Print characters to LCD.
  //lcd.setCursor(1,1);
  lcd.print(TempC); // Print variable value to LCD
  lcd.print(char(223)); // the degree sign
  lcd.print("C");
  //lcd.setCursor(10,0)

  //lcd.setCursor(1,1);
  //lcd.print("jimin babo");


  if(TempC < maxTempC && TempC > minTempC){
    lcd.setCursor(10,0);
    //lcd.print("The temperature is in range.");
    lcd.print("  OK  "); // display "OK" when temperture is in range
  } else{
    lcd.setCursor(10,0);
    lcd.print("NOT OK"); // display "NOT OK" when temperture isn't in range
    lcd.setCursor(13,0);

    int size = sizeof(durations) / sizeof(int);
    digitalWrite(red_led, HIGH);  // turn the LED on (HIGH is the voltage level)

    for (int note = 0; note < size; note++) { //pitches and notes from https://github.com/hibit-dev/buzzer

      int duration = 1000 / durations[note];
      tone(BUZZER_PIN, melody[note], duration);


      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);
      
      //stop the tone playing:
      noTone(BUZZER_PIN);
    }
    
    digitalWrite(red_led, LOW);   // turn the LED off by making the voltage LOW

  }
  delay(500); // updates the temperture every 0.5s
}