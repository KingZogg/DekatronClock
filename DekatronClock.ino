class dekatronStep
{

public:
	byte Guide1;
	byte Guide2;
	byte Index;
	byte previousGuideState;
	int stepDelay;
	bool clockwise;
	unsigned long previousMillis;
	

public:
	dekatronStep(byte pin1, byte pin2, byte pin3, bool direction,int sDelay)	//Guide1, Guide2, Index, StepDelay, Direction
	//dekatronStep(byte pin1, byte pin2, byte pin3, bool direction, int sDelay)	//Guide1, Guide2, Index, StepDelay, Direction
	{
		Guide1 = pin1;
		Guide2 = pin2;
		Index = pin3;
		stepDelay = sDelay;
		clockwise = direction;
		
		pinMode(Guide1, OUTPUT);
		pinMode(Guide2, OUTPUT);
		pinMode(Index, INPUT);
	}

	void updateStep(bool clockwise,int stepDelay)
	{
		//Delay needed if there is not enough delay in the loop when calling.
		// will need adjusting depending on processor speed. This is runing at 16mHz.

		//delayMicroseconds(40);
		int atIndex=0;
		atIndex = digitalRead(Index);
		
		if (atIndex) 
		{
			digitalWrite(LED_BUILTIN, HIGH);
		}
		else digitalWrite(LED_BUILTIN, LOW);

				
		if ((millis() - previousMillis >= stepDelay))
		{
			switch (previousGuideState) {
			case 0:
				previousGuideState = 1;
				digitalWrite(Guide1, LOW);
				digitalWrite(Guide2, LOW);
				previousMillis = millis();
				break;

			case 1:
				previousGuideState = 2;
				if (clockwise == true)
				{
					digitalWrite(Guide1, HIGH);
					digitalWrite(Guide2, LOW);
				}
				else
				{
					digitalWrite(Guide1, LOW);
					digitalWrite(Guide2, HIGH);
				}
				previousMillis = millis();
				break;

			case 2:
				previousGuideState = 0;
				if (clockwise == true)
				{
					digitalWrite(Guide1, LOW);
					digitalWrite(Guide2, HIGH);
				}
				else
				{
					digitalWrite(Guide1, HIGH);
					digitalWrite(Guide2, LOW);
				}
				previousMillis = millis();
				break;
			} // end of switch case
			
		}


	}

};

//setup physical pins here. In this case 22 and 23 are G1 and G2. The index is 24.
dekatronStep Dek1(22, 23, 24, true, 0); 
dekatronStep Dek2(25, 26, 27, true, 0); //error not moving.
dekatronStep Dek3(28, 29, 30, true, 0);
dekatronStep Dek4(31, 32, 33, true, 0); //error not moving.
dekatronStep Dek5(34, 35, 36, true, 0); 
dekatronStep Dek6(37, 38, 39, true, 0); //error wobbling
dekatronStep Dek7(40, 41, 42, true,0);
dekatronStep Dek8(43, 44, 45, true, 0); //error not moving.
dekatronStep Dek9(46, 47, 48, true, 0);
dekatronStep Dek10(0, 1, 2, true,  0);	//error not moving

//guide wiring back the front.
dekatronStep Dek11(4, 3, 5, true, 0);	//WORKING
dekatronStep Dek12(7, 6, 8, true, 0);	//WORKING
dekatronStep Dek13(10, 9, 11, true, 0); //Working but only when 11 and 12 are also plugged in.



//index ignore timout settings.
byte oldIndexState = HIGH;
const unsigned long ignoreTime = 5;  // milliseconds
unsigned long indexHighTime;	// when the index last changed state




void setup()
{
	// TIMER 1 for interrupt frequency 2000 Hz:
	cli(); // stop interrupts
	TCCR1A = 0; // set entire TCCR1A register to 0
	TCCR1B = 0; // same for TCCR1B
	TCNT1 = 0; // initialize counter value to 0
			   // set compare match register for 2000 Hz increments
	OCR1A = 7999; // = 16000000 / (1 * 2000) - 1 (must be <65536)
				  // turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS12, CS11 and CS10 bits for 1 prescaler
	TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei(); // allow interrupts

	pinMode(LED_BUILTIN, OUTPUT);

	//Serial.begin(2000000);

}

// Interrupt 
ISR(TIMER1_COMPA_vect)
{
	Dek1.updateStep(true, 100);
	Dek2.updateStep(true, 10);
	Dek3.updateStep(true, 15);
	Dek4.updateStep(true, 20);
	Dek5.updateStep(true, 25);
	Dek6.updateStep(true, 30);
	Dek7.updateStep(true, 35);
	Dek8.updateStep(true, 40);
	Dek9.updateStep(true, 45);
	Dek10.updateStep(true, 50);
	Dek11.updateStep(true, 55);
	Dek12.updateStep(true, 60);
	Dek13.updateStep(true, 15);

	updateIndex();

}


void updateIndex() {

	// see if Index is High or Low
	byte indexState = digitalRead(Dek13.Index);

	// has index state changed since last time?
	if (indexState != oldIndexState)
	{
		// ignore time.
		if (millis() - indexHighTime >= ignoreTime)
		{
			indexHighTime = millis();  // when index was high
			oldIndexState = indexState;  // remember for next time 

			if ((indexState == HIGH) && (Dek13.clockwise == false))
			{
				Dek12.clockwise = true;
				Serial.println("Clockwise");
				//Serial.println("index high");

			}
			else if (((indexState == HIGH)) && (Dek13.clockwise == true))
			{
				Dek12.clockwise = false;
				Serial.println("Counter Clockwise");
				//Serial.println("index low");

			}

		}  // end if ignore time up


	}  // end of state change

}

void loop() {


}
