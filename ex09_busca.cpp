// Ex 9 - Busca paralela: omp for percorre, critical atualiza a posição, single imprime
#include <algorithm>
#include <iostream>
#include <vector>
#include <omp.h>

constexpr int N = 1000000;

int main() {
    std::vector<int> v(N);
    for (int i = 0; i < N; i++)
        v[i] = i % 1000; // o alvo aparece várias vezes, em partes de threads diferentes

    const int alvo = 777;
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
                std::cout << "Valor " << alvo << " encontrado primeiro na posição " << pos
                          << " (impresso pela thread " << omp_get_thread_num() << ")\n";
            else
                std::cout << "Valor " << alvo << " não encontrado\n";
        }
    }

    int serial = std::find(v.begin(), v.end(), alvo) - v.begin(); // N se não achar
    std::cout << "Busca serial: " << serial << " -> " << (pos == serial ? "OK" : "ERRO") << '\n';
    return pos == serial ? 0 : 1;
}
