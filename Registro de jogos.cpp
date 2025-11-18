#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define ARQUIVO "jogos.dat"

typedef struct {
    char nome[50];
    char genero[30];
    int ano;
    char tamanho[20];  
} Jogo;


void limpaBuffer(void);
long tamanho(FILE *fp);
void cadastrar(FILE *fp);
void consultar(FILE *fp);
void listar_todos(FILE *fp);

int main(void) {
    FILE *fp;

    
    fp = fopen(ARQUIVO, "r+b");
    if (fp == NULL) {
        fp = fopen(ARQUIVO, "w+b");
        if (fp == NULL) {
            fprintf(stderr, "Erro: não foi possível abrir/criar o arquivo.\n");
            return 1;
        }
    }

    int opc;
    do {
        printf("\n=== SISTEMA DE JOGOS ===\n");
        printf("1 - Cadastrar Jogo\n");
        printf("2 - Consultar por indice\n");
        printf("3 - Listar todos\n");
        printf("4 - Mostrar numero de registros\n");
        printf("0 - Sair\n");
        printf("Escolha uma dessas opções para continuar: ");
        if (scanf("%d", &opc) != 1) {
            printf("Entrada inválida.\n");
            limpaBuffer();
            continue;
        }
        limpaBuffer();

        switch (opc) {
            case 1: cadastrar(fp); break;
            case 2: consultar(fp); break;
            case 3: listar_todos(fp); break;
            case 4: {
                long n = tamanho(fp);
                printf("Total de registros: %ld\n", n);
                break;
            }
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n");
        }
    } while (opc != 0);

    fclose(fp);
    return 0;
}

void limpaBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

long tamanho(FILE *fp) {
    long pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long bytes = ftell(fp);
    fseek(fp, pos, SEEK_SET);
    return bytes / sizeof(Jogo);
}

void cadastrar(FILE *fp) {
    Jogo j;

    printf("Nome do jogo: ");
    fgets(j.nome, sizeof(j.nome), stdin);
    j.nome[strcspn(j.nome, "\n")] = '\0';

    printf("Genero: ");
    fgets(j.genero, sizeof(j.genero), stdin);
    j.genero[strcspn(j.genero, "\n")] = '\0';

    printf("Ano de lancamento: ");
    if (scanf("%d", &j.ano) != 1) {
        printf("Ano inválido.\n");
        limpaBuffer();
        return;
    }
    limpaBuffer();

    printf("Tamanho do jogo (exemplos:450MB,2GB): ");
    fgets(j.tamanho, sizeof(j.tamanho), stdin);
    j.tamanho[strcspn(j.tamanho, "\n")] = '\0';

    
    fseek(fp, 0, SEEK_END);
    fwrite(&j, sizeof(Jogo), 1, fp);
    fflush(fp);

    printf("Jogo cadastrado com sucesso!\n");
}

void consultar(FILE *fp) {
    long n = tamanho(fp);
    if (n == 0) {
        printf("Nenhum registro cadastrado.\n");
        return;
    }

    printf("Indice do registro (1 a %ld): ", n);
    long idx;
    if (scanf("%ld", &idx) != 1) {
        printf("Entrada inválida.\n");
        limpaBuffer();
        return;
    }
    limpaBuffer();

    if (idx < 1 || idx > n) {
        printf("Indice inválido.\n");
        return;
    }

    Jogo j;
    fseek(fp, (idx - 1) * sizeof(Jogo), SEEK_SET);
    fread(&j, sizeof(Jogo), 1, fp);

    printf("\n--- Registro %ld ---\n", idx);
    printf("Nome: %s\n", j.nome);
    printf("Genero: %s\n", j.genero);
    printf("Ano: %d\n", j.ano);
    printf("Tamanho: %s\n", j.tamanho);
}

void listar_todos(FILE *fp) {
    long n = tamanho(fp);
    if (n == 0) {
        printf("Nenhum registro cadastrado.\n");
        return;
    }

    fseek(fp, 0, SEEK_SET);
    Jogo j;

    printf("\n--- LISTA DE JOGOS ---\n");
    for (long i = 0; i < n; i++) {
        fread(&j, sizeof(Jogo), 1, fp);
        printf("\n[%ld]\n", i + 1);
        printf("Nome: %s\n", j.nome);
        printf("Genero: %s\n", j.genero);
        printf("Ano: %d\n", j.ano);
        printf("Tamanho: %s\n", j.tamanho);
    }
}
