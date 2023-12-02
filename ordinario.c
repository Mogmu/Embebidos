#include <xc.h>
#include "configs.h"
#include "LCD_PORTD.h"
#include "EMBEDDEDFUNCTIONS.h"

#define SENSOR 0
#define DEGREE 223

#define R_out TRISEbits.RE0
#define G_out TRISEbits.RE1
#define B_out TRISEbits.RE2
#define R_pin LATEbits.LE0
#define G_pin LATEbits.LE1
#define B_pin LATEbits.LE2
#define Tau 500

double PID(double temp);
void tempColor(double t);
void updateLCD1(double t);
void updateLCD2(double t);
void updateHEAT(double temp);
void configInterrupts();

void RED(void);
void GREEN(void);
void YELLOW(void);
void CYAN(void);

char INICIAR = 0;
double t_objetive = 38;
char H_DC = 0;

double Kp = 10;
double Ki = 1;
double Kd = 0;
double I = 0;

void main(void) {
    OSCCON = 0x7F;
    ADCON1 = 0x0F;
    TRISD = 0;
    TRISEbits.RE0 = 0;
    TRISEbits.RE1 = 0;
    //TRISB = 0x0F;
    iniLCD();
    LCDcommand(DispOn);
    configInterrupts();
    MoveCursor(0,LINE_UP);
    LCDprint(11,"PRESS START",0);
    while(INICIAR == 0);
    configHEATER();
    setHEATER_DC(H_DC);
    configADC(1);
    unsigned int lec1;
    R_out = OUT;
    G_out = OUT;
    B_out = OUT;
    double volts;
    double temp;
    MoveCursor(0,LINE_UP);
    LCDprint(11,"Temperature",0);
    int caso=0;
    while(INICIAR == 1){
        lec1 = analogRead(SENSOR);
        volts = 5.0*lec1/1023.0;
        temp = volts*100;//10.018 - 0.15;
        updateHEAT(temp);
        tempColor(temp);
        switch(caso){
            case 0:
                updateLCD1(temp);
                break;
            case 1:
                break;
            case 2:
                updateLCD2(t_objetive);
                break;
            case 3:
                caso=-10;
                break;
        }
        caso++;
        __delay_ms(Tau);    
    }
    
    return;
}

void tempColor(double t){
    if(t < t_objetive-0.5){
        CYAN();
    }else if(t >= t_objetive-0.5 && t <= t_objetive+0.5){
        GREEN();
    }else if(t > t_objetive+0.5){
        RED();
    }
}
void updateLCD1(double t){
    double decimals;
    MoveCursor(0,LINE_DOWN);
    LCDint(t);
    LCDchar('.');
    decimals=t-(int)t;
    decimals*=100;
    if(decimals<10){
        LCDchar('0');
    }
    LCDint(decimals);
    LCDchar(' ');
    LCDchar(DEGREE);
    LCDchar('C');
    if(t < t_objetive-0.5){
        LCDprint(8,"LOWER   ",0);
    }else if(t >= t_objetive-0.5 && t <= t_objetive+0.5){
        LCDprint(8,"OK      ",0);
    }else if(t > t_objetive+0.5){
        LCDprint(8,"HIGHER  ",0);
    }
}

void updateLCD2(double t){
    double decimals;
    MoveCursor(0,LINE_DOWN);
    LCDint(t);
    LCDchar('.');
    decimals=t-(int)t;
    decimals*=100;
    if(decimals<10){
        LCDchar('0');
    }
    LCDint(decimals);
    LCDchar(' ');
    LCDchar(DEGREE);
    LCDchar('C');
    LCDchar(' ');
    LCDprint(8,"objetive",0);
}

double PID(double temp){
    double err = t_objetive-temp;
    double P = Kp*err;
    I = I + Ki*err*(Tau/1000);
    return P + I;
}

void updateHEAT(double temp){
    H_DC = PID(temp);
    setHEATER_DC(H_DC);
    /*
    if(temp < t_objetive){
        H_DC++;
        setHEATER_DC(H_DC);
    }else{
        H_DC--;
        setHEATER_DC(H_DC);
    }
    */
}

void RED(){
    R_pin = LOW;
    G_pin = LOW;
    B_pin = HI;
}

void GREEN(){
    R_pin = LOW;
    G_pin = LOW;
    B_pin = LOW;
}

void YELLOW(){
    
    R_pin = HI;
    G_pin = LOW;
    B_pin = LOW;
}

void CYAN(){
    R_pin = HI;
    G_pin = LOW;
    B_pin = LOW;
}

void configInterrupts(){
    INTCONbits.GIE = ON; //Habilita la interrupción externa0;
    INTCONbits.PEIE = LOW; //flanco descendente
    INTCON3bits.INT2E = ON; //Habilita la interrupción externa1;
    INTCON2bits.INTEDG2 = HI; //flanco descendente
    RCONbits.IPEN = OFF; //no priorities
}

void __interrupt () miISR( ){
    if(INTCON3bits.INT2IF==HI){
        if(INICIAR == 0)   INICIAR = 1;
        else if(INICIAR == 1) INICIAR = 0;
        INTCON3bits.INT2IF=LOW;
    }
}
