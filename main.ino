#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
int ADCV=A0; // Analog input connected to TMP36
#include "pitches.h"

const float minTempC = 10.0;
const float maxTempC = 25.0;

const int BUZZER_PIN = 5;
const int red_led = 3;

int melody[] = {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4
};

int durations[] = {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2
};


void setup()
{
  lcd.init(); // initialize the lcd
  pinMode(red_led, OUTPUT);
  pinMode(BUZZER_PIN,OUTPUT);

}
void loop()
{
  int VoltageRead=analogRead(ADCV); //Read ADC value (0-1023)
  float Vo=(VoltageRead/1023.0)*5; //Convert ADC value to voltage
  float TempC=(Vo-0.5)*100 - 1; //Convert TMP36 voltage to temp C
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("T:"); // Print characters to LCD.
  //lcd.setCursor(1,1);
  lcd.print(TempC); // Print variable value to LCD
  lcd.print(char(223));
  lcd.print("C");
  //lcd.setCursor(10,0)

  //lcd.setCursor(1,1);
  //lcd.print("jimin babo");


  if(TempC < maxTempC && TempC > minTempC){
    lcd.setCursor(10,0);
    //lcd.print("The temperature is in range.");
    lcd.print("  OK  ");
  } else{
    lcd.setCursor(10,0);
    lcd.print("NOT OK");
    lcd.setCursor(13,0);

    int size = sizeof(durations) / sizeof(int);
    digitalWrite(red_led, HIGH);  // turn the LED on (HIGH is the voltage level)

    for (int note = 0; note < size; note++) { //https://github.com/hibit-dev/buzzer
      //to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int duration = 1000 / durations[note];
      tone(BUZZER_PIN, melody[note], duration);

      //to distinguish the notes, set a minimum time between them.
      //the note's duration + 30% seems to work well:
      int pauseBetweenNotes = duration * 1.30;
      delay(pauseBetweenNotes);
      
      //stop the tone playing:
      noTone(BUZZER_PIN);
    }
    
    digitalWrite(red_led, LOW);   // turn the LED off by making the voltage LOW

  }
  delay(500);
}