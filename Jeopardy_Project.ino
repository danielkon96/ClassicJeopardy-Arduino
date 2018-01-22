/*  
 *  Written By: Daniel Kondrashevich
 *  Last Edit: December 28th, 2017.
 *  Title: A 4 Player Jeopardy System.
 *  
 *  Note: The blinking system was created with the help
 *  of Arduino's "Blink without Delay" example.
 */

// 0 is the output LED that shows the system is good to go.
const int sysHealthPin = 2;

// 3 - 6 is output LEDs. The Lights.
const int output1Pin = 3;
const int output2Pin = 4;
const int output3Pin = 5;
const int output4Pin = 6;

// 7 - 10 is input pins.
const int input1Pin = 7;
const int input2Pin = 8;
const int input3Pin = 9;
const int input4Pin = 10;

// input Reset button.
const int resetPin = 11;

// Input and output statuses.
int in1State = 0;
int in2State = 0;
int in3State = 0;
int in4State = 0;
int resetState = 0;

//This is a unique sort of queue. 
//It is a FIFO array with a counter.
int buttonOrder[4] = {0,0,0,0};
int inCount = 0;

//If a button is pressed, begin animations :)
boolean pressedButton = false;

//This will check if a certain button has been pressed already.
//This will make sure the program doesn't read a users input twice
//in one round.
boolean button1Pressed = false;
boolean button2Pressed = false;
boolean button3Pressed = false;
boolean button4Pressed = false;

//This will hold the previousMillis that is used in blinking.
//We update this only if we create a blink.
unsigned long previousMillis_Slow = 0;
unsigned long previousMillis_Fast = 0;


void setup() {

  //Initialize system health LED pin.
  pinMode(sysHealthPin, OUTPUT);

  //Initialize output LEDs 1-4.
  pinMode(output1Pin, OUTPUT);
  pinMode(output2Pin, OUTPUT);
  pinMode(output3Pin, OUTPUT);
  pinMode(output4Pin, OUTPUT);

  //Initialize input LEDs 1-4.
  pinMode(input1Pin, INPUT);
  pinMode(input2Pin, INPUT);
  pinMode(input3Pin, INPUT);
  pinMode(input4Pin, INPUT);

  pinMode(resetPin, INPUT);
  digitalWrite(sysHealthPin, HIGH);

  //2 second sequence with 2 different orders.
  //Lighting order: 1, 2, 3, 4
  digitalWrite(output1Pin, HIGH);
  delay(200);
  digitalWrite(output2Pin, HIGH);
  digitalWrite(output1Pin, LOW);
  delay(200);
  digitalWrite(output3Pin, HIGH);
  digitalWrite(output2Pin, LOW);
  delay(200);
  digitalWrite(output4Pin, HIGH);
  digitalWrite(output3Pin, LOW);
  delay(200);
  digitalWrite(output4Pin, LOW);

  //Delay between next sequence.
  delay(500);

  //Lighting order: 1, 4, 2, 3
  digitalWrite(output1Pin, HIGH);
  delay(200);
  digitalWrite(output4Pin, HIGH);
  digitalWrite(output1Pin, LOW);
  delay(200);
  digitalWrite(output2Pin, HIGH);
  digitalWrite(output4Pin, LOW);
  delay(200);
  digitalWrite(output3Pin, HIGH);
  digitalWrite(output2Pin, LOW);
  delay(200);
  digitalWrite(output3Pin, LOW);
}

void loop() {
  //Setup for the state of the input.
  in1State = digitalRead(input1Pin);
  in2State = digitalRead(input2Pin);
  in3State = digitalRead(input3Pin);
  in4State = digitalRead(input4Pin);
  resetState = digitalRead(resetPin);

  //If the resetButton was pressed, reset the system.
  if (resetState == HIGH) {
    resetSystem();


  } else {

    //If we haven't gotten all input from players, keep reading inputs.
    if (inCount < 3) {
      
      //Check the buttons and set the correct order the LEDs are pressed.
      if (in1State == HIGH && button1Pressed == false) {
        buttonOrder[inCount] = output1Pin;
        inCount++;
        pressedButton = true;
        button1Pressed = true;
      } else if (in2State == HIGH && button2Pressed == false) {
        buttonOrder[inCount] = output2Pin;
        inCount++;
        pressedButton = true;
        button2Pressed = true;
      } else if (in3State == HIGH && button3Pressed == false) {
        buttonOrder[inCount] = output3Pin;
        inCount++;
        pressedButton = true;
        button3Pressed = true;
      } else if (in4State == HIGH && button4Pressed == false) {
        buttonOrder[inCount] = output4Pin;
        inCount++;
        pressedButton = true;
        button4Pressed = true;
      }
    }

    //If a button has been pressed, begin the animations.
    if (pressedButton) {

      //Get the current time in Millis used for blinking.
      unsigned long currentMillis = millis();

      //If button[#] is not 0, then a button has already been pressed.
      //Call a function that will begin blinking for player 2 & 3.
      if (buttonOrder[0] != 0) {
        digitalWrite(buttonOrder[0], HIGH);
      }
      if (buttonOrder[1] != 0) {
        blinkSlow(buttonOrder[1], currentMillis);
      } 
      if (buttonOrder[2] != 0) {
        blinkFast(buttonOrder[2], currentMillis);
      } 
      if (buttonOrder[3] != 0) {
        digitalWrite(buttonOrder[3], LOW);
      }
    
    }
  }
}


void blinkSlow(int outputLED, unsigned long currentMillis) {
  const int blinkSpeedMS = 1000;

  //If the amount of time passed after the last blink is higher than
  //our blinkSpeedMS, it's time to blink again.
  if (currentMillis - previousMillis_Slow >= blinkSpeedMS) {
    
    //We have to set two separate previousMillis so that we
    //can get a blinking effect. If we set this outside of the
    //if statement, it never enters into here.
    previousMillis_Slow = currentMillis;
    
    //If the previous state is off, turn it on. This gives it the blinking effect.
    if (digitalRead(outputLED) == LOW) {
      digitalWrite(outputLED, HIGH); 
    } else {
      digitalWrite(outputLED, LOW); 
    }
  }
}

void blinkFast(int outputLED, unsigned long currentMillis) {
  const int blinkSpeedMS = 100;

  //If the amount of time passed after the last blink is higher than
  //our blinkSpeedMS, it's time to blink again.
  if (currentMillis - previousMillis_Fast >= blinkSpeedMS) {
    
    //We have to set two separate previousMillis so that we
    //can get a blinking effect. If we set this outside of the
    //if statement, it never enters into here.
    previousMillis_Fast = currentMillis;
    
    //If the previous state is off, turn it on. This gives it the blinking effect.
    if (digitalRead(outputLED) == LOW) {
      digitalWrite(outputLED, HIGH); 
    } else {
      digitalWrite(outputLED, LOW); 
    }
  }
}


void resetSystem() {
  //Turn off all the LEDs
  digitalWrite(output1Pin, LOW);
  digitalWrite(output2Pin, LOW);
  digitalWrite(output3Pin, LOW);
  digitalWrite(output4Pin, LOW);
  
  pressedButton = false;
  button1Pressed = false;
  button2Pressed = false;
  button3Pressed = false;
  button4Pressed = false;

  //Reset the queue.
  for(int i = 0; i < 4; i++) {
    buttonOrder[i] = 0;
  }
  inCount = 0;
}
