// Ex 5 - Soma paralela de um vetor com #pragma omp for + reduction
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 10000000

int main(void) {
    int *v = malloc(N * sizeof *v);
    if (!v) {
        perror("malloc");
        return 1;
    }
    for (int i = 0; i < N; i++)
        v[i] = i % 100;

    long long soma = 0;

    #pragma omp parallel
    {
        // cada thread soma sua parte numa cópia privada; no fim o OpenMP junta tudo em soma
        #pragma omp for reduction(+:soma)
        for (int i = 0; i < N; i++)
            soma += v[i];
    }

    long long serial = 0;
    for (int i = 0; i < N; i++)
        serial += v[i];

    printf("Threads: %d\n", omp_get_max_threads());
    printf("Soma paralela: %lld\n", soma);
    printf("Soma serial:   %lld -> %s\n", serial, soma == serial ? "OK" : "ERRO");

    free(v);
    return soma == serial ? 0 : 1;
}
