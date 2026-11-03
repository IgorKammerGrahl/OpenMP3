// Ex 6 - Inicialização de matriz: gerador inicializado uma vez com single, preenchimento com for
#define _POSIX_C_SOURCE 200809L // rand_r
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define LIN 6
#define COL 8

static int m[LIN][COL];
static unsigned int semente;
static int vezes_inicializado = 0;

static void inicializar_gerador(void) {
    semente = (unsigned int) time(NULL);
    vezes_inicializado++;
}

int main(void) {
    #pragma omp parallel
    {
        #pragma omp single
        {
            inicializar_gerador();
            printf("Thread %d inicializou o gerador\n", omp_get_thread_num());
        }
        // barreira implícita do single: nenhuma thread lê a semente antes de ela existir

        // rand() não é thread-safe; cada thread deriva seu estado da semente única
        unsigned int estado = semente + (unsigned int) omp_get_thread_num();

        #pragma omp for
        for (int i = 0; i < LIN; i++)
            for (int j = 0; j < COL; j++)
                m[i][j] = rand_r(&estado) % 100;
    }

    for (int i = 0; i < LIN; i++) {
        for (int j = 0; j < COL; j++)
            printf("%3d", m[i][j]);
        printf("\n");
    }

    printf("Gerador inicializado %d vez(es) -> %s\n", vezes_inicializado,
           vezes_inicializado == 1 ? "OK" : "ERRO");
    return vezes_inicializado == 1 ? 0 : 1;
}
