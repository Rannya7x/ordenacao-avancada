#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "sort.h"
#include "util.h"

typedef enum { IN_RANDOM, IN_SORTED, IN_REVERSE } InputKind;

static void make_input(int *a, size_t n, InputKind k) {
    if (k == IN_RANDOM) fill_array_random(a, n);
    else if (k == IN_SORTED) fill_array_sorted(a, n);
    else fill_array_reverse(a, n);
}

static const char* kind_str(InputKind k) {
    return k == IN_RANDOM ? "random" : (k == IN_SORTED ? "sorted" : "reverse");
}

static void run_single(const char *algo, InputKind k, size_t n, unsigned seed, FILE *csv) {
    seed_rng(seed);
    int *a = (int*)malloc(n * sizeof(int));
    if (!a) { fprintf(stderr, "Erro: sem memória para n=%zu\n", n); exit(2); }
    make_input(a, n, k);

    Metrics m = {0};
    if (strcmp(algo,"insertion")==0)       insertion_sort(a, n, &m);
    else if (strcmp(algo,"merge")==0)      merge_sort(a, n, &m);
    else if (strcmp(algo,"quick")==0)      quick_sort(a, n, &m);
    else if (strcmp(algo,"heap")==0)       heap_sort(a, n, &m);
    else if (strcmp(algo,"hybrid")==0) {
        HybridParams p = { .insertion_threshold = 24, .disorder_switch = 0.35, .max_depth_factor = 2 };
        hybrid_sort(a, n, &p, &m);
    } else {
        fprintf(stderr, "Algoritmo desconhecido: %s\n", algo);
        free(a); exit(3);
    }

    if (csv) csv_row(csv, algo, kind_str(k), n, &m);
    else printf("%s,%s,%zu,%.9f,%lld,%lld\n", algo, kind_str(k), n, m.seconds, m.comps, m.moves);

    free(a);
}

/* parse lista de tamanhos "10000,50000,100000" em um vetor alocado */
static size_t* parse_sizes(const char *s, size_t *count) {
    char *tmp = strdup(s);
    size_t cap = 8, n = 0;
    size_t *arr = (size_t*)malloc(cap*sizeof(size_t));
    char *tok = strtok(tmp, ",");
    while (tok) {
        size_t v = (size_t)strtoull(tok, NULL, 10);
        if (n==cap) { cap*=2; arr = (size_t*)realloc(arr, cap*sizeof(size_t)); }
        arr[n++] = v;
        tok = strtok(NULL, ",");
    }
    free(tmp);
    *count = n;
    return arr;
}

int main(int argc, char **argv) {
    const char *algo = "quick";
    InputKind k = IN_RANDOM;
    size_t n = 100000;
    unsigned seed = 123;
    bool bench = false;
    const char *csv_path = NULL;
    const char *sizes_str = NULL;

    for (int i=1;i<argc;++i) {
        if (strcmp(argv[i],"--algo")==0 && i+1<argc) { algo = argv[++i]; }
        else if (strcmp(argv[i],"--n")==0 && i+1<argc) { n = (size_t)strtoull(argv[++i], NULL, 10); }
        else if (strcmp(argv[i],"--input")==0 && i+1<argc) {
            const char *v = argv[++i];
            if (strcmp(v,"random")==0) k = IN_RANDOM;
            else if (strcmp(v,"sorted")==0) k = IN_SORTED;
            else if (strcmp(v,"reverse")==0) k = IN_REVERSE;
            else { fprintf(stderr, "input inválido: %s\n", v); return 1; }
        }
        else if (strcmp(argv[i],"--seed")==0 && i+1<argc) { seed = (unsigned)strtoul(argv[++i], NULL, 10); }
        else if (strcmp(argv[i],"--bench")==0) { bench = true; }
        else if (strcmp(argv[i],"--csv")==0 && i+1<argc) { csv_path = argv[++i]; }
        else if (strcmp(argv[i],"--sizes")==0 && i+1<argc) { sizes_str = argv[++i]; }
        else if (strcmp(argv[i],"--help")==0) {
            printf("Uso:\n");
            printf("  %s --algo [insertion|merge|quick|heap|hybrid] --n N --input [random|sorted|reverse] [--seed S]\n", argv[0]);
            printf("  %s --bench --csv OUT.csv --input [random|sorted|reverse] --sizes 10000,50000,100000 --algo [quick|merge|heap|hybrid]\n", argv[0]);
            return 0;
        }
    }

    if (!bench) {
        run_single(algo, k, n, seed, NULL);
        return 0;
    }

    if (!csv_path || !sizes_str) {
        fprintf(stderr, "Modo --bench requer --csv e --sizes\n");
        return 1;
    }

    FILE *csv = fopen(csv_path, "w");
    if (!csv) { perror("csv"); return 1; }
    csv_header(csv);

    size_t cnt = 0;
    size_t *sizes = parse_sizes(sizes_str, &cnt);
    for (size_t i=0;i<cnt;++i) {
        run_single(algo, k, sizes[i], seed + (unsigned)i, csv);
    }
    fclose(csv);
    free(sizes);
    return 0;
}
