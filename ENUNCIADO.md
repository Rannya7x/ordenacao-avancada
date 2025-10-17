# 🧩 Trabalho Prático 02 – Ordenação Avançada e Estratégias Adaptativas (C)

## 🎯 Objetivo
Projetar, implementar e analisar algoritmos de ordenação **eficientes e adaptativos**, capazes de lidar com grandes volumes e diferentes padrões de entrada. Avaliar estabilidade, adaptatividade, sensibilidade a cache e decisões híbridas.

## 🧠 Competências
- Projeto e justificativa de algoritmos híbridos/adaptativos;
- Instrumentação de código (tempo, comparações, movimentações);
- Análise empírica rigorosa e discussão crítica;
- Comunicação técnica (relatório).

## ⚙️ Tarefas
1. **Algoritmo avançado (dupla):**
   - Escolher **uma** abordagem: *Híbrido* (ex.: Quick+Insertion), *Adaptativo* (ex.: IntroSort), *Linear otimizado* (ex.: Radix por blocos), ou *Paralelo* (opcional).
   - Implementar em `src/hybrid_sort.c`.
   - O algoritmo deve **ajustar estratégia** conforme o **grau de desordem** estimado (vide `util.h: estimate_disorder()`), o **tamanho do subproblema**, e/ou restrições de memória.

2. **Baselines para comparação:**
   - Usar pelo menos **dois** entre: Insertion, Merge, Quick, Heap (já implementados em sala).

3. **Cenários de entrada:**
   - `--input sorted`, `--input reverse`, `--input random` (vide gerador em `util.c`).

4. **Avaliação experimental:**
   - Tamanhos sugeridos: `1e4`, `1e5`, `1e6` (ajuste conforme seu hardware).
   - Métricas coletadas por execução: **comparações (comps)**, **movimentações (moves)**, **tempo (s)**.
   - CSV gerado pelo binário (use `--bench` e `--csv`).

5. **Relatório (máx. 5 páginas):**
   1. Introdução e motivação;
   2. Descrição do algoritmo (pseudocódigo e fluxograma);
   3. Metodologia experimental (hardware, datasets, parâmetros);
   4. Resultados (tabelas, gráficos) e discussão (ponto de troca, estabilidade, memória);
   5. Conclusões e limitações.

## 🧾 Critérios (10,0)
- **3,0** Projeto e justificativa do algoritmo (originalidade, correção teórica);
- **3,0** Implementação e modularização em C (clareza, eficiência, comentários);
- **3,0** Análise experimental e discussão (consistência, leitura crítica);
- **1,0** Clareza e organização do relatório.


## 🚀 Execução
- Execução única (um algoritmo/um tamanho):
  ```bash
  ./bin/tp02 --algo quick --n 100000 --input random --seed 42
  ```
- Benchmark (CSV com vários tamanhos):
  ```bash
  ./bin/tp02 --bench --csv out.csv --input random --sizes 10000,50000,100000
  ```

## 🗂️ Entrega
- Código completo da dupla;
- `README` com instruções e como reproduzir experimentos;
- `relatorio.pdf` (até 5 páginas) segundo o template;
- CSV(s) gerados e gráficos.

**Prazo e submissão:** conforme anunciado em sala/AVA.

___________________________________________________________________________

# 📏 Grau de Desordem (como é calculado)

Esta seção explica como o **grau de desordem** de um vetor é estimado no esqueleto do TP02 e como usar esse valor para **decidir estratégias adaptativas** de ordenação.

## Definição (intuição e formalismo)

* Para um vetor (A) de tamanho (n), uma **inversão** é um par de índices $((i,j))$ com $(i<j)$ tal que $(A[i] > A[j])$.
* O **grau de desordem verdadeiro** pode ser definido como a fração de pares invertidos:

  $
  d^\star = \frac{{(i,j):, 0\le i<j<n,\ A[i]>A[j]}}{\binom{n}{2}} \in [0,1].
  $

  * Vetor estritamente crescente → ($d^\star = 0$)
  * Vetor estritamente decrescente → ($d^\star \approx 1$)
  * Permutação aleatória (sem empates) → ($\mathbb{E}[d^\star]\approx 0{,}5$)

Computar ($d^\star$) exatamente custa ($O(n\log n)$) (via contagem de inversões com merge sort). Para **decisão rápida**, usamos uma **estimativa amostral** ( $\hat d$ ) de custo ($O(S)$), onde ($S$) é o número de pares amostrados.

## Estimador amostral usado no código

A função `estimate_disorder(const int *a, size_t n, size_t samples)`:

1. Sorteia **pares aleatórios** de posições ($(i,j)$) (com ($i \ne j$)); conta-se um teste por par.
2. Para cada par, incrementa o contador de inversões se o par está invertido (isto é, se o elemento “mais à esquerda” é maior que o “mais à direita”).
3. Retorna a fração **inversões / testes**:
   
   $
   \hat d = \frac{\text{inversões amostradas}}{\text{pares testados}} \in [0,1].
   $

### Pseudocódigo (equivalente)

```
inv = 0; tot = 0
repetir S vezes:
    i ← índice aleatório em [0, n-1]
    j ← índice aleatório em [0, n-1], j ≠ i
    (l, r) ← (min(i, j), max(i, j))
    if A[l] > A[r]: inv++
    tot++
retornar inv / tot
```

### Complexidade

* **Tempo:** (O(S)) (linear no número de amostras)
* **Espaço:** (O(1))

## Escolha de (S) (tamanho da amostra)

No esqueleto, usamos por padrão:

```
samples = min(n, 5 + n/10000)
```

Racional:

* Para **n pequenos**, garantir ao menos algumas dezenas de testes.
* Para **n grandes**, crescer lentamente (custo quase constante) e ainda assim capturar tendências globais.

> Você pode **aumentar `samples`** (ex.: ($S = 0{.}001\cdot n$) ou ($S = \min(n, 1000)$)) para reduzir a variância quando a decisão entre estratégias é sensível.

## Propriedades e interpretação

* **Intervalo:** ($\hat d \in [0,1]$).
* **Casos canônicos:**

  * Ordenado crescente → ($\hat d \approx 0$)
  * Ordenado decrescente → ($\hat d \approx 1$)
  * Aleatório (sem empates) → ($\hat d \approx 0{.}5$)
* **Empates (valores iguais):** no código, pares com ($A[i] = A[j]$) **não contam como inversão**; isso tende a **reduzir** ($\hat d$) em vetores com muitos empates. Se quiser penalizar empates, você pode:

  * contar ($A[i] \ge A[j]$) como “meia inversão”, ou
  * ignorar pares com empate (como já é feito) porém **ajustar o limiar** de decisão.

## Uso para algoritmos adaptativos

A estimativa ($\hat d$) guia a escolha da estratégia:

* **Quase ordenado:** se ($\hat d$) menor que um **limiar** (t) (ex.: `disorder_switch = 0.35`), usar **InsertionSort** (ou TimSort/híbrido) — costuma ser superior nesse regime.
* **Geral:** caso contrário, usar **Quick/Intro/Heap** conforme seu projeto .

### Limiar recomendado

* Valor inicial razoável: `t ≈ 0.30–0.40`.
* **Ajuste empírico:** meça desempenho em seus dados e refine `t`.

  * Se o híbrido escolhe Insertion muitas vezes sem ganho → diminua `t`.
  * Se deixa de aproveitar casos quase ordenados → aumente `t`.

## Boas práticas

* **Semente do RNG:** fixe `--seed` para reprodutibilidade.
* **Repetições:** para métricas estáveis, rode múltiplas vezes e **médias** no CSV.
* **Sensibilidade:** se a decisão muda ao redor do limiar, aumente `samples` para reduzir ruído.
* **Documente no relatório**: o valor de `samples`, o limiar (t), e como eles afetam a decisão.

---

### Exemplo de quadro de decisão (ilustrativo)

|  ($\hat d$) estimado | Estratégia sugerida                                           |
| -----------------: | ------------------------------------------------------------- |
|      $([0; 0{.}15])$ | InsertionSort completo (ou TimSort)                           |
| $((0{.}15; 0{.}35])$ | Híbrido: partições pequenas com Insertion; demais Quick/Intro |
|      $((0{.}35; 1])$ | IntroSort (Quick + Heap)                                      |

> Ajuste as faixas conforme seus experimentos. O importante é **explicar e justificar** no relatório.
