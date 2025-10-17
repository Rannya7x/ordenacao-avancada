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


