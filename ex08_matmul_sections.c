// Ex 8 - Multiplicação de matrizes: cada section calcula uma faixa de linhas de C,
// e dentro dela um omp for divide essas linhas entre um time interno de threads
#include <stdio.h>
#include <omp.h>

#define N 512
#define THREADS_POR_SECAO 2

static double A[N][N], B[N][N], C[N][N], Cs[N][N];

static void multiplicar_linhas(int ini, int fim) {
    int externa = omp_get_thread_num();

    // omp for direto dentro de uma section é aninhamento proibido de worksharing;
    // por isso a seção abre um parallel próprio e o omp for divide o trabalho nele
    #pragma omp parallel num_threads(THREADS_POR_SECAO)
    {
        #pragma omp single
        printf("Linhas %3d-%3d: thread externa %d, time interno de %d threads\n",
               ini, fim - 1, externa, omp_get_num_threads());

        #pragma omp for
        for (int i = ini; i < fim; i++)
            for (int j = 0; j < N; j++) {
                double s = 0;
                for (int k = 0; k < N; k++)
                    s += A[i][k] * B[k][j];
                C[i][j] = s;
            }
    }
}

int main(void) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A[i][j] = (i + j) % 10;
            B[i][j] = (i * j + 1) % 7;
        }

    omp_set_max_active_levels(2); // sem isso o parallel interno roda com 1 thread só

    #pragma omp parallel num_threads(4)
    {
        #pragma omp sections
        {
            #pragma omp section
            multiplicar_linhas(0, N / 4);

            #pragma omp section
            multiplicar_linhas(N / 4, N / 2);

            #pragma omp section
            multiplicar_linhas(N / 2, 3 * N / 4);

            #pragma omp section
            multiplicar_linhas(3 * N / 4, N);
        }
    }

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            double s = 0;
            for (int k = 0; k < N; k++)
                s += A[i][k] * B[k][j];
            Cs[i][j] = s;
        }

    // valores inteiros pequenos: a soma em double é exata, dá para comparar com ==
    int erros = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (C[i][j] != Cs[i][j])
                erros++;

    printf("C[0][0] = %.0f, C[%d][%d] = %.0f\n", C[0][0], N - 1, N - 1, C[N - 1][N - 1]);
    printf("Comparação com a versão serial: %s (%d diferenças)\n", erros ? "ERRO" : "OK", erros);
    return erros ? 1 : 0;
}
