#include <stdio.h>


int main (){
    int i = 0;
    int fim = 2923819;
    float progresso;
    int n = 0;
    printf("\033[?25l");
    for (i = 0; i < fim; i++){
        if(i == (n * 10)){
            progresso = (i* 100.0 / fim);
            printf("\r%.2f", progresso);
            fflush(stdout);
            n++;
        }
    }
    printf("\033[?25h");
    printf("\n");

}