// Ex 9 - Busca paralela: omp for percorre, critical atualiza a posição, single imprime
#include <stdio.h>
#include <omp.h>

#define N 1000000

static int v[N];

int main(void) {
    for (int i = 0; i < N; i++)
        v[i] = i % 1000; // o alvo aparece várias vezes, em partes de threads diferentes

    int alvo = 777;
    int pos = N; // compartilhada; N significa "não encontrado"

    #pragma omp parallel
    {
        // break não é permitido dentro de omp for, então o vetor é percorrido inteiro
        #pragma omp for
        for (int i = 0; i < N; i++)
            if (v[i] == alvo) {
                #pragma omp critical
                if (i < pos) // as threads acham ocorrências fora de ordem; fica a menor
                    pos = i;
            }
        // barreira implícita do for: aqui pos já tem o valor final

        #pragma omp single
        {
            if (pos < N)
                printf("Valor %d encontrado primeiro na posição %d (impresso pela thread %d)\n",
                       alvo, pos, omp_get_thread_num());
            else
                printf("Valor %d não encontrado\n", alvo);
        }
    }

    int serial = N;
    for (int i = 0; i < N; i++)
        if (v[i] == alvo) {
            serial = i;
            break;
        }

    printf("Busca serial: %d -> %s\n", serial, pos == serial ? "OK" : "ERRO");
    return pos == serial ? 0 : 1;
}
