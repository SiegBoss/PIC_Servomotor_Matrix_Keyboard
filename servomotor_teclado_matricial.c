//Contralando la posicion de un servomotor con un teclado matricial | Control the position of a servomotor with a matrix keyboard
#include <16f877A.h>
#fuses XT,NOWDT,NOLVP,NOPROTECT       
#use delay(clock=4000000)
#include <lcd.c>
#use standard_io(B)
#use fast_io(D)

//-----------------------------------
//Teclado matricial | Matrix keyboard
//Definicion de los pines del teclado | Definition of keyboard pins
#define ROW1 PIN_B0
#define ROW2 PIN_B1
#define ROW3 PIN_B2
#define ROW4 PIN_B3
#define COL1 PIN_B4
#define COL2 PIN_B5
#define COL3 PIN_B6
#define COL4 PIN_B7

//Declaracion de variables | Declaration of variables
char tecla;
char numeros[3];
int i,cambio;
float ang1,ang2,ang3,ang4,ang5,ang6;

//Funcion para leer el teclado | Function to read the keyboard
char read_kb() {

  //Configuracion de los pines del teclado | Configuration of the keyboard pins
  output_low(ROW1);
  output_high(ROW2);
  output_high(ROW3);
  output_high(ROW4);
  //Lectura de las teclas | Reading the keys
  if(!input(COL1)) return '7';
  if(!input(COL2)) return '4';
  if(!input(COL3)) return '1';
  if(!input(COL4)) return 'C';

  //Configuracion de los pines del teclado | Configuration of the keyboard pins
  output_high(ROW1);
  output_low(ROW2);
  output_high(ROW3);
  output_high(ROW4);
  //Lectura de las teclas | Reading the keys
  if(!input(COL1)) return '8';
  if(!input(COL2)) return '5';
  if(!input(COL3)) return '2';
  if(!input(COL4)) return '0';

  //Configuracion de los pines del teclado | Configuration of the keyboard pins
  output_high(ROW1);
  output_high(ROW2);
  output_low(ROW3);
  output_high(ROW4);
  //Lectura de las teclas | Reading the keys
  if(!input(COL1)) return '9';
  if(!input(COL2)) return '6';
  if(!input(COL3)) return '3';
  if(!input(COL4)) return '=';

  //Configuracion de los pines del teclado | Configuration of the keyboard pins
  output_high(ROW1);
  output_high(ROW2);
  output_high(ROW3);
  output_low(ROW4);
  if(!input(COL1)) return '/';
  if(!input(COL2)) return '*';
  if(!input(COL3)) return '-';
  if(!input(COL4)) return '+';

  return 0;
}
//-----------------------------------

//Funcion de interrupcion | Interrupt function
#int_ccp1
 void ccp1_int(){ 
 
 if(cambio==0){
   setup_ccp1(CCP_COMPARE_CLR_ON_MATCH); 
   cambio= 1; 
  }
  else{  
   setup_ccp1(CCP_COMPARE_SET_ON_MATCH);
   cambio= 0;
  }
   set_timer1(0); 
}

//Funcion principal | Main function
void main(){
  //Configuracion de los pines de entrada y salida | Configuration of input and output pins
  set_tris_B(0b00000000);
  set_tris_D(0b00001000);
  //Configuracion del timer 1 | Configuration of timer 1
  setup_timer_1(T1_INTERNAL | T1_DIV_BY_1); 
  //Configuracion del CCP1 | Configuration of CCP1
  setup_ccp1(CCP_COMPARE_SET_ON_MATCH); 
  //Habilita la interrupcion CCP1 | Enable CCP1 interrupt
  enable_interrupts(int_ccp1);
  //Habilita las interrupciones | Enable interrupts
  enable_interrupts(global);
  //Inicializacion del LCD | LCD initialization
  lcd_init();
  //Avila el pull-up interno | Enable internal pull-up
  port_b_pullups(true);

  //Mensaje en el LCD | Message on the LCD
  lcd_gotoxy(1,1);
  printf(lcd_putc,"Ang. Serv.=");
  lcd_gotoxy(1,2);
  printf(lcd_putc,"Ingr. Ang.=");
  
  //Ciclo infinito | Infinite cycle
  while(true){
 
    for(i = 0 ; i <= 2; i++){
      i = 0;

      While(i <= 2){

        tecla = read_kb();
       
        if(tecla != 0){
          delay_ms(250);
          
          numeros[i] = tecla;

          lcd_gotoxy(12,2);
          printf(lcd_putc,"%c",numeros[0]);
          lcd_gotoxy(13,2);
          printf(lcd_putc,"%c",numeros[1]);
          lcd_gotoxy(14,2);
          printf(lcd_putc,"%c",numeros[2]);
    
          i++;

        }
      }
    }

    //Cadena de caracteres a numero | Character string to number
    ang1 = numeros[0]-48;
    ang2 = numeros[1]-48;
    ang3 = numeros[2]-48;

    //Calcular el angulo | Calculate the angle
    ang4 = ang3 + ang2*10 + ang1*100;
    ang5 = (((499)*(ang4))+85410)/90;
    ang6 = ang4;
 
    //Configuracion del CCP1 | Configuration of CCP1
    CCP_1 = ang5; 

    //Mensaje en el LCD | Message on the LCD
    lcd_gotoxy(1,1);
    printf(lcd_putc,"Ang. Serv.=%.0f  ",ang6);
  }
}
