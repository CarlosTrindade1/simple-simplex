import numpy as np

def ler_entrada(nome_arquivo):
    """Lê e interpreta o arquivo do problema."""
    with open(nome_arquivo, 'r') as arquivo:
        linhas = arquivo.readlines()
    
    # Leitura do problema
    p_line = linhas[0].strip().split()
    n, m = int(p_line[1]), int(p_line[2])
    
    # Leitura dos coeficientes da função objetivo
    f_line = linhas[1].strip().split()[1:]
    c = np.array([float(x) for x in f_line])
    
    # Leitura das restrições
    A = []
    b = []
    for i in range(2, 2 + m):
        linha = linhas[i].strip().split()[1:]
        A.append([float(x) for x in linha[:-1]])
        b.append(float(linha[-1]))
    
    A = np.array(A)
    b = np.array(b)
    
    # Leitura das variáveis básicas
    b_line = linhas[2 + m].strip().split()[1:]
    base_indices = [int(i) - 1 for i in b_line]  # Ajuste de índices para Python
    
    return n, m, c, A, b, base_indices

def construir_tableau(n, m, c, A, b, base_indices):
    """Constrói o tableau inicial."""
    tableau = np.zeros((m + 1, n + m + 1))
    
    # Preencher o tableau com a matriz A e o vetor b
    tableau[:-1, :n] = A
    tableau[:-1, -1] = b
    
    # Preencher o tableau com a função objetivo
    tableau[-1, :n] = -c
    
    return tableau

# Funções adicionais como cálculo de custo reduzido, pivoteamento e verificação de otimalidade...
