/*2. Gerenciador de Contatos
Crie um sistema de agenda que permite:
- Cadastrar contatos (nome, telefone, e-mail).
- Listar todos os contatos.
- Buscar contato por nome.
- Remover contato.*/

// ----- includes/define-----
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define max 100
#define arq "contato.bin"

// ------struct -----
typedef struct
{
    char nome[max];
    int telefone;
    char email[max];
}Contato;



void create_Accont(FILE  *);
void list(FILE  *);
void search(FILE *);
void remover(FILE *);
void clean_buff();
void limpa();
int show_men(void);
void remove_newline(char *str);


int main()
{
    Contato vazio = {"",0,""};
    FILE *ptFl;
    int op=0;

   // if((ptFl = fopen(arq, "r")) == NULL)
   // {
        if((ptFl = fopen(arq,"w")) ==  NULL)
        {
            printf("\n%50s Erro ao abrir arquivo binario!!!","");
            return 1;
        }else
        {
            for(int i = 0; i<50; i++)
            {
                fwrite(&vazio, sizeof(Contato), 1, ptFl);
            }
            rewind(ptFl);
        }

        printf("\n%50s Arquivo preencido com sucesso!!!", "");
    //}

    while(op !=  5)
    {
        //limpa();
        switch(op = show_men())
        {
        case 1:
            create_Accont(ptFl);
            break;
        case 2:
            list(ptFl);
            break;
        case 3:
            search(ptFl);
            break;
        case 4:
            remover(ptFl);
            break;
        case 5:
            printf("\n%50s Saindoo do programa.....","");
            break;

        default:
            printf("\n%50s Opcao invalida!!!", "");
            break;
        }
    }
}

void limpa()
{
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux/macOS
    #endif

}

void create_Accont(FILE *ptFl)
{
    Contato conta;
    Contato temp;
    int pos_vazia = -1;

    printf("\n%45s =======================================:", "");
    printf("\n%50s CADASTRO DE CONTATOS:", "");
    printf("\n%45s =======================================:", "");
    printf("\n%50s Prenxa as infoemcoes do contato:", "");
    printf("\n%50s Nome:", "");
    clean_buff();
    fgets(conta.nome, sizeof(conta.nome), stdin);
    remove_newline(conta.nome);

    printf("\n%50s Telefone:", "");
    scanf("%d", &conta.telefone);
    clean_buff();

    printf("\n%50s Email:", "");
    fgets(conta.email, sizeof(conta.email), stdin);
    remove_newline(conta.email);
    printf("\n%45s =======================================:", "");

    for(int i=0; i<50; i++)
    {
        fseek(ptFl, i *  sizeof(Contato), SEEK_SET);
        fread(&temp, sizeof(Contato), 1, ptFl);
        printf("\n%s", temp.nome);
        if(strcmp(temp.nome, "") == 0)
        {
            pos_vazia = i;
            break;
        }
    }
    if(pos_vazia == -1) {
        printf("\n%50s Agenda cheia! Nao e possivel adicionar mais contatos.", "");
        return;
    }

    fseek(ptFl, pos_vazia * sizeof(Contato), SEEK_SET);
    fwrite(&conta, sizeof(Contato), 1, ptFl);
    printf("\n%50s Contato cadastrado com sucesso!", "");
}

void clean_buff()
{
    char c;
    while((c = getchar()) !=  '\n' && c != EOF);
}

void remove_newline(char *str) {
    int len = strlen(str);
    if(len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

void list(FILE *ptFl)
{
    Contato conta;
    int contador = 0;

    printf("\n%45s =======================================", "");
    printf("\n%50s LISTAGEM DOS CONTATOS", "");
    printf("\n%45s =======================================", "");
    printf("\n%50s Contas:", "");
    for(int i = 0; i<50; i++)
    {
        fseek(ptFl, i *sizeof(Contato), SEEK_SET);
        fread(&conta, sizeof(Contato), 1, ptFl);
        if(strcmp(conta.nome, "") != 0)
        {
            printf("\n%50s %-16s%-11d%16s","", conta.nome,conta.telefone,conta.email);
            contador++;
        }
    }
    if(contador == 0) {
        printf("\n%50s Nenhum contato cadastrado.", "");
    }

    printf("\n%50s Pressione Enter para continuar...", "");
    clean_buff();
}

void search(FILE *ptFl)
{
    Contato temp;
    char name[max];

    printf("\n%45s =======================================:", "");
    printf("\n%50s BUSCA DE CONTATOS", "");
    printf("\n%45s =======================================:", "");
    printf("\n%50s informe o nome do contato:", "");
    clean_buff();
    fgets(name, sizeof(name), stdin);
    remove_newline(name);

    for(int i=0; i<50; i++)
    {
        fseek(ptFl, i * sizeof(Contato), SEEK_SET);
        fread(&temp, sizeof(Contato), 1, ptFl);

        if(strcmp(temp.nome, name) == 0)
        {
            printf("\n%45s =======================================", "");
            printf("\n%50s Conta foi achada:", "");
            printf("\n%50s %-16s%-11d%16s","", temp.nome, temp.telefone, temp.email);
            printf("\n%45s =======================================", "");
        }
    }

    printf("\n%45s =======================================", "");
    printf("\n%50s Pressione Enter para continuar...", "");
    clean_buff();
}

void remover(FILE *ptFl)
{
    Contato temp;
    Contato vazio = {"",0,""};
    char name[max];
    int x=0;

    printf("\n%45s =======================================", "");
    printf("\n%50s REMOVER  CONTATO", "");
    printf("\n%45s =======================================", "");
    printf("\n%50s Informe o nome da conta a ser removida:", "");
    clean_buff();
    fgets(name, sizeof(name), stdin);
    remove_newline(name);

    for(int i=0; i<50; i++)
    {
        fseek(ptFl, i *sizeof(Contato), SEEK_SET);
        fread(&temp, sizeof(Contato), 1, ptFl);

        if(strcmp(temp.nome, name) ==  0)
        {
            fseek(ptFl, i*sizeof(Contato), SEEK_SET);
            fwrite(&vazio, sizeof(Contato), 1, ptFl);
            x++;
        }
    }

    if(x > 0)
        printf("\n%50s Contato removido...", "");

    printf("\n%50s Pressione Enter para continuar...", "");
    clean_buff();
}

int show_men()
{
    int op;

    printf("\n%45s =======================================", "");
    printf("\n%50s MENU PRINCIPAL", "");
    printf("\n%45s =======================================", "");
    printf("\n%50s [1]Cadastrar contatos", "");
    printf("\n%50s [2]Listar contatos", "");
    printf("\n%50s [3]Buscar contato", "");
    printf("\n%50s [4]Remover contato", "");
    printf("\n%50s [5]Sair", "");
    scanf("%d", &op);

    return op;
}

