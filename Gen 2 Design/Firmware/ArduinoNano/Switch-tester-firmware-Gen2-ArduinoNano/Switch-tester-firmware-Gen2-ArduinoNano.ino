#include <HX711.h>
#include <Adafruit_NeoPixel.h>

/*
   Load cell calibration parameters - these need to be calibrated frequently
   These parameters convert the raw ADC readings from the HX711 chip into a gram-force reading
   The load cell amplifier output is linear, and follow the form y = mx+b, with y being the force reading in gram-force, m being 1/Units_per_gram (so grams-per-unit),  x being the raw ADC reading, and b being the the variable Zero_intercept
   These are found by doing a 2-point calibration - one with no load on the load cell, and the other with a known load on the load cell
*/
float Units_per_gram = 3093.16;
float Zero_intercept  = -28.268;

/*This is the force reading at which the switch curve tester reverses, in gf. You may wish to increase this for switches with very high bottom out weights, but be aware of what your load cell can physically handle! The default TAL221 load cell used is only good for 500gf (with an absolute maximum of 750gf - the load cell will be permanently damaged if you exceed this)*/
float Reverse_limit = 110.0; //in gram-force

/*This is how long to wait between successive measurements, in ms. 15ms delay for 80Hz mode on the HX711, 110ms delay for 10hz mode on the HX711  */
#define Delay 15 //

/*RGBLED parameters - there is a single RGBLED for status indication*/
#define RGB_pin 4
#define N_LEDs 1
Adafruit_NeoPixel LED = Adafruit_NeoPixel(N_LEDs, RGB_pin);

/*Load cell amplifier (Avia HX711) pins */
#define Data_pin 3
#define SCK_pin 2

/*Stepper motor and stepper motor driver parameters - we are using a Trinamic TMC2209 driver with STEP/DIR control */
#define DIR_pin 9 //HIGH = away from load cell, LOW = toward
#define STEP_pin 8
#define MS1 5 //used to set microstepping 
#define MS2 6 //used to set microstepping
#define MS3 7 //used to set microstepping
#define R_SENSE 0.11f
#define Steps_per_loop 8 //This is the number of steps to advance per measurement loop cycle
#define Microns_per_loop 5 //This is how much the leadscrew advances per measurement loop cycle - this will vary depending on your stepper motor, microstepping, and leadscrew lead
#define Step_time 120 //How long to wait, in us, after the STEP signal goes high to go back low, and after it goes low to go back up high. Basically the length of the pulse in us 

//Pin assignments for the 3 control buttons
#define Up_switch 12
#define Down_switch 11
#define Start_switch A0

//Pin that is used to see if the switch is actuated
#define Switch_pin 10

/*Declaration of various variables */
long Raw_reading; //raw ADC reading
float Reading_gf; //ADC reading converted to gf
HX711 Load_cell; //Load cell object, using the HX711 library
long int Position; //in microns
bool Actuated; //whether the switch is actuated or not. True = actuated, False = not actuated
bool Reverse; //this becomes true when the force reading on the load cell reaches a defined value, and initiates the reverse cycle (to measure the upstroke and avoid load cell damage)
bool Start_button_pressed; //this becomes true if the Start button is pressed - used to initiate the test cycle
bool Returned_to_start; //becomes true at the end of the cycle

void setup() {
  // put your setup code here, to run once:

  //set various variables
  Reverse = false;
  Position = 0;
  Start_button_pressed = false;
  Returned_to_start = false;
  Actuated = false;

  Serial.begin(9600);

  //Load-cell setup
  Load_cell.begin(Data_pin, SCK_pin);

  //RGBLED setup
  LED.begin();
  LED.setPixelColor(0, 128, 128, 128); //white for the Setup state
  LED.show();

  //set microstepping (see Section 3.4 in the TMC2208/2209 datasheet - MS1 and MS2 both being low means microstepping set to 8). May need to change if using other stepper motor drivers;
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);

  pinMode(STEP_pin, OUTPUT);
  pinMode(DIR_pin, OUTPUT);

  //set switch actuation pin
  pinMode(Switch_pin, INPUT_PULLUP);

  //set buttons
  pinMode(Up_switch, INPUT_PULLUP);
  pinMode(Start_switch, INPUT_PULLUP);
  pinMode(Down_switch, INPUT_PULLUP);

}
//Function for sending stepper commands
void Drive_stepper(int steps, bool dir) { //Direction - false = toward load cell, //true = away from load cell
  if (dir) {
    digitalWrite(DIR_pin, LOW);
  }

  else {
    digitalWrite(DIR_pin, HIGH);
  }

  while (steps > 0) {
    digitalWrite(STEP_pin, HIGH);
    delayMicroseconds(Step_time);
    digitalWrite(STEP_pin, LOW);
    delayMicroseconds(Step_time);
    steps--;
  }
}

void loop() {

  while (Start_button_pressed) { //If the start button is pressed, then the measurement begins. This is the measurement cycle. 
    
    // Step 1) Read the load cell 
    if (Load_cell.is_ready()) { //If the load cell is ready - read the load cell 
      Raw_reading = Load_cell.read();
      Reading_gf = (((float(Raw_reading)) / Units_per_gram)) - Zero_intercept ; //convert to gf
      while (Reading_gf < -30.0) { //This is to throw out HX711 errors, which frequently manifest as extremely negative readings 
        delayMicroseconds(Delay * 5); //wait a bit so the error (hopefully) clears
        Raw_reading = Load_cell.read();
        Reading_gf = (((float(Raw_reading)) / Units_per_gram)) - Zero_intercept ;
      }
    }

    else { //if the load cell is not ready - wait a bit
      delay(Delay * 5); //wait a bit so the error clears
      while (Reading_gf < -30.0) { //This is to throw out HX711 errors, which frequently manifest as extremely negative readings 
        delayMicroseconds(Delay * 5);  //wait a bit so the HX711 error (hopefully) clears
        Raw_reading = Load_cell.read();
        Reading_gf = (((float(Raw_reading)) / Units_per_gram)) - Zero_intercept ;
      }
    }

    // Step 2) check if load cell reversed limit (as set by the variable Reverse_limit) is reached. If it has been, initiate the reverse cycle 
    if (Reading_gf > Reverse_limit) {
      Reverse = true;
      LED.setPixelColor(0, 128, 128, 0); //change indicator LED color for the reverse cycle
      LED.show();
    }

    // Step 3) Check if the switch is actuated 
    Actuated = !digitalRead(Switch_pin); 

    // Step 4a) - If Reverse_limit has not been reached, move switch towards the load cell: 
    if (Reverse == false) {
      //Steo 4a1) Advance towards the load cell (this is 8 microsteps, or one whole step)
      Drive_stepper(Steps_per_loop, 0);

      //Step 4a2) - Report the logged position, measured force, and switch actuation status over Serial Monitor 
      Serial.print(Position);
      Serial.print(", ");
      Serial.print(Reading_gf);
      Serial.print(",");
      Serial.print(Actuated);
      Serial.print("\n");

      //Step 4a3) - Keep track of position by updating it 
      Position = Position + Microns_per_loop; 
    }

    // Step 4b) - If Reverse_limit has been reached, move switch away from the load cell back to the start position 
    if (Reverse & (Position > 0) ) {
      //Step 4b1) - Advance away from the load cell (this is 8 microsteps, or one whole step)
      Drive_stepper(Steps_per_loop, 1); 
      Position = Position - Microns_per_loop;

      //Steo 4b2 - check to see if we've returned to the start position - if so, exit the while loop. Our measurement is done. 
      if (Position == 0) { 
        Start_button_pressed = false; //Exit the measurement 
        Returned_to_start = true;
        LED.setPixelColor(0, 128, 256, 0); //Orange - once the measurement is done, the LED is yellow. Press the reset button on the Arduino if you want to start a new measurement. 
        LED.show();
      }

     //Step 4b3 - Report the logged position, measured force, and switch actuation status over Serial Monitor 
      Serial.print(Position);
      Serial.print(", ");
      Serial.print(Reading_gf);
      Serial.print(",");
      Serial.print(Actuated);
      Serial.print("\n");

    }

    delay(Delay); //wait between measurement loop cycles. This is determined by the HX711 load cell amplifier speed (it's set to either 10Hz or 80Hz) 

  }

  if (!digitalRead(Start_switch)) { //press Start button to initiate the measurement
    Serial.print("Raw X (microns),Raw Force (gf),Actuated?"); //Print this initially on the first line of the Serial Monitor - this is needed to label t he data for the processing Python script 
    Serial.print("\n");
    Start_button_pressed = true;
    LED.setPixelColor(0, 128, 0, 0); //green - LED is green over the course of the measurement
    LED.show();
  }

  /*This is used to move the carriage before and after the measurement 
   * */
  if (!digitalRead(Up_switch)) {

    //Read load cell - for overload protection to avoid running the switch into the load cell and damaging it 
    if (Load_cell.is_ready()) {
      Raw_reading = Load_cell.read();
      Reading_gf = (((float(Raw_reading)) / Units_per_gram)) - Zero_intercept ;
    }

    if (Reading_gf < Reverse_limit) { //if the load cell reading does not exceeds the Reverse_limit - move towards the load cell 
        Drive_stepper(Steps_per_loop, 0);
      }


    }

  if (!digitalRead(Down_switch)) {
    Drive_stepper(Steps_per_loop, 1);
  }

}
