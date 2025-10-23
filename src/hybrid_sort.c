#include <stdlib.h>
#include <math.h>
#include <sys/types.h> /* ssize_t */
#include "sort.h"
#include "util.h"

/*
 * Espaço para a dupla implementar o algoritmo avançado.
 * O exemplo abaixo ilustra um "IntroSort" simplificado:
 *  - Se subproblema pequeno: InsertionSort.
 *  - Se profundidade excede limite: HeapSort (para evitar pior caso do Quick).
 *  - Caso geral: QuickSort com partição Lomuto (já contamos comparações/movimentações).
 *
 * Ajuste os parâmetros em HybridParams conforme sua proposta (documente no relatório).
 */
/*
void hybrid_sort(int *a, size_t n, const HybridParams *p, Metrics *m) {
    printf("Hybrid sort em desenvolvimento...\n");
    Se n < insertion_threshold => usar insertion

    Pegar valor maximo de profundidade
    Se a profundidade degradar para 0(n) => usar heap_sort

    /*Chamar estimate_disorder*/
    /*Se a desordem dos dados > estimate_disorder => usar Heap_sort

    /* Função recursiva interna  */

    /*Chamar função recursiva */
//}

// Função de particionamento (Lomuto) para o QuickSort.
// Recebe o vetor 'a', limites 'low' e 'high', e as métricas 'm' para contagem.
static size_t partition(int *a, size_t low, size_t high, Metrics *m) {
    int pivot = a[high]; // Escolhe o último elemento como pivô
    size_t i = low;      // 'i' é o índice do elemento "menor" (onde o pivô será colocado no final)

    for (size_t j = low; j < high; j++) {
        m->comps++; // Incrementa o contador de comparações a cada checagem
        // Se o elemento atual 'a[j]' é menor que o pivô, ele pertence à seção de "menores"
        if (a[j] < pivot) {
            // Troca a[i] e a[j] para mover a[j] (menor que o pivô) para a esquerda
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            m->moves += 3; // Contabiliza 3 movimentações (troca)
            i++;           // Avança o índice do elemento "menor"
        }
    }

    // Coloca o pivô (originalmente em a[high]) na sua posição correta (entre os menores e maiores)
    int temp = a[i];
    a[i] = a[high];
    a[high] = temp;
    m->moves += 3;

    return i;
}

// Função recursiva do IntroSort.
// Controla a profundidade ('depth_limit') para mudar para HeapSort se necessário.
static void introsort_recursive(int *a, ssize_t low, ssize_t high, int depth_limit,
                                const HybridParams *p, Metrics *m){
    if (high <= low)
        return;

    size_t n = high - low + 1;

    // CASO HÍBRIDO 1: Subvetor pequeno (Cut-off) → usa InsertionSort
    // InsertionSort é mais rápido para vetores pequenos devido à baixa sobrecarga.
    if (n <= p->insertion_threshold) {
        insertion_sort(a + low, n, m);
        return;
    }
    // CASO HÍBRIDO 2: Limite de profundidade atingido → usa HeapSort
    // Previne o pior caso O(N^2) do QuickSort (quando o pivô é sempre o menor/maior).
    if (depth_limit <= p->max_depth_factor) {
        heap_sort(a + low, n, m);
        return;
    }
    // CASO GERAL: Particionamento e recursão do QuickSort
    // Reduz o limite de profundidade para o próximo nível de recursão.
    size_t pivot = partition(a, low, high, m);
    // Recorre para a parte esquerda (elementos <= pivô)
    introsort_recursive(a, low, pivot > 0 ? pivot - 1 : 0, depth_limit - 1, p, m);
    // Recorre para a parte direita (elementos > pivô)
    introsort_recursive(a, pivot + 1, high, depth_limit - 1, p, m);
}

// Função principal de ordenação híbrida.
void hybrid_sort(int *a, size_t n, const HybridParams *p, Metrics *m){
    // CASO BÁSICO: Se o vetor inicial é pequeno, vai direto para InsertionSort.
    if(n <= p->insertion_threshold){
        insertion_sort(a, n, m);
    } else {
        // PASSO DE ADAPTATIVIDADE: Estima o grau de desordem do vetor completo (0 a 1)
        double disorder = estimate_disorder(a, n, /* samples */ n/1000 + 5); 

        // CASO ADAPTATIVO: Se o vetor grande está "quase ordenado" (disorder baixo).
        // Isso explora a adaptatividade O(N+I) do InsertionSort, que é eficiente
        // mesmo para N grande se I (inversões) for pequeno.
        if (disorder < p->disorder_switch) {
            insertion_sort(a, n, m);
        } else {
            // CASO PRINCIPAL: Vetor grande e desordenado.
            // Usa o IntroSort, que combina a velocidade do QuickSort com a robustez do HeapSort.
            // Calcula a profundidade máxima: fator * log2(n).
            int max_depth = (int)(p->max_depth_factor * log2((double)n));
            introsort_recursive(a, 0, n - 1, max_depth, p, m); // Inicia o IntroSort
        }
    }
}
