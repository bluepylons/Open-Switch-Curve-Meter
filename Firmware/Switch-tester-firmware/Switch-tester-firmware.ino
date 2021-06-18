#include <HX711.h>
#include <TMCStepper.h>
#include <TMCStepper_UTILITY.h>
#include <Adafruit_NeoPixel.h>

//RGBLED - there is a single RGBLED for status indication
#define RGB_pin 4
#define N_LEDs 1

Adafruit_NeoPixel LED = Adafruit_NeoPixel(N_LEDs, RGB_pin);

//Load cell + amplifier - using Avia HX711
#define Data_pin 3
#define SCK_pin 2
float Units_per_gram = 3179.013; //measured from calibration step
float Zero = 45.024; //measured no load weight, in gf

long Raw_reading;
float Reading_g;
HX711 Load_cell;
float Reverse_limit = 110.0; //This is the force reading at which the switch curve tester reverses, in  gf. You may wish to increase this for switches with very high bottom out weights, but be aware of what your load cell can physically handle! 

//stepper motor driver - using Trinamic TMC2209 using STEP/DIR control 
#define DIR_pin 9 //HIGH = away from load cell, LOW = toward
#define STEP_pin 8
#define MS1 5 //used to set microstepping 
#define MS2 6 //used to set microstepping
#define R_SENSE 0.11f

#define Steps_per_loop 8 //This is the number of steps per loop cycle
#define Microns_per_loop 5 //This is how much the leadscrew advances per loop cycle - this will vary depending on your stepper motor, microstepping, and leadscrew lead
//int Reverse_steps = 40000;

//#define Backlash 10 
long int Position; //in microns

//control buttons
#define Left_switch 11
#define Right_switch 12

//keep track of switch actuation
#define Switch_pin 10
bool Actuated;

//other
bool Reverse; //this becomes true when the force reading on the load cell reaches a defined value, and initiates the reverse cycle  (to avoid load cell damage)
bool Button_pressed; //this becomes true if the left button is pressed - used to initiate the test cycle
bool Returned_to_start; //becomes true at the end of the cycle 

void setup() {
  // put your setup code here, to run once:

  Reverse = false;
  Position = 0;
  
  Serial.begin(9600);
  //Serial.println("Test!");

  //Load-cell setup
  Load_cell.begin(Data_pin, SCK_pin);
 
  //RGBLED setup
  LED.begin();
  LED.setPixelColor(0, 128, 128, 128); //white
  LED.show();

  //set microstepping (see Section 3.4 in the TMC2209 datasheet - MS1 and MS2 both being low means microstepping set to 8);
  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);
  digitalWrite(MS1,LOW);
  digitalWrite(MS2,LOW);

  pinMode(STEP_pin, OUTPUT);
  pinMode(DIR_pin, OUTPUT);

  //switch actuation
  pinMode(Switch_pin, INPUT);

  //buttons
  pinMode(Left_switch, INPUT);
  pinMode(Right_switch, INPUT);

  Button_pressed = false;
  Returned_to_start=false;
  Actuated = false;
  
}

void Drive_stepper(int steps, bool dir) { //Direction - false = toward, //true = away
  if(dir) {
    digitalWrite(DIR_pin, HIGH);
  }

  else {
    digitalWrite(DIR_pin, LOW);
  }

  while(steps>0){
    digitalWrite(STEP_pin, HIGH);
    delayMicroseconds(120);
    digitalWrite(STEP_pin, LOW);
    delayMicroseconds(120);
    steps--;
  }
}
  
  
void loop() {
  while(Button_pressed) { 

    //read load cell
    if(Load_cell.is_ready()){
        Raw_reading = Load_cell.read();
        Reading_g = (((float(Raw_reading))/Units_per_gram))-Zero;
    }

    //check if load cell limit reached, initiates reverse otherwise
    if(Reading_g>Reverse_limit) {
      Reverse = true;
      LED.setPixelColor(0, 0, 255, 0);
      LED.show();
    }

    Actuated = !digitalRead(Switch_pin);


    //Advabce towards the load cell 
    if(Reverse==false) {
      //Advance towards the load cell 8 steps 
      Drive_stepper(Steps_per_loop,0);
      Serial.print(Position);
      Serial.print(", ");
      Serial.print(Reading_g);
      Serial.print(",");
      Serial.print(Actuated);
      Serial.print("\n");
      
      Position = Position + Microns_per_loop; //track position
    }
    
    //return to start position
    if(Reverse & (Position >0) ) {
        Drive_stepper(Steps_per_loop,1);
        Position = Position - Microns_per_loop;
        if(Position==0) { //End of cycle, exit the while loop 
            Button_pressed = false;
            Returned_to_start = true;
            LED.setPixelColor(0, 128, 128, 0);
            LED.show();
        }
          Serial.print(Position);
          Serial.print(", ");
          Serial.print(Reading_g);
          Serial.print(",");
          Serial.print(Actuated);
          Serial.print("\n");
          
    }



    delay(140); //HX711 can only read ~10Hz, so 140ms delay between loops 

  }

  if(!digitalRead(Left_switch)) { //press left button to start test cycle
    Button_pressed = true;
    LED.setPixelColor(0, 128, 0, 0); //green
    LED.show();
  }

//Used to move the carriage before and after teh test cycle
//before test cycle - press right button to move carriage closer to load cell. 
//After test cycle - press right button to move carriage away from load cell
  if(!digitalRead(Right_switch)) {

    //read load cell for overload protection
    if(Load_cell.is_ready()){
        Raw_reading = Load_cell.read();
        Reading_g = (((float(Raw_reading))/Units_per_gram))-Zero;        
    }

    if(Reading_g < 250) {
      if(Returned_to_start) {
        
        Drive_stepper(Steps_per_loop,1);
      }
      
      else {
        Drive_stepper(Steps_per_loop,0);
      }
    }
  }



//this does not really work right now - I should probably place it in the while loop
  /*
  if(Reverse==false){
    LED.setPixelColor(0, 128, 128, 128);
    LED.show();
  }

  if(Reverse){
     LED.setPixelColor(0, 0, 255, 0);
     LED.show();
  }
    
 
*/

/*
//manual controls
  if(!digitalRead(Left_switch)) {
     LED.setPixelColor(0, 0, 128, 0);
     LED.show();
     digitalWrite(DIR_pin, HIGH);
     digitalWrite(STEP_pin, HIGH);
     delayMicroseconds(50);
     digitalWrite(STEP_pin, LOW);
     delayMicroseconds(50);
     //delay(1);
  }
  
  else if(!digitalRead(Right_switch )) {
     LED.setPixelColor(0, 128, 0, 0);
     LED.show();
     digitalWrite(DIR_pin, LOW);
     digitalWrite(STEP_pin, HIGH);
     delayMicroseconds(50);
     digitalWrite(STEP_pin, LOW);
     delayMicroseconds(50);
     //delay(1);
  }
*/

  
/*
  for (int i = 1000; i>0; i--) {
    digitalWrite(DIR_pin, HIGH);
    delayMicroseconds(160);
    digitalWrite(DIR_pin, LOW);
    delayMicroseconds(160);
  }
 */


}
