// Contralando la posicion de un servomotor con un teclado matricial | Control the position of a servomotor with a matrix keyboard

// Librerias | Libraries
#include <16f877A.h>
#fuses XT, NOWDT, NOLVP, NOPROTECT
#use delay(clock = 4000000)
#include <lcd.c>
#use standard_io(B)
#use fast_io(D)

// Declaracion de variables | Declaration of variables
char key;
char number[3];
int i, change;
float ang1, ang2, ang3, ang4, ang5, ang6;

//-----------------------------------
// Teclado matricial | Matrix keyboard

// Definicion de los pines del teclado | Definition of keyboard pins
#define ROW1 PIN_B0
#define ROW2 PIN_B1
#define ROW3 PIN_B2
#define ROW4 PIN_B3
#define COL1 PIN_B4
#define COL2 PIN_B5
#define COL3 PIN_B6
#define COL4 PIN_B7

// Funcion para leer el teclado | Function to read the keyboard
char read_kb()
{
    //Primera fila | First row

    // Configuracion de los pines del teclado | Configuration of the keyboard pins
    output_low(ROW1);
    output_high(ROW2);
    output_high(ROW3);
    output_high(ROW4);

    // Lectura de las teclas | Reading the keys
    if (!input(COL1))
        return '7';
    if (!input(COL2))
        return '4';
    if (!input(COL3))
        return '1';
    if (!input(COL4))
        return 'C';


    // Segunda fila | Second row

    // Configuracion de los pines del teclado | Configuration of the keyboard pins
    output_high(ROW1);
    output_low(ROW2);
    output_high(ROW3);
    output_high(ROW4);

    // Lectura de las teclas | Reading the keys
    if (!input(COL1))
        return '8';
    if (!input(COL2))
        return '5';
    if (!input(COL3))
        return '2';
    if (!input(COL4))
        return '0';


    // Tercera fila | Third row

    // Configuracion de los pines del teclado | Configuration of the keyboard pins
    output_high(ROW1);
    output_high(ROW2);
    output_low(ROW3);
    output_high(ROW4);

    // Lectura de las teclas | Reading the keys
    if (!input(COL1))
        return '9';
    if (!input(COL2))
        return '6';
    if (!input(COL3))
        return '3';
    if (!input(COL4))
        return '=';


    // Cuarta fila | Fourth row

    // Configuracion de los pines del teclado | Configuration of the keyboard pins
    output_high(ROW1);
    output_high(ROW2);
    output_high(ROW3);
    output_low(ROW4);

    if (!input(COL1))
        return '/';
    if (!input(COL2))
        return '*';
    if (!input(COL3))
        return '-';
    if (!input(COL4))
        return '+';

    return 0;
}
//-----------------------------------

// Funcion de interrupcion | Interrupt function
#int_ccp1
void ccp1_int()
{
    if (change == 0)
    {
        setup_ccp1(CCP_COMPARE_CLR_ON_MATCH);
        change = 1;
    }
    else
    {
        setup_ccp1(CCP_COMPARE_SET_ON_MATCH);
        change = 0;
    }
    set_timer1(0);
}

// Funcion principal | Main function
void main()
{
    // Configuracion de los pines de entrada y salida | Configuration of input and output pins
    set_tris_B(0b00000000);
    set_tris_D(0b00001000);
    // Configuracion del timer 1 | Configuration of timer 1
    setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
    // Configuracion del CCP1 | Configuration of CCP1
    setup_ccp1(CCP_COMPARE_SET_ON_MATCH);
    // Habilita la interrupcion CCP1 | Enable CCP1 interrupt
    enable_interrupts(int_ccp1);
    // Habilita las interrupciones | Enable interrupts
    enable_interrupts(global);
    // Inicializacion del LCD | LCD initialization
    lcd_init();
    // Activar las resistencias pull-ups para el teclado | Activate the pull-ups resistors for the keyboard
    port_b_pullups(true);

    // Mensaje en el LCD | Message on the LCD
    lcd_gotoxy(1, 1);
    printf(lcd_putc, "Ang. Serv.=");
    lcd_gotoxy(1, 2);
    printf(lcd_putc, "Ingr. Ang.=");

    // Ciclo infinito | Infinite cycle
    while (true)
    {
        // Ciclo para ingresar el angulo | Cycle to enter the angle
        for (i = 0; i <= 2; i++)
        {
            i = 0;

            While(i <= 2)
            {
                // Lectura del teclado | Reading the keyboard
                key = read_kb();

                if (key != 0)
                {
                    delay_ms(250);

                    number[i] = key;

                    lcd_gotoxy(12, 2);
                    printf(lcd_putc, "%c", number[0]);
                    lcd_gotoxy(13, 2);
                    printf(lcd_putc, "%c", number[1]);
                    lcd_gotoxy(14, 2);
                    printf(lcd_putc, "%c", number[2]);

                    i++;
                }
            }
        }

        // Cadena de caracteres a numero | Character string to number
        ang1 = number[0] - 48;
        ang2 = number[1] - 48;
        ang3 = number[2] - 48;

        // Calcular el angulo | Calculate the angle
        ang4 = ang3 + ang2 * 10 + ang1 * 100;
        ang5 = (((499) * (ang4)) + 85410) / 90;
        ang6 = ang4;

        // Configuracion del CCP1 | Configuration of CCP1
        CCP_1 = ang5;

        // Mensaje en el LCD | Message on the LCD
        lcd_gotoxy(1, 1);
        printf(lcd_putc, "Ang. Serv.=%.0f  ", ang6);
    }
}
