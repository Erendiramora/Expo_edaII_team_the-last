#include <stdio.h>
#include <omp.h>

int saldo = 1000;
omp_lock_t lock;
omp_nest_lock_t nest_lock;  

void retirar() {
    omp_set_lock(&lock);
        saldo -= 100;
        printf("Retirando $100. Saldo actual: $%d\n", saldo);
    omp_unset_lock(&lock);
}

void consultar() {
    omp_set_lock(&lock);
        printf("Consultando saldo: $%d\n", saldo);
    omp_unset_lock(&lock);
}

void retirar_y_consultar() {
    omp_set_nest_lock(&nest_lock);       
        saldo -= 100;
        printf("[NEST] Retiro hecho.\n");

        omp_set_nest_lock(&nest_lock);   
            printf("[NEST] Saldo tras retiro: $%d\n", saldo);
        omp_unset_nest_lock(&nest_lock);

    omp_unset_nest_lock(&nest_lock);
}

int main() {
    omp_init_lock(&lock);
    omp_init_nest_lock(&nest_lock);     

    printf("=== Demo con omp_lock_t ===\n");
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            for (int i = 0; i < 5; i++)
                retirar();
        }
        #pragma omp section
        {
            for (int i = 0; i < 5; i++)
                consultar();
        }
    }

    printf("\n=== Demo con omp_nest_lock_t ===\n");
    #pragma omp parallel sections num_threads(2)
    {
        #pragma omp section
        {
            for (int i = 0; i < 3; i++)
                retirar_y_consultar();   
        }
        #pragma omp section
        {
            for (int i = 0; i < 3; i++)
                consultar();
        }
    }

    omp_destroy_lock(&lock);
    omp_destroy_nest_lock(&nest_lock);   
    return 0;
}