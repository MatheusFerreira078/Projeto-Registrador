#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[30];
    char genero[20];
    int ano;
    char tamanho[20];
} Jogo;

long tamanho(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    return ftell(fp) / sizeof(Jogo);
}

void cadastrar(FILE *fp) {
    Jogo j;
    fseek(fp, 0, SEEK_END);

    printf("Nome do jogo: ");
    fgets(j.nome, 30, stdin);
    j.nome[strcspn(j.nome, "\n")] = 0;

    printf("Genero: ");
    fgets(j.genero, 20, stdin);
    j.genero[strcspn(j.genero, "\n")] = 0;

    printf("Ano: ");
    scanf("%d", &j.ano);
    getchar();

    printf("Tamanho (ex: 2GB, 500MB): ");
    fgets(j.tamanho, 20, stdin);
    j.tamanho[strcspn(j.tamanho, "\n")] = 0;

    fwrite(&j, sizeof(Jogo), 1, fp);
    printf("Jogo cadastrado.\n");
}

void consultar(FILE *fp) {
    long total = tamanho(fp);
    if (total == 0) { printf("Nenhum registro.\n"); return; }

    long pos;
    printf("Indice do jogo: ");
    scanf("%ld", &pos);
    getchar();

    if (pos < 1 || pos > total) { printf("Índice inválido.\n"); return; }

    Jogo j;
    fseek(fp, (pos - 1) * sizeof(Jogo), SEEK_SET);
    fread(&j, sizeof(Jogo), 1, fp);

    printf("\nNome: %s\nGenero: %s\nAno: %d\nTamanho: %s\n",
           j.nome, j.genero, j.ano, j.tamanho);
}

void listar(FILE *fp) {
    long total = tamanho(fp);
    if (total == 0) { printf("Nenhum registro.\n"); return; }

    fseek(fp, 0, SEEK_SET);
    Jogo j;

    printf("\n=============================================================\n");
    printf("| %-3s | %-30s | %-10s | %-4s | %-8s |\n",
           "N", "Nome", "Genero", "Ano", "Tam");
    printf("=============================================================\n");

    for (long i = 0; i < total; i++) {
        fread(&j, sizeof(Jogo), 1, fp);
        printf("| %-3ld | %-30s | %-10s | %-4d | %-8s |\n",
               i + 1, j.nome, j.genero, j.ano, j.tamanho);
    }

    printf("=============================================================\n");
}

void excluir(FILE *fp) {
    long total = tamanho(fp);
    if (total == 0) { printf("Nenhum registro para excluir.\n"); return; }

    long pos;
    printf("Indice do registro a excluir: ");
    scanf("%ld", &pos);
    getchar();

    if (pos < 1 || pos > total) { printf("Índice inválido.\n"); return; }

    FILE *temp = fopen("temp.dat", "wb");
    if (!temp) { printf("Erro.\n"); return; }

    fseek(fp, 0, SEEK_SET);
    Jogo j;

    for (long i = 0; i < total; i++) {
        fread(&j, sizeof(Jogo), 1, fp);
        if (i != (pos - 1)) fwrite(&j, sizeof(Jogo), 1, temp);
    }

    fclose(fp);
    fclose(temp);

    remove("jogos.dat");
    rename("temp.dat", "jogos.dat");

    fp = fopen("jogos.dat", "rb+");

    printf("Registro excluído.\n");
}

void gerar_txt(FILE *fp) {
    long total = tamanho(fp);
    if (total == 0) { printf("Nenhum registro para gerar arquivo.\n"); return; }

    FILE *txt = fopen("relatorio_jogos.txt", "w");
    if (!txt) { printf("Erro ao criar arquivo.\n"); return; }

    fseek(fp, 0, SEEK_SET);
    Jogo j;

    fprintf(txt, "LISTA DE JOGOS CADASTRADOS\n\n");
    fprintf(txt, "%-3s | %-30s | %-15s | %-4s | %-8s\n",
            "N", "Nome", "Genero", "Ano", "Tamanho");
    fprintf(txt, "-------------------------------------------------------------\n");

    for (long i = 0; i < total; i++) {
        fread(&j, sizeof(Jogo), 1, fp);

        fprintf(txt, "%-3ld | %-30s | %-15s | %-4d | %-8s\n",
                i + 1, j.nome, j.genero, j.ano, j.tamanho);
    }

    fclose(txt);

    printf("Arquivo relatorio_jogos.txt gerado com sucesso.\n");
}

int main() {
    FILE *fp = fopen("jogos.dat", "rb+");
    if (!fp) fp = fopen("jogos.dat", "wb+");

    int op;

    do {
        printf("\n=== SISTEMA DE JOGOS ===\n");
        printf("1 - Cadastrar Jogo\n");
        printf("2 - Consultar por indice\n");
        printf("3 - Listar todos\n");
        printf("4 - Mostrar numero de registros\n");
        printf("5 - Excluir registro\n");
        printf("6 - Gerar arquivo texto\n");
        printf("0 - Sair\n");
        printf("Escolha um desses itens para continuar: ");
        scanf("%d", &op);
        getchar();

        switch(op) {
            case 1: cadastrar(fp); break;
            case 2: consultar(fp); break;
            case 3: listar(fp); break;
            case 4: printf("Total de registros: %ld\n", tamanho(fp)); break;
            case 5: excluir(fp); fp = fopen("jogos.dat", "rb+"); break;
            case 6: gerar_txt(fp); break;
        }

    } while(op != 0);

    fclose(fp);
    return 0;
}
