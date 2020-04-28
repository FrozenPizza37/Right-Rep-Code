/*The Right Rep Biosensor
  This code collects data from an EMG BITalino and goniometer
  When the EMG and goniometer readings are above a threshold, a green LED turns on, a rep is counted and displayed in the LCD
  When the concluding for a set is done, a blue LED turns on
  At anytime, the button can be pressed to reset the rep count
  Date Created: April 28, 2020
  Created By: Sam Spark and Harrison Crosley
  */

#include <LiquidCrystal.h>  //Library for LCD display

int EMG = 0;   //EMG varible
int POT = 0;   //Goniometer varible
int REPcount = 0;  //Varible for number of reps
int Currenttime;   //varible for time when rep is completed 
int Previoustime = 0;  //Variable for time of previous rep
int dt = 0;       //Variable for time between two reps
int buttonState = 0;  //Variable state of button

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int THRESHOLD = 600;  //Threshold for goniometer reading
const int MINTIME = 500;    //Minimum value for dt
const int REPAMOUNT = 10;   //Number of reps per set
const int MUSCLES = 400;    //Threshold for EMG reading

void setup() {

  pinMode(8, OUTPUT);   //Set green LED as output
  pinMode(9, OUTPUT);   //Set blue LED as output
  pinMode(7, INPUT);    //Set Button as input
  lcd.begin(16, 2);     //start LCD display
  Serial.begin(9600);   //start serial monitor
}

void loop() {

  EMG = analogRead(A0);     //read EMG signal
  POT = analogRead(A1);     //read goniometer signal

  if (EMG > MUSCLES && POT > THRESHOLD) {   //both EMG and goniometer readings are greater than their respective thresholds
    digitalWrite(8, HIGH);                  //turn on green LED
    Currenttime = millis();                 //check time of rep
    dt = Currenttime - Previoustime;        //calculate time between current rep and previous rep

    if (dt > MINTIME) {                     //time between current rep and previous rep is greater than the minimum value for dt
      REPcount = REPcount + 1;              //increase rep count by 1
      updateRepCount();                     //update rep count on LCD display
    }
    Previoustime = Currenttime;             //set currennt time as the previous time for the next loop
  }
  else {                                    //EMG or goniometer readings are less than their respective thresholds
    digitalWrite(8, LOW);                   //turn off green LED
  }

  if (REPcount >= REPAMOUNT) {              //number of reps completed are equal to or greater than the number of reps per set
    digitalWrite(9, HIGH);                  //turn on blue LED
  }
 
  else {                                    //number of reps completed is less than the number of reps per set
    digitalWrite(9, LOW);                   //turn off blue LED
  }
  
  buttonState = digitalRead(7);             //check is button is pressed
  
  if (buttonState == HIGH) {                //button is pressed
    REPcount = 0;                           //set rep count to 0
    updateRepCount();                       //update rep count on LCD display
  }
  Serial.println(buttonState);              //this line is for testing varibles in the serial monitor

}

void updateRepCount() {                     //function to update LCD display
            
  lcd.clear();                              //clear LCD display
  lcd.setCursor(0, 0);                      //set cursor to top left of display
  lcd.print(REPcount);                      //print the rep count
  lcd.print(" Reps");                       
  lcd.setCursor(0, 1);                      //set cursor to bottom left of display
  lcd.print("Bicep Curls");                 //print words, "Bicep Curls"
  return;                                     
}
