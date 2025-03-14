/*----------------------------------------------------------------------
TRABALHO 1 DE PROGRAMAÇÃO 2024/2025
Realizado por:
Beatriz de Carvalho Vaz n.113407
Filipe Braz Gomes n.114217
LEAer2021
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>


//CUIDADO AO MEXER NO NOME DESTAS DEFINIÇÕES POR FAVOR!!! PODE ESTRAGAR O CÓDIGO TODO!!!
//DEFINICOES DE LETRAS GREGAS(VALOR UNICODE DELAS, MOSTROU OS CARACTERES EM WSL)
//se não funcionar, basta alterar este define para algo como "alpha"
#define ALPHASIMBOLO "\u03b1"
#define GAMMASIMBOLO "\u03b3"

//DEFINICOES DE CONSTANTES MATEMATICAS OU ARBITRARIAS
#define PI 3.14159
#define MARGEMPARAVERIFICARPROGRESSO 512 
/*valor arbitrario: ISTO DIZ DE QUANTOS EM QUANTOS CICLOS AO LONGO DOS CALCULOS O PROGRAMA VAI FAZER UMA ESTIMATIVA DO PROGRESSO DOS CALCULOS. 
QUANTO MENOR O NUMERO, MAIS LENTO FICA O PROGRAMA!
*/

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

//DEFINICOES PARA O VETOR DOS MAXIMOS E MINIMOS DA OPCAO 2
#define VETORMINMAX vetor_opcao2
#define NUMERODEDADOSVETORMINMAX 15

#define TLIDO VETORMINMAX[0]
#define VLIDO VETORMINMAX[1]
#define GAMMALIDO VETORMINMAX[2]
#define XLIDO VETORMINMAX[3]
#define HLIDO VETORMINMAX[4]

#define TMIN VETORMINMAX[5]
#define TMAX VETORMINMAX[6]

#define VMIN VETORMINMAX[7]
#define VMAX VETORMINMAX[8]

#define GAMMAMIN VETORMINMAX[9]
#define GAMMAMAX VETORMINMAX[10]

#define XMIN VETORMINMAX[11]
#define XMAX VETORMINMAX[12]

#define HMIN VETORMINMAX[13]
#define HMAX VETORMINMAX[14]

//Só para encurtar um parametro mais tarde...
#define LIXOOPCAO2 &GAMMALIDO,&GAMMALIDO,&GAMMALIDO,&GAMMALIDO,&GAMMALIDO,&GAMMALIDO,&GAMMALIDO,&GAMMALIDO,&GAMMALIDO
//ESTAS DEFINICOES SERAO USADAS PARA NOS REFERIRMOS A ESTES VALORES!

//---------------Zona de cria��o de fun��es----------------------//

//FUNCOES PARA ORDENS DO TERMINAL (a pesquisa demorou, mas serve para esconder e mostrar o cursor
//Útil para as barras de progresso nao terem o cursor a ir de um lado para o outro constantemente)

void escondercursor(void){
    printf("\033[?25l");
}

void mostrarcursor(void){
    printf("\033[?25h");
}

//Imprime as opcoes que vao correr em ciclo
void opcoes(){
    printf("Lista de opcoes:\n");
    printf("0 - Terminar programa\n");
    printf("1 - Simular movimento da aeronave\n");
    printf("2 - Determinar valores minimos e maximos absolutos de t, v(0), %s, x, h\n",GAMMASIMBOLO);
    printf("Selecionar opcao:");
}


/*Caso o ficheiro config_modelo.txt nao exista, esta funcao é chamada 
para perguntar se o utilizador pretende criar um ficheiro com dados predefinidos (os do enunciado) 
Bastante útil numa primeira utilização, em que seria seria pouco provável o utilizador 
já ter este ficheiro no diretório em que vai correr o programa, fornecendo-lhe orientação
*/
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
            fprintf(cfg,"%% %s - angulo de ataque [rad] (cuidado com o valor desta variavel !!!)\n0.1\n",ALPHASIMBOLO);
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
/*Dá return ao numero de dados extraídos/encontrados dentro do ficheiro. 
Se o ficheiro não existir, dá return a -1*/
int lerconfig(float dados[]){
        FILE * config;
        int nconv, i = 0;
        char linha[255];
        char * pchar;
        float aux;
    config = fopen("config_modelo.txt","r");
        if (config==NULL){
            criarconfigpredefinida();
            return -1;
        }
    
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

/*Verifica se os valores indicados no config_modelo.txt estão numa gama de valores razoavelmente 
realista para englobar a maior variedade de aeronaves possível.
Dá return ao numero de valores que estão fora da gama definida
*/
int intervalos_seguros (float VETOR_INICIAL[]){
    int erros = 0;
    if ((TF<1) || (TF>1000)){
        fprintf(stderr,"Valor de tf tem de estar entre 1 e 1000 [s]. \nPor favor verifique valor.\n");
        erros++;}

    if ((DT<0.000001)||(DT>0.1)){
        fprintf(stderr,"Valor de dt tem de estar entre 1e-6 e 1e-1 [s]. \nPor favor verifique valor.\n");
        erros++;}

    if ((S<0.0)||(S>910)){
        printf("Valor de s tem de estar entre 0 e 910 [m2]. \nPor favor verifique valor.\n");
        erros++;}

    if ((B<0.1)||(B>88)){
        fprintf(stderr,"Valor de b tem de estar entre 0.1 e 88 [m]. \nPor favor verifique valor.\n");
        erros++;}

    if ((M<0.001)||(M>640000)){
        fprintf(stderr,"Valor de m tem de estar entre 0.001 e 640000 [kg]. \nPor favor verifique valor.\n");
        erros++;}

    if ((CD0<0.002)||(CD0>1.5)){
        fprintf(stderr,"Valor de cd tem de estar entre 0.02 e 1.5. \nPor favor verifique valor.\n");
        erros++;}

    if ((G < 9.5)||(G> 10.5)){
        fprintf(stderr,"O valor de g tem de estar entre 9.5 e 10.5 [m/s2].\nTodos os calculos e variaveis estao de acordo com o comportamento de uma aeronave na Terra.\nLogo valores diferentes da aceleracao gravitica da Terra vao dar resultados inesperados.\nPor favor verifique valor.\n");
        erros++;}

    if ((E<0.7)||(E>1.0)){
        fprintf(stderr,"Valor de e tem de estar entre 0.7 e 1.0. \nPor favor verifique valor.\n");
        erros++;}

    if ((V0<1.0)||(V0>280)){
        fprintf(stderr,"Valor de v(0) tem de estar entre 1.0 e 280 [m/s] . \nPor favor verifique valor.\n");
        erros++;}

    if ((H0<1)||(H0>12000)){
        fprintf(stderr,"Valor de h0 (altitude inicial) tem de estar entre 1 e 12000 [m] . \nPor favor verifique valor.\n");
        erros++;}
    return erros;
}

//Funcao para mostrar os conteudos de um vetor do tipo float de dimensao dim, bastante util para debug
/*void print_vetor(float vetor[], int dim){
    int i;
    for(i = 0;i<dim;i++){
        printf("v[%d] = %f\n",i,vetor[i]);
    }
}*/

//funcao que vai mostrar no ecra algo parecido a uma barra de progresso
/*argumentos: um auxiliar que vai incrementando com o loop, para permitir que a verificação do progresso seja feita de n em n ciclos
a linha em que a leitura/escrita se encontra, 
aquilo que deve acompanhar o progresso dos calculos (aqui vai ser o tempo) 
e a variavel que deve marcar uma estimativa do fim(neste caso o tempo final da simulacao)*/

//para funcionar bem, a barra de progresso deve ser definida e levada de fora desta funcao!

// barra comeca com 10 segmentos [----------] e vai substituindo uma linha por um # a cada 10%
//output parecido com "[###-------]32.38% linha 13457"

/*LIMITACOES: 
funcao feita a pensar num ciclo com MUITOS calculos, 
mas sem poder ser chamada em todos os ciclos(muito lento). 
Coloquei passagem por endereco nos argumentos da funcao por achar que, intuitivamente, 
vai ser mais eficiente usar partes da memoria ja existentes 
do que estar a copiar esses dados para novas variaveis locais sempre que a funcao for chamada. 
É muito importante esconder o cursor com o escondercursor() antes de a utilizar!*/

/*A percentagem e a barra de progresso apenas faz uma estimativa em relacao ao tempo final da simulacao.
 Se a aeronave bater no chao antes, a simulacao pode chegar ao fim muito antes do tempo final!*/
void progresso(int *auxprogresso, int * indicadorlinhas, float *indicadorprogresso, float *fim, char barraprogresso[11]){
    if(*indicadorlinhas == *auxprogresso * MARGEMPARAVERIFICARPROGRESSO){
        float percentagemprogresso;
        int dezemdez;
        percentagemprogresso = *indicadorprogresso / *fim * 100;
        dezemdez = (int)percentagemprogresso / 10 - 1;
        barraprogresso[dezemdez] = '#';
        //Uma forma de garantir que a barra fica a 100% no final
        if(percentagemprogresso>95.0){
            percentagemprogresso=100.0;
            barraprogresso[9] = '#';
        }
        printf("\r[%s] %.2f %% linha %d",barraprogresso,percentagemprogresso, *indicadorlinhas);
        *auxprogresso = *auxprogresso + 1;
    }
}

//Realiza os calculos da opcao 1 e escreve os valores no ficheiro voo_sim.txt. 
/*Tem como argumentos o vetor com os dados do config_modelo.txt. 
O vetor em que os dados processados vao sendo alojados vai ser declarado dentro da propria funcao, 
nao vai ser muito util leva-lo para fora, ja que no fim so ficam guardados os valores do ultimo instante*/
//Dá return ao numero de entradas (linhas) que o programa escreveu no ficheiro! 
int opcaoum(float VETOR_INICIAL[]){ //Uso das definicoes estabelecidas no topo do ficheiro!
    int i,auxprogresso = 1, linhasescritas = 1;
    char barraprogresso[11] = "----------\0";
    float vtemp; 
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
    
    //loop que vai fazer os calculos todos. A variavel linhasescritas vai permitir acompanhar o progresso do loop
    printf("PROGRESSO ESTIMADO:\n");
    escondercursor();
    while((T<= TF)&&(H > 0)){
        LIFT = CL * 0.5 * RHO * V * V * S;
        DRAG = CD * 0.5 * RHO * V * V * S;
        X = X + DT * V * cos(GAMMA);
        H = H + DT * V * sin(GAMMA);
        vtemp = V; //guarda o valor antigo de V para o gamma, necessario para calculos!
        V = V + (DT / M) *  (-DRAG - M * G * sin(GAMMA));
        GAMMA = GAMMA + (DT / (M * vtemp)) * (LIFT - M * G * cos(GAMMA));
        
        progresso(&auxprogresso,&linhasescritas,&T,&TF,barraprogresso);
        
        fprintf(fresultados, "%g %g %g %g %g\n", T, V, GAMMA, X, H);
        T += DT;
        ++linhasescritas;
    }
    printf("\r\n");
    mostrarcursor();
    fclose(fresultados);
    printf("Simulacao realizada com sucesso! Os resultados foram guardados em voo_sim.txt!\n");
    return linhasescritas;
}

/*A cada valor lido, vai verificar se o mesmo é menor ou maior que os respetivos minimos e maximos ate agora conhecidos.
 Se tal for o caso, a variavel em que o valor minimo ou maximo sao guardados passa a guardar o valor lido*/
//pede primeiro o valor que vai sendo lido, e depois os endereços das variaveis em que os valores minimos e maximos sao guardados, respetivamente
void escolheminmax(float valorlido, float *valormin, float *valormax){
    if(valorlido > *valormax)
        *valormax = valorlido;
    if(valorlido < *valormin)
        *valormin = valorlido;
}

//Realiza a leitura do ficheiro e procura os valores maximos e minimos para a opcao 2
//Dá return ao numero de entradas (linhas) com valores lidos! 
//Dá return de -1 se nao conseguir abrir o ficheiro voo_sim.txt!
//Dá return de -2 se conseguir abrir o ficheiro, mas houver algum impedimento no formato
int opcaodois(){
    char barraprogresso[11] = "----------\0";
    int aux, entradaslidas = 1;
    int auxprogresso = 1;
    float VETORMINMAX[NUMERODEDADOSVETORMINMAX];
    float tfinal;
    FILE * fresultados = fopen("voo_sim.txt","r");
    if(fresultados == NULL)
        return -1;
    if(fscanf(fresultados, "(%g, %g, %g, %g, %g, %g, %g, %g, %g, %g)", &tfinal, LIXOOPCAO2)!=10)
        return -2;
    aux = fscanf(fresultados,"%g %g %g %g %g", &TLIDO, &VMAX, &GAMMAMAX, &XMAX, &HMAX);
    if(aux != 5)
        return -2;
    VMIN = VMAX;
    GAMMAMIN = GAMMAMAX;
    XMIN = XMAX;
    HMIN = HMAX;
    printf("PROGRESSO ESTIMADO:\n");
    escondercursor();
    do{
        aux = fscanf(fresultados,"%g %g %g %g %g", &TLIDO, &VLIDO, &GAMMALIDO, &XLIDO, &HLIDO);
        escolheminmax(TLIDO, &TMIN, &TMAX);
        escolheminmax(VLIDO, &VMIN, &VMAX);
        escolheminmax(GAMMALIDO, &GAMMAMIN, &GAMMAMAX);
        escolheminmax(XLIDO, &XMIN, &XMAX);
        escolheminmax(HLIDO, &HMIN, &HMAX);
        
        progresso(&auxprogresso,&entradaslidas, &TLIDO, &tfinal,barraprogresso);
        ++entradaslidas;
    }while(aux == 5);
    mostrarcursor();
    printf("\r\n");

    printf("DADO\t\tMINIMO\t\tMAXIMO\n");
    printf("Tempo\t\t%6.4g\t\t%6.4g\n",TMIN,TMAX);
    printf("Velocidade\t%6.4g\t\t%6.4g\n",VMIN,VMAX);
    printf("%s\t\t%6.4g\t\t%6.4g\n",GAMMASIMBOLO,VMIN,VMAX);
    printf("Posicao x\t%6.4g\t\t%6.4g\n",XMIN,XMAX);
    printf("Altura\t\t%6.4g\t\t%6.4g\n",HMIN,HMAX);
    return entradaslidas;
}

void implementa_opcao1(void) {
    float VETOR_INICIAL[NUMERODEDADOSINICIAIS];
    int dadoslidos, errosnosvalores, ignoraravisos, aux;
    char input[10];  // Buffer para leitura segura do input

    dadoslidos = lerconfig(VETOR_INICIAL);
    errosnosvalores = intervalos_seguros(VETOR_INICIAL);

    if ((dadoslidos != NUMERODEDADOSINICIAIS) && (dadoslidos != -1)) {
        fprintf(stderr, "ERRO: O numero de dados recolhido de config_modelo.txt nao corresponde ao numero de dados esperado. Foram recolhidos %d dados, esperavam-se %d!\n", dadoslidos, NUMERODEDADOSINICIAIS);
        mostrardadosrecolhidos(VETOR_INICIAL);
    } 
    else if (errosnosvalores != 0) {
        fprintf(stderr, "\nERRO: %d valores fora da gama considerada! Por favor corrigir!\n\n", errosnosvalores);
        printf("Se deseja manter estes parametros e seguir para a simulacao, escreva 1 e enter;\nQualquer outro valor leva de volta ao menu: ");

        if (fgets(input, sizeof(input), stdin)) {  // Lê uma linha do stdin
            if (sscanf(input, "%d", &ignoraravisos) == 1 && ignoraravisos == 1) {
                printf("\nAvisos ignorados!\nSimulando movimento da aeronave...\n");
                mostrardadosrecolhidos(VETOR_INICIAL);
                dadoslidos = opcaoum(VETOR_INICIAL);
                printf("O programa escreveu %d linhas no ficheiro!\n", dadoslidos);
            }
        }
    } 
    else {
        mostrardadosrecolhidos(VETOR_INICIAL);
        dadoslidos = opcaoum(VETOR_INICIAL);
        printf("O programa escreveu %d linhas no ficheiro!\n", dadoslidos);
    }
}

void implementa_opcao2(){
    int dadoslidos;
    dadoslidos = opcaodois();
                    if(dadoslidos == -1)
                        fprintf(stderr,"ERRO: o ficheiro com os resultados de uma simulacao ainda nao existe! Por favor correr primeiro a opcao 1!\n");
                    if(dadoslidos == -2)
                        fprintf(stderr,"ERRO: o ficheiro foi aberto, mas nao se encontra no formato esperado, por favor corrigir qualquer alteracao manual pelo utilizador no ficheiro ou voltar a correr a simulacao!\n");
                    else
                        printf("Foram lidas %d linhas do ficheiro voo_sim.txt!\n",dadoslidos);
}


//---------------------Zona da funcao principal----------------------//
int main(){
    /*DECLARAÇÃO DE VARIÁVEIS LOCAIS*/
    int op;
        do{
            opcoes();
            scanf("%d", &op);
            switch (op){
                case 1:
                    printf("\nSimulando movimento da aeronave...\n");
                    implementa_opcao1();                                                       
                break;                                                               
                case 2:                                                                  
                    printf("\nDeterminando minimos e maximos absolutos...\n");
                    implementa_opcao2();
                    break;                                                               
                case 0:
                    printf("\nPrograma terminado.\n\n");
                    break;
                default:
                    printf("\nEscolha invalida. Tente novamente.\n\n");
            }
        } while (op != 0);
        return 0;
}