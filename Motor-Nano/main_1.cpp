/*
Simple Routine for Nucleo Board for ROCO103PP Buggy Motor COntrol and Microswitches
Plymouth University
M.Simpson 31st October 2016
Editted 03/02/2017
*/
#include "mbed.h"
#include "motor.h"
#include "tunes.h"
#define TIME_PERIOD 2             //Constant compiler Values here 2 equates to 2ms or 500Hz base Frequency
#define DUTY 0.9                  //DUTY of 1.0=100%, 0.4=40% etc.,

DigitalIn microswitch1(D4);       //Instance of the DigitalIn class called 'microswitch1'
DigitalIn microswitch2(D3);       //Instance of the DigitalIn class called 'microswitch2'

Motor motor_A(D7,D8);             //Instances of the Motor Class see motor.h anf motor.cpp
Motor motor_B(D9,D10);            //They must be connected to these ports D7,D8 & D9,D10

DigitalIn myButton(USER_BUTTON);  //USER_BUTTON is the Blue Button on the NUCLEO Board

DigitalOut led(LED1);             //LED1 is the Green LED on the NUCLEO board
                                  //N.B. The RED LED is the POWER Indicator
                                  //and the Multicoloured LED indicates status of the ST-LINK Programming cycle

Serial pc(USBTX,USBRX);           //Instance of the Serial class to enable much faster BAUD rates then standard 9600 i.e. 115200
                                  //This is Pseudo RS232 over USB the NUCLEO will appear as a COMx Port see device Manager on PC used
                                  //Use PuTTY to monitor check COMx and BAUD rate (115200)

//The Following line is a Function Prototype
int motor(float speedA, float speedB);    //call as motor('Speed of MotorA Left','Speed of MotorB Right')
                                          //Where speed ranges from -1.0 to +1.0 inclusive rto give full reverse to full forward
                                          //And of course 0.0 will stop the Motor

//Variable 'duty' for programmer to use to vary speed as required set here to #define compiler constant see above
float duty=DUTY;
//
int main ()
{
  pc.baud(115200);            //BAUD Rate to 115200
  pc.printf("ROCO103PP Demonstration Robot Buggy Plymouth University 2016/17\n\r");

  motor_A.Period_in_ms(TIME_PERIOD);    //Set frequency of the PWMs
  motor_B.Period_in_ms(TIME_PERIOD);
  //
  //--------------------------- your strategy goes between the two dashed lines ---------------------------------------------	
  //
  motor(0.0f,0.0f);                     // Ensure Motors are stopped - For the curious, this function is defined at the end of this file.
	
  close_encounter(1);                     //tune to play Announce start!
                                          //see tunes.h for alternatives or make your own!

	while(myButton==1)
	{                                       //Wait here for USER Button (Blue) on Nucleo Board (goes to zero when pressed)
		led=0;                                //and flash green LED whilst waiting
		wait(0.1);
		led=1; 
		wait(0.1);
    if(microswitch1==1)
    {
        pc.printf("Switch1 = %4.2fV\n\r",(float)microswitch1*3.3f);//printing value of microswitch1 in PuTTy window on PC
                                                                  //NB this is a Digital Input and so returns a Boolean 1 or 0
                                                                  //and so 'cast' the result into a 'float' type and multiply by 3.3!
                                                                  // see the instruction doc on how to install putty.
        tone1();
    }
    //Test Microswitches with two different tones see tunes.cpp tunes.h
    if(microswitch2==1)
    {
        pc.printf("Switch 2 pressed\n\r");	//Another example of how to print a message telling about the program workings.
				tone2();
    }
  }
  
  while(true)			      //Repeat the following forever
	{
      motor(duty,duty);	    //Start Moving forward 
    
			while(microswitch1==0&&microswitch2==0){wait(0.05);} //short delay for debounce/noise

      motor(0,0);         //STOP Motors
      wait(0.1);          //Allow time for motors to stop
      
			if(microswitch1==1)   //Execute the following code if microswitch1 is activated
			{ 
				motor(0.0f,0.0f);   //Stop the Motors
				tone1();
				motor(-duty,-duty);
				wait(2.0f);
				motor(0,0);
				wait(1.0f);
				motor(-duty,duty);
				wait(2.0f);
				motor(0,0);
				wait(1.0f);
			}

			if(microswitch2==1)   //Execute the following code if microswitch2 is activated
			{
				motor(0.0f,0.0f);   //Stop the Motors
				tone2();
				motor(-duty,-duty);
				wait(2.0f);
				motor(0,0);
				wait(1.0f);
				motor(duty,-duty);
				wait(2.0f);
				motor(0,0);
				wait(1.0f);
			}
  }
}
//
//----------------------------------------------------------------------------------------------
//

//Small function to control motors use as motor(1.0,-0.5) Motor A full speed forward Motor B half speed reversed
int motor(float speedA, float speedB){
   if(speedA>1.0f||speedA<-1.0f){ //CHECK speedA Value is in Range!
      return -1;                  //return ERROR code -1=speedA Value out of range! EXIT Function
   }
   if(speedB>1.0f||speedA<-1.0f){ //CHECK speedB Value is in Range!
      return -2;                  //return ERROR code -2=speedB Value out of range! EXIT Function
   }
   //If speed values have passed the checks above then the following code will be executed
   if(speedA<0.0f){
     motor_A.Rev(-speedA);
   }
   else{
     motor_A.Fwd(speedA);
   }
   if(speedB<0.0f){
     motor_B.Rev(-speedB);
   }
   else{
     motor_B.Fwd(speedB);
   }
   return 0;                      //Return ERROR code Zero i.e. NO ERROR success!
}

/*      //Consider these lines of code to Accelerate the motors
//      for (float i=0.5f; i<=1.0f; i+=0.01f) //Accelerate  from 50% to 100%
//      { 
//        motor(i,i);
//        wait(0.1f);
//      }
*/
