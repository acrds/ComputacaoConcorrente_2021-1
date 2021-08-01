## Avaliação de Dsempenho - LAB 3
Codigo: Implementar uma solução sequencial e uma solução concorrente para o problema de encontrar o menor e o maior valor em um  vetor de números reais

Foi executado 8 vezes cada código

# Rodando com N=10⁵
 ```
   Qtd    |  Tm seq  | Tm conc  | Tseq/Tconc média
1 thread  | 0,001057 | 0,001595 | ++++
2 threads | 0,001245 | 0,001297 | 0,959759482
4 threads | 0,001495 | 0,001385 | 1,079422383

 ```

# Rodando com N=10⁷
 ```
   Qtd    |  Tm seq  | Tm conc  | Tseq/Tconc média
1 thread  | 0,028670 | 0,034709 | ++++
2 threads | 0,031243 | 0,019412 | 1,60946837
4 threads | 0,027558 | 0,014726 | 1,871383947


 ```

# Rodando com N=10⁹
 ```
   Qtd    |  Tm seq  | Tm conc  | Tseq/Tconc média
1 thread  | 2,604070 | 2,674850 | ++++
2 threads | 2,612606 | 1,443537 | 1,809864243
4 threads | 2,616837 | 0,812105 | 3,22228899


 ```
 
 Resultados esperados já que a execuções que tiveram maior desempenho foram o com 4 threads pelo meu computador ter 4 core
