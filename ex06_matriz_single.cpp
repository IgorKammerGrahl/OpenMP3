// Ex 6 - Inicialização de matriz: gerador inicializado uma vez com single, preenchimento com for
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

constexpr int LIN = 6;
constexpr int COL = 8;

static unsigned int semente;
static int vezes_inicializado = 0;

static void inicializar_gerador() {
    semente = std::random_device{}();
    vezes_inicializado++;
}

int main() {
    std::vector<std::vector<int>> m(LIN, std::vector<int>(COL));

    #pragma omp parallel
    {
        #pragma omp single
        {
            inicializar_gerador();
            std::cout << "Thread " << omp_get_thread_num() << " inicializou o gerador\n";
        }
        // barreira implícita do single: nenhuma thread lê a semente antes de ela existir

        // um gerador por thread: compartilhar um só seria condição de corrida no estado dele
        std::mt19937 gerador(semente + omp_get_thread_num());
        std::uniform_int_distribution<int> dist(0, 99);

        #pragma omp for
        for (int i = 0; i < LIN; i++)
            for (int j = 0; j < COL; j++)
                m[i][j] = dist(gerador);
    }

    for (const auto &linha : m) {
        for (int x : linha)
            std::cout << std::setw(3) << x;
        std::cout << '\n';
    }

    std::cout << "Gerador inicializado " << vezes_inicializado << " vez(es) -> "
              << (vezes_inicializado == 1 ? "OK" : "ERRO") << '\n';
    return vezes_inicializado == 1 ? 0 : 1;
}
