/*1. Sistema de Biblioteca
Crie um sistema para cadastro e busca de livros. Cada livro deve ter título, autor, ano de publicação e número de exemplares.
- Cadastro de livros.
- Empréstimo de livros (diminui o número de exemplares).
- Devolução de livros.
- Busca por título ou autor.*/

//    -----bibliotecas-----
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define max_titulo 50
#define max_nome 100
#define arqBin "livaria.bin"

typedef struct
{
    int id;
    char titulo[max_titulo];
    char autor[max_nome];
    int publicacao;
    int estoque;
}Livro;


//  ------- Funções -------
void cadastrar(FILE *);
void emprestimo(FILE *);
void devolucao(FILE *);
void buscar_nome(FILE *);
void converte_txt(FILE *);
void limpa_buff(void);
int show_Menu(void);

int main()
{
    setlocale(LC_ALL, "portuguese");
    Livro vazio = {0,"","",0,0};
    FILE *ptFl;
    int op;

    if((ptFl = fopen(arqBin, "r+")) == NULL)
    {
        if((ptFl = fopen(arqBin, "w+")) == NULL)
        {
            printf("\n%45s Erro ao abrir arquivo binario!!!", "");
            return 1;
        }
        else
        {
            for(int i = 0; i < 100; i++)
            {
                fwrite(&vazio, sizeof(Livro), 1, ptFl);
            }
            rewind(ptFl);
        }

        //rewind(ptFl);
        printf("\n%50s Arquivo preencido com sucesso!!!", "");
    }
    while((op = show_Menu()) != 6)
    {
        switch (op)
        {
            case 1:
                cadastrar(ptFl);
                break;
            case 2:
                emprestimo(ptFl);
                break;
            case 3:
                devolucao(ptFl);
                break;
            case 4:
                buscar_nome(ptFl);
                break;
            case 5:
                converte_txt(ptFl);
                break;
            case 6:
                printf("\n%50s Saindo...","");
                break;
            default:
                printf("\n%50s Opecao invalida!!!", "");
                break;
        }
    }

    fclose(ptFl);
    return 0;
}

int show_Menu()
{
    int op;

    printf("\n%45s =============================", "");
    printf("\n%50s MENU PRINCIPAL", "");
    printf("\n%45s =============================", "");
    printf("\n%50s [1]Cadastro de livros", "");
    printf("\n%50s [2]Emprestimo de livros", "");
    printf("\n%50s [3]Devolucao de livros", "");
    printf("\n%50s [4]Busca por titulo ou autor", "");
    printf("\n%50s [5]Converter para txt", "");
    printf("\n%50s [6]Sair", "");
    printf("\n%45s =============================", "");
    printf("\n%50s Opcao:", "");

    scanf("%d", &op);
    return op;
}

void cadastrar(FILE *ptFl)
{
    Livro livros;
    int new_id;

    printf("\n%45s =============================", "");
    printf("\n%50s Cadastrar livro", "");
    printf("\n%45s =============================", "");

    printf("\n%50s Informe o id do novo livro:", "");
    scanf("%d", &new_id);
    limpa_buff();
    printf("\n%45s =============================", "");

    fseek(ptFl, (new_id-1) * sizeof(livros), SEEK_SET);
    fread(&livros, sizeof(Livro), 1, ptFl);

    if(livros.id != 0)
    {
        printf("\n%50s Conta ja existe!", "");
    }else
    {
        printf("\n%50s Informe:", "");
        printf("\n%50s titulo:", "");
        //fgets(livros.titulo, max_titulo, stdin);
        fgets(livros.titulo, sizeof(livros.titulo), stdin);
        livros.titulo[strcspn(livros.titulo, "\n")] = '\0';
        limpa_buff();

        printf("\n%50s autor:", "");
        //fgets(livros.autor, max_nome, stdin);
        fgets(livros.autor, sizeof(livros.autor), stdin);
        livros.autor[strcspn(livros.autor, "\n")] = '\0';
        limpa_buff();

        printf("\n%50s ano de publicacao:", "");
        scanf("%d", &livros.publicacao);
        limpa_buff();

        printf("\n%50s estoque:", "");
        scanf("%d", &livros.estoque);
        limpa_buff();
        printf("\n%45s =============================", "");
        livros.id = new_id;

        fseek(ptFl, (new_id - 1) * sizeof(Livro), SEEK_SET);
        fwrite(&livros, sizeof(Livro), 1, ptFl);
    }

}

void emprestimo(FILE *ptFl)
{
    Livro livros;
    int id, quant_livro;

    printf("\n%45s =============================", "");
    printf("\n%50s EMPRESTIMO DE LIVROS", "");
    printf("\n%45s =============================", "");
    printf("\n%50s Qual o id do livro", "");
    printf("\n%50s Opcao:", "");
    scanf("%d", &id);
    limpa_buff();
    printf("\n%45s =============================", "");

    fseek(ptFl, (id - 1) * sizeof(Livro), SEEK_SET);
    fread(&livros, sizeof(Livro), 1, ptFl);

    if(livros.id == 0)
    {
        printf("\n%50s Id de livro nao cadastrado ainda!!!", "");
    }else
    {
        printf("\n%50s Livro selecionado:", "");
        printf("\n%50s %-6d%-16s%-16s%-6d%-6d", "", livros.id,
                                                livros.titulo,
                                                livros.autor,
                                                livros.publicacao,
                                                livros.estoque);
        printf("\n%50s Quantidade do livro '%s' a ser levado:", "", livros.titulo);
        scanf("%d", &quant_livro);
        limpa_buff();
        printf("\n%45s =============================", "");

        livros.estoque -= quant_livro;

        fseek(ptFl, (id - 1)*sizeof(Livro), SEEK_SET);
        fwrite(&livros, sizeof(Livro), 1, ptFl);

        printf("\n%50s Estoque ajustado:", "");
        printf("\n%50s %-6d%-16s%-16s%-6d%-6d", "", livros.id,
                                                livros.titulo,
                                                livros.autor,
                                                livros.publicacao,
                                                livros.estoque);
        printf("\n%45s =============================", "");
    }

}

void devolucao(FILE *ptFl)
{
    Livro livros;
    int id, quant_devol;
    printf("\n%45s =============================", "");
    printf("\n%50s DEVOLUCAO DE LIVROS", "");
    printf("\n%45s =============================", "");
    printf("\n%50s Informe o  id do livro devolvido:", "");
    scanf("%d", &id);
    limpa_buff();
    printf("\n%45s =============================", "");

    fseek(ptFl, (id - 1) * sizeof(Livro), SEEK_SET);
    fread(&livros, sizeof(Livro), 1, ptFl);

    if(livros.id == 0)
    {
        printf("\n%50s Id de livro nao existente!!!", "");
    }else
    {
        printf("\n%50s Informe a quantidade de livros devolvidos:", "");
        scanf("%d", &quant_devol);
        limpa_buff();
        printf("\n%45s =============================", "");

        livros.estoque +=  quant_devol;
        fseek(ptFl, (id - 1) * sizeof(Livro), SEEK_SET);
        fwrite(&livros, sizeof(Livro), 1, ptFl);

        printf("\n%50s Estoque ajustado:", "");
        printf("\n%50s %-6d%-16s%-16s%-6d%-6d", "", livros.id,
                                                livros.titulo,
                                                livros.autor,
                                                livros.publicacao,
                                                livros.estoque);
        printf("\n%45s =============================", "");
    }

}

void buscar_nome(FILE *ptFl)
{
    Livro livros[100];
    int valida=0;
    char nome[max_titulo];

    printf("\n%45s =============================", "");
    printf("\n%50s BUSCA DE LIVRO POR NOME", "");
    printf("\n%45s =============================", "");
    printf("\n%50s Informe o nome do titulo/autor:", "");
    limpa_buff();
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';
    printf("\n%45s =============================", "");

    rewind(ptFl);
    for(int i = 0; i < 100; i++)
    {
        fseek(ptFl, i * sizeof(Livro), SEEK_SET);
        fread(&livros[i], sizeof(Livro), 1, ptFl);
    }


    for(int  i = 0; i<100; i++)
    {
       if(strcmp(livros[i].titulo, nome) == 0 || strcmp(livros[i].autor, nome) == 0)
        {
            printf("\n%50s Livro achado:", "");
            printf("\n%50s %-6d%-16s%-16s%-6d%-6d", "", livros[i].id,
                                                livros[i].titulo,
                                                livros[i].autor,
                                                livros[i].publicacao,
                                                livros[i].estoque);
            printf("\n%45s =============================", "");
            valida++;
        }

    }

    if(valida == 0)
        printf("\n%50s Nome de livro NAO encontrado!!!", "");
    if(valida == 1)
        printf("\n%50s Nome de livro encontrado!!!", "");
    if(valida > 1)
        printf("\n%50s Nome tem dois ou mais cadastros!!!", "");
}

void converte_txt(FILE *ptFl)
{
    Livro livros;
    FILE *grava;

    if((grava =  fopen("converte.txt", "w")) == NULL)
    {
        printf("\n%50s Arquivo txt nao consegue abrir", "");
    }else
    {
        rewind(ptFl);
        fprintf(grava, "%-6s%-16s%-11s%-6s%-6s\n", "ID", "TITULO", "AUTOR", "DATA", "ESTOQUE");

        while(fread(&livros, sizeof(Livro), 1, ptFl) == 1)
        {
            if(livros.id != 0)
            {
                fprintf(grava, "%-6d%-16s%-11s%-6d%-6d\n", livros.id,  livros.titulo, livros.autor, livros.publicacao, livros.estoque);
            }
        }
        fclose(grava);
    }
}

void limpa_buff()
{
    char c;
    while((c = getchar()) != '\n'  && c !=EOF);
}

