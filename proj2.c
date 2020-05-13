/*      2º Projeto - ASA 2019/20
        António Lopes- 89415
        Pedro Galhardo - 89517      */

#include <stdio.h>
#include <stdlib.h>


int C = 0;                                      /* número de cidadaos */
int S = 0;                                      /* número de supermercados */
int M = 0;                                      /* número de avenidas (vertical) */
int N = 0;                                      /* número de ruas (horizontal) */
	
typedef struct graph *Graph;
typedef struct node *link;

struct node {
    int n;  /*precisa de n? ja ta na posicao mas ok*/                                    /* cada elemento das listas, guarda um índice de um vertice */
    int visited;
    int supermarket;
    int citizen;
};


struct graph {
    int total;                                      /* número de vértices (vertexes) */
    link* adj;
    link* c;
    link* s;
};

Graph initializeGraph() {
    int i, av, rua;
    link l;
    Graph G = malloc(sizeof(struct graph));
    G->total = N*M;
    G->c = malloc(C*sizeof(link)); /*começa no 0*/
    G->s = malloc(S*sizeof(link)); /*começa no 0*/
    G->adj = malloc((G->total+1) * sizeof(link)); /*começa no 1*/
    

    for (i = 0; i <= G->total; i++) {
        l = malloc(sizeof(struct node));
        l->n = i;
        l->visited = 0;
        l->supermarket = 0;
        l->citizen = 0;
        G->adj[i] = l;
    }
    
    for (i = 0; i < S; i++) { /*ver se fica igual nas duas listas*/
        scanf("%d %d", &av, &rua);
        l = G->adj[av + (rua-1) * M];
        l->supermarket = 1;
        G->s[i] = l;
    }

    for (i = 0; i < C; i++) { /*ver se fica igual nas duas listas*/
        scanf("%d %d", &av, &rua);
        l = G->adj[av + (rua-1) * M];
        l->citizen = 1;
        G->s[i] = l;
    }

    return G;
}



void printGrid(Graph G) {
    int i;
    

    for (i = 1; i < G->total + 1; i++) {
        if (G->adj[i]->citizen != 0) {
                printf("C  ");
        }
        else if (G->adj[i]->supermarket != 0) {
                printf("S  ");   
        }
        else {
                printf(".  ");
            }    
        if (i % M == 0) {
            printf("\n\n");
        }
    }
}

int main() {

    Graph G;

    scanf("%d %d", &M, &N); /*avenidas e ruas*/
    scanf("%d %d", &S, &C); /*supermercados e cidadaos*/


    G = initializeGraph();

    printGrid(G);
    

    return 0;
}