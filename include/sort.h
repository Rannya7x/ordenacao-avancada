#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    long long comps;
    long long moves;
    double seconds;
} Metrics;

typedef struct {
    int *data;
    size_t n;
} IntArray;

/* ---------- Medidores ---------- */
void metrics_reset(Metrics *m);
void metrics_accumulate(Metrics *dst, const Metrics *src);

/* ---------- Clássicos (baseline) ---------- */
void insertion_sort(int *a, size_t n, Metrics *m);
void merge_sort(int *a, size_t n, Metrics *m);
void quick_sort(int *a, size_t n, Metrics *m);
void heap_sort(int *a, size_t n, Metrics *m);

/* ---------- Espaço p/ os alunos (híbrido/adaptativo) ---------- */
typedef struct {
    /* Parâmetros ajustáveis do algoritmo da dupla */
    size_t insertion_threshold; /* exemplo: fallback */
    double disorder_switch;     /* 0..1: ponto de troca por desordem */
    int max_depth_factor;       /* para introsort (opcional) */
} HybridParams;

void hybrid_sort(int *a, size_t n, const HybridParams *p, Metrics *m);

#endif
