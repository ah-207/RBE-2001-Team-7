#include <Arduino.h>
#include <BlueMotor.h>
#include <Romi32U4.h>

long oldValue = 0;
long newValue;
volatile long count = 0;
unsigned time = 0;
short previousState = 0; //we will use 0-3 to represent the 4 states

const int ENCA = 0;
const int ENCB = 1;

BlueMotor::BlueMotor()
{
}

void BlueMotor::setup()
{
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    TCCR1A = 0xA8; //0b10101000; //gcl: added OCR1C for adding a third PWM on pin 11
    TCCR1B = 0x11; //0b00010001;
    ICR1 = 400;
    OCR1C = 0;

    attachInterrupt(digitalPinToInterrupt(ENCA), isr, CHANGE);
    reset();
}

long BlueMotor::getPosition()
{
    long tempCount = 0;
    noInterrupts();
    tempCount = count;
    interrupts();
    return tempCount;
}

void BlueMotor::reset()
{
    noInterrupts();
    count = 0;
    interrupts();
}

short BlueMotor::getState(bool ENCA, bool ENCB)
{
    if (!ENCA && !ENCB)
        return 3;
    if (!ENCA && ENCB)
        return 2;
    if(ENCA && ENCB)
        return 1;
    else 
        return 0;
}

void BlueMotor::isr()
{
    //read pins
    short state = getState(digitalRead(ENCA), digitalRead(ENCB));
    
    //compute difference with previous state
    short delta = (state-previousState+4)%4;

    //handle movement
    if (delta == 1)
        count++;
    else if (delta == 3)
        count--;
    
    previousState = state;


}

void BlueMotor::setEffort(int effort)
{
    if (effort < 0)
    {
        setEffort(-effort, true);
    }
    else
    {
        setEffort(effort, false);
    }
}

void BlueMotor::setEffort(int effort, bool clockwise)
{
    if (clockwise)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    OCR1C = constrain(effort, 0, 400);
}

void BlueMotor::moveTo(long target)  //Move to this encoder position within the specified
{                                    //tolerance in the header file using proportional control
                                     //then stop
    
    setEffort(0);
}
