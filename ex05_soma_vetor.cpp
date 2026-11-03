// Ex 5 - Soma paralela de um vetor com #pragma omp for + reduction
#include <iostream>
#include <numeric>
#include <vector>
#include <omp.h>

constexpr int N = 10000000;

int main() {
    std::vector<int> v(N);
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

    long long serial = std::accumulate(v.begin(), v.end(), 0LL);

    std::cout << "Threads: " << omp_get_max_threads() << '\n'
              << "Soma paralela: " << soma << '\n'
              << "Soma serial:   " << serial << " -> " << (soma == serial ? "OK" : "ERRO") << '\n';
    return soma == serial ? 0 : 1;
}
