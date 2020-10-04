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

tcb_t STACK[];
int control_aptos = 0;
int task_on_turn = 0;

void create_task(tarefa *func);
int scheduler();
void agent();

void task_1();
void task_2();

void main(void) {
    
    create_task(task_1());
    create_task(task_2());
    
    while (1) { 
      agent();
      __delay_ms(10);
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