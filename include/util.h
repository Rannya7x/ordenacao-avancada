#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> /* FILE* */
#include "sort.h"

/* RNG e geração de entradas */
void seed_rng(unsigned int seed);
void fill_array_random(int *a, size_t n);
void fill_array_sorted(int *a, size_t n);
void fill_array_reverse(int *a, size_t n);

/* Grau de desordem (estimativa entre 0 e 1) */
double estimate_disorder(const int *a, size_t n, size_t samples);

/* Cronometria */
double now_seconds(void);

/* I/O CSV */
void csv_header(FILE *f);
void csv_row(FILE *f, const char *algo, const char *input_kind, size_t n, const Metrics *m);

/* Utilitários de contagem */
static inline void count_swap_int(int *a, int *b, Metrics *m) {
    int tmp = *a; *a = *b; *b = tmp;
    if (m) m->moves += 3;
}
static inline int count_cmp_int(int x, int y, Metrics *m) {
    if (m) m->comps += 1;
    return (x - y);
}

#endif
