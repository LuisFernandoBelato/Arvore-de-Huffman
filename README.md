# 📂 Codificador/Decodificador Huffman por Palavras em C \ ***5º Termo - Pesquisa e Ordenação***

Este projeto implementa o algoritmo de codificação de Huffman para comprimir e descomprimir frases com base na frequência das palavras (e espaços), conforme as especificações do trabalho de **Estruturas de Dados II**. O projeto consiste em dois programas C++ separados: um codificador (Huffman Parte 1.cpp) e um decodificador (Huffman Parte 2.cpp).

## Visão Geral do Projeto

1.  **Programa Codificador:**

    *   Analisa uma frase de entrada para construir uma árvore de Huffman baseada na frequência de suas palavras e espaços.
    *   Gera códigos de Huffman para cada palavra/espaço.
    *   Armazena uma tabela contendo os símbolos, palavras, frequências e seus respectivos códigos de Huffman em um **arquivo binário**.
    *   Codifica uma *outra frase* (diferente da usada para construir a árvore) usando os códigos gerados.
    *   Salva a sequência de bits resultante (sem espaços) em um **arquivo de texto**.
      
3.  **Programa Decodificador:**

    *   Lê a tabela de códigos Huffman do arquivo binário.
    *   Lê a frase codificada (sequência de bits) do arquivo de texto.
    *   Reconstrói a árvore de Huffman (ou usa a tabela diretamente para mapeamento) para decodificar a sequência de bits.
    *   Exibe a frase original decodificada na tela.
