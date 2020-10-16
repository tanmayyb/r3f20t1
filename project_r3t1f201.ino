/*******************************
Task 1 - Simple Control Program for 2 Bidirectional Motor Control
@version 1.0
@author Tanmay Bishnoi <tbishnoi@ryerson.ca>
*******************************/

/*
Defining motor pins for easy identification.
Motor 1 is mounted on the RIGHT with:
L293D OUT 1 connected @ Pin 6
L293D OUT 2 connected @ Pin 5

Motor 2 is mounted on the LEFT with:
L293D OUT 3 connected @ Pin 10
L293D OUT 4 connected @ Pin 11
*/
#define MOTOR1_1 6
#define MOTOR1_2 5
#define MOTOR2_3 10
#define MOTOR2_4 11

/*
We define Potentiometer pin address and Dip Switch Pin addresses.
The variable 'speed' stores the mapped potentiometer signal and,
the array 'switchStates' stores the mapped states of the Dip 
Switches.
*/
#define PotPin A5
int SwitchPins[4] = { A1, A2, A3, A4 };
int speed = 0; 
int switchStates[4] = {0, 0, 0, 0};

void setup()
{
  //Setup Motor 1 (R) Output Pins
  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  //Setup Motor 2 (L) Output Pins
  pinMode(MOTOR2_3, OUTPUT);
  pinMode(MOTOR2_4, OUTPUT);
  //Potentiometer as Input
  pinMode(PotPin, INPUT);
  for(int i =0;i<4;i++){
  	pinMode(SwitchPins[i], INPUT_PULLUP);
  }
  //Pour a bowl of Serial
  Serial.begin(9600);
}



void loop()
{
  speed = map(analogRead(PotPin),0,1023,0,255);
  
  for(int i=0;i<4;i++){
    switchStates[i] = map(digitalRead(SwitchPins[i]), 0, 1, 1, 0);
  }
  
  int mode = modeSelector(switchStates);
  
  switch (mode) {
  case 0:
    dontMove();
    break;
  case 1:
    moveForward(speed);
    break;
  case 2:
    moveBackward(speed);
    break;
  case 3:
    turnLeft(speed);
    break;
  case 4:
    turnRight(speed);
    break;
  case -1:
    Serial.println("OPERATOR ERROR: Multiple modes selected!");
    dontMove();
    break;
	}
/*
Just some Serial outputs for debugging/signal check.
*/  
  if(mode!=-1){
    for(int i =0;i<4;i++){
  	Serial.print(switchStates[i]);
    Serial.print("\t");
    }
    Serial.println(speed);
  }
}


/*
This function determines operational modes from switch states. 
Defined return signals and their operations:
-1		Error
 0		Neutral
 1		Forwards
 2		Backwards
 3		Turns Left
 4		Turns Right
*/
int modeSelector(int array[]){
  int mode=0;
  int sum = array[0]+array[1]+array[2]+array[3];
  if(sum>1){
    return -1;
  }
/*
If more than One switch is active 
THEN the sum of all Switch states > 1
This means the operator is trying to execute 2 modes 
simultaneously, which is outside the scope of our control 
definitions, therefore, an error code (-1) is returned.
*/
  else{
    for(int i =0;i<4;i++){
      if(array[i]==1){
      	mode = i+1;//'+1' because 0 is set to be neutral.
      }
    }
  }
  return mode;
}


/*
These functions change the potential difference on the motors 
according to the input speed parameter (num) and the lowest
possible signal 0.
*/
void turnLeft(int num){
	MOTOR1CONTROL(num, 0);
  	MOTOR2CONTROL(0, num);
}

void turnRight(int num){
	MOTOR1CONTROL(0, num);
	MOTOR2CONTROL(num, 0);
}

void moveForward(int num){
	MOTOR1CONTROL(num, 0);
    MOTOR2CONTROL(num, 0);
}

void moveBackward(int num){
  	MOTOR1CONTROL(0, num);
    MOTOR2CONTROL(0, num);
}

void dontMove(){
	MOTOR1CONTROL(0, 0);
    MOTOR2CONTROL(0, 0);
}


/*
These functions operate indidual motor pins and are called with
varying input parameters for executing set actions.
f - forward bias, b - backward bias
By convention, a high 'f' value and a low 'b' value makes a motor
turn forwards (+RPMs). Writing a function like this makes code 
more modular and beautiful.
*/

//MOTOR1 - R Side
void MOTOR1CONTROL(int f, int b){
  analogWrite(MOTOR1_1,f);
  analogWrite(MOTOR1_2,b);
}
//MOTOR2 - L Side
void MOTOR2CONTROL(int f, int b){
  analogWrite(MOTOR2_3,b);
  analogWrite(MOTOR2_4,f);
}
