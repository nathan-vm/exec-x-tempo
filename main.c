#include <xc.h>
#define _XTAL_FREQ 4000000
#define APTOS 2
#define TMR2PRESCALE 4
#define PWM_freq 5000

//ponteiro para a tarefa
typedef void tarefa();

// estado de exec
typedef enum {READY=0,RUNNING,WAITING} state_t;

typedef struct TCB {
    int task_ID;
    tarefa *task_func;
    state_t task_state;
} tcb_t;

tcb_t STACK[APTOS];
int control_aptos = 0;
int task_on_turn = 0;
unsigned int input_value = 0;

void create_task(tarefa *func);
int scheduler();
void agent();

void task_1();
void task_2();

// tratar interrupções
void __interrupt() interrupt_for_timer0();
void PWM_Initialize(){
  PR2 = (_XTAL_FREQ/(PWM_freq*4*TMR2PRESCALE)) - 1; //Setting the PR2 formulae using Datasheet // Makes the PWM work in 5KHZ
  CCP1M3 = 1; 
  CCP1M2 = 1;  //Configure the CCP1 module 
  T2CKPS0 = 1;
  T2CKPS1 = 0; 
  TMR2ON = 1; //Configure the Timer module
  TRISC2 = 0; // make port pin on C as output
}

void Periodo_PWM(unsigned char Valor){ // 0b1001010
   T2CON = 0x04;         // Timer2 desligado, Prescaler = 1 (p. 137 do datasheet)
   PR2 = Valor;          // Registrador que contém o período do Timer2 (p. 137 do datasheet)
}

void main(void) {
    //Config do input
    TRISAbits.RA0 = 1;
    TRISCbits.RC2 = 0;
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    PWM_Initialize();
    
    // config analogico to digital
    ADCON0bits.ADON = 1;
    ADCON0 = 0b00000001 ; 
//    ADCON0bits.CHS0 = 1;
    
    //Config do timer
    T0CONbits.TMR0ON = 1;
    T0CONbits.T08BIT = 0;
    T0CONbits.T0CS = 0; // clock interno
    T0CONbits.PSA = 1;
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    TMR0L = 0;
    
    create_task(task_1);
    create_task(task_2);
    
    while (1) { 
       
   }
    return;
}

int scheduler(){
    task_on_turn = (task_on_turn+1) % control_aptos;
    return task_on_turn;
}

void agent(){
    int turn = scheduler();
    (*STACK[turn].task_func)(); 
}

void create_task(tarefa *func){
   STACK[control_aptos].task_ID = control_aptos+1;
   STACK[control_aptos].task_func = func;
   STACK[control_aptos].task_state = READY;
   control_aptos++;
}

void task_1(){
    // PORTDbits.RD1 = 1;
//    PORTDbits.RD1 = 1;
    // ler o potenciometro 
    ADCON0bits.GO = 1;
    
    while(ADCON0bits.GO_DONE);
    input_value = ADRESL + (ADRESH *256);
    
    return;
}
void task_2(){
    PORTDbits.RD1 = 0;
    
    // tensao de saida do potenciometro 
    Periodo_PWM((unsigned char)input_value);
    
    if(input_value > 32000 ){
        PORTDbits.RD1 = 1;
    }
    

    return;
}

void interrupt_for_timer0(){
    if(INTCONbits.TMR0IF) {
       agent();
       INTCONbits.TMR0IF = 0;
       TMR0L = 0;
    }
}