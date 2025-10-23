#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h> /* ssize_t */
#include "sort.h"
#include "util.h"


void insertion_sort(int *a, size_t n, Metrics *m){
    for (size_t i = 1; i < n; i++) {
        int key = a[i];
        m->moves++;
        size_t j = i;
        while (j > 0) {
            m->comps++;
            if (a[j - 1] > key) {
                a[j] = a[j - 1];
                m->moves++;
                j--;
            } else {
                break;
            }
        }
        a[j] = key;
        m->moves++;
    }
}
void merge_sort(int *a, size_t n, Metrics *m){
    if (n < 2) return;

    size_t mid = n / 2;
    merge_sort(a, mid, m);
    merge_sort(a + mid, n - mid, m);

    int *temp = (int *)malloc(n * sizeof(int));
    size_t i = 0, j = mid, k = 0;

    while (i < mid && j < n) {
        m->comps++;
        if (a[i] <= a[j]) {
            temp[k++] = a[i++];
            m->moves++;
        } else {
            temp[k++] = a[j++];
            m->moves++;
        }
    }

    while (i < mid) {
        temp[k++] = a[i++];
        m->moves++;
    }

    while (j < n) {
        temp[k++] = a[j++];
        m->moves++;
    }

    memcpy(a, temp, n * sizeof(int));
    free(temp);
}
void quick_sort(int *a, size_t n, Metrics *m){
    if (n < 2) return;

    int pivot = a[n - 1];
    size_t i = 0;

    for (size_t j = 0; j < n - 1; j++) {
        m->comps++;
        if (a[j] < pivot) {
            int temp = a[i];
            a[i] = a[j];
            a[j] = temp;
            m->moves += 3;
            i++;
        }
    }

    int temp = a[i];
    a[i] = a[n - 1];
    a[n - 1] = temp;
    m->moves += 3;

    quick_sort(a, i, m);
    quick_sort(a + i + 1, n - i - 1, m);
}
// Função auxiliar para "afundar" um elemento na heap
void heapify(int *a, size_t n, size_t i, Metrics *m) {
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    if (left < n) {
        m->comps++;
        if (a[left] > a[largest]) {
            largest = left;
        }
    }

    if (right < n) {
        m->comps++;
        if (a[right] > a[largest]) {
            largest = right;
        }
    }

    if (largest != i) {
        int temp = a[i];
        a[i] = a[largest];
        a[largest] = temp;
        m->moves += 3;
        heapify(a, n, largest, m);
    }
}

void heap_sort(int *a, size_t n, Metrics *m) {
    // Construir a heap
    for (ssize_t i = (n / 2) - 1; i >= 0; i--) {
        heapify(a, n, i, m);
    }

    // Extrair elementos da heap
    for (ssize_t i = n - 1; i > 0; i--) {
        int temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        m->moves += 3;
        heapify(a, i, 0, m);
    }
}