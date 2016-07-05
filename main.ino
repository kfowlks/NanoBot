/**
 *  NanoBot - v1.0
 *  Author: Jasmine Fowlks
 *          Angela Fowlks
 *          Kevin Fowlks
 *
 *  This basic application is a starting place for using the 3D printed two-wheeled robot 
 *  found here ( http://www.thingiverse.com/thing:1582398 ). 
 * 
 *  The hardware used in this project is listed below:
 *  
 *  Arduino nano v3 (clone)
 *  Motor Controller (L9110S Dual DC motor Driver Controller Board H-bridge)
 *  Distance Measurment ( Ultrasonic Module HC-SR04 Distance Measuring )
 *  Smart Car Robot Plastic DC 3V-6V Drive Gear Motor 
 *  IR Remote ( Infrared Wireless Remote Control with Sensor Receiver Modules 3 pin variety )
 *  NTE 54‑140 SPST On‑Off Subminiature Toggle
 * 
 *  
 * 
 **/

#include "IRremote.h"

const int ir_receiver_signalPin   = 11;

const int motor_controller_AIAPin = 5;  // PWM Pin
const int motor_controller_AIBPin = 6;  // PWM Pin
const int motor_controller_BIAPin = 9;  // PWM Pin
const int motor_controller_BIBPin = 10; // PWM Pin

const int ultrasonic_sensor_trigPin = 2;
const int ultrasonic_sensor_echoPin = 12;

const int led_Pin = 3; // PWM Pin

const int motor_controller_speed = 200;

const int IR_UP       = 0x3D9AE3F7;
const int IR_DOWN     = 0x1BC0157B;
const int IR_LEFT     = 0x8C22657B;
const int IR_RIGHT    = 0x449E79F;

/*-----( Declare objects )-----*/
IRrecv irrecv(ir_receiver_signalPin);     // create instance of 'irrecv'
decode_results results;             // create instance of 'decode_results'

long duration, cm;
long last_cm;

const boolean debug = true;

void setup()
{
  pinMode(motor_controller_AIAPin, OUTPUT);
  pinMode(motor_controller_AIBPin, OUTPUT);
  pinMode(motor_controller_BIAPin, OUTPUT);
  pinMode(motor_controller_BIBPin, OUTPUT);

  pinMode(ultrasonic_sensor_trigPin, OUTPUT);
  pinMode(ultrasonic_sensor_echoPin, INPUT); 

  if (debug) {
    Serial.begin(9600);  
    Serial.println("NanoBot v1 Debug Console");
  }
  
  fadeLED( led_Pin );

  irrecv.enableIRIn(); // Start the receiver

  initialize_motor_driver();
}

void fadeLED( int ledPin ) {

 // fade in from min to max in increments of 5 points:
  for(int fadeValue = 0 ; fadeValue <= 255; fadeValue +=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(50);                            
  } 
 
  // fade out from max to min in increments of 5 points:
  for(int fadeValue = 255 ; fadeValue >= 0; fadeValue -=5) { 
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, fadeValue);         
    // wait for 30 milliseconds to see the dimming effect    
    delay(50);                            
  } 

}

void initialize_motor_driver()
{ 
  pinMode(motor_controller_AIAPin, OUTPUT); // set pins to output
  pinMode(motor_controller_AIBPin, OUTPUT);
  pinMode(motor_controller_BIAPin, OUTPUT);
  pinMode(motor_controller_BIBPin, OUTPUT);
}

void go_backward()
{
  analogWrite(motor_controller_AIAPin, 0);
  analogWrite(motor_controller_AIBPin, motor_controller_speed);
  analogWrite(motor_controller_BIAPin, 0);
  analogWrite(motor_controller_BIBPin, motor_controller_speed);
}

void go_forward()
{
  analogWrite(motor_controller_AIAPin, motor_controller_speed);
  analogWrite(motor_controller_AIBPin, 0);
  analogWrite(motor_controller_BIAPin, motor_controller_speed);
  analogWrite(motor_controller_BIBPin, 0);
}

void go_left()
{
  analogWrite(motor_controller_AIAPin, (motor_controller_speed *.75) );
  analogWrite(motor_controller_AIBPin, 0);
  analogWrite(motor_controller_BIAPin, 0);
  analogWrite(motor_controller_BIBPin, (motor_controller_speed *.75) );
}

void go_right()
{
  analogWrite(motor_controller_AIAPin, 0);
  analogWrite(motor_controller_AIBPin, (motor_controller_speed *.75) );
  analogWrite(motor_controller_BIAPin, (motor_controller_speed *.75) );
  analogWrite(motor_controller_BIBPin, 0);
}


void go_stop()
{
  analogWrite(motor_controller_AIAPin, 0);
  analogWrite(motor_controller_AIBPin, 0 );
  analogWrite(motor_controller_BIAPin, 0 );
  analogWrite(motor_controller_BIBPin, 0);
}

void loop()
{
 
  duration = checkDistance();
  
  // convert the time into a distance
  // inches = microsecondsToInches(duration);
  cm     = microsecondsToCentimeters(duration);

  if ( cm < 65 )
  {
     fadeLED( led_Pin );
  }
  
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    /* Output distance on button press */
    Serial.print( cm );
    Serial.print( " cm  ");
    Serial.print( last_cm );
    Serial.println( " last cm  ");
    
    move();
    
    irrecv.resume(); // receive the next value
  }

  last_cm = cm;
}


void move() 
{

  switch(results.value)
  {
  case 0x3D9AE3F7:
    if (debug) {
      Serial.print( results.value, HEX );
      Serial.println(" UP " );
    }
    go_forward();
    break;
  case 0x1BC0157B:
    if (debug) {
      Serial.print( results.value, HEX );
      Serial.println(" DOWN " );
    }
    go_backward();
    break;
  case 0x8C22657B:
    if (debug) {
      Serial.print( results.value, HEX );
      Serial.println(" LEFT " );
    }
    go_left();
    break;
  case 0x449E79F:
      if (debug) {
       Serial.print( results.value, HEX );
       Serial.println(" RIGHT " );
      }
      go_right();
      break;
  case 0x488F3CBB: // STOP
    if (debug) {
      Serial.print( results.value, HEX );
      Serial.println(" STOP " );
    }
    go_stop();  
    break;

  default:  
    if (debug) {      
      Serial.print( results.value, HEX );
      Serial.println(" other button   " );
    }

  } // End Case

  delay(500); // Do not get immediate repeat

}

long checkDistance()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  //pinMode(ultrasonic_sensor_trigPin, OUTPUT);
  digitalWrite(ultrasonic_sensor_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonic_sensor_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_sensor_trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
 // pinMode(ultrasonic_sensor_echoPin, INPUT);
  return pulseIn(ultrasonic_sensor_echoPin, HIGH);
  
  //Serial.print(inches);
  //Serial.print("in, ");
  //Serial.print(cm);
  //Serial.print("cm");
  //Serial.println();
  
  //delay(100);
  
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The motor_controller_speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
