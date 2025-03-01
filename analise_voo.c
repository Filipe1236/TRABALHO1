/*----------------------------------------------------------------------
Um cabeçalho bonito, pôr aqui os nossos nomes ou algo do género
-------------------------------------------------------------------------*/
#include <stdio.h>


/*FUNÇÕES*/
/*--------------------------
FUNCIONALIDADES E I/O      |
--------------------------*/

/*lê do config ou de outro ficheiro (tem de já estar aberto! e depois fechá-lo manualmente) e dá return do valor de ordem n que conseguiu captar (ex: na função main, lerconfig(config,2) vai dar o segundo valor numérico do ficheiro config_modelo.txt, que deverá corresponder a dt)*/
float lerconfig(FILE * file, unsigned int n){
        float resultado;
        int i=0;
    while ((fscanf(file,"%g",&resultado)!=EOF)&&(i!=n)){
        /*A parte abaixo trata de ignorar os comentários, vai ficar a procurar percentagens, espaços e outros chars que não vão permitir que o fscanf realize o seu trabalho(scanf==1 quando consegue captar 1 número do tipo %g), depois usamos o fgetc continuamente para avançar o cursor até ao fim da linha, ignorando assim as linhas de comentários*/
        if(fgetc(file)=='%'||fgetc(file)==' '){
            while(fgetc(file)!='\n'){}
        }
        while(fscanf(file,"%g", &resultado)!=1){
            fgetc(file);
        }
        /*FIM DO MECANISMO PARA OS COMENTÁRIOS DO CONFIG*/
        /*Quando o scanf conseguir realizar bem o seu trabalho, que vai acontecer de certeza no final deste loop, guardando o valor que encontrou, o i incrementa 1, ao ponto do ciclo não se realizar mais quando estiver guardado no resultado o valor de ordem n do ficheiro*/
        i++;
        //printf("i=%d, resultado = %f\n",i,resultado);//para debug apenas
    }
    /*Como não estou a abrir e a fechar o ficheiro de cada vez que chamo a função, deparei-me com o caso do cursor permanecer no mesmo sítio da última vez que usei a função, dando valores estranhos a partir da segunda vez.
    Para isso, usa-se o fseek ou o fsetpos do stdio.h para deslocar o cursor para o início do ficheiro sempre que a função chega ao fim, através de uma constante já definida, que é o SEEK_SET (vai para o início do ficheiro, dando reset ao cursor)
    */
    fseek(file,0,SEEK_SET);
    return resultado;
}


/*TABELA DE VARIÁVEIS LOCAIS DA MAIN

NOME        ORIGEM              SIGNIFICADO/OBSERVAÇÕES
tf          config_modelo.txt   tempo final da simulação [s]
dt          config_modelo.txt   Passo de integração dt [s]
S           config_modelo.txt   área da asa [m2]
b           config_modelo.txt   comprimento da asa [m]
m           config_modelo.txt   massa da aeronave [kg]
g           config_modelo.txt   Aceleração da gravidade [m/s2]
rho         config_modelo.txt   densidade de ar ao nível do mar [kg/m3]
CD0         config_modelo.txt   valor do coeficiente de drag para Cl = 0
e           config_modelo.txt   factor de eficiência de Oswald
alpha0      config_modelo.txt   ângulo de ataque [rad].CUIDADO COM ESTE VALOR!ELE VAI SER EM RADIANOS!!!!!
v0          config_modelo.txt   velocidade inicial [m/s]
gamma0      config_modelo.txt   flight path angle inicial
x0          config_modelo.txt   posição horizontal inicial [m]
h0          config_modelo.txt   altitude inicial [m]
*/

/*----------
FUNÇÃO MAIN|
----------*/
int main(){
    /*DECLARAÇÃO DE VARIÁVEIS LOCAIS*/
    //float debug;
        float tf, dt, S, b, m, g, rho, CD0 , e, alpha0, v0, gamma0, x0, h0;
    /*lerconfig->vai abrir o ficheiro config_modelo.txt e retirar dele os dados necessários. Caso o ficheiro não exista ou faltem dados por preencher, vai dar mensagem de erro, não vai criar um ficheiro, se bem que seria boa ideia criar uma espécie de template de ficheiro caso ele não exista, para facilitar a vida ao utilizador*/
    FILE * config;
    config = fopen("config_modelo.txt","r");
        if (config==NULL){
            fprintf(stderr, "ERRO: O FICHEIRO DE CONFIG NAO EXISTE!\n");
        }
        /*atribuição de valores das variáveis indicadas no config_modelo.txt*/
        tf = lerconfig(config,1); 
        dt = lerconfig(config,2); 
        S = lerconfig(config,3); 
        b = lerconfig(config,4); 
        m = lerconfig(config,5); 
        g = lerconfig(config,6);
        rho = lerconfig(config,7);
        CD0 = lerconfig(config,8);
        e = lerconfig(config,9);
        alpha0 = lerconfig(config,10);
        v0 = lerconfig(config,11);
        gamma0 = lerconfig(config,12);
        x0 = lerconfig(config,13);
        h0 = lerconfig(config,14);
        /*fim da atribuição das variáveis do ficheiro*/
        /*printf("%f %f %f %f %f %f %f %f %f %f %f %f %f %f\n",tf, dt, S, b, m, g, rho, CD0 , e, alpha0, v0, gamma0, x0, h0);*//*O LERCONFIG FUNCIONAAAAAAAA!!!!!*/
    fclose(config);
    return 0;
}