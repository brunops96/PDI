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
#define JANELA_A 21//Altura da janela para filtro ingenup
#define JANELA_L 51//Largura da imagem para filtro ingenuo
#define JANELAINT_A 3//Altura da imagem para filtro integral
#define JANELAINT_L 13//Largura da imagem para filtro integral

void filtroIng(){
    int i,x,y;
    int Y,X;
    Imagem* img = abreImagem (INPUT_IMAGE, INPUT_CANAIS);
    if (!img)
    {
        printf ("Erro abrindo a imagem.\n");
        exit (1);
    }
    Imagem* img_out = criaImagem (img->largura, img->altura, img->n_canais);
    for(i=0;i<img_out->n_canais;i++)//Preenche saida com zeros. Faz a borda ficar preta
        for(y=0;y<img->altura;y++)
            for(x=0;x<img->largura;x++)
                img_out->dados[i][y][x]=0;
    
  //preenche a imagem de saida onde a janela cabe
    for(i=0;i<img->n_canais;i++){
        for(y=(JANELA_A-1)/2;y<img->altura-(JANELA_A-1)/2;y++){
            for(x=(JANELA_L-1)/2;x<img->largura-(JANELA_L-1)/2;x++){
                for(Y=y-((JANELA_A-1)/2);Y<=y+((JANELA_A-1)/2);Y++)
                    for(X=x-((JANELA_L-1)/2);X<=x+((JANELA_L-1)/2);X++)
                        img_out->dados[i][y][x]+=img->dados[i][Y][X];
                img_out->dados[i][y][x]/=(JANELA_A*JANELA_L);
            }
        }
    }
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
    altura_int =img->altura+(JANELAINT_A/2)*2+1;//A imagem integral tem a altura da imagem + (JANELAINT_A/2)+1 para a esquerda(Borda com posição extra para fora da janela) e (JANELAINT_A/2)+1 para a direita (Borda necessária)
    largura_int =img->largura+(JANELAINT_L/2)*2+1;//A imagem integral tem a largura da imagem + (JANELAINT_A/2)+1 para cima (Borda com posição extra para fora da janela) e (JANELAINT_A/2)+1 para a baixo (Borda necessária)
    Imagem* buff = criaImagem(largura_int,altura_int,img->n_canais);
    
    for(i=0;i<img_out->n_canais;i++)//isso provavelmente é inútil, deixei por medo de tirar mesmo
        for(y=0;y<img->altura;y++)
            for(x=0;x<img->largura;x++){
                img_out->dados[i][y][x]=0;
                buff->dados[i][y][x]=0;
            }
    
  //Preenchimento da imagem integral com borda de azulejo        
    for(i=0;i<img->n_canais;i++){
        yI=0;//Posição Y da imagem integral
        for(y=-(JANELAINT_A)/2-1;y<altura_int-(JANELAINT_A)/2-1;y++){/*Loop que incrementa altura na imagem original. Quando y é negativo quer dizer que estamos na borda de cima da imagem(y+img->altura para azulejo),
                                                                       e quando ele é maior que img->altura quer dizer que estamos na borda de baixo(y%img->altura para azulejo))*/
            xI=0;//Posição X da imagem integral
            for(x=-(JANELAINT_L)/2-1;x<largura_int-(JANELAINT_L)/2-1;x++){/*Loop que incrementa largura na imagem original. Quando x é negativo quer dizer que estamos na borda da esquerda da imagem(x+img->largura para azulejo),
                                                                            e quando ele é maior que img->largura quer dizer que estamos na borda da direita(x%img->largura para azulejo)*/
                if(xI==0 && yI==0){//primeira posição
                    if(y<0)//Se a janela sai da tela(Precaução)
                        buff->dados[i][yI][xI]=img->dados[i][y+img->altura][x+img->largura];
                    else
                        buff->dados[i][yI][xI]=img->dados[i][y%img->altura][x%img->largura];
                }
                else if(yI==0) {//Se estamos na primeira linha da imagem integral
                    if(x<0){//Se a janela sai da tela para a esquerda
                        if(y<0)//Se a janela sai da tela para cima (talvez isso seja inutil na verdade, eu fiz isso meio que no piloto automático)
                            buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y+img->altura][x+img->largura];
                        else
                            buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y%img->altura][x+img->largura];
                    }
                    else if(y<0)//Se a janela só sai da tela para cima
                        buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y+img->altura][x%img->largura];
                    else
                        buff->dados[i][yI][xI]=buff->dados[i][yI][xI-1]+img->dados[i][y%img->altura][x%img->largura];
                }
                else if(xI==0) {//Se estamos na primeira coluna da imagem integral
                    if(x<0){//Se a janela sai da tela para a esquerda (talvez isso seja inutil na verdade, eu fiz isso meio que no piloto automático)
                        if(y<0)//Se a janela sai da tela para cima 
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y+img->altura][x+img->largura];
                        else
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y%img->altura][x+img->largura];
                    }
                    else if(y<0)//Se a janela sai da tela somente para cima
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y+img->altura][x%img->largura];
                    else
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+img->dados[i][y%img->altura][x%img->largura];
                }
                else{//Se estamos no meio da imagem integral
                    if(x<0){//Se a janela sai da tela para a esquerda
                        if(y<0)//Se a janela sai da tela para cima
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y+img->altura][x+img->largura];
                        else
                            buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y%img->altura][x+img->largura];
                    }
                    else if(y<0)//Se a janela sai da tela somente para cima
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y+img->altura][x%img->largura];
                    else
                        buff->dados[i][yI][xI]=buff->dados[i][yI-1][xI]+buff->dados[i][yI][xI-1]-buff->dados[i][yI-1][xI-1]+img->dados[i][y%img->altura][x%img->largura];
                }
                xI++;
            }
            yI++;
        }
    }
    
  //Percorre imagem integal(buff) nas posições correspondentes à imagem original sem borda e realisa o cauculo da média com as propriedades da imagem integral
    for(i=0;i<img->n_canais;i++){
        for(y=0;y<img->altura;y++){
            for(x=0;x<img->largura;x++){
                yI=y+JANELAINT_A/2+1;//Posição correspondente de y na imagem original sem borda na imagem integral
                xI=x+JANELAINT_L/2+1;//Posição correspondente de x na imagem original sem borda na imagem integral
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
