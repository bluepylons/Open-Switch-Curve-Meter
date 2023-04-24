//#include <TMCStepper.h>
//#include <TMCStepper_UTILITY.h>
#include <Adafruit_NeoPixel.h>
#include <HX711.h>

/*
   Load cell calibration parameters - these need to be calibrated frequently
   These parametersr convert the raw ADC readings from the HX711 chip into a gram-force reading
   The load cell amplifier output is linear, and follow the form y = mx+b, with y being the force reading in gram-force, m being 1/Units_per_gram (so grams-per-unit),  x being the raw ADC reading, and b being the the variable Zero_intercept
   These are found by doing a 2-point calibration - one with no load on the load cell, and the other with a known load on the load cell
*/
float Units_per_gram = 3172.421;
float Zero_intercept  = 45.701;

/*This is the force reading at which the switch curve tester reverses, in gf. You may wish to increase this for switches with very high bottom out weights, but be aware of what your load cell can physically handle! The default TAL221 load cell used is only good for 500gf (with an absolute maximum of 750gf - the load cell will be permanently damaged if you exceed this)*/
float Reverse_limit = 110.0; //in gram-force

/* Stepper Motor driver - Uncomment the stepper motor driver you're using and comment out the others */
#define Stepper_driver "TMC2208" 
//#define Stepper_driver "TMC2209" 
//#define Stepper_driver "A4988" 

/*Stepper motor and stepper motor driver parameters - we are using a Trinamic TMC2208 driver with STEP/DIR control */
#define DIR_pin 25 //HIGH = away from load cell, LOW = toward
#define STEP_pin 24
#define MS1 8 //used to set microstepping 
#define MS2 9 //used to set microstepping 
#define MS3 10 //used to set microstepping 
#define R_SENSE 0.11f
#define Microns_per_loop 5 //This is how much the leadscrew advances per measurement loop cycle - this will vary depending on your stepper motor, microstepping, and leadscrew lead
#define Step_time 120 //How long to wait, in microseconds, after the STEP signal goes high to go back low, and after it goes low to go back up high. Basically the length of the pulse in us 
int Steps_per_loop; 

/*RGBLED parameters - there is a single RGBLED for status indication*/
#define RGB_pin 16
#define N_LEDs 1
Adafruit_NeoPixel LED = Adafruit_NeoPixel(N_LEDs, RGB_pin);

/*Load cell amplifier (Avia HX711) pins */
#define Data_pin 20
#define SCK_pin 19

//Pin assignments for the 4 control buttons
#define Up_switch 13
#define Start_switch 8
#define Down_switch 11

//Pin that is used to see if the switch is actuated
#define Switch_pin 29

/*Declaration of various variables */
long Raw_reading; //raw ADC reading
float Reading_g; //ADC reading converted to gf
HX711 Load_cell; //Load cell object, using the HX711 library
long int Position; //in microns
bool Actuated; //whether the switch is actuated or not. True = actuated, False = not actuated
bool Reverse; //this becomes true when the force reading on the load cell reaches a defined value, and initiates the reverse cycle  (to avoid load cell damage)
bool Button_pressed; //this becomes true if the left button is pressed - used to initiate the test cycle
bool Returned_to_start; //becomes true at the end of the cycle


void setup() {
  //set various variables
  Reverse = false;
  Position = 0;
  Button_pressed = false;
  Returned_to_start = false;
  Actuated = false;

  Serial.begin(9600);

  //Load-cell setup
  Load_cell.begin(Data_pin, SCK_pin);

  //RGBLED setup
  LED.begin();
  LED.setPixelColor(0, 128, 128, 128); //white
  LED.show();

  //Switches setup
  pinMode(Up_switch,INPUT_PULLUP);
  pinMode(Start_switch,INPUT_PULLUP);
  pinMode(Down_switch,INPUT_PULLUP);  

  //Switch_pin setup
  pinMode(Switch_pin,INPUT_PULLUP);
  
  //OUtput pin setup
  pinMode(DIR_pin,OUTPUT);
  pinMode(STEP_pin,OUTPUT);
  
  //set microstepping (see Section 3.4 in the TMC2209 datasheet - MS1 and MS2 both being low means microstepping set to 8);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);

  //set microstepping depending on the driver
  if(Stepper_driver=="A4988") {
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, LOW);
    digitalWrite(MS3, LOW);
    Steps_per_loop = 1; 
    }

  else if(Stepper_driver=="TMC2208") {
    digitalWrite(MS1, HIGH);
    digitalWrite(MS2, LOW);
    digitalWrite(MS3, LOW);
    Steps_per_loop = 2; 
  }

  else if(Stepper_driver=="TMC2209") {
    digitalWrite(MS1, LOW);
    digitalWrite(MS2, LOW);
    digitalWrite(MS3, LOW);
    Steps_per_loop = 8; 
  }
} 

void Drive_stepper(int steps, bool dir) { //Direction - false = toward load cell, //true = away from load cell
  if (dir) {
    digitalWrite(DIR_pin, HIGH);
  }

  else {
    digitalWrite(DIR_pin, LOW);
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

if(!digitalRead(Up_switch)){
     Drive_stepper(Steps_per_loop, 1);
     LED.setPixelColor(0,0,0,128);
     LED.show();
}

else if(!digitalRead(Down_switch)) {
    Drive_stepper(Steps_per_loop, 0);
    LED.setPixelColor(0,0,128,0);
    LED.show();
}

else {
    LED.setPixelColor(0,128,128,0);
    LED.show();
}
}
