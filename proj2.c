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
typedef struct elem *element;
typedef struct node *link;

struct node {
    int n;  /*precisa de n? ja ta na posicao mas ok*/                                    /* cada elemento das listas, guarda um índice de um vertice */
    int visited;
    int supermarket;
    int citizen;
};

struct elem {
    int n;
    element next;
};

struct graph {
    int total;                                      /* número de vértices (vertexes) */
    link* adj;
    link* c;
    link* s;
};

element createElement(int n) {
    element q;
    q = malloc(sizeof(struct elem)); /*começa no 1*/
    q->n = n;
    q->next = NULL;
    return q;
}

int getN(int av, int rua) {
    return av + (rua-1) * M;
}

void enQueue(element *head, int n) {
    element i, q;
    q = createElement(n);

    
    if (*head == NULL) {
        *head = q;
    }
    else {
        for (i = *head; i->next != NULL; i = i->next) {
            ;
        }
    
        i->next = q;
    }
}

int deQueue(element* head) {
    element q;
    int n;
    
    if (*head == NULL) {
        return 0;
    }
    q = *head;
    *head = (*head)->next;
    n = q->n;
    free(q);
    return n;
}

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
        l = G->adj[getN(av, rua)];
        l->supermarket = 1;
        G->s[i] = l;
    }

    for (i = 0; i < C; i++) { /*ver se fica igual nas duas listas*/
        scanf("%d %d", &av, &rua);
        l = G->adj[getN(av, rua)];
        l->citizen = 1;
        l->visited = 1;
        G->c[i] = l;
    }

    return G;
}

void getCoords(int n, int *av, int *rua) {
    *av = (n % M);
    *rua = (n / M);
    if (*av == 0)
        *av = M;
    else
        (*rua)++;
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

/*void visitNeighbour(Graph G, int ** color, int ** parent, element head, int av, int rua) {
    int neighbour;
    neighbour = getN(av+1, rua);
    
    if (G->adj[neighbour]->visited != 1 && color[neighbour] == 0 && (av) < M) {
        parent[neighbour] = j;
        if (G->adj[neighbour]->supermarket) {
            s = neighbour;
            break;
        }
        color[neighbour] = 1;
        enQueue(&head, neighbour);
    }
}**/

void resetVisited(Graph G) {
    int i;
    link last;
    for (i = 0; i <= G->total; i++) {
        G->adj[i]->visited = 0;
    }
    last = G->c[0];
    for (i = 0; i < C-1; i++) {
        G->c[i] = G->c[i+1];
    }
    G->c[C-1] = last;
}

void bfs(Graph G) {
    int *parent, *color;
    element head;
    int i, j, h, k, av, rua, s, neighbour;
    int paths = 0;
    int finalpaths = 0;
    for (k = 0; k < C; k++) {
        paths = 0;
        /*printf("\nNUEVA ORDEM\n\n");*/
        for (i = 0; i < C; i++) {
            color = malloc((G->total+1) * sizeof(int)); /*começa no 1*/
            parent = malloc((G->total+1) * sizeof(int)); /*começa no 1*/
            for (h = 1; h < G->total + 1; h++) {
                color[h] = 0; /*white = 0 gray = 1 black = 2*/
                parent[h] = 0; 
            }
            head = NULL;
            enQueue(&head, G->c[i]->n);
            s = 0;
            /*printf("NOVO CICLO\n");*/
            while ((j = deQueue(&head)) != 0) {
                getCoords(j, &av, &rua);
                if((av+1) <= M) {
                    neighbour = getN(av+1, rua);
                    if (G->adj[neighbour]->visited != 1 && color[neighbour] == 0) {
                        parent[neighbour] = j;
                        if (G->adj[neighbour]->supermarket) {
                            s = neighbour;
                            break;
                        }
                        color[neighbour] = 1;
                        enQueue(&head, neighbour);
                    }
                }

                if((av-1) > 0) {
                    neighbour = getN(av-1, rua);
                    if (G->adj[neighbour]->visited != 1 && color[neighbour] == 0) {
                        parent[neighbour] = j;
                        if (G->adj[neighbour]->supermarket) {
                            s = neighbour;
                            break;
                        }
                        color[neighbour] = 1;
                        enQueue(&head, neighbour);
                    }
                }

                if((rua+1) <= N) {
                    neighbour = getN(av, rua+1);
                    if (G->adj[neighbour]->visited != 1 && color[neighbour] == 0) {
                        parent[neighbour] = j;
                        if (G->adj[neighbour]->supermarket) {
                            s = neighbour;
                            break;
                        }
                        color[neighbour] = 1;
                        enQueue(&head, neighbour);
                    }
                }

                if((rua-1) > 0) {
                    neighbour = getN(av, rua-1);
                    if (G->adj[neighbour]->visited != 1 && color[neighbour] == 0) {
                        parent[neighbour] = j;
                        if (G->adj[neighbour]->supermarket) {
                            s = neighbour;
                            break;
                        }
                        color[neighbour] = 1;
                        enQueue(&head, neighbour);
                    }
                }

                color[getN(av, rua)] = 2;
            }
            while (head != NULL) {
                deQueue(&head);
            }
            /*printf("PATH: ");*/
            if (s != 0) {
                paths++;
            }
            while (s != 0) {
                G->adj[s]->visited = 1;
                /*printf("%d ", s);*/
                s = parent[s];
            }
            free(color);
            free(parent);
            /*printf("\n");*/
        }
        if (paths > finalpaths) {
            finalpaths = paths;
        }
        if (finalpaths == C || finalpaths == S) {
            break;
        }
        resetVisited(G);
    }

    printf("%d\n", finalpaths);
}

int main() {

    Graph G;

    scanf("%d %d", &M, &N); /*avenidas e ruas*/
    scanf("%d %d", &S, &C); /*supermercados e cidadaos*/


    G = initializeGraph();

    /*printGrid(G);*/
    bfs(G);

    return 0;
}