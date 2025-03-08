/*----------------------------------------------------------------------
Um cabeçalho bonito, pôr aqui os nossos nomes ou algo do género
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>


//CUIDADO AO MEXER NO NOME DE NENHUMA DESTAS DEFINIÇÕES POR FAVOR!!! PODE ESTRAGAR O CÓDIGO TODO!!!
#define NUMERODEDADOSINICIAIS 14 //tamanho do vetor que guarda os dados do config_modelo.txt
//DEFINICOES DOS DADOS LIDOS DO FICHEIRO config_modelo.txt
#define VETOR_INICIAL vetor_dados_iniciais
#define TF VETOR_INICIAL[0]
#define DT VETOR_INICIAL[1]
#define S VETOR_INICIAL[2]
#define B VETOR_INICIAL[3]
#define M VETOR_INICIAL[4]
#define G VETOR_INICIAL[5]
#define RHO VETOR_INICIAL[6]
#define CD0 VETOR_INICIAL[7]
#define E VETOR_INICIAL[8]
#define ALPHA0 VETOR_INICIAL[9]
#define V0 VETOR_INICIAL[10]
#define GAMMA0 VETOR_INICIAL[11]
#define X0 VETOR_INICIAL[12]
#define H0 VETOR_INICIAL[13]
//ESTAS DEFINICOES SERAO USADAS PARA NOS REFERIRMOS A ESTES VALORES!

//---------------Zona de cria��o de fun��es----------------------//


//Imprime as opcoes que vao correr em ciclo
void opcoes(){
    printf("Lista de opcoes:\n");
    printf("0 - Terminar programa\n");
    printf("1 - Simular movimento da aeronave\n");
    printf("2 - Determinar valores minimos e maximos absolutos de t, v(0), gama, x, h\n");
    printf("Selecionar opcao:");
}


//Caso o ficheiro config_modelo.txt nao exista, esta funcao é chamada para perguntar se o utilizador pretende criar um ficheiro com dados predefinidos (os do enunciado), bastante útil numa primeira utilização, em que seria seria pouco provável o utilizador já ter este ficheiro no diretório em que vai correr o programa, fornecendo-lhe orientação
void criarconfigpredefinida(){
    int op;
    FILE * cfg;
    fprintf(stderr, "ERRO: O FICHEIRO DE CONFIG NAO EXISTE!\n");
    printf("Gostaria de criar um ficheiro com valores predefinidos? (1=sim; 0 ou qualquer outro valor/caracter=nao):");
    scanf("%d",&op);
    if(op==1){
        cfg = fopen("config_modelo.txt","w");
            fprintf(cfg,"%% tf tempo final da simulacao [s]\n6.0\n");
            fprintf(cfg,"%% Passo de integracao dt [s]\n1e-5\n");
            fprintf(cfg,"%% S - area da asa [m2]\n2.0e-2\n");
            fprintf(cfg,"%% b comprimento da asa [m]\n14e-2\n");
            fprintf(cfg,"%% m - massa da aeronave [kg]\n5e-3\n");
            fprintf(cfg,"%% Aceleracao da gravidade [m/s2]\n9.801\n");
            fprintf(cfg,"%% rho - densidade de ar ao nível do mar [kg/m3]\n1.225\n");
            fprintf(cfg,"%% CD0 - valor do coeficiente de drag para Cl = 0\n0.02\n");
            fprintf(cfg,"%% e - factor de eficiencia de Oswald\n0.9\n");
            fprintf(cfg,"%% alpha - angulo de ataque [rad] (cuidado com o valor desta variavel !!!)\n0.1");
            fprintf(cfg,"%% V(0) -velocidade inicial [m/s]\n11.0\n");
            fprintf(cfg,"%% gamma(0) [rad] - flight path angle inicial\n0.0\n");
            fprintf(cfg,"%% x(0) posicao horizontal inicial [m]\n0.0\n");
            fprintf(cfg,"%% h(0) - altitude inicial [m]\n5.0");
        fclose(cfg);
        printf("O ficheiro foi criado com sucesso, pronto a ser usado!\nA voltar ao menu...\n");
    }
    else{
        printf("O ficheiro nao vai ser criado! Espera-se entao que o utilizador crie por conta propria um ficheiro com o nome config_modelo.txt no diretorio em que o executavel do programa se encontra!\nA voltar ao menu...\n");
    }

}

//mostra os dados recolhidos de config_modelo.txt
//PARA DEPOIS: verificar também se os valores estão em intervalos adequados
void mostrardadosrecolhidos(float dados[]){
    int i;
printf("DADOS RECOLHIDOS:\n");
for(i = 0;i<NUMERODEDADOSINICIAIS;i++){
    printf("%d->%g\n",i+1,dados[i]);
}
printf("\nLEGENDA:\n1-tf[s]\t2-dt[s]\t3-S[m2]\t4-b[m]\t5-m[kg]\t6-g[m/s2]\t7-rho[kg/m3]\t8-CD0\t9-e\t10-alpha0[radianos]\t11-v0[m/s]\t12-gamma0[radianos]\t13-x0[m]\t14-h0[m]\n");
printf("\nFIM DOS DADOS RECOLHIDOS!\n");
}

//Ler dados do ficheiro config_modelo.txt e atribuir valores lidos ao vetor do parametro.
//Dá return ao numero de dados extraídos/encontrados dentro do ficheiro
int lerconfig(float dados[]){
        FILE * config;
        int nconv, i = 0;
        char linha[255];
        char * pchar;
        float aux;
    config = fopen("config_modelo.txt","r");
        if (config==NULL)
            criarconfigpredefinida();

    do{     
        linha[0] = '\0';
        pchar = fgets(linha,255,config);
        nconv = sscanf(linha,"%g", &aux);
        if((linha[0]=='%')||(linha[0]==' ')||(nconv!=1)){
                linha[0] = '\0';
            }
            if(nconv==1){
                dados[i] = aux;
                i++;
            }
    }while (pchar!=NULL);
    fclose(config);
    return i;
}

//Funcao para mostrar os conteudos de um vetor do tipo float de dimensao dim, bastante util para debug
void print_vetor(float vetor[], unsigned int dim){
    int i;
    for(i = 0;i<dim;i++){
        printf("v[%d] = %f\n",i,vetor[i]);
    }
}

//---------------------Zona da funcao principal----------------------//
int main(){
    /*DECLARAÇÃO DE VARIÁVEIS LOCAIS*/
    float VETOR_INICIAL[NUMERODEDADOSINICIAIS];
    int op, aux;
        do{
            opcoes();
            scanf("%d", &op);
            switch (op){
                case 1:
                    printf("\nSimulando movimento da aeronave.\n.\n.\n.\n\n");
                    aux = lerconfig(VETOR_INICIAL);
                    if (aux!=NUMERODEDADOSINICIAIS){
                        fprintf(stderr,"ERRO: O numero de dados recolhido de config_modelo.txt nao corresponde ao numero de dados esperado. Foram recolhidos %d dados, esperavam-se %d!\n",aux,NUMERODEDADOSINICIAIS);
                        mostrardadosrecolhidos(VETOR_INICIAL);
                        break;
                  }
                  else{
                    mostrardadosrecolhidos(VETOR_INICIAL);
                  }                                                                      //Não te preocupes com o número de linhas ainda, e prefiro não compactar muito o código, se for preciso transferimos as coisas para novas funções
                                                                                        //Side note: � poss�vel reduzir este c�digo para menos linhas
                    break;                                                               //onde cada "case" tem as suas instru��es todas na mesma linha
                case 2:                                                                  //contudo a legibilidade � capaz de diminuir.
                    printf("\nDeterminando minimos e maximos absolutos\n.\n.\n.\n\n");   //ex.: case1:printf("---");break;
                    break;                                                               //outra side note: estou a pensar deixar o loop de opções aqui na função main, em cada opção colocamos as funções das operações que devem ser feitas!
                case 0:
                    printf("\nPrograma terminado.\n\n");
                    break;
                default:
                    printf("\nEscolha invalida. Tente novamente.\n\n");
            }
        } while (op != 0);
        return 0;
}