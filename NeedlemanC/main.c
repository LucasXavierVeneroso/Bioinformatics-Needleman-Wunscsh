#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matrixcell {
    int value;
    int direction;
} cell;

//Converting letter,1s to their respective positions in Blosum62 matrix
int lettertopos(char letter){
    if ('A'==letter) return 0;
    if ('C'==letter) return 4;
    if ('D'==letter) return 3;
    if ('E'==letter) return 6;
    if ('F'==letter) return 13;
    if ('G'==letter) return 7;
    if ('H'==letter) return 8;
    if ('I'==letter) return 9;
    if ('K'==letter) return 11;
    if ('L'==letter) return 10;
    if ('M'==letter) return 12;
    if ('N'==letter) return 2;
    if ('P'==letter) return 14;
    if ('Q'==letter) return 5;
    if ('R'==letter) return 1;
    if ('S'==letter) return 15;
    if ('T'==letter) return 16;
    if ('V'==letter) return 19;
    if ('W'==letter) return 17;
    if ('Y'==letter) return 18;
    else printf("something went wrong\n");
    return 0;
}

//Implementing Needleman-Wunsch
void needleman (char* string1, char* string2, int penalty, int pointMatrix[20][20]){
    int len1 = strlen(string1)+1;
    int len2 = strlen(string2)+1;

    cell** matrix = malloc(len1*sizeof(cell*));
    for(int i = 0; i < len1; i++){
        matrix[i] = malloc(len2*sizeof(cell));
    }

    for(int i = 0; i < len1; i++){
        matrix[i][0].value = 0;
    }
    
    for(int i = 0; i < len2; i++){
        matrix[0][i].value = 0;
    }

    

    for(int i = 1; i < len1; i++){
        for(int j = 1; j < len2; j++){
            int match = matrix[i-1][j-1].value + pointMatrix[lettertopos(string1[i-1])][lettertopos(string2[j-1])];
            int del = matrix[i-1][j].value - penalty;
            int ins = matrix[i][j-1].value - penalty;
            if (match > del && match > ins){
                matrix[i][j].direction = 0;
                matrix[i][j].value = match;
            }
            else if (del > ins){
                matrix[i][j].direction = 1;
                matrix[i][j].value = del;
            }
            else{
                matrix[i][j].direction = 2;
                matrix[i][j].value = ins;
            }
        }
    }

    printf("Score: %d\n",matrix[len1-1][len2-1].value);

    int buffer = 4*(len1+len2);
    char aux[buffer],finalstr1[buffer],finalstr2[buffer];
    int i = len1-1;
    int j = len2-1;

    memset(aux,0,sizeof(aux));
    memset(finalstr1,0,sizeof(finalstr1));
    memset(finalstr2,0,sizeof(finalstr2));

    while (i > 0 || j > 0) {
        switch (matrix[i][j].direction){
        case 0:
            aux[0] = string1[i-1];
            strcat(aux,finalstr1);
            strcpy(finalstr1,aux);
            memset(aux,0,sizeof(aux));
            aux[0] = string2[j-1];
            strcat(aux,finalstr2);
            strcpy(finalstr2,aux);
            memset(aux,0,sizeof(aux));
            i--;
            j--;
            break;
        case 1:
            aux[0] = string1[i-1];
            strcat(aux,finalstr1);
            strcpy(finalstr1,aux);
            memset(aux,0,sizeof(aux));
            aux[0] = '-';
            strcat(aux,finalstr2);
            strcpy(finalstr2,aux);
            memset(aux,0,sizeof(aux));
            i--;
            break;
        case 2:
            aux[0] = '-';
            strcat(aux,finalstr1);
            strcpy(finalstr1,aux);
            memset(aux,0,sizeof(aux));
            aux[0] = string2[j-1];
            strcat(aux,finalstr2);
            strcpy(finalstr2,aux);
            memset(aux,0,sizeof(aux));
            j--;
            break;
        }
    }
    printf("%s\n%s\n",finalstr1,finalstr2);
}


int main(int argc, char** argv){
    //Blosum62
    int blosum62[20][20] = {
       {4, -1, -2, -2,  0, -1, -1,  0, -2, -1, -1, -1, -1, -2, -1,  1,  0, -3, -2,  0},
       {-1, 5,  0, -2, -3,  1,  0, -2,  0, -3, -2,  2, -1, -3, -2, -1, -1, -3, -2, -3},
       {-2, 0,  6,  1, -3,  0,  0,  0,  1, -3, -4,  0, -2, -3, -2,  1,  0, -4, -2, -3},
       {-2,-2,  1,  6, -3,  0,  2, -1, -1, -3, -4, -1, -3, -3, -1,  0, -1, -4, -3, -3},
       { 0,-3, -3, -3,  9, -3, -4, -3, -3, -1, -1, -3, -1, -2, -3, -1, -1, -2, -2, -1},
       {-1, 1,  0,  0, -3,  5,  2, -2,  0, -3, -2,  1,  0, -3, -1,  0, -1, -2, -1, -2},
       {-1, 0,  0,  2, -4,  2,  5, -2,  0, -3, -3,  1, -2, -3, -1,  0, -1, -3, -2, -2},
       { 0,-2,  0, -1, -3, -2, -2,  6, -2, -4, -4, -2, -3, -3, -2,  0, -2, -2, -3, -3},
       {-2, 0,  1, -1, -3,  0,  0, -2,  8, -3, -3, -1, -2, -1, -2, -1, -2, -2,  2, -3},
       {-1,-3, -3, -3, -1, -3, -3, -4, -3,  4,  2, -3,  1,  0, -3, -2, -1, -3, -1,  3},
       {-1,-2, -4, -4, -1, -2, -3, -4, -3,  2,  4, -2,  2,  0, -3, -2, -1, -2, -1,  1},
       {-1, 2,  0, -1, -3,  1,  1, -2, -1, -3, -2,  5, -1, -3, -1,  0, -1, -3, -2, -2},
       {-1,-1, -2, -3, -1,  0, -2, -3, -2,  1,  2, -1,  5,  0, -2, -1, -1, -1, -1,  1},
       {-2,-3, -3, -3, -2, -3, -3, -3, -1,  0,  0, -3,  0,  6, -4, -2, -2,  1,  3, -1},
       {-1,-2, -2, -1, -3, -1, -1, -2, -2, -3, -3, -1, -2, -4,  7, -1, -1, -4, -3, -2},
       { 1,-1,  1,  0, -1,  0,  0,  0, -1, -2, -2,  0, -1, -2, -1,  4,  1, -3, -2, -2},
       { 0,-1,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1, -2, -1,  1,  5, -2, -2,  0},
       {-3,-3, -4, -4, -2, -2, -3, -2, -2, -3, -2, -3, -1,  1, -4, -3, -2, 11,  2, -3},
       {-2,-2, -2, -3, -2, -1, -2, -3,  2, -1, -1, -2, -1,  3, -3, -2, -2,  2,  7, -1},
       { 0,-3, -3, -3, -1, -2, -2, -3, -3,  3,  1, -2,  1, -1, -2, -2,  0, -3, -1,  4}
    };

    if(argc < 4){
        printf("Formato correto: ./main string1 string2 penalidade");
    }

    needleman(argv[1],argv[2],atoi(argv[3]),blosum62);
}