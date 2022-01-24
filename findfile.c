//---BIBLIOTECAS----//
#include <dirent.h> //para as ações referente a diretorios
#include <stdio.h> //entrada e saida padrão
#include <stdlib.h> //malloc
#include <string.h> //para as strings 
#include <ctype.h>  //para funções como tollower
#include <sys/types.h>//temporização
//#include <sys/stat.h>//
#include <sys/wait.h>//para espera
#include <sys/shm.h>//para memoria compartilhada
#include <fcntl.h>
#include <errno.h>//para detecção de de erros mais precisa
#include <unistd.h>//acesso a ficheiros e diretorios
#include <time.h>//para mostrar as horas,minutos e segundos
#include <pthread.h>//para as threads
#include "findfile.h"//biblioteca com os prototipos da funções e definições gerais


//------------------------------------------------VARIAVEIS GLOBAIS---------------------------------------// 
long int ocorrencia;//usada na função pesquisar no metodo multithread
long int linhasss[100]; //usada na função pesquisar no multithread
//--------------------------------------STRUCT PARA ARMAZENAR OS DADOS PARA PTHREAD-----------------------//
struct armazena
{
    char *fname ;
    char *str;
    long int *shared_memory;
    long int ocorre;
    int k;
    int i;
     int line;
    int find_result;
    char *result;
    char temp[512];
    long int linhas[100];
    char *argv[1];
};
//------------------------------FUNÇÃO PESQUISA STRING POR MEKO DE THREADS--------------------------------//
void *pesquisar(void* armazenar)
{
 struct armazena *ar=(struct armazena *)armazenar;
 
    ar->ocorre=0;
    ar->line=1;
    FILE *pFile;

     pFile=fopen(dir->d_name,"r");
     
      while (!feof(pFile))//lendo ate o final do arquivo 
    {
        if(fgets(ar->temp,512,pFile)!=NULL)
        {               
         ar->result = stristr(ar->temp,ar->str);
       if (ar->result)
        {
             ar->ocorre=ar->ocorre +1;
             ocorrencia=ar->ocorre;//variavel global em ação
             ar->linhas[ar->k]=ar->line;
             linhasss[ar->k]=ar->linhas[ar->k];//variave global em ação
             ar->k++;
             ar->find_result++;   
       }
       ar->line ++;  
     }
    }
    free(armazenar);//liberando area para não haver sobreposição
    pthread_exit(NULL);
}
//----------------------------FUNÇÃO BUSCA EXTENÇÃO-----------------------------------//
int extensao(char const *name,char *ext)
{
    size_t len = strlen(name);
    if (len > 4 && strcmp(name + len - 4, ext) == 0)
        return 1;
    else 
    return 0;
}

//----------------------------FUNÇÃO PESQUISA ANALOGA A STRSTR-------------------------//
//----------------------------POREM NAO DIFERENCIA MAIUSCULA DE MINUSCULA-------------//
//essa função realiza a busca de string

char* stristr( const char* str1, const char* str2 )
{
    const char* p1 = str1 ;
    const char* p2 = str2 ;
    const char* r = *p2 == 0 ? str1 : 0 ;

    while( *p1 != 0 && *p2 != 0 )
    {
        if( tolower( (unsigned char)*p1 ) == tolower( (unsigned char)*p2 ) )
        {
            if( r == 0 )
            {
                r = p1 ;
            }

            p2++ ;
        }
        else
        {
            p2 = str2 ;
            if( r != 0 )
            {
                p1 = r + 1 ;
            }

            if( tolower( (unsigned char)*p1 ) == tolower( (unsigned char)*p2 ) )
            {
                r = p1 ;
                p2++ ;
            }
            else
            {
                r = 0 ;
            }
        }

        p1++ ;
    }

    return *p2 == 0 ? (char*)r : 0 ;
}

//---------------------------- PROCESSO FILHO PESQUISA-------------------------------//
void processo_filho_pesquisa(char *diretorioarq,char argv[],int i,long int *shared_memory,Tipo_relatorio *e,long int ocorre,int k,int processo)
{
    int id;


   
    //criando processo fillho
      id=fork();
    //verificando se houve erro na criação do processo filho
    if (id<0)
    
        printf("\nerro na criação do processo filho");
     if (id==0)
     {
        

    /*como não consegui implementar uma solução para criar somente 4 processos deixo aqui a representação
    de como seria visualmente*/
    
         if(i<5)
         {
      // printf("\tPID: %d\t",getpid());
        printf("\n processo %d pesquisando  no arquivo: %s\n",i, diretorioarq);
         }
       if (i>=5)
       {
         i=5;
       // printf("\tPID: %d\t",getpid());
        printf("\n processo %d pesquisando  no arquivo: %s\n",i, diretorioarq);
       }
        
         search_in_file2(diretorioarq,argv,shared_memory,&ocorre,k);
         exit(0);
         
        
        
     
        
    }
    


    }

//----------------------------FUNÇÃO ENCONTRA STRING------------------------/
void search_in_file2(char *fname ,char *str,long int *shared_memory,long int *ocorre,int k)
{
    FILE *pFile;


    int line=1;
    int find_result=0;
    char *result;
    char temp[512];


    *ocorre=0;
    long int linhas[100];
   
     pFile=fopen(fname,"r");
    
      while (!feof(pFile))//lendo ate o final do arquivo 
    {
        if(fgets(temp,512,pFile)!=NULL)//passando o conteudo para a variavel temp.
        {   
         result = stristr(temp,str);//comparando o conteudo com a string fornecida
        if (result)
        {
            //printf("\n string encontrada na linha %d ",line);
             *ocorre=*ocorre +1;
             shared_memory[0]=*ocorre;
            linhas[k]=line;
            shared_memory[k]=linhas[k];
              k++;
            //printf("\n%s",temp);
            find_result++;    
       }
       line ++;  
     }
    }    
}
//-----------------------------------METODO PROCESSO---------------------------------//

int metodo_processo(int  argc ,char *argv[] ,long int *shared_memory ,int k)
{
            DIR *d,*sub;
            Tipo_relatorio e;
            char nomeDire[4096],buffer[100],buffer2[100],diretorio[4096],nomearq[100],vetordechar[100];
            int line[1000],*resul,*resul2,i=1,j=1,processoTrab=FALSE;
            const char *dire,*caminho;
            int quantarq=0,quantisub=0;
            long int quantidade;
            size_t comprimentovetor=100;
            time_t agora = time(NULL);
            k=1;
        
            strcpy(nomearq,"Findfile.log");
            FILE *pFile;
            FILE *findfile;
            strftime(vetordechar,comprimentovetor,"%F  %Hh %Mm %Ss",localtime(&agora));//temporização e data atual
            findfile = fopen (nomearq,"w+"); //abre o arquivo
        if (argc==2)
            dire=getcwd(buffer2,100);//se argumento for igual a 2 ,sera contabilizado o diretorio corrente
            
        if (argc==3)
            dire=argv[2];//se argumento for igual a 3 ,sera contabilizado o diretorio fornecido (de preferencia caminho absoluto)
                
        d = opendir(dire); //diretório atual ou fornecido;

            //Tempo inicial.
            time_t tempoInicial = time(NULL), segundos;
            time(&segundos);
            tm = localtime(&segundos);
            clock_t contagem[2];
            contagem[0] = clock();
            contagem[1] = clock();
            time_t rawtime;
            struct tm * timeinfo;

            //Guarda a hora inicial de execução do programa.
            sprintf(e.inicio, "%.2d:%.2d:%.2d",tm->tm_hour,tm->tm_min,tm->tm_sec);
            sprintf(diretorio,"%s",dire);

            setbuf(findfile,NULL);//limpando o buffer do arquivo para nao dar loop infinito
            fprintf(findfile,"arquivo %s  criado em  %s",nomearq,vetordechar);
            fprintf(findfile,"\nString a ser pesquisada :\"%s\" ",argv[1]);
            fprintf(findfile,"\n-Arquivo de textos pesquisados");  
            
            if (d != NULL)//verificando se o diretorio existe 
            {
                while ((dir = readdir(d)) != NULL) //listar todos os arquivos do diretorio é trabalho do processo pai o programa
                {
                    //"ignora os diretorios . e .. "
                    if (strcmp(dir->d_name,".")==0    ||   strcmp(dir->d_name,"..")==0 )
                        continue;
                    chdir(dire);//acessa o diretorio dire
                    if (extensao(dir->d_name,".txt")==1 || extensao(dir->d_name,".log")|| dir->d_type==DT_DIR)
                    { 
                    if (dir->d_type==DT_DIR)  //checando se é um diretorio 
                    {
                    if(sub=opendir(dir->d_name))//abrindo subdiretorio
                    {
                        quantisub=quantisub+1;//contando o numero de subdireetorios
                        chdir(dir->d_name);//acessa o diretorio 
                       
                        while((dir =readdir(sub))!=NULL)//listando arquivos do subdiretorio
                            { 
                                //"ignora as pasta . e .. "
                                if (strcmp(dir->d_name,".")==0    ||   strcmp(dir->d_name,"..")==0)
                                    continue;
                            
                                if (extensao(dir->d_name,".txt")==1 || extensao(dir->d_name,".log")==1)
                                {  
                                    
                                    FILE *pFile;
                                    pFile = fopen (dir->d_name,"r"); //abre o arquivo
                                    if (pFile!=NULL)
                                        {
                                
                            
                                        quantarq=quantarq+1;//contando os arquivos
                                        
                                     /*como não consegui implementar uma solução para criar somente 4 proccessos deixo aqui minha tentativa*/
                                        if(i==4)
                                            processoTrab=TRUE;
                                        
                                        processo_filho_pesquisa(dir->d_name,argv[1],i,shared_memory,&e,quantidade,k,processoTrab);  //realizar a busca da string é papael dos processos filhos
                                         sleep(1);
                                        i++;
                                        fprintf(findfile,"\n%d . %s",j,dir->d_name);//armazenando no arquivo o nome do arquivo lido
                                        fprintf(findfile,"\nQuantidade :%ld",shared_memory[0]);
                                        fprintf(findfile,"\nLinhas: ");
                                        for(int l=1;l<=shared_memory[0];l++)
                                        {
                                            setbuf(findfile,NULL);
                                            fprintf(findfile,"%ld,",shared_memory[k]);
                                            
                                            k++;
                                        }
                                            fprintf(findfile,"\n\n");
                                        
                                        j++;
                                        //search_in_file2(dir->d_name,argv[1]);//realizar a busca da string é papael dos processos filhos
                                        fclose(pFile);
                                        }
                                        else
                                        printf("\nfalha na abertura %s\n",strerror(errno)); 
                            }
                                        }
                                    closedir(sub);
                        } 
                            chdir("..");   
                         //printf("%s\n", getcwd(buffer, 100));//printar o diretorio corrente 
                    }else
                        {
                                
                           
                            pFile = fopen (dir->d_name,"r"); //abre o arquivo
                            if (pFile!=NULL)
                            {
                            quantarq=quantarq+1;//contando os arquivos do diretorio corrente
                            if (argc<2)
                            {
                                printf("\n poucos argumentos ,faça o favor");

                            }else
                            
                            {
                                
                                 if(i==4)
                                    processoTrab=TRUE;


                                processo_filho_pesquisa(dir->d_name,argv[1],i,shared_memory,&e,quantidade,k,processoTrab);//realizar a busca da string é papael dos processos filho
                                sleep(1);
                                i++;
                                fprintf(findfile,"\n%d . %s",j,dir->d_name);//armazenando no arquivo o nome do arquivo lido
                                fprintf(findfile,"\nQuantidade :%ld",shared_memory[0]);
                                fprintf(findfile,"\nLinhas:");
                                for(int l=1;l<=shared_memory[0];l++)
                                {
                                    
                                    setbuf(findfile,NULL);
                                    fprintf(findfile,"%ld,",shared_memory[k]);
                                    k++;
                                }
                                fprintf(findfile,"\n\n");
                                j++;
                                    fclose (pFile);
                            
                            }
                        }
                        }
                        }    
                    }
                        sprintf(e.diretorio,"%s",dire);
                        e.quantsubd=quantisub;//armazenando o numero de subdiretorios na struct relatorios
                        e.numarq=quantarq;//armazenando o numero de arquivos na strruct relatorio
                        closedir(d);//fechando diretorio
                    }
                    fclose(findfile);

                    //Tempo final.
                    time(&segundos);   
                    tm = localtime(&segundos);
                    sprintf(e.termino, "%.2d:%.2d:%.2d",tm->tm_hour,tm->tm_min,tm->tm_sec); 
                    double tempoSegundos = difftime(time(NULL),tempoInicial);
                    double tempoMS = (contagem[1] - contagem[0]) * 1000.0 / CLOCKS_PER_SEC;
                    e.duracao = tempoSegundos; 
                    relatorio(e,1);
                    printf("\n\n");
                    
    return 0;
}


//------------------------------------METODO MULTITHREADING-------------------------------------------//

int metodo_Multithread(int  argc ,char *argv[] ,long int *shared_memory ,int k)
{
            //----------------------------------VARIAVEIS----------------------------------//
                DIR *d,*sub;
                Tipo_relatorio e;
                char nomeDire[4096], buffer2[100],buffer[100],diretorio[4096],nomearq[100],vetordechar[100];
                int line[1000],*resul,*resul2,quantarq=0,quantisub=0;
                const char *dire,*caminho;
                long int quantidade;
                size_t comprimentovetor=100;
                time_t agora = time(NULL);
                char *guardar;
                struct armazena *armazenar;
                int i=1,j=1;
                k=0;
                strcpy(nomearq,"Findfile_thread.log");
                FILE *pFile;
                FILE *findfile;
                strftime(vetordechar,comprimentovetor,"%F  %Hh %Mm %Ss",localtime(&agora));//TEMPORIZAÇÃO E DATA ATUAL
                findfile = fopen (nomearq,"w+"); //abre o arquivo

            if (argc==2)
                dire=getcwd(buffer2,100);
            if (argc==3)
                dire=argv[2];        
            d = opendir(dire); //diretório atual ou fornecido;

                //Tempo inicial.
                time_t tempoInicial = time(NULL), segundos;
                time(&segundos);
                tm = localtime(&segundos);
                clock_t contagem[2];
                contagem[0] = clock();
                contagem[1] = clock();
                time_t rawtime;
                struct tm * timeinfo;

                //Guarda a hora inicial de execução do programa.
                sprintf(e.inicio, "%.2d:%.2d:%.2d",tm->tm_hour,tm->tm_min,tm->tm_sec);
                sprintf(diretorio,"%s",dire);

                setbuf(findfile,NULL);//limpando o buffer do arquivo para nao dar loop infinito
                fprintf(findfile,"arquivo %s  criado em  %s",nomearq,vetordechar);
                fprintf(findfile,"\nString a ser pesquisada :\"%s\" ",argv[1]);
                fprintf(findfile,"\n-Arquivo de textos pesquisados");  
                
                if (d != NULL)
                {
                   
                    while ((dir = readdir(d)) != NULL) //listar todos os arquivos do diretorio é trabalho do processo pai o programa
                    {
                         
                        //"ignora os diretorios anterios e proximos "
                        if (strcmp(dir->d_name,".")==0    ||   strcmp(dir->d_name,"..")==0 )
                            continue;
                        chdir(dire);
                        if (extensao(dir->d_name,".txt")==1 || extensao(dir->d_name,".log")|| dir->d_type==DT_DIR)
                        { 
                        if (dir->d_type==DT_DIR)//checando se é um diretorio
                        {
                        
                        if(sub=opendir(dir->d_name))//abrindo subdiretorio
                        {
                            quantisub=quantisub+1;//contando o numero de subdireetorios
                            chdir(dir->d_name); 
                            //printf("%s\n", getcwd(buffer, 100)); //printa  diretorio corrente
                                while((dir =readdir(sub))!=NULL)//listando arquivos do subdiretorio
                                {   
                                    //"ignora as pasta . e .. "
                                    if (strcmp(dir->d_name,".")==0    ||   strcmp(dir->d_name,"..")==0)
                                        continue;

                                    if (extensao(dir->d_name,".txt")==1 || extensao(dir->d_name,".log")==1)
                                {  
                                        
                                        FILE *pFile;
                                        pFile = fopen (dir->d_name,"r"); //abre o arquivo
                                        if (pFile!=NULL)
                                            {
                                            
                                        
                                            quantarq=quantarq+1;//contando os arquivos

                                            //aqui criaremos as threads
                                            //----------------------------------ALOCANDO MEMORIA PARA THREAD----------------------------//
                                            
                                            
                                                armazenar=malloc(sizeof(struct armazena));
                                            
                                                armazenar->str=argv[1];
                                            
                                                pthread_t thread;

                                                if(i<5)
                                                printf("\n Thread %d pesquisando  no arquivo: %s\n",i, dir->d_name);
                                                if (i>=5)
                                                {
                                                i=5;
                                                printf("\n Thread %d pesquisando  no arquivo: %s\n",i, dir->d_name);
                                                }
                                                pthread_create(&thread,NULL,pesquisar,(void*)armazenar);//cria thread e realiza busca pela string no arquivo
                                                pthread_join(thread,NULL);
                                                sleep(1);
                                             i++;
                                            
                                            fprintf(findfile,"\n%d . %s",j,dir->d_name);//armazenando no arquivo o nome do arquivo lido
                                            fprintf(findfile,"\nQuantidade :%ld",ocorrencia);
                                            fprintf(findfile,"\nLinhas: ");    
                                            for(int l=1;l<=ocorrencia;l++)
                                            {
 
                                                setbuf(findfile,NULL);
                                                fprintf(findfile,"%ld,",linhasss[k]);
                                                k++;
                                            }
                                                fprintf(findfile,"\n\n");
                                            
                                            j++;
                                            fclose(pFile);
                                            }
                                            else
                                            printf("\nfalha na abertura %s\n",strerror(errno));
                                }
                                        
                                            }
                                        closedir(sub);
                            }  
                                chdir("..");  
                               // printf("%s\n", getcwd(buffer, 100));//printar o diretorio corrente   
                        }else
                        {
                        
                        pFile = fopen (dir->d_name,"r"); //abre o arquivo
                        if (pFile!=NULL)
                        {
                        quantarq=quantarq+1;//contando os arquivos do diretorio corrente
                        
                        if (argc<2)
                            printf("\n poucos argumentos ,faça o favor");

                        else
                        {
                        
                    //aqui criaremos as threads filhas


                    //----------------------------------ALOCANDO MEMORIA PARA THREAD----------------------------//
                                    
                                    
                                        armazenar=malloc(sizeof(struct armazena));//alocando memoria 
                                    
                                        armazenar->str=argv[1];//atribuindo a string ao campo string da struct armazena
                                    
                                        pthread_t thread;
                                        if(i<5)
                                        printf("\n Thread %d pesquisando  no arquivo: %s\n",i, dir->d_name);
                                        if (i>=5)
                                        {
                                        i=5;
                                        printf("\n Thread %d pesquisando  no arquivo: %s\n",i, dir->d_name);
                                        }
                                        pthread_create(&thread,NULL,pesquisar,(void*)armazenar);//cria thread filha  e realiza a busca pela string no arquivo
                                        pthread_join(thread,NULL);
                                        sleep(1);

                                        i++;
                        
                                        fprintf(findfile,"\n%d . %s",j,dir->d_name);//armazenando no arquivo o nome do arquivo lido
                                        fprintf(findfile,"\nQuantidade :%ld",ocorrencia);
                                        fprintf(findfile,"\nLinhas:");
                                        for(int l=1;l<=ocorrencia;l++)
                                        {
                                            setbuf(findfile,NULL);
                                            fprintf(findfile,"%ld,",linhasss[k]);
                                            k++;
                                        }
                                        
                                        fprintf(findfile,"\n\n");
                                        j++;
                                            fclose (pFile);
                                        }
                                    }
                                    }
                                        }  
                                    }
                                    sprintf(e.diretorio,"%s",dire);
                                    e.quantsubd=quantisub;//armazenando o numero de subdiretorios na struct relatorios
                                    e.numarq=quantarq;//armazenando o numero de arquivos na strruct relatorio
                                    
                                    closedir(d);
                                }
                                fclose(findfile);

                                //Tempo final.
                                time(&segundos);   
                                tm = localtime(&segundos);
                                sprintf(e.termino, "%.2d:%.2d:%.2d",tm->tm_hour,tm->tm_min,tm->tm_sec); 
                                double tempoSegundos = difftime(time(NULL),tempoInicial);
                                double tempoMS = (contagem[1] - contagem[0]) * 1000.0 / CLOCKS_PER_SEC;
                                e.duracao = tempoSegundos;
                                relatorio(e,2);
                                printf("\n\n");
                                
    return 0;
}
//------------------------------RELATORIO IPC------------------------------------//

int relatorio(Tipo_relatorio e, int metodo){
    /*Se método for igual a 1 então é o método de comunicação, caso
      contrário é o método de utilizando multithread.*/
    
    if(metodo == 1){
        printf("\n-Método: IPC - Inter-Process Comunication");
        printf("\n-Diretório: %s\n",e.diretorio);

        printf("\n-Conteúdo do diretório");
        printf("\n\tArquivos de texto pesquisados  = %d",e.numarq);
        printf("\n\tNumero total de subdiretórios = %d",e.quantsubd);
       

        printf("\n-Tempo usando IPC");
        printf("\n\tInicio: %s",e.inicio);  
        printf("\n\tTérmino: %s",e.termino);
        printf("\n\tDuração: %.0lf segundos.\n\n",e.duracao);
    }else{
        printf("\n-Método: Multithread");
        printf("\n-Diretório: %s\n",e.diretorio);

        printf("\n-Conteúdo do diretório");
        printf("\n\tArquivos = %d",e.numarq);
        printf("\n\tSubdiretórios = %d",e.quantsubd);
      

        printf("\n-Tempo usando Multithread");
        printf("\n\tInicio: %s",e.inicio);  
        printf("\n\tTérmino: %s",e.termino);
        printf("\n\tDuração: %.0lf segundos.\n\n",e.duracao);
    }
    return 0;
}

int iniciar(int argc ,char *argv[])
{
    int k=1;
    //referente a area de memoria compartilhada 

    int segment_id;
    long int *shared_memory;
    struct shmid_ds shmbuffer;
    int segment_size;
    const int shared_segment_size=0x6400;
//---------------------------------------------------//

/*Aloca o espaço de memoria compartilhada.
    IPC_CREATE: Indica que um novo segmento deve ser criado.
    IPC_EXCL: Faz com que o sistema falhe se uma chave de segmento especificada já existir.
    S_IRUSRand S_IWUSR: especificam permissões de leitura e gravação para o proprietário do segmento de memória compartilhada.
    */
    segment_id = shmget (IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    //Aloca a memória compartilhada.
    shared_memory = (long int*) shmat (segment_id, 0, 0); 
    //Determina o tamanho do segmento.
	shmctl (segment_id, IPC_STAT, &shmbuffer); 
	segment_size = shmbuffer.shm_segsz;  
  //------------------------------------------------------------------------------------//  



    metodo_processo(argc,argv,shared_memory,k);
    metodo_Multithread(argc,argv,shared_memory,k);


      //Desaloca o segmento de memória compartilhada.
    shmctl (segment_id, IPC_RMID, 0);

     return 0; 
}
int main(int argc,char *argv[])
{
    return iniciar(argc,argv);
}