#include <stdio.h>

//---------------Zona de cria��o de fun��es----------------------//

//Imprime as op��es que v�o correr em ciclo
void opcoes(){
    printf("Lista de opcoes:\n");
    printf("0 - Terminar programa\n");
    printf("1 - Simular movimento da aeronave\n");
    printf("2 - Determinar valores minimos e maximos absolutos de t, v(0), gama, x, h\n");
    printf("Selecionar opcao:");
}

//---------------------Zona da fun��o principal----------------------//
int main()
{
    int op;
//----------Loop de op��es ------------//
    do{
        opcoes();
        scanf("%d", &op);
        switch (op){
            case 1:
                printf("\nSimulando movimento da aeronave.\n.\n.\n.\n\n");           //Side note: � poss�vel reduzir este c�digo para menos linhas
                break;                                                               //onde cada "case" tem as suas instru��es todas na mesma linha
            case 2:                                                                  //contudo a legibilidade � capaz de diminuir.
                printf("\nDeterminando minimos e maximos absolutos\n.\n.\n.\n\n");   //ex.: case1:printf("---");break;
                break;
            case 0:
                printf("\nPrograma terminado.\n\n");
                break;
            default:
                printf("\nEscolha invalida. Tente novamente.\n\n");
        }
    } while (op != 0);
//----------Fim do loop de op��es-------------//
    return 0;
}
