/*      2º Projeto - ASA 2019/20
        António Lopes- 89415
        Pedro Galhardo - 89517      */

#include <stdio.h>
#include <stdlib.h>


int C = 0;                                      /* número de cidadaos */
int S = 0;                                      /* número de supermercados */
int M = 0;                                      /* número de avenidas (vertical) */
int N = 0;                                      /* número de ruas (horizontal) */
	

int** createList (int size) {
    int** list;
    int i;
    list = malloc(size * sizeof(int*));
    for (i = 0; i < size; i++) {
        list[i] = malloc(2 * sizeof(int));
    }
    
    return list;
}

void fillLists(int*** super, int*** citi) {
    
    int i;
    int av, rua;

    for (i = 0; i < S; i++) {
        scanf("%d %d", &av, &rua);
        (*super)[i][0] = av;
        (*super)[i][1] = rua;
    }

    for (i = 0; i < C; i++) {
        scanf("%d %d", &av, &rua);
        (*citi)[i][0] = av;
        (*citi)[i][1] = rua;
    }

}

int isCitizen(int av, int rua, int **cidadaos) {
    int i, c;
    c = 0;
    
    for (i = 0; i < C; i++) {
        if (av == cidadaos[i][0] && rua == cidadaos[i][1]) {
            c++;
        }
    }

    return c;
}

int isSupermarket(int av, int rua, int **supermercados) {
    int i, s;
    s = 0;
    
    for (i = 0; i < S; i++) {
        if (av == supermercados[i][0] && rua == supermercados[i][1]) {
            s++;
        }
    }

    return s;
}

void printGrid(int **supermercados, int **cidadaos) {
    int i, j;
    int citi, super;
    

    for (i = 0; i < N; i++) {
        for (j = M; j > 0; j--) {

            citi = isCitizen(i+1, j, cidadaos);
            super = isSupermarket(i+1, j, supermercados);
            if (citi != 0) {
                printf("%d  ", citi);
            }
            else if (super != 0) {
                printf("S  ");
            }
            else {
                printf(".  ");
            }
        }
        printf("\n\n");
    }
}

int main() {

    scanf("%d %d", &M, &N); /*avenidas e ruas*/
    scanf("%d %d", &S, &C); /*supermercados e cidadaos*/
    
    int **supermarkets = createList(S);
    int **citizens = createList(C);
    
    fillLists(&supermarkets, &citizens);
    

    printGrid(supermarkets, citizens);
    

    return 0;
}