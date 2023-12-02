#include "LCD_PORTD.h"
#include "xc.h"

void iniLCD(){
    __delay_ms(20); //1. Esperar mas de 15ms
    LCDcommand(Set8); //2. Comando set 8 bits
    __delay_ms(5); //3. Esperar mas de 4.1ms
    LCDcommand(Set8); //4. Comando set 8 bits
    __delay_ms(1); //5. Esperar mas de 100us
    LCDcommand(Set8); //6. Comando set 8 bits
    LCDcommand(Set8_2lin); //N = 1 (2 lines), F=0 (5x8)
    LCDcommand(DispOnCB); //D<2> ON; C<1> ON; B<0> ON;
    LCDcommand(ClearDisp); //Disp Clear
    LCDcommand(IncNoShif); //I/D = 1, S=0
    return;
}

void LCDcommand(char a){
    RS = 0; //RE0 --> RS=0
    PORTD = a;
    EN = 1; //RE1 --> E=1
    __delay_ms(1);
    EN = 0; //RE1 --> E=0
    __delay_ms(1);
    return;
}

void LCDchar(char a){
    RS = 1;
    PORTD = a; //Valor ASCII
    EN = 1; //RE1 --> E=1
    __delay_us(40);
    EN = 0; //RE1 --> E=0
}

void LCDprint(int size, char a[], int wait_ms){
    for(int i = 0; i<size; i++){
    LCDchar(a[i]);
    for(int j = 0; j<wait_ms; j++){
    __delay_ms(1);
        }
    }
}

void MoveCursor(char x, char y){
    char a;
    if(y <= 1 && x <= 39){
        if(y ==0){
            a = x;
         } else{
            a = x + 64;
        }
        a = a | SetDDRAM;
    }
    RS = 0; //RE0 --> RS=0
    PORTD = a;
    EN = 1; //RE1 --> E=1
    __delay_ms(2);
    EN = 0; //RE1 --> E=0
    return;
}

void MoveLCD(char dir, char inc){
    if(dir==0){
        for(int i = 0; i < inc; i++){
            LCDcommand(DispSfL);
            __delay_ms(200);
    }
    }else{
        for(int i = 0; i < inc; i++){
            LCDcommand(DispSfR);
            __delay_ms(200);
        }
    }
}

void GenChar(char a[8]){
    for(int i = 0; i < 8; i++){
        LCDchar(a[i]);
    }
}

void LCDint(int a){
    char U=0;
    char D=0;
    char C=0;
    char UM=0;
    char DM=0;
    if (a < 0){
        LCDchar("-");
        a *=-1;
    }
    if(a < 10){
        U = a;
        LCDchar("0"+U);
    }
    else{
        if(a <100){
            U = a%10;
            D = ((a-U)/10)%10;
            LCDchar('0'+D);
            LCDchar('0'+U);
            
        }else{
            if(a < 1000){
                U = a%10;
                D = ((a-U)/10)%10;
                C = ((a-U-10*D)/100)%10;
                LCDchar('0'+C);
                LCDchar('0'+D);
                LCDchar('0'+U);
            }else {
                if(a < 10000){
                    U = a%10;
                    D = ((a-U)/10)%10;
                    C = ((a-U-10*D)/100)%10;
                    UM = ((a-U-10*D-100*C)/1000)%10;
                    LCDchar('0'+UM);
                    LCDchar('0'+C);
                    LCDchar('0'+D);
                    LCDchar('0'+U);
                }else {
                    U = a%10;
                    D = ((a-U)/10)%10;
                    C = ((a-U-10*D)/100)%10;
                    UM = ((a-U-10*D-100*C)/1000)%10;
                    DM = ((a-U-10*D-100*C-1000*UM)/10000)%10;
                    LCDchar('0'+DM);
                    LCDchar('0'+UM);
                    LCDchar('0'+C);
                    LCDchar('0'+D);
                    LCDchar('0'+U);
                }
            }
        }
    }
}
