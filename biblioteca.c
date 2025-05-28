/*1. Sistema de Biblioteca
Crie um sistema para cadastro e busca de livros. Cada livro deve ter título, autor, ano de publicação e número de exemplares.
- Cadastro de livros.
- Empréstimo de livros (diminui o número de exemplares).
- Devolução de livros.
- Busca por título ou autor.*/

//    -----bibliotecas-----
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

//    -----define------
#define max_titulo 500
#define max_autor 100
#define LIVROS "livros_cadastrados.bin"

//    ----structs------
typedef struct
{
    char titulo[max_titulo];
    char autor[max_autor];
    int ano_publicacao;
    int numero_exemplares;
}Livro;

//    -----variaveis globais---
Livro *livros = NULL;
int num_Livros_cadast = 0;// salva_num_livro;

//    -----Funcion------
void cadastrar_livros();
void salva_livros_arq();
void emprestimo_livros();
void ajuste_estoque(Livro *temp[], int i, int livros_levados);
void quant_livros_est();
void valida_abertura_arq(FILE *arq);
int menu_Principla();

int main()
{
    setlocale(LC_ALL, "portuguese");

    int opcao = menu_Principla();



    switch(opcao)
    {
        case 1:
            cadastrar_livros();
            salva_livros_arq();
        break;

        case 2:
            emprestimo_livros();
            quant_livros_est();
        break;

        case 3:
            quant_livros_est();
        break;

    }
    return 0;
}

void cadastrar_livros()
{
    int opcao_invalida=0;
    char valida_saida;

    do
    {
        Livro *temp = realloc(livros, (num_Livros_cadast + 1) * sizeof(Livro));
        if(temp == NULL){printf("\n%45sErro ao alocar memoria", ""); return;}

        livros = temp;

        int c;
        while((c = getchar()) != '\n' && c !=  EOF);

        printf("\n%45s==============================", "");
        printf("\n%50s CADASTRAR LIVROS", "");
        printf("\n%45s==============================", "");

        printf("\n%50s TITULO: ", "");
        fgets(livros[num_Livros_cadast].titulo, sizeof(max_titulo), stdin);
        livros[num_Livros_cadast].titulo[strcspn(livros[num_Livros_cadast].titulo, "\n")] = '\0';
        while((c = getchar()) != '\n' && c !=  EOF);

        printf("\n%50s NOME AUTOR: ", "");
        fgets(livros[num_Livros_cadast].autor, max_autor, stdin);
        livros[num_Livros_cadast].autor[strcspn(livros[num_Livros_cadast].autor, "\n")]  = '\0';

        printf("\n%50s ANO PUBLICACAO: ", "");
        scanf("%d", &livros[num_Livros_cadast].ano_publicacao);

        printf("\n%50s NUMERO EXEMPLARES: ", "");
        scanf("%d", &livros[num_Livros_cadast].numero_exemplares);

        do
        {
            while ((c = getchar()) != '\n' && c != EOF);
            if(opcao_invalida > 0){printf("\n%45sOpcao invalida!!! insira(s/n)", "");}

            printf("\n%50s DESEJA ADICIONAR MAIS LIVROS (s/n): ", "");
            scanf("%c", &valida_saida);
            opcao_invalida++;
        } while (valida_saida != 's' && valida_saida != 'n');
        opcao_invalida=0;

        num_Livros_cadast++;
    } while (valida_saida != 'n');
    //int *p = &salva_num_livro;
    //p  =  num_Livros_cadast;

}

void salva_livros_arq()
{
    FILE *arq = fopen(LIVROS, "ab");
    valida_abertura_arq(arq);

    fwrite(livros, sizeof(Livro), num_Livros_cadast, arq);

    fclose(arq);
}

void emprestimo_livros()
{
    int livros_levados, x=0;
    char nome_livro[max_titulo];
    Livro *temp = (Livro *) malloc(num_Livros_cadast * sizeof(Livro));

    FILE *arq =  fopen(LIVROS, "rb");
    valida_abertura_arq(arq);

    printf("\n%45s==============================", "");
    printf("\n%50s Emprestimo de Livro", "");
    printf("\n%45s==============================", "");

    printf("\n%50s Qual livro deseja levar: ", "");
    fgets(nome_livro, sizeof(max_titulo), stdin);
    nome_livro[strcspn(nome_livro, "\n")] =  '\0';
    getchar();

    fread(&temp, sizeof(Livro), num_Livros_cadast, arq);

    for(int i = 0; i < num_Livros_cadast; i++)
    {
        if(nome_livro == temp[i].titulo)
        {
            x = x + 1;
            printf("\n%50s Quantidade de exeplares velados: ", "");
            scanf("%d", &livros_levados);
            getchar();

            ajuste_estoque(&temp, i, livros_levados);
            livros[i].numero_exemplares = temp[i].numero_exemplares;
        }
    }
    if(x == 0){printf("\n%45s  Erro ao alterar estoque!!!","");}

    free(temp);
    fclose(arq);
}

//                  Livro *temp, [].  != Livro *temp[],  []->
void ajuste_estoque(Livro *temp[], int i, int livros_levados)
{
    Livro troca;

    FILE *arq = fopen(LIVROS, "a+b");
    valida_abertura_arq(arq);

    temp[i]->numero_exemplares -= livros_levados;

    strcpy(troca.titulo, temp[i]->titulo);
    strcpy(troca.autor, temp[i]->autor);
    //troca.titulo = temp[i]->titulo;
    //troca.autor = temp[i]->autor;
    troca.ano_publicacao = temp[i]->ano_publicacao;
    troca.numero_exemplares =  temp[i]->numero_exemplares;

    fseek(arq, i * -sizeof(Livro), SEEK_SET);
    fwrite(&troca, sizeof(Livro), 1, arq);

    fclose(arq);
}

void quant_livros_est()
{
    Livro *temp = (Livro*) malloc(num_Livros_cadast  * sizeof(Livro));

    FILE *arq =fopen(LIVROS, "rb");
    valida_abertura_arq(arq);

    fread(&temp, sizeof(Livro), num_Livros_cadast, arq);

    for(int i = 0; i < num_Livros_cadast; i++)
    {
        printf("\n%45s==============================", "");
        printf("\n%50s LIVROS EM ESTOQUE", "");
        printf("\n%45s==============================", "");
        printf("\n%50s %s: %d", "", temp[i].titulo, temp[i].numero_exemplares);
        printf("\n%45s==============================", "");
    }

    fclose(arq);
}

void valida_abertura_arq(FILE *arq)
{
    if(arq != NULL)
        printf("\nArquico abreto com sucesso!!");
    else
    {
        printf("\nErro ao abrir arquivo!!!");
        fclose(arq);
    }

}

int menu_Principla()
{
    int opcao;

    printf("\n%45s==============================", "");
    printf("\n%50sMENU PRINCIPAL", "");
    printf("\n%45s==============================", "");

    printf("\n%50s[1] Cadastro de livros", "");
    printf("\n%50s[2] Emprestimo de livros", "");
    printf("\n%50s[3] Devolucao de livros ", "");
    printf("\n%50s[4] Busca por titulo ou autor", "");
    printf("\n%45s==============================", "");

    scanf("%d", &opcao);
    return opcao;
}


