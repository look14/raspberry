#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

int main(void)
{
    int i;

    //printf("test\n");

    wiringPiSetup();
    pinMode(25, OUTPUT);

    for(i=0; i<20; i++)
    {
	digitalWrite(25, HIGH);	delayMicroseconds(1);
	digitalWrite(25, LOW);	delayMicroseconds(1);
    }
    
    return 0;
}
