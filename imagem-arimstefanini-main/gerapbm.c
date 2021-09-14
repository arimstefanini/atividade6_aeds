#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN64__) || defined(__CYGWIN__)
#define VIEW "./i_view32"
#elif defined(__linux__)
#define VIEW "eog"
#endif

typedef int *image;

#define TEXTO "# Imagem exemplo\n"

image alloc_memo(int nl, int nc){
    return (image)malloc(nl * nc * sizeof(int));
}

// --------------------------------------------
// 1. criar o arquivo texto 'name'
// 2. gravar o numero mágico 'P1' no arquivo
// 3. gravar uma linha de comentário '#...'
// 4. gravar o número de linhas e colunas
// 5. gravar os 0's ou 1's da imagem
//    (no máximo 100 valores por linha)
// --------------------------------------------
void write_pbm(image img, char *name, int nl, int nc){

    int x, getLin, getCol, l=0, c=1;
    FILE *f;

    getLin = img[l];
    getCol = img[c];

    if ((f = fopen(name, "wt")) == NULL) {
        printf("Erro na CRIACAO do arquivo <%s>\n\n", name);
    } else {
        fputs("P1\n", f);
        fputs(TEXTO, f);
        fprintf(f, "%d  %d\n", nl, nc);

        printf("linha %d %d \n", nl, nc);
        for (int i = 0; i < nl; i++){
            for (int j = 0; j < nc; j++) {

                if(i == getLin && j == getCol){
                    printf("%d  ", getLin);
                    printf("%d \n ", getCol);
                    fprintf(f, "%d ", 1);
                    getLin = img[l+=2];
                    getCol = img[c+=2];
                } else {
                    fprintf(f, "%d ", 0);
                }
                
            }
            fprintf(f, "\n");
        }

    }

    fclose(f);
}

// --------------------------------------------
// 1. abrir o arquivo texto 'nome' para leitura
// 2. ler o número de linhas e colunas
// 3. ler o número 'n' de pontos
// 4. alocar o vetor 'img' com nl x nc
//    elementos
// 5. preencher o vetor imagem com zeros
// 6. ler as 'n' coordenadas (i, j) e preencher
//    a posição correspondente de img com 1.
// --------------------------------------------

image read_pts(char *name, int *nl, int *nc){
    FILE *f;
    image img = NULL;
    char c, LINHA[100];
    int LIMX, LIMY, N, k;

    if ((f = fopen(name, "r")) == NULL) {
        printf("Erro na ABERTURA do arquivo <%s>\n\n", name);
        return 0;
    }

    fgets(LINHA, 80, f);

    sscanf(LINHA, "%d %d", &LIMX, &LIMY);

    fgets(LINHA, 80, f);

    sscanf(LINHA, "%d", &N);

    img = alloc_memo(LIMX, LIMY);

    if (alloc_memo(LIMX, LIMY)) {
        for (int i = 0; i < N*2; i++) {
            fscanf(f, "%d", &k);
            img[i] = k;
        }

        fclose(f);

    } else {
        printf("Erro na ALOCACAO DE MEMORIA para o arquivo <%s>\n\n", name);
        printf("Rotina: le_imagem_pgm\n\n");
        fclose(f);
        return 0;
    }
    *nl = LIMX;
    *nc = LIMY;
    return img;
}

int main(int argc, char *argv[]){
    image in;
    char *p;
    char nameIn[40];
    char nameOut[40];
    char command[100];
    int nl, nc;
    if (argc < 2)
    {
        printf("Uso: %s <imagem[.pts]>\n\n", argv[0]);
        return 1;
    }
   if (p = strstr(argv[1], ".pts"))
       *p = 0;
   strcpy(nameIn, argv[1]);
   strcat(nameIn, ".pts");
   strcpy(nameOut, argv[1]);
   strcat(nameOut, ".pbm");

    // transformation
    in = read_pts(nameIn, &nl, &nc);
    write_pbm(in, nameOut, nl, nc);

    sprintf(command, "%s %s &", VIEW, nameOut);
    system(command);
    free(in);
    return 0;
}