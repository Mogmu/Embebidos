#ifndef EMBEDDEDFUNCTIONS_H
#define	EMBEDDEDFUNCTIONS_H

#define _XTAL_FREQ 8000000
#define HEATER_out TRISCbits.RC2
#define ON 1
#define OFF 0
#define OUT 0
#define HI 1
#define LOW 0
#define LINE_UP 0
#define LINE_DOWN 1
#define PWM_FREQ 2500
double multiplier = _XTAL_FREQ/(PWM_FREQ*16.0*100.0);

void configADC(char CHANNELS);
unsigned int analogRead(char canal);
void configHEATER(void);
void setHEATER_DC(char x);

#endif	/* XC_HEADER_TEMPLATE_H */
