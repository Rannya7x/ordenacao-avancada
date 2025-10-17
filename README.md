# TP02 — Ordenação Avançada (AED II)

Este pacote contém o enunciado, o esqueleto de código em C, Makefile e templates de relatório para o **Trabalho Prático 02 – Ordenação Avançada e Estratégias Adaptativas**.

## Estrutura
```
tp02_ordenacao_avancada/
  ENUNCIADO.md
  README.md
  Makefile
  include/
    sort.h
    util.h
  src/
    classic_sort.c
    hybrid_sort.c
    util.c
    main.c
  templates/
    report_template.md
```
## Compilação
```bash
make
```
Gera o executável `bin/tp02`.

## Uso rápido
### Execução única
```bash
./bin/tp02 --algo quick --n 100000 --input random --seed 42
```
### Benchmark (gera CSV)
```bash
./bin/tp02 --bench --csv resultados.csv --input random --sizes 10000,50000,100000
```

## Tarefas da dupla
- Implementar e justificar **um algoritmo avançado** (híbrido/adaptativo) em `src/hybrid_sort.c`.
- Medir **tempo**, **comparações** e **movimentações**.
- Comparar com pelo menos **dois algoritmos clássicos** (ex.: Quick, Merge, Heap).
- Gerar CSV e construir gráficos para o relatório.


