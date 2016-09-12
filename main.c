#include <stdio.h>
#include <stdlib.h>
#include "pdi.c"
/*
  BRUNO PEDROSO SCHMOCKEL
  CELSO LUIZ CHANE JUNIOR
*/
#define INPUT_IMAGE "a01 - Original.bmp"
#define INPUT_CANAIS 1
//Altura e largura dos exemplos parecem invertidas, com exceção do 21x51
#define JANELA_A 21
#define JANELA_L 51
#define JANELAINT_A 3
#define JANELAINT_L 13

void filtroIng(){
    int i,x,y;
    int Y,X;
    float soma=0;
    Imagem* img = abreImagem (INPUT_IMAGE, INPUT_CANAIS);
    if (!img)
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }
    Imagem* img_out = criaImagem (img->largura, img->altura, img->n_canais);
    for(i=0;i<img_out->n_canais;i++)
        for(y=0;y<img->altura;y++)
            for(x=0;x<img->largura;x++)
                img_out->dados[i][y][x]=0;
    for(i=0;i<img->n_canais;i++){
        for(y=(JANELA_A-1)/2;y<img->altura-(JANELA_A-1)/2;y++){
            for(x=(JANELA_L-1)/2;x<img->largura-(JANELA_L-1)/2;x++){
                soma=0;
                for(Y=y-((JANELA_A-1)/2);Y<=y+((JANELA_A-1)/2);Y++)
                    for(X=x-((JANELA_L-1)/2);X<=x+((JANELA_L-1)/2);X++)
                        soma+=img->dados[i][Y][X];
                img_out->dados[i][y][x]=soma/(JANELA_A*JANELA_L);
            }
        }
    }
    printf("hsuahua");
    salvaImagem(img_out,"saidaIng.bmp");
}

void filtroInt(){
    int i,y,x,yI=0,xI=0;
    int altura_int,largura_int;
    Imagem* img = abreImagem (INPUT_IMAGE, INPUT_CANAIS);
    if (!img)
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }
    Imagem* img_out = criaImagem (img->largura, img->altura, img->n_canais);
    altura_int =img->altura+(JANELAINT_A/2)*2+1;
    largura_int =img->largura+(JANELAINT_L/2)*2+1;
    Imagem* buff = criaImagem(largura_int,altura_int,img->n_canais);
    for(i=0;i<img_out->n_canais;i++)
        for(y=0;y<img->altura;y++)
            for(x=0;x<img->largura;x++){
                img_out->dados[i][y][x]=0;
                buff->dados[i][y][x]=0;
            }
    for(i=0;i<img->n_canais;i++){
        yI=0;
        for(y=-(JANELAINT_A)/2-1;y<altura_int-(JANELAINT_A)/2-1;y++){
            xI=0;
            for(x=-(JANELAINT_L)/2-1;x<largura_int-(JANELAINT_L)/2-1;x++){
                if(xI==0 && yI==0){
                    if(y<0)
                        buff->dados[i][yI][xI]=img->dados[i][y+img->altura][x+img->largura];
                    else
                        buff->dados[i][yI][xI]=img->dados[i][y%img->altura][x%img->largura];
                }
                else if(yI==0) {
                    if(x<0){
                        if(y<0)
                            buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y+img->altura][x+img->largura];
                        else
                            buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y%img->altura][x+img->largura];
                    }
                    else if(y<0)
                        buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y+img->altura][x%img->largura];
                    else
                        buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y%img->altura][x%img->largura];
                }
                else if(xI==0) {
                    if(x<0){
                        if(y<0)
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y+img->altura][x+img->largura];
                        else
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y%img->altura][x+img->largura];
                    }
                    else if(y<0)
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y+img->altura][x%img->largura];
                    else
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y%img->altura][x%img->largura];
                }
                else{
                    if(x<0){
                        if(y<0)
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y+img->altura][x+img->largura];
                        else
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y%img->altura][x+img->largura];
                    }
                    else if(y<0)
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y+img->altura][x%img->largura];
                    else
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y%img->altura][x%img->largura];
                }
                xI++;
            }
            yI++;
        }
    }
    for(i=0;i<img->n_canais;i++){
        for(y=0;y<img->altura;y++){
            for(x=0;x<img->largura;x++){
                yI=y+JANELAINT_A/2+1;
                xI=x+JANELAINT_L/2+1;
                /*printf("%d %d ",yI,xI);
                printf("%f.2 ",buff->dados[i][yI+JANELAINT_A/2][xI+JANELAINT_L/2]);
                printf("%f.2 ",buff->dados[i][yI+JANELAINT_A/2][xI-JANELAINT_L/2-1]);
                printf("%f.2 ",buff->dados[i][yI-JANELAINT_A/2-1][xI+JANELAINT_L/2]);
                printf("%f.2 ",buff->dados[i][yI-JANELAINT_A/2-1][xI-JANELAINT_L/2-1]);
                printf("\n");*/
                img_out->dados[i][y][x]=(buff->dados[i][yI+JANELAINT_A/2][xI+JANELAINT_L/2]//canto direito dentro
                                        -buff->dados[i][yI+JANELAINT_A/2][xI-JANELAINT_L/2-1]//esquerda fora
                                        -buff->dados[i][yI-JANELAINT_A/2-1][xI+JANELAINT_L/2]//cima fora
                                        +buff->dados[i][yI-JANELAINT_A/2-1][xI-JANELAINT_L/2-1]/*canto esquerdo dentro */)/(JANELAINT_A*JANELAINT_L);
            }
        }
    }
    salvaImagem(img_out,"saidaInt.bmp");
    destroiImagem (img);
    destroiImagem (img_out);
    destroiImagem (buff);
}

int main()
{
    int i=0,j;
    while(i!=1 && i!=2){
        printf("Entre o número do algoritmo que deseja\n1)Ingenuo\n2)Integral\n");
        scanf("%d",&i);
        if(i==1){
            filtroIng();
        }
        else if(i==2){
            filtroInt();
        }
        else{
            printf("Valor invalido, precione enter para continuar.");
            scanf("%d",&j);
            system("cls");
        }
    }
    return 0;
}
