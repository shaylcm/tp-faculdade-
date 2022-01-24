#ifndef FINDFILE_H
#define FINDFILE_H
#include <time.h>

#define PATH_MAX 4096
#define TRUE 1
#define FALSE 0

struct dirent *dir,*sd;// dir->d_name
struct stat s;
struct tm *tm;
struct  tm  *timeinfo;






typedef struct
{
    int numarq;
    char diretorio[260];
    int quantsubd;
    char inicio[9];
    char termino[9];
    double duracao;
}Tipo_relatorio;








//-------------------------PROTÓTIPOS DAS FUNÇÕES ---------------------------/

int metodo_processo(int  argc ,char *argv[],long int *shared_memory,int k); 
void search_in_file2(char *fname ,char *str,long int *shared_memory,long int *ocorre,int k);
char* stristr( const char* str1, const char* str2 ); 
int extensao(char const *name,char *ext);
void processo_filho_pesquisa(char *diretorioarq,char argv[],int i,long int *shared_memory,Tipo_relatorio *e,long int ocorre,int k,int processo);
int relatorio(Tipo_relatorio e, int metodo);
int metodo_Multithread(int  argc ,char *argv[] ,long int *shared_memory ,int k);
void *pesquisar(void* armazenar);


#endif //FINDFILE_H