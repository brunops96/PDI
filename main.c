#include <stdio.h>
#include <stdlib.h>
#include "pdi.c"
/*
  BRUNO PEDROSO SCHMOCKEL
  CELSO LUIZ CHANE JUNIOR
*/
#define INPUT_IMAGE "a01 - Original.bmp"
#define INPUT_CANAIS 1
#define JANELA_A 21
#define JANELA_L 51

//Filtro ingenuo
void filtroIng(){
    int i,x,y;
    int Y,X;
    float soma=0;
    Imagem* img = abreImagem (INPUT_IMAGE, INPUT_CANAIS);
    if (!img)//teste de entrada
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }
    Imagem* img_out = criaImagem (img->largura, img->altura, img->n_canais);
    //percorre a imagem
    for(i=0; i < img->n_canais ;i++){//percorre cainais
        for(y = (JANELA_A-1) / 2 ; y < img->altura - (JANELA_A-1) / 2 ;y++){//percorre linhas 
            for( x = (JANELA_L-1) / 2 ;x < img->largura - (JANELA_L-1) / 2 ;x++){//percorre colunas
                soma=0;
                //Preenche bordas
                if( y < (JANELA_A / 2) - 1 || y > img->altura - ((JANELA_A / 2) + 1) || x < (JANELA_L / 2) - 1 ||  x > img->largura - ((JANELA_L / 2) + 1))
                    img_out->dados[i][y][x]=0;
                //JANELA DESLIZANTE
                else
                    for(Y=y-JANELA_A/2;Y<=y+JANELA_A/2;Y++)
                        for(X=x-JANELA_L/2;X<=x+JANELA_L/2;X++)
                            soma+=img->dados[i][Y][X];
                img_out->dados[i][y][x]=soma/(JANELA_A*JANELA_L);
            }
        }
    }

    salvaImagem(img_out,"saidaIng.bmp");
    destroiImagem (img);
    destroiImagem (img_out);
}
//filtro integral
void filtroInt(){
    int i,x,y;
    Imagem* img = abreImagem (INPUT_IMAGE, INPUT_CANAIS);
    if (!img)//testa abertura da imagem
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }
    Imagem* img_out = criaImagem (img->largura, img->altura, img->n_canais);
    Imagem* buff = criaImagem (img->largura, img->altura, img->n_canais);
    /*
    for(i=0; i < img_out->n_canais ;i++)
        for(y=0;y<img->altura;y++)
            for(x=0;x<img->largura;x++){
                img_out->dados[i][y][x]=0;
                buff->dados[i][y][x]=0;
            }
    */
    //editar pos 0/0
    buff->dados[i][0][0]=img->dados[i][0][0];
    for(i=0; i < img->n_canais ; i++)
        for(y=1;y < img->altura;y++){
          buff->dados[i][y][0] = buff->dados[i][y-1][0]+img->dados[i][y][0];
          buff->dados[i][0][y] = buff->dados[i][0][y-1]+img->dados[i][0][y];
        }
  
    for(i = 1; i < img->n_canais ;i++)
        for(y = 1; y < img->altura ;y++)
            for( x = 0; x < img->largura ;x++)
                buff->dados[i][y][x]=buff->dados[i][y-1][x]+buff->dados[i][y][x-1]-buff->dados[i][y-1][x-1]+img->dados[i][y][x];
  
    for(i=0;i<img->n_canais;i++){
        for(y=0;y<img->altura;y++){
            for(x=0;x<img->largura;x++){
                Coordenada direita,esquerda, cima ,inter;
                direita.x = (x+JANELA_L/2);
                if(direita.x > img->largura-1)
                    direita.x = img->largura - 1;
                direita.y = (y+JANELA_A/2);
                if (direita.y > img->largura-1)
                    direita.y = img->altura -1;
                esquerda.x = (x-JANELA_L/2)-1;
                if(esquerda.x<0)
                    esquerda.x = 0;
                esquerda.y = direita.y;
                cima.x = direita.x;
                cima.y = (y-JANELA_A/2)-1;
                if(cima.y<0)
                    cima.y = 0;
                inter.x = esquerda.x;
                inter.y = cima.y;
                img_out->dados[i][y][x] = (+buff->dados[i][direita.y][direita.x]
                                          -buff->dados[i][esquerda.y][esquerda.x]
                                          -buff->dados[i][cima.y][cima.x]
                                          +buff->dados[i][inter.y][inter.x])/(JANELA_A*JANELA_L);
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
    int i,j;
    while(i!=1 || i!=2){
        printf("Entre o número do algoritmo que deseja\n1)Ingenuo\n2)Integral\n");
        scanf("%d",&i);
        if(i==1){
            filtroIng();
        }
        if(i==2){
        }
        else{
            printf("Valor invalido, precione enter para continuar.");
            scanf("%d",&j);
            system("cls");
        }
    }
    return 0;
}
