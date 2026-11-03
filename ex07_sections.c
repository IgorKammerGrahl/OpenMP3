// Ex 7 - Duas tarefas independentes com sections: soma de A e produto de B
#include <stdio.h>
#include <omp.h>

#define N 20

int main(void) {
    int a[N], b[N];
    for (int i = 0; i < N; i++) {
        a[i] = i + 1;
        b[i] = i + 1;
    }

    long long soma = 0, produto = 1; // B = 1..20, e 20! ainda cabe em long long

    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for (int i = 0; i < N; i++)
                    soma += a[i];
                printf("Soma calculada pela thread %d\n", omp_get_thread_num());
            }

            #pragma omp section
            {
                for (int i = 0; i < N; i++)
                    produto *= b[i];
                printf("Produto calculado pela thread %d\n", omp_get_thread_num());
            }
        }
    }

    int ok = soma == 210 && produto == 2432902008176640000LL;
    printf("Soma de A    = %lld\n", soma);
    printf("Produto de B = %lld\n", produto);
    printf("%s\n", ok ? "OK" : "ERRO");
    return ok ? 0 : 1;
}
