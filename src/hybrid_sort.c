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

void hybrid_sort(int *a, size_t n, const HybridParams *p, Metrics *m) {
    printf("Hybrid sort em desenvolvimento...\n");
    /*Se n < insertion_threshold => usar insertion*/
    
    /*Pegar valor maximo de profundidade*/

    /*Chamar estimate_disorder*/
    /*Se a desordem dos dados > estimate_disorder => usar Heap_sort*/

    /* Função recursiva interna  */

    /*Chamar função recursiva */
    
}


