#include "IRremote.h"
//#include <TimerOne.h>

const int ir_receiver_signalPin   = 3;

const int motor_controller_AIAPin = 8;
const int motor_controller_AIBPin = 7;
const int motor_controller_BIAPin = 2;
const int motor_controller_BIBPin = 4;
const int motor_controller_speed  = 255;

const int ultrasonic_sensor_trigPin = 2;
const int ultrasonic_sensor_echoPin = 12;

/*-----( Declare objects )-----*/
IRrecv irrecv(ir_receiver_signalPin);     // create instance of 'irrecv'
decode_results results;      			  // create instance of 'decode_results'

boolean debug = false;

/* RUNS ONCE */
void setup()
{
  if (debug) {
  	Serial.begin(9600);  
  	Serial.println("IR Receiver Button Decode"); 
  }
  
  initialize_motor_driver();
  
  //Timer1.initialize(TIMER_US);                        // Initialise timer 1
  //Timer1.attachInterrupt( timerIsr );
  
  attachInterrupt(digitalPinToInterrupt(ultrasonic_sensor_trigPin), checkDistance, CHANGE);
  
  irrecv.enableIRIn(); // Start the receiver
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
  analogWrite(motor_controller_AIAPin, motor_controller_speed);
  analogWrite(motor_controller_AIBPin, 0);
  analogWrite(motor_controller_BIAPin, 0);
  analogWrite(motor_controller_BIBPin, motor_controller_speed);
}

void go_right()
{
  analogWrite(motor_controller_AIAPin, 0);
  analogWrite(motor_controller_AIBPin, motor_controller_speed);
  analogWrite(motor_controller_BIAPin, motor_controller_speed);
  analogWrite(motor_controller_BIBPin, 0);
}

// --------------------------
// timerIsr() 50uS second interrupt ISR()
// Called every time the hardware timer 1 times out.
// --------------------------
void timerIsr()
{
    checkDistance();                                 // Schedule the trigger pulses
    //distance_flasher();                              // Flash the onboard LED distance indicator
}


void loop()
{
  long duration, inches, cm;
  
  duration = checkDistance();
  
  // convert the time into a distance
  // inches = microsecondsToInches(duration);
  cm     = microsecondsToCentimeters(duration);
  
  
  //Serial.println( results.value); 
  if (irrecv.decode(&results)) // have we received an IR signal?
  {

  //  Serial.println( results.value); 
    
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}

/*-----( Function )-----*/
void translateIR() // takes action based on IR code received
{

  switch(results.value)
  {
  case 0xFF629D: Serial.println(" FORWARD"); break;
  case 0xFF22DD: Serial.println(" LEFT");    break;
  case 0xFF02FD: Serial.println(" -OK-");    break;
  case 0xFFC23D: Serial.println(" RIGHT");   break;
  case 0xFFA857: Serial.println(" REVERSE"); break;
  case 0xFF6897: Serial.println(" 1");    break;
  case 0xFF9867: Serial.println(" 2");    break;
  case 0xFFB04F: Serial.println(" 3");    break;
  case 0xFF30CF: Serial.println(" 4");    break;
  case 0xFF18E7: Serial.println(" 5");    break;
  case 0xFF7A85: Serial.println(" 6");    break;
  case 0xFF10EF: Serial.println(" 7");    break;
  case 0xFF38C7: Serial.println(" 8");    break;
  case 0xFF5AA5: Serial.println(" 9");    break;
  case 0xFF42BD: Serial.println(" *");    break;
  case 0xFF4AB5: Serial.println(" 0");    break;
  case 0xFF52AD: Serial.println(" #");    break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;  

  default: 
    Serial.println(" other button   " );
    Serial.print( results.value, HEX );
    

  }// End Case

  delay(500); // Do not get immediate repeat


} //END translateIR


long checkDistance()
{
	 // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:


  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(ultrasonic_sensor_trigPin, OUTPUT);
  digitalWrite(ultrasonic_sensor_trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(ultrasonic_sensor_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_sensor_trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(ultrasonic_sensor_echoPin, INPUT);
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
