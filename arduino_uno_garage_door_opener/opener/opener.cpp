                                                                       
#include "Arduino.h"
#include "avr/wdt.h"

#define WALL_SW  2  // defined I/O pins
#define RADIO_SW 3
#define LIGHT_SW 4
#define LIMIT_UP 5
#define LIMIT_DN 6

#define LIGHT_ON 8
#define MOTOR_UP 9
#define MOTOR_DN 10

// NOTICE : the function millis() will temp. disable interrupts
#define DEBOUNCE 100 // 50 //=0.1 sec. delay for debounce in ms.
#define MAX_RUN_TIME 13000 // 6500 //= 13 sec. Max. motor run time
#define MAX_LIGHT_TIME 30000 // 15000 //= 30 sec. Max light on time

int light_state=LOW; //LOW=off, HIGH=on
long light_time; // when light turned on, ms.
void light_sw_set(void);

unsigned int wall_sw_set(void);
unsigned int radio_sw_set(void);

unsigned int door_dir=1; //up = 1, down = 0
unsigned int door_run=0; //stopped=0, running = 1
long run_time; // when door started running, ms.
void toggle_door(void);

int get_limit_up(void);
int get_limit_dn(void);

int main(void)
{
	init();
	pinMode(13, INPUT);

	pinMode(LIGHT_SW, INPUT);
	pinMode(RADIO_SW, INPUT);
	pinMode(WALL_SW, INPUT);
	pinMode(LIMIT_UP, INPUT);
	pinMode(LIMIT_DN, INPUT);

	pinMode(LIGHT_ON, OUTPUT);
	pinMode(MOTOR_UP, OUTPUT);
	pinMode(MOTOR_DN, OUTPUT);

	//enable watchdog with 2sec reset
	wdt_enable(WDTO_2S);
	while(1)
	{	wdt_reset();//reset watchdog before 2sec.

		// check manual light switch
		light_sw_set();

		// if wall switch is pressed 
		if(wall_sw_set())
			toggle_door();

		//else is remote pressed ?
		else if(radio_sw_set())
			toggle_door();

		//if limit switch hit. Always poll limit switches
		if(get_limit_up() &&  door_dir && door_run) toggle_door();
		if(get_limit_dn() && !door_dir && door_run) toggle_door();

		// timeout if door runs too long
		if (door_run
		  && (millis() - run_time > MAX_RUN_TIME))
			toggle_door();

		// timeout if light is on too long
		if (light_state==HIGH
		  && (millis() - light_time > MAX_LIGHT_TIME))
			digitalWrite(LIGHT_ON, (light_state=LOW) );
	}
	return 0;
}





void toggle_door(void)
{
	//if door is running then stop
	if(door_run)
	{	digitalWrite(MOTOR_UP, LOW);
		digitalWrite(MOTOR_DN, LOW);
		door_run=0;
	}
	else //door is NOT running
	{	//if door is stoped on up then run down
		if(door_dir)
		{	digitalWrite(MOTOR_UP, LOW);
			digitalWrite(MOTOR_DN, HIGH);
			door_dir=0;
		}
		//else door is stoped on down, run up
		else
		{	digitalWrite(MOTOR_UP, HIGH);
			digitalWrite(MOTOR_DN, LOW);
			door_dir=1;
		}
		//turn on light
		light_time=millis();
		digitalWrite(LIGHT_ON, (light_state=HIGH) );
		//set door running start time
		run_time=millis();
		door_run=1;
	}
}




unsigned int radio_sw_set(void)
{	static unsigned int last_state=0;

	if(!digitalRead(RADIO_SW))
		return(last_state=0);
	else if(!last_state)
		return(last_state=1);
	else return(0);
}




unsigned int wall_sw_set(void)
{	static int last_state = LOW;
	static long last_time = 0;
	static int sw_setting=-1;

	// grab current switch state
	int state = digitalRead(WALL_SW);

	// if switch state changed record current state and time
	if (state != last_state)
	{	last_state = state;
		last_time = millis();
	}

	else
	//if switch state is stable and has changed from prior setting
	if ((millis() - last_time) > DEBOUNCE
	  && state != sw_setting)
	{	sw_setting = state;
		// if switch setting is pressed(HIGH) then toggle door
		if (sw_setting == HIGH)
			return(1);
			//digitalWrite(LIGHT_ON, (light_state=!light_state) );
	}
	return(0);
}




void light_sw_set(void)
{
	static int last_state = LOW;
	static long last_time = 0;
	static int sw_setting=-1;

	// grab current switch state
	int state = digitalRead(LIGHT_SW);

	// if switch state changed record current state and time
	if (state != last_state)
	{	last_state = state;
		last_time = millis();
	}

	else
	//if switch state is stable and has changed from prior setting
	if ((millis() - last_time) > DEBOUNCE
	  && state != sw_setting)
	{	sw_setting = state;
		// if switch setting is pressed(HIGH) then toggle light
		if (sw_setting == HIGH)
		{	light_state=!light_state;
			digitalWrite(LIGHT_ON, light_state );
			if(light_state) light_time=millis();
		}
	}
}




int get_limit_up(void)
{
	static int last_state = LOW;
	static long last_time = 0;
	static int sw_setting=LOW;

	// grab current switch state
	int state = digitalRead(LIMIT_UP);

	// if switch state changed record current state and time
	if (state != last_state)
	{	last_state = state;
		last_time = millis();
	}

	else
	//if switch state is stable
	if ( (millis() - last_time) > DEBOUNCE)
		sw_setting = state;
	
	return(!sw_setting);
}




int get_limit_dn(void)
{
	static int last_state = LOW;
	static long last_time = 0;
	static int sw_setting=LOW;

	// grab current switch state
	int state = digitalRead(LIMIT_DN);

	// if switch state changed record current state and time
	if (state != last_state)
	{	last_state = state;
		last_time = millis();
	}

	else
	//if switch state is stable
	if ( (millis() - last_time) > DEBOUNCE)
		sw_setting = state;
	
	return(!sw_setting);
}




