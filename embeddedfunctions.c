
#include <pic18f4550.h>

#include "EMBEDDEDFUNCTIONS.h"


void configADC(char CHANNELS){
    char allchannelsoff = 0x0F;
    ADCON1 = (allchannelsoff-CHANNELS);
    ADCON2 = 0x80;
    ADCON0bits.ADON = 1;
}

unsigned int analogRead(char canal){
    ADCON0 = 1+canal*4;
    ADCON0bits.GO_DONE = 1;
    while(ADCON0bits.GO_DONE == 1){
        
    }
    char lowbits = ADRESL;
    char highbits = ADRESH;
    unsigned int adresult = highbits*256+lowbits;
    return adresult;   
}

void configHEATER(){
    HEATER_out = OUT;
    PR2 = _XTAL_FREQ/(4.0*PWM_FREQ*16.0)-1;
    setHEATER_DC(50);
    T2CON = 0b00000111;
    CCP1CON = 0b00001100;
    return;
}

void setHEATER_DC(char x){
    if(x > 100){
        x = 0;
    }
    int val = x*multiplier;
    CCPR1L = (val - val%4)/4;
    CCP1CONbits.DC1B = val%4;
}
