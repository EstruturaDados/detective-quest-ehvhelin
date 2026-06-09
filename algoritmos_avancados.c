#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* =========================================================
   DETECTIVE QUEST - NÍVEL MESTRE
   Árvores + BST + Tabela Hash
   ========================================================= */

#define TAM_HASH 10

/* ================= ÁRVORE DA MANSÃO ================= */

/* Sala da mansão (árvore binária) */
typedef struct Sala
{
    char nome[30];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* ================= BST DE PISTAS ================= */

typedef struct NoBST
{
    char pista[50];
    struct NoBST *esq;
    struct NoBST *dir;
} NoBST;

/* ================= HASH DE SUSPEITOS ================= */

typedef struct NoHash
{
    char pista[50];
    char suspeito[30];
    struct NoHash *prox;
} NoHash;

/* Tabela hash global */
NoHash *tabela[TAM_HASH];

/* ================= FUNÇÕES AUXILIARES ================= */

/* Função hash simples baseada em soma ASCII */
int hash(char *str)
{
    int soma = 0;

    for(int i = 0; str[i] != '\0'; i++)
        soma += str[i];

    return soma % TAM_HASH;
}

/* ================= MANSÃO (ÁRVORE BINÁRIA) ================= */

/* Cria uma sala */
Sala* criarSala(char nome[])
{
    Sala *novo = (Sala*) malloc(sizeof(Sala));

    strcpy(novo->nome, nome);
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

/* Exploração da mansão */
void explorarSalas(Sala *atual)
{
    char opcao;

    while(atual != NULL)
    {
        printf("\nVoce esta em: %s\n", atual->nome);

        if(atual->esq == NULL && atual->dir == NULL)
        {
            printf("Fim do caminho!\n");
            break;
        }

        printf("Ir esquerda (e), direita (d) ou sair (s): ");
        scanf(" %c", &opcao);

        if(opcao == 'e')
            atual = atual->esq;

        else if(opcao == 'd')
            atual = atual->dir;

        else
            break;
    }
}

/* ================= BST DE PISTAS ================= */

/* Inserir pista na BST */
NoBST* inserirBST(NoBST *raiz, char pista[])
{
    if(raiz == NULL)
    {
        NoBST *novo = (NoBST*) malloc(sizeof(NoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }

    if(strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else
        raiz->dir = inserirBST(raiz->dir, pista);

    return raiz;
}

/* Exibir pistas em ordem */
void emOrdem(NoBST *raiz)
{
    if(raiz != NULL)
    {
        emOrdem(raiz->esq);
        printf("- %s\n", raiz->pista);
        emOrdem(raiz->dir);
    }
}

/* ================= HASH DE SUSPEITOS ================= */

/* Inicializa tabela hash */
void inicializarHash()
{
    for(int i = 0; i < TAM_HASH; i++)
        tabela[i] = NULL;
}

/* Inserir relação pista → suspeito */
void inserirHash(char pista[], char suspeito[])
{
    int pos = hash(suspeito);

    NoHash *novo = (NoHash*) malloc(sizeof(NoHash));

    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);

    novo->prox = tabela[pos];
    tabela[pos] = novo;
}

/* Mostrar associações */
void listarHash()
{
    printf("\n===== SUSPEITOS E PISTAS =====\n");

    for(int i = 0; i < TAM_HASH; i++)
    {
        NoHash *aux = tabela[i];

        while(aux != NULL)
        {
            printf("Suspeito: %s | Pista: %s\n",
                   aux->suspeito,
                   aux->pista);

            aux = aux->prox;
        }
    }
}

/* Suspeito mais citado */
void suspeitoMaisCitado()
{
    char nomes[100][30];
    int cont[100] = {0};
    int total = 0;

    for(int i = 0; i < TAM_HASH; i++)
    {
        NoHash *aux = tabela[i];

        while(aux != NULL)
        {
            int encontrado = -1;

            for(int j = 0; j < total; j++)
            {
                if(strcmp(nomes[j], aux->suspeito) == 0)
                {
                    encontrado = j;
                    break;
                }
            }

            if(encontrado == -1)
            {
                strcpy(nomes[total], aux->suspeito);
                cont[total]++;
                total++;
            }
            else
            {
                cont[encontrado]++;
            }

            aux = aux->prox;
        }
    }

    int max = 0;

    for(int i = 1; i < total; i++)
    {
        if(cont[i] > cont[max])
            max = i;
    }

    printf("\n===== SUSPEITO MAIS PROVAVEL =====\n");
    printf("%s com %d pistas\n",
           nomes[max],
           cont[max]);
}

/* ================= MAIN ================= */

int main()
{
    /* ================= MANSÃO ================= */

    Sala *hall = criarSala("Hall");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *sotao = criarSala("Sotao");
    Sala *quarto = criarSala("Quarto");

    hall->esq = biblioteca;
    hall->dir = cozinha;

    biblioteca->esq = quarto;
    biblioteca->dir = NULL;

    cozinha->dir = sotao;

    /* ================= BST ================= */

    NoBST *raiz = NULL;

    raiz = inserirBST(raiz, "pegadas");
    raiz = inserirBST(raiz, "digital");
    raiz = inserirBST(raiz, "arma");

    /* ================= HASH ================= */

    inicializarHash();

    inserirHash("pegadas", "Carlos");
    inserirHash("digital", "Ana");
    inserirHash("arma", "Carlos");
    inserirHash("dna", "Carlos");

    /* ================= JOGO ================= */

    printf("\n=== EXPLORACAO DA MANSÃO ===\n");
    explorarSalas(hall);

    printf("\n=== PISTAS ENCONTRADAS ===\n");
    emOrdem(raiz);

    listarHash();

    suspeitoMaisCitado();

    return 0;
}
