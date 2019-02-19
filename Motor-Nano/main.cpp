/*
Simple Routine for Nucleo Board for ROCO103PP Singel Pixel Camera Colour Response
Plymouth University
M.Simpson 31st October 2016
*/
#include "mbed.h"
#include "motor.h"
#include "tunes.h"
#define TIME_PERIOD 2             //Constant compiler Values here 2 equates to 2ms or 500Hz base Frequency
#define DUTY 0.9                  //DUTY of 1.0=100%, 0.4=40% etc.,
#define RED   0
#define GREEN 1
#define BLUE  2

AnalogIn ldr(A1);                 //Instance of the AnalogIn class called 'ldr'

DigitalIn myButton(USER_BUTTON);  //USER_BUTTON is the Blue Button on the NUCLEO Board
DigitalOut led(LED1);             //LED1 is the Green LED on the NUCLEO board
                                  //N.B. The RED LED is the POWER Indicator

//and the Multicoloured LED indicates status of the ST-LINK Programming cycle
DigitalOut red(D13);			  //Three coloured LEDs for the single Pixel Camera
DigitalOut green(D12);
DigitalOut blue(D11);

DigitalIn microswitch1(D4);       //Instance of the DigitalIn class called 'microswitch1'
DigitalIn microswitch2(D3);       //Instance of the DigitalIn class called 'microswitch2'

Motor motor_A(D7,D8);             //Instances of the Motor Class see motor.h anf motor.cpp
Motor motor_B(D9,D10);            //They must be connected to these ports D7,D8 & D9,D10

int motor(float speedA, float speedB); 

#include "stm32f4xx.h"                  // Device header

           // Component selection
Serial pc(USBTX,USBRX);           //Instance of the Serial class to enable much faster BAUD rates then standard 9600 i.e. 115200
                                  //This is Pseudo RS232 over USB the NUCLEO will appear as a COMx Port see device Manager on PC used


DigitalOut Trigger(D6);     //Instance of the DigitalInOut class called 'TriggerEcho'
DigitalIn Echo(D2);
Timer pulse;                      //Instance of the Timer class called 'pulse' so we can measure timed events

void ultra_sonic_distance(void);

float GetDistance(void);          //Function Prototype see function for desciption

float redlight=0.0f; 
float bluelight=0.0f; 
float greenlight=0.0f;  
float Distance;                   //Assign global variables here
float distance=0.0f;              //distance is varible used by Ultrasnic code in mm
float duty = DUTY;

int main (){
    pc.baud(115200);    
		red=0;green=0;blue=0;		//ALL LEDs OFF!	
    pc.printf("ROCO103PP Demonstration Robot Buggy Plymouth University 2016/17\n\r");
  	pulse.start();                        //Start the instance of the class timer

    while(1){
					
		wait (0.5f);
			
		distance=GetDistance();
		


		if (distance < 35){
		
			motor (0.65, 0.73);
			
			wait (1.5f);
			 
			motor (0,0);
			
			red=1;green=0;blue=0;	//ILLUMINATE WITH RED ONLY!
			redlight=ldr*3.3f/0.95;      	//Read the Analogue input MBED Normalised 0.0 to 1.0f Multiply by 3.3f to Calibrate to a Voltage read

			wait(0.5f);
		
			red=0;green=1;blue=0;	//ILLUMINATE WITH GREEN ONLY!
			bluelight=ldr*3.3f/1.07;        	//Read the Analogue input MBED Normalised 0.0 to 1.0f Multiply by 3.3f to Calibrate to a Voltage read

			wait(0.5f);
		
			red=0;green=0;blue=1;	//ILLUMINATE WITH BLUE ONLY!
			greenlight=ldr*3.3f/1.26;        	//Read the Analogue input MBED Normalised 0.0 to 1.0f Multiply by 3.3f to Calibrate to a Voltage read
			
			wait(0.5f);
						
		
			if (bluelight > redlight && redlight < greenlight){
				
				red=0;green=0;		//ALL LEDs OFF!	
				
				for (int i = 0;i<5;i++){ //flash BLUE 5 times
					
					blue=1;wait(0.1);
					blue=0;wait(0.1);
				}
				blue = 0;

			}
			
			
			else if (greenlight > bluelight && bluelight < redlight){
				
				green=0;blue=0;		//ALL LEDs OFF!	
				
				for (int i = 0;i<5;i++){ //flash BLUE 5 times
					
					red=1;wait(0.1);
					red=0;wait(0.1);
				}
				red = 0;
			}
			
			
			motor (1,-1.8);
			 
			wait (2.0f);
			 
			motor (duty,0.98);
			 
			wait (6.0f);
			
			motor (0,0);
			
		}		
		 
		else if (distance < 150){
			
			motor(0.70, 0.78);

			wait (0.50f);
			
			motor (0,0);			
			
		}		
		
		else if (distance < 300){
			
			motor(0.80, 0.88);

			wait (2.0f);
			
			motor (0,0);			
			
		}

		else if (distance < 480){		
			
			motor(duty, 0.98);		

			wait (3.0f);
			
			motor (0,0);			

		}		
		
		else if (distance > 480){		
			
			motor (1, -1.8);
			
			wait (0.45f);
			
			motor (0,0);		

		}			
		
			
		else {
			
			motor (1, -0.8);
			
			wait (0.45f);
			
			motor (0,0);
			
						
		}
	}
}

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



// Function ultra_sonic_distance() will load the global variable distance with Ultra Sonic Sensor value in mm
// and then send the value to the stdio ouput i.e serial over USB
void ultra_sonic_distance(void){
   distance=GetDistance();
   pc.printf("%dmm \n\r",(int)distance);
    
}
// Function GetDistance() will return a float value of the Distance from the Ultrasonic Sensor in millimeters
float GetDistance(){                                  //Function Name to be called
  int EchoPulseWidth=0,EchoStart=0,EchoEnd=0;         //Assign and set to zero the local variables for this function
  Trigger = 1;                                        //Signal goes High i.e. 3V3
  wait_us(100);                                       //Wait 100us to give a pulse width Triggering the Ultrasonic Module
  Trigger = 0;                                        //Signal goes Low i.e. 0V
  pulse.reset();                                      //Reset the instance of the Timer Class
  while(Echo == 0 && EchoStart < 25000){              //wait for Echo to go high
    EchoStart=pulse.read_us();                        //Conditional 'AND' with timeout to prevent blocking!      
    }
    while(Echo == 1 && ((EchoEnd - EchoStart) < 25000)){ //wait for echo to return low
      EchoEnd=pulse.read_us();                        //Conditional 'OR' with timeout to prevent blocking!   
    }
    EchoPulseWidth = EchoEnd - EchoStart;             //time period in us
    return (float)EchoPulseWidth/5.8f;                //calculate distance in mm and reurn the value as a float
}