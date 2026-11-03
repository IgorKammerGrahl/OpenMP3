// Ex 8 - Multiplicação de matrizes: cada section calcula uma faixa de linhas de C,
// e dentro dela um omp for divide essas linhas entre um time interno de threads
#include <iostream>
#include <vector>
#include <omp.h>

constexpr int N = 512;
constexpr int THREADS_POR_SECAO = 2;

using Matriz = std::vector<std::vector<double>>;

static void multiplicar_linhas(const Matriz &A, const Matriz &B, Matriz &C, int ini, int fim) {
    int externa = omp_get_thread_num();

    // omp for direto dentro de uma section é aninhamento proibido de worksharing;
    // por isso a seção abre um parallel próprio e o omp for divide o trabalho nele
    #pragma omp parallel num_threads(THREADS_POR_SECAO)
    {
        #pragma omp single
        {
            // as 4 seções imprimem ao mesmo tempo; sem critical os pedaços do cout se misturam
            #pragma omp critical(saida)
            std::cout << "Linhas " << ini << "-" << fim - 1 << ": thread externa " << externa
                      << ", time interno de " << omp_get_num_threads() << " threads\n";
        }

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

int main() {
    const std::vector<double> linha(N);
    Matriz A(N, linha), B(N, linha), C(N, linha), Cs(N, linha);

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
            multiplicar_linhas(A, B, C, 0, N / 4);

            #pragma omp section
            multiplicar_linhas(A, B, C, N / 4, N / 2);

            #pragma omp section
            multiplicar_linhas(A, B, C, N / 2, 3 * N / 4);

            #pragma omp section
            multiplicar_linhas(A, B, C, 3 * N / 4, N);
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
    bool ok = C == Cs;
    std::cout << "C[0][0] = " << C[0][0] << ", C[" << N - 1 << "][" << N - 1 << "] = " << C[N - 1][N - 1] << '\n'
              << "Comparação com a versão serial: " << (ok ? "OK" : "ERRO") << '\n';
    return ok ? 0 : 1;
}
