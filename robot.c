#include <avr/io.h>
#include "delay.h"
void InitAdc(void) //function to initialize adc
{
ADMUX=0x60;
ADCSRA=0x87;
}
uint8_t ReadAdc(void) //function to read analog value and convert it to 8 bit digital value
{
ADCSRA|=1<<ADSC;
while(!(ADCSRA & (1<<ADIF)));
ADCSRA|=1<<ADIF;
uint8_t adcresult=ADCH;
return(adcresult);
}
int main(void)
{
DDRD=0x0F; //ports initialization
DDRB=0b11111100;
DDRC=0xF0;
PORTC=0x0F;
PORTD=0xF0;
PORTB=0x03;
uint8_t sensor_value = 0x00;
uint8_t temp_value = 0x00;
uint8_t control_value = 0x00;
uint8_t gasleak_value =0x00;
uint8_t pir_value =0x00;
uint8_t temprange_value =0x00;
InitAdc();
while(1)
{
ADMUX=0x60; //setting the channel to ADC0.
temp_value=ReadAdc(); //Reading the ADC output to variable of temperature sensor.
sensor_value = PINB & 0b00000011; //Reading Gas and PIR sensor outputs.



switch(sensor_value) //Assigning values to sensor variables
{ //for various cases.
case(0x01):
pir_value = 0x0F;
gasleak_value=0xEF;
break;
case(0x02):
gasleak_value = 0x0F;
pir_value = 0xDF;
break;
case(0x03):
gasleak_value=0xEF;
pir_value = 0xDF;
break;
default:
pir_value = 0x0F;
gasleak_value = 0x0F;
break;
}
//Checking TemperatureâĂęâĂę..
if((temp_value >=0x00) && (temp_value < 0x05))
{
temprange_value = 0x1F;
}
else if((temp_value >=0x05) && (temp_value < 0x0A))
{
temprange_value = 0x2F;
}
else if((temp_value >=0x0A) && (temp_value < 0x0D))
{
temprange_value = 0x3F;
}
else if((temp_value >=0x0D) && (temp_value < 0x0F))
{



temprange_value = 0x4F;
}
else if((temp_value >=0x0F) && (temp_value < 0x12))
{
temprange_value = 0x5F;
}
else if((temp_value >=0x12) && (temp_value < 0x15))
{
temprange_value = 0x6F;
}
else if((temp_value >=0x15) && (temp_value < 0x1A))
{
temprange_value = 0x7F;
}
else if((temp_value >=0x1A) && (temp_value < 0x1F))
{
temprange_value = 0x8F;
}
else if((temp_value >=0x1F) && (temp_value < 0x24))
{
temprange_value = 0x9F;
}
else if((temp_value >=0x24) && (temp_value < 0x29))
{
temprange_value = 0xAF;
}
else if((temp_value >=0x29) && (temp_value < 0x2E))
{
temprange_value = 0xBF;
}


else if((temp_value >=0x2E) &&(temp_value < 0x33))
{
temprange_value = 0xCF;
}
else
{
temprange_value =0x0F;
}
//Decoding Gesture Codes to the required inputs to drive the motor as per requirements
control_value = PINC & 0x0F; //Reading movement code to variable.
switch(control_value) //Sending movement codes for various cases
{ //to motor driver.
case(0x0A): //No movement
PORTD = 0xF0;
TCCR0 = 0x48; //Timer Disabled
PORTB = 0x03; //output PB3 set as 0 and inputs PB1 and PB0 pull-up initialized to 1.
break;
//max speed cases, PWM duty cycle set to 100%
case(0x09):
PORTD = 0x06;
TCCR0 = 0x6B;
OCR0 =255;
break;
case(0x0C):
PORTD = 0x09;
TCCR0 = 0x6B;
OCR0 =255;
break;
case(0x0B):
PORTD = 0x05;
TCCR0 = 0x6B;
OCR0 =255;


break;
case(0x0E):
PORTD = 0x0A;
TCCR0 = 0x6B;
OCR0 =255;
break;
//Low Speed Cases, PWM Duty Cycle set to 60%.
case(0x01):
PORTD = 0x06;
TCCR0 = 0x6B;
OCR0 =153;
break;
case(0x04):
PORTD = 0x09;
TCCR0 = 0x6B;
OCR0 =153;
break;
case(0x03):
PORTD = 0x05;
TCCR0 = 0x6B;
OCR0 =153;
break;
case(0x06):
PORTD = 0x0A;
TCCR0 = 0x6B;
OCR0 =153;
break;
default:
PORTD = 0xF0;
TCCR0 = 0x48;
PORTB = 0x03; //output PB3 set as 0 and inputs PB1 and PB0 pull-up initialized to 1.



break;
}
PORTC=gasleak_value; //Sending the sensor values to the respective
delayms(100); //port connected to HT12E encoder
PORTC= pir_value;
delayms(100);
PORTC=temprange_value;
delayms(100);
}
}
