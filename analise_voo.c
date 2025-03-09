/*----------------------------------------------------------------------
Um cabeçalho bonito, pôr aqui os nossos nomes ou algo do género
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//CUIDADO AO MEXER NO NOME DESTAS DEFINIÇÕES POR FAVOR!!! PODE ESTRAGAR O CÓDIGO TODO!!!
//DEFINICOES DE LETRAS GREGAS(VALOR UNICODE DELAS, MOSTROU OS CARACTERES EM WSL)
//se não funcionar, basta alterar este define para algo como "alpha"
#define ALPHASIMBOLO "\u03b1"
#define GAMMASIMBOLO "\u03b3"

//DEFINICOES DE CONSTANTES MATEMATICAS
#define PI 3.14159

//DEFINICOES DOS DADOS LIDOS DO FICHEIRO config_modelo.txt
#define VETOR_INICIAL vetor_dados_iniciais
#define NUMERODEDADOSINICIAIS 14 //tamanho do vetor que guarda os dados do config_modelo.txt

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

//DEFINICOES DOS DADOS PROCESSADOS PARA FAZER A OPCAO 1
#define VETOR_PROCESSAMENTO vetor_dados_opcao1
#define NUMERODEVALORESPROCESSADOS 10

#define AR VETOR_PROCESSAMENTO[0]
#define CL VETOR_PROCESSAMENTO[1]
#define CD VETOR_PROCESSAMENTO[2]
#define LIFT VETOR_PROCESSAMENTO[3]
#define DRAG VETOR_PROCESSAMENTO[4]
#define V VETOR_PROCESSAMENTO[5]
#define GAMMA VETOR_PROCESSAMENTO[6]
#define X VETOR_PROCESSAMENTO[7]
#define H VETOR_PROCESSAMENTO[8]
#define T VETOR_PROCESSAMENTO[9]

//ESTAS DEFINICOES SERAO USADAS PARA NOS REFERIRMOS A ESTES VALORES!

//---------------Zona de cria��o de fun��es----------------------//


//Imprime as opcoes que vao correr em ciclo
void opcoes(){
    printf("Lista de opcoes:\n");
    printf("0 - Terminar programa\n");
    printf("1 - Simular movimento da aeronave\n");
    printf("2 - Determinar valores minimos e maximos absolutos de t, v(0), %s, x, h\n",GAMMASIMBOLO);
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
            fprintf(cfg,"%% %s - angulo de ataque [rad] (cuidado com o valor desta variavel !!!)\n0.1",ALPHASIMBOLO);
            fprintf(cfg,"%% V(0) -velocidade inicial [m/s]\n11.0\n");
            fprintf(cfg,"%% %s(0) [rad] - flight path angle inicial\n0.0\n",GAMMASIMBOLO);
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
printf("\nLEGENDA:\n1-tf[s]\t2-dt[s]\t3-S[m2]\t4-b[m]\t5-m[kg]\t6-g[m/s2]\t7-rho[kg/m3]\t8-CD0\t9-e\t10-%s0[radianos]\t11-v0[m/s]\t12-%s0[radianos]\t13-x0[m]\t14-h0[m]\n",ALPHASIMBOLO,GAMMASIMBOLO);
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
        //condição inicial-> (linha[0]=='%')||(linha[0]==' ')||(nconv!=1)
        if((linha[0]==' ')||(nconv!=1)){ //o (linha[0]==' ')||(nconv!=1) basta, mas tenho de fazer mais testes para confirmar
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

//Realiza os calculos da opcao 1 e escreve os valores no ficheiro voo_sim.txt. Tem como argumentos o vetor com os dados do config_modelo.txt. O vetor em que os dados processados vao sendo alojados vai ser declarado dentro da propria funcao, nao vai ser muito util leva-lo para fora, ja que no fim so ficam guardados os valores do ultimo instante
void opcaoum(float VETOR_INICIAL[]){ //COLOQUEI ASSIM O NOME DO VETOR PARA APROVEITAR AS DEFINICOES JA FEITAS ACIMA, VAO SER MUITO UTEIS
    int i;
    float VETOR_PROCESSAMENTO[NUMERODEVALORESPROCESSADOS];
    FILE * fresultados = fopen("voo_sim.txt","w"); //aqui julgamos nao haver preocupacao com o caso do ficheiro existir ou nao, ele vai criar sempre um novo ou "apagar" o ja existente e colocar novos dados. A unica situacao em que fazer um mecanismo assim seria justificavel ia ser no caso do utilizador estar a tentar correr o programa num diretorio em que o mesmo nao tem permissao para escrever
    AR = (B * B) / S;
    CL = ALPHA0 * PI * AR / (1 + sqrt(1 + pow(AR / 2, 2)));
    CD = CD0 + pow(CL, 2) / (PI * E * AR);
    //PRIMEIRO TEMOS DE BUSCAR OS VALORES INICIAIS DA CONFIG E COLOCÁ-LOS NO VETOR DE PROCESSAMENTO!
    V = V0;
    GAMMA = GAMMA0;
    X = X0;
    H = H0;
    T = 0;
    //convem tambem escrever ja a primeira e a segunda linhas do voo_sim.txt
    fprintf(fresultados, "(");
    for(i = 0;i<= 8;i++)
        fprintf(fresultados,"%g, ",VETOR_INICIAL[i]); //coloca os 9 primeiros valores do vetor com os dados da config pela ordem certa (a ordem das definicoes la acima nao foram aleatorias)
    fprintf(fresultados, "%g)\n",ALPHA0);
    fprintf(fresultados, "%g %g %g %g %g\n", T, V0, GAMMA0, X0, H0);
    //e agora podemos comecar o grande loop que vai fazer os calculos todos
    while((T<= TF)&&(H > 0)){
        LIFT = CL * 0.5 * RHO * V * V * S;
        DRAG = CD * 0.5 * RHO * V * V * S;
        X = X + DT * V * cos(GAMMA);
        H = H + DT * V * sin(GAMMA);
        V = V + (DT / M) *  (-DRAG - M * G * sin(GAMMA));
        GAMMA = GAMMA + (DT / (M * V)) * (LIFT - M * G * sin(GAMMA));

        fprintf(fresultados, "%g %g %g %g %g\n", T, V, GAMMA, X, H);
        T += DT;
    }
    fclose(fresultados);
    printf("Simulacao realizada com sucesso! Os resultados foram guardados em voo_sim.txt!\n");
}
//---------------------Zona da funcao principal----------------------//
int main(){
    /*DECLARAÇÃO DE VARIÁVEIS LOCAIS*/
    float VETOR_INICIAL[NUMERODEDADOSINICIAIS];
    int op, dadoslidos;
        do{
            opcoes();
            scanf("%d", &op);
            switch (op){
                case 1:
                    printf("\nSimulando movimento da aeronave.\n.\n.\n.\n\n");
                    dadoslidos = lerconfig(VETOR_INICIAL);
                    if (dadoslidos!=NUMERODEDADOSINICIAIS){
                        fprintf(stderr,"ERRO: O numero de dados recolhido de config_modelo.txt nao corresponde ao numero de dados esperado. Foram recolhidos %d dados, esperavam-se %d!\n",dadoslidos,NUMERODEDADOSINICIAIS);
                        mostrardadosrecolhidos(VETOR_INICIAL);
                        break;
                  }
                  else{
                    mostrardadosrecolhidos(VETOR_INICIAL);
                    opcaoum(VETOR_INICIAL);
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