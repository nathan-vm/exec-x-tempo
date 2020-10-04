#include <xc.h>

#define APTOS 4

typedef void tarefa();

typedef struct TCB {
    unsigned int task_ID;
    unsigned int task_prior;
    
    tarefa *task_func;
} tcb_t;

tcb_t FILA_APTOS[];

void main(void) {
    return;
}
