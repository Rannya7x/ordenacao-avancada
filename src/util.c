#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "util.h"

/* ---------- Metrics ---------- */
void metrics_reset(Metrics *m) {
    if (!m) return;
    m->comps = 0;
    m->moves = 0;
    m->seconds = 0.0;
}
void metrics_accumulate(Metrics *dst, const Metrics *src) {
    if (!dst || !src) return;
    dst->comps += src->comps;
    dst->moves += src->moves;
    dst->seconds += src->seconds;
}

/* ---------- RNG / Geradores ---------- */
static unsigned int g_seed = 1234567u;

void seed_rng(unsigned int seed) { g_seed = seed ? seed : 1234567u; }
static inline unsigned int xorshift32(void) {
    unsigned int x = g_seed;
    x ^= x << 13; x ^= x >> 17; x ^= x << 5;
    g_seed = x;
    return x;
}

void fill_array_random(int *a, size_t n) {
    for (size_t i=0;i<n;++i) a[i] = (int)(xorshift32() & 0x7fffffff);
}

void fill_array_sorted(int *a, size_t n) {
    for (size_t i=0;i<n;++i) a[i] = (int)i;
}

void fill_array_reverse(int *a, size_t n) {
    for (size_t i=0;i<n;++i) a[i] = (int)(n - 1 - i);
}

/* ---------- Grau de desordem ---------- */
double estimate_disorder(const int *a, size_t n, size_t samples) {
    if (n < 2) return 0.0;
    if (samples > n) samples = n;
    /* amostra pares aleatórios e estima fração de inversões */
    size_t inv = 0, tot = 0;
    for (size_t s=0; s<samples; ++s) {
        size_t i = xorshift32() % n;
        size_t j = xorshift32() % n;
        if (i == j) continue;
        if (i < j) { tot++; if (a[i] > a[j]) inv++; }
        else       { tot++; if (a[j] > a[i]) inv++; }
    }
    if (tot == 0) return 0.0;
    return (double)inv / (double)tot; /* 0..1 */
}

/* ---------- Cronometria ---------- */
double now_seconds(void) {
#if defined(_WIN32) || defined(_WIN64)
    return (double)clock() / (double)CLOCKS_PER_SEC;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;
#endif
}

/* ---------- CSV ---------- */
void csv_header(FILE *f) {
    fprintf(f, "algo,input,n,seconds,comps,moves\n");
}
void csv_row(FILE *f, const char *algo, const char *input_kind, size_t n, const Metrics *m) {
    fprintf(f, "%s,%s,%zu,%.9f,%lld,%lld\n",
        algo, input_kind, n, m?m->seconds:0.0,
        (long long)(m?m->comps:0), (long long)(m?m->moves:0));
}
