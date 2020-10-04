#include <xc.h>
#define _XTAL_FREQ 4000000
#define APTOS 2
//ponteiro para a tarefa
typedef void tarefa();

// estado de exec
typedef enum {READY=0,RUNNING,WAITING} state_t;

typedef struct TCB {
    unsigned int task_ID;
    tarefa *task_func;
    state_t task_state;
} tcb_t;

tcb_t STACK[APTOS];
unsigned int control_aptos = 0;
int task_on_turn = 0;

void create_task(tarefa *func);
int scheduler();
void agent();

void task_1();
void task_2();

// tratar interrupções
void __interrupt() interrupt_for_timer0();

void main(void) {
    //Config do timer
    T0CONbits_t.TMR0ON = 1;
    T0CONbits_t.T08BIT = 0;
    T0CONbits_t.T0CS = 0; // clock interno
    T0CONbits_t.PSA = 1;
    INTCONbits_t.TMR0IE = 1;
    INTCONbits_t.TMR0IF = 0;
    INTCONbits_t.PEIE = 1;
    INTCONbits_t.GIE = 1;
    TMR0L = 0;
    
    create_task(task_1());
    create_task(task_2());
    
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
    // ler o potenciometro 
    return;
}
void task_2(){
    // tensao de saida do potenciometro 
    return;
}

void interrupt_for_timer0(){
    if(INTCONbits_t.TMR0IF) {
       agent()
       INTCONbits_t.TMR0IF = 0;
       TMR0L = 0;
    }
}