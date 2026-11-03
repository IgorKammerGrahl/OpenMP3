// Ex 7 - Duas tarefas independentes com sections: soma de A e produto de B
#include <array>
#include <iostream>
#include <numeric>
#include <omp.h>

constexpr int N = 20;

int main() {
    std::array<int, N> a, b;
    std::iota(a.begin(), a.end(), 1);
    b = a;

    long long soma = 0, produto = 1; // B = 1..20, e 20! ainda cabe em long long

    #pragma omp parallel num_threads(2)
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for (int i = 0; i < N; i++)
                    soma += a[i];
                // as duas seções imprimem ao mesmo tempo; sem critical os pedaços do cout se misturam
                #pragma omp critical(saida)
                std::cout << "Soma calculada pela thread " << omp_get_thread_num() << '\n';
            }

            #pragma omp section
            {
                for (int i = 0; i < N; i++)
                    produto *= b[i];
                #pragma omp critical(saida)
                std::cout << "Produto calculado pela thread " << omp_get_thread_num() << '\n';
            }
        }
    }

    bool ok = soma == 210 && produto == 2432902008176640000LL;
    std::cout << "Soma de A    = " << soma << '\n'
              << "Produto de B = " << produto << '\n'
              << (ok ? "OK" : "ERRO") << '\n';
    return ok ? 0 : 1;
}
