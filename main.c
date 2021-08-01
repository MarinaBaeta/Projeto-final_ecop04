//Marina Baeta de Oliveira 
//2020020917

#include <pic18f4520.h>
#include "config.h"
#include "atraso.h"
#include "bits.h"
#include "lcd.h" //biblioteca do PICSimLab
#include "serial.h" //biblioteca do PICSimLab
#include "pwm.h"
#include "keypad.h"
#include "rtc.h"

void main(void) {
 unsigned int tecla = 16;
 unsigned char i, cont=0, k, j;
 unsigned char tmp[14], atmp[14], btmp[14], ctmp[14];
 
 ADCON1 = 0x00; //0x06
 TRISB = 0x01;
 TRISC = 0x00;
 TRISD = 0x00; 
 TRISE = 0x00;
 TRISB = 0xF8;
 
    serialInit();
    lcd_init(); 
    pwmInit();
    
    lcd_cmd(L_L1);
    lcd_str("Bem vindo ao");
    lcd_cmd(L_L2);
    lcd_str("Khanban!");
    atraso_ms(2000);
    lcd_cmd(L_CLR);
    kpInit();
    pwmFrequency(1800);
                            for (k = 0; k < 3; k++) {
                                for (j = 1; j > 0; j = j * 2) {
                                    bitSet(TRISC, 1);
                                    atraso_ms(700);
                                    break;
                                }
                                bitClr(TRISC, 1);
                            }
                            PORTB = 0x00;
                            PORTD = 0x00;

    while(1){
        kpDebounce(); 
        if (kpRead() != tecla){
            tecla = kpRead();
            lcd_cmd(L_CLR);
            lcd_cmd(L_L1);
            lcd_str("Escolha:");
            lcd_cmd(L_L2);
            lcd_str("1)Ver data atual");
            lcd_cmd(L_L3);
            lcd_str("2)Cadastrar task");
            lcd_cmd(L_L4);
            lcd_str("3)Mais opcoes");
                if(bitTst(tecla,11)){ //opçoes
                    while(!bitTst(tecla,0)){
                        kpDebounce(); 
                        if (kpRead() != tecla){
                            tecla = kpRead();
                            lcd_cmd(L_CLR);
                            lcd_cmd(L_L1);
                            lcd_str("5)Proximas tasks");
                            lcd_cmd(L_L2);
                            lcd_str("*)Voltar");
                            
                            if(bitTst(tecla,6)){ //tasks
                                lcd_cmd(L_CLR);
                                lcd_cmd(L_L1);
                                lcd_str("-Proximas tasks-");
                                lcd_cmd(L_L2);
                                lcd_str("1)");
                                    for(i=0;i<14;i++){
                                       if(atmp[i]=='*'){
                                           i=14;
                                       }
                                       else{lcd_dat(atmp[i]);}
                                    }
                                lcd_cmd(L_L3);
                                lcd_str("2)");
                                    for(i=0;i<14;i++){
                                       if(btmp[i]=='*'){
                                           i=14;
                                       }
                                       else{lcd_dat(btmp[i]);}
                                    }
                                lcd_cmd(L_L4);
                                lcd_str("3)");
                                    for(i=0;i<14;i++){
                                       if(ctmp[i]=='*'){
                                           i=14;
                                       }
                                       else{lcd_dat(ctmp[i]);}
                                    }
                                atraso_ms(5000);
                            }
                        }
                    }
                }
            if(bitTst(tecla,3)){//rtc 
                lcd_cmd(L_CLR);
                lcd_cmd(L_L1);
                lcd_str("Data atual:");
                lcd_cmd(L_L2);
                rtc_r();
                lcd_str((const char*)date);
                atraso_ms(3000);
            }
            if(bitTst(tecla,7)){ //serial  
                cont ++;
                while(!bitTst(tecla,0)){
                    kpDebounce(); 
                    if (kpRead() != tecla){
                        tecla = kpRead();
                        lcd_cmd(L_CLR);
                        lcd_cmd(L_L1);
                        lcd_str("-Use o serial-");
                        lcd_cmd(L_L2);
                        lcd_str("Confirme (2)");
                        lcd_cmd(L_L3);
                        lcd_str("Termine com *");
                        lcd_cmd(L_L4);
                        lcd_str("*)Voltar");
                        //atraso_ms(2000);
                        TRISCbits.TRISC7 = 1; //RX
                        TRISCbits.TRISC6 = 0; //TX
                        if(bitTst(tecla,7)){
                            while(tmp[13]!= '*'){
                                for(i=0;i<14;i++){
                                    if(bitTst(PIR1, 5)){    //biblioteca serial, vê se tem valor
                                        tmp[i] = serialRead();
                                        if(i==13){
                                          tmp[13]= '*';
                                        }
                                        else if(tmp[i]== '*'){
                                            tmp[13]= '*';
                                            i=14;
                                        }
                                   }
                                   else{i--;}
                                } 
                            }
                            if(cont == 1){
                                    for(i=0;i<14;i++){
                                        atmp[i]= tmp[i];
                                        tmp[i] = ' ';
                                    }
                                }
                            else if(cont == 2){
                                for(i=0;i<14;i++){
                                    btmp[i]= tmp[i];
                                    tmp[i] = ' ';
                                }
                            }
                            else if(cont == 3){
                                for(i=0;i<14;i++){
                                    ctmp[i]= tmp[i];
                                }
                            }
                        }
                    }
                }   
            }
        }
    }
}