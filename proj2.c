/*      2º Projeto - ASA 2019/20
        António Lopes- 89415
        Pedro Galhardo - 89517      */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int C = 0;                                      /* número de cidadaos */
int S = 0;                                      /* número de supermercados */
int M = 0;                                      /* número de avenidas (vertical) */
int N = 0;                                      /* número de ruas (horizontal) */
	
int print = 0;

typedef struct graph *Graph;
typedef struct elem *element;
typedef struct node *laink;

struct node {
    int n;  /*precisa de n? ja ta na posicao mas ok*/                                    /* cada elemento das listas, guarda um índice de um vertice */
    int visited;
    int supermarket;
    int citizen;
    int solution;
    element path;
};

struct elem {
    int n;
    element next;
};

struct graph {
    int total;                                      /* número de vértices (vertexes) */
    laink* adj;
    laink* c;
    laink* s;
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

void initializeBfsArrays(Graph G, int** color, int ** parent, int** suspects) {
    int i;
    *color = malloc((G->total+1) * sizeof(int)); /*começa no 1*/
    *parent = malloc((G->total+1) * sizeof(int)); /*começa no 1*/
    *suspects = malloc(C * sizeof(int));
    for (i = 1; i < G->total + 1; i++) {
        (*color)[i] = 0; /*white = 0 gray = 1 black = 2*/
        (*parent)[i] = 0; 
    }
    for (i = 0; i < C; i++) {
        (*suspects)[i] = 0;
    }
}

Graph initializeGraph() {
    int i, av, rua;
    laink l;
    Graph G = malloc(sizeof(struct graph));
    G->total = N*M;
    G->c = malloc(C*sizeof(laink)); /*começa no 0*/
    G->s = malloc(S*sizeof(laink)); /*começa no 0*/
    G->adj = malloc((G->total+1) * sizeof(laink)); /*começa no 1*/
    

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

void setSolution(Graph G, int s, int* parent, int at) {
    element p;
    
    p = NULL;
    G->adj[s]->solution = at;
    G->adj[at]->solution = s;
    if (print == 1)
        printf("SOLUCAO: %d\n", G->adj[s]->solution);
    while (s != 0) {
        G->adj[s]->visited = 1;
        if (print == 1)
            printf("%d ", s);
        enQueue(&p, s);        
        s = parent[s];
        
    }
    G->adj[at]->path = p;
    
        
}

int visitNeighbour(Graph G, int neighbour, int ** color, int ** parent, element *head, int j, int* s) {

    if (G->adj[neighbour]->visited != 1 && (*color)[neighbour] == 0) {
        (*parent)[neighbour] = j;
        if (G->adj[neighbour]->supermarket) {
            *s = neighbour;
            return 1;
        }
        (*color)[neighbour] = 1;
        enQueue(head, neighbour);
    }
    return 0;
}

void resetVisited(Graph G, int at) {
    element l;
    l = G->adj[at]->path;
    while (l != NULL) {
        if (!G->adj[l->n]->citizen && !G->adj[l->n]->supermarket) {
            G->adj[l->n]->visited = 0;
        }
        l = l->next;
    }
}

void setVisited(Graph G, int at) {
    element l;
    l = G->adj[at]->path;
    while (l != NULL) {
        G->adj[l->n]->visited = 1;
        l = l->next;
    }
}

void verifySuspect(int visited, int supermarket, int** suspects, int suspect, int at) {
    int i;
    if (visited && supermarket && suspect != at) {
        for (i = 0; i < C && (*suspects)[i] != 0; i++) {
            ;
        }
        (*suspects)[i] = suspect;
    }
}

int bfsVisit(Graph G, int at, int backtrack) {

    int *parent, *color, *suspects;
    element head;
    int j, s, av, rua, neighbour;
    initializeBfsArrays(G, &color, &parent, &suspects);
            
    head = NULL;
    enQueue(&head, at);  
    s = 0;
    if (print == 1)
        printf("\nNOVO CICLO\n");
    while ((j = deQueue(&head)) != 0) {
                
        getCoords(j, &av, &rua);
                            
        if((av+1) <= M) {
            neighbour = getN(av+1, rua);
            if (visitNeighbour(G, neighbour, &color, &parent, &head, j, &s)) {
                break;
            }
            verifySuspect(G->adj[neighbour]->visited, G->adj[neighbour]->supermarket, &suspects, G->adj[neighbour]->solution, at);
        }

        if((av-1) > 0) {
            neighbour = getN(av-1, rua);
            if (visitNeighbour(G, neighbour, &color, &parent, &head, j, &s)) {
                break;
            }
            verifySuspect(G->adj[neighbour]->visited, G->adj[neighbour]->supermarket, &suspects, G->adj[neighbour]->solution, at);
        }

        if((rua+1) <= N) {
            neighbour = getN(av, rua+1);
            if (visitNeighbour(G, neighbour, &color, &parent, &head, j, &s)) {
                break;
            }
            verifySuspect(G->adj[neighbour]->visited, G->adj[neighbour]->supermarket, &suspects, G->adj[neighbour]->solution, at);
        }

        if((rua-1) > 0) {
            neighbour = getN(av, rua-1);
            if (visitNeighbour(G, neighbour, &color, &parent, &head, j, &s)) {
                break;
            }
            verifySuspect(G->adj[neighbour]->visited, G->adj[neighbour]->supermarket, &suspects, G->adj[neighbour]->solution, at);
        }

        color[j] = 2;
    }
            
    while (head != NULL) {
        deQueue(&head);
    }
    if (print == 1)
        printf("\nPATH: at: %d | ", at);
    if (s != 0) {
        setSolution(G, s, parent, at);
    }
    else if (backtrack--) {
        int p, l;
        for (p = 0; suspects[p] != 0; p++) {
            if (print==1)
                sleep(1);
            if (print == 1)
                printf("SUSPECT: %d\n", suspects[p]);
            l = G->adj[suspects[p]]->solution;
            resetVisited(G, suspects[p]);
            if (bfsVisit(G, suspects[p], backtrack) == 1) {
                setSolution(G, l, parent, at);
                s = l;
                break;
            }
            else {
                if (print == 1)
                    printf("VOLTEI\n");
                setVisited(G, suspects[p]);
            }
            
        }
    }
    
    free(color);
    free(parent);
    free(suspects);
    if (s != 0) {
        return 1;
    }
    else {
        return 0;
    }
    if (print == 1)
        printf("\n");
}

void bfs(Graph G) {
    
    int i;
    int paths = 0;

    
    paths = 0;
    for (i = 0; i < C; i++) {
        if (bfsVisit(G, G->c[i]->n, 2)) {
            paths++;
        }
    }
    
    
    printf("%d\n", paths);
}

int main() {

    Graph G;

    scanf("%d %d", &M, &N); /*avenidas e ruas*/
    scanf("%d %d", &S, &C); /*supermercados e cidadaos*/


    G = initializeGraph();
    if (print == 1)
        printGrid(G);
    bfs(G);

    return 0;
}