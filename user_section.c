#include <avr/io.h>
#include "delay.h"
#include "lcd.h"
void InitAdc(void) //function to initialize adc using ADC registers ADMUX and ADCSRA
{
ADMUX=0x62;
ADCSRA=0x87;
}
uint8_t ReadAdc(void) //function to read analog value and convert it to 8 bit digital value
{
ADCSRA|=1<<ADSC; //start conversion
while(!(ADCSRA & (1<<ADIF))); //waiting for conversion complete flag t be set
ADCSRA|=1<<ADIF; //conversion complete flag reset
uint8_t adcresult=ADCH; //transferring 8 bit digital value to local variable from ADCH register
return(adcresult); //return the 8-bit digital result
}
int main(void)
{
DDRB=0xF0; //ports initialization
PORTB=0x0F;
DDRD=0x03;
PORTD=0xFC;
uint8_t gesture_value = 0x00; //declaration of variables to be used
uint8_t xadc_value = 0x00;
uint8_t yadc_value = 0x00;
uint8_t sensor_value = 0x00;
uint8_t sensor_status = 0x00;


lcd_init(); //Initializing LCD
lcd_cmd(0x01); //Clearing LCD
InitAdc(); //Initializing ADC
while(1)
{
int x=0; //Initializing gesture range determining variables
int y=0; //for X and Y values .
ADMUX=0x62; //Selecting ADC channel 2 by writing into ADMUX register
xadc_value = ReadAdc();
ADMUX=0x63; //Selecting ADC channel 3 by writing into ADMUX register
yadc_value = ReadAdc();
gesture_value=0x00;
sensor_status = PIND & 0x40; //read VT pin status for Valid transmission
switch(sensor_status)
{
case(0x40): //if VT pin is high then read sensor_value
sensor_value = PINB & 0x0F;
switch(sensor_value)
{
case(0x0E):
PORTD = 0xF2; //report gas leak at PD1
break;
case(0x0D):
PORTD = 0xF1; //report presence of human at PD0
break;
case(0x01): //cases for various temperature ranges to be displayed in lcd
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x00);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x0A);
lcd_gotoxy(0,13);


lcd_char(’C’);
break;
case(0x02):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x0A);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x14);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x03):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x14);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x19);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x04):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x19);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x1E);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;


case(0x05):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x1E);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x23);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x06):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x23);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x28);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x07):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x28);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x32);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x08):



lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x32);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x3C);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x09):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x3C);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x46);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x0A):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x46);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x50);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x0B):
lcd_cmd(0x01);
lcd_gotoxy(0,3);



lcd_showvalue(0x50);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x5A);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
case(0x0C):
lcd_cmd(0x01);
lcd_gotoxy(0,3);
lcd_showvalue(0x5A);
lcd_gotoxy(0,7);
lcd_char(’-’);
lcd_gotoxy(0,9);
lcd_showvalue(0x64);
lcd_gotoxy(0,13);
lcd_char(’C’);
break;
default: PORTD = 0xFC;
lcd_cmd(0x01);
break;
}
break;
default:
//if no valid transmission has occured clear outputs and lcd and pull up initialize inputs
PORTD = 0xFC;
lcd_cmd(0x01);
break;
}
///gesture checking....
if (yadc_value>=0x41)
{
if (yadc_value<=0x48)
{


y=1;
//Speed-2 Backward
}
else if (yadc_value<=0x50)
{
y=2;
//Speed-1 Backward
}
else if(yadc_value<=0x58)
{
y=3;
//No Movement or Check X value
}
else if(yadc_value<=0x5D)
{
y=4;
//Speed-1 Front
}
else if(yadc_value<=0x64)
{
y=5;
//Speed-2 Front
}
else
{
y=10;
//No movement
}
}
else
{
y=10;
//No movement
}
///////////
if (xadc_value>=0x42)



{
if (xadc_value<=0x49)
{
x=1;
//Speed-2 Left
}
else if (xadc_value<=0x51)
{
x=2;
//Speed-1 Left
}
else if(xadc_value<=0x57)
{
x=3;
//No movement
}
else if(xadc_value<=0x5E)
{
x=4;
//Speed-1 Right
}
else if(xadc_value<=0x64)
{
x=5;
//Speed-2 Right
}
else
{
x=10;
//No movement
}
}
else
{
x=10;
//No Movement
}


switch(y)
{
case(1):
gesture_value=0xCF; //Speed-2 Backward
break;
case(2):
gesture_value=0x4F; //Speed-1 Backward
break;
case(3): //No Movement or Check X value
switch(x)
{
case(1):
gesture_value=0xBF; //Speed-2 Left
break;
case(2):
gesture_value=0x3F; //Speed-1 Left
break;
case(3):
gesture_value=0xAF; //No Movement
break;
case(4):
gesture_value=0x6F; //Speed-1 Right
break;
case(5):
gesture_value=0xEF; //Speed-2 Right
break;
default:
gesture_value=0xFF; //Send Default value
break;
}
break;
case(4): //Speed-1 Front
gesture_value=0x1F;
break;
case(5): //Speed-2 Front
gesture_value=0x9F;
break;





default:
gesture_value=0xAF; //No movement
break;
}
//clearing outputs and pull up initializing inputs
lcd_cmd(0x01);
PORTB=gesture_value; //Send Gesture Value
delayms(10);

}
}
