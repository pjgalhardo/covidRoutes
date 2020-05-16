/*  2º Projeto - ASA 2019/20
    António Lopes- 89415
    Pedro Galhardo - 89517    */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int C = 0;                  /* número de cidadaos */
int S = 0;                  /* número de supermercados */
int M = 0;                  /* número de avenidas (vertical) */
int N = 0;                  /* número de ruas (horizontal) */
int pathsFinal = 0;

typedef struct graph *graph;
typedef struct node *node;
typedef struct queue queue;
typedef struct element element;

struct node {
    int n;
    int visited;
    int supermarket;
    int citizen;
    int solution;
};

struct graph {
    int total;
    node *nodes;
    node *supermarkets;
    node *citizens;
};

struct element {
    int data;
    element *next;
};

struct queue {
    int count;
    element *front;
    element *rear;
};


int getN(int av, int st) {
    return (av + (st-1) * M);
}

void getCoords(int n, int *av, int *st) {
    *av = (n % M);
    *st = (n / M);

    if (*av == 0)
        *av = M;
    else
        (*st)++;
}

void printGrid(graph G) {
    int i;
    
    printf("\n\n");
    for (i = 1; i < G->total + 1; i++) {
        /*printf("%2d - ", i);*/
        if (G->nodes[i]->visited != 0) {
            if (G->nodes[i]->supermarket != 0) {
                printf("%2d", G->nodes[G->nodes[i]->solution]->solution);
            }
            else {
                printf("%2d", G->nodes[i]->solution);   
            }
        }
        else if (G->nodes[i]->visited == 0) {
                printf("--");   
        }
        if (G->nodes[i]->citizen != 0) {
                printf("C  ");
        }
        else if (G->nodes[i]->supermarket != 0) {
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

void initializeQueue(queue *q) {
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

int isQueueEmpty(queue *q) {
    return (q->rear == NULL);
}

void enQueue(queue *q, int value) {
    element *tmp;
    tmp = malloc(sizeof(element));

    tmp->data = value;
    tmp->next = NULL;
    
    if(!isQueueEmpty(q)) {
        q->rear->next = tmp;
        q->rear = tmp;
    }

    else {
        q->front = tmp; 
        q->rear = tmp;
    }
    
    q->count++;
}

int deQueue(queue *q) {
    int n;
    element *tmp;
    n = q->front->data;
    
    tmp = q->front;
    q->front = q->front->next;
    
    q->count--;
    if (q->count == 0) {
        q->rear = NULL;
    }
    free(tmp);
    return n;
}

graph initializeGraph() {
    int i, j, av, st;
    node l;
    graph G = malloc(sizeof(graph));

    G->total = N *M;
    G->nodes = malloc(((G->total) + 1) * sizeof(node));
    G->supermarkets = malloc(S * sizeof(node));
    G->citizens = malloc(C * sizeof(node));
    
    for (i = 0; i <= G->total; i++) {
        l = malloc(sizeof(node));
        l->n = i;
        l->visited = 0;
        l->citizen = 0;
        l->supermarket = 0;
        l->solution = 0;
        G->nodes[i] = l;
    }
    
    for (i = 0; i < S; i++) {
        scanf("%d %d", &av, &st);
        for (j = 0; j < i; j++) {
            if (G->supermarkets[j]->n == getN(av, st)) {
                S--;
                scanf("%d %d", &av, &st);
            }
        }
        l = G->nodes[getN(av, st)];
        l->supermarket = 1;
        G->supermarkets[i] = l;
    }

    for (i = 0; i < C; i++) {
        scanf("%d %d", &av, &st);
        l = G->nodes[getN(av, st)];
        l->citizen = 1;
        l->visited = 1;
        G->citizens[i] = l;
    }

    return G;
}

graph initializeGraphdosMercados(graph Gold) {
    int i;
    node l;
    graph G = malloc(sizeof(graph));

    G->total = N *M;
    G->nodes = malloc(((G->total) + 1) * sizeof(node));
    G->supermarkets = malloc(S * sizeof(node));
    G->citizens = malloc(C * sizeof(node));
    
    for (i = 0; i <= G->total; i++) {
        l = malloc(sizeof(node));
        l->n = i;
        l->visited = 0;
        l->citizen = 0;
        l->supermarket = 0;
        l->solution = 0;
        G->nodes[i] = l;
    }
    
    for (i = 0; i < S; i++) {
        l = G->nodes[Gold->supermarkets[i]->n];
        l->supermarket = 1;
        l->visited = 1;
        G->supermarkets[i] = l;
    }

    for (i = 0; i < C; i++) {
        l = G->nodes[Gold->citizens[i]->n];
        l->citizen = 1;
        G->citizens[i] = l;
    }

    return G;
}

void getNeighbours(int node, int neighbours[]) {
    int i, av, st;

    getCoords(node, &av, &st);
    
    for (i = 0; i < 4; i++) {
        if (i == 0 && (av + 1) <= M)
            neighbours[i] = node + 1;
        else if (i == 1 && (av - 1) > 0)
            neighbours[i] = node - 1;
        else if (i == 2 && (st + 1) <= N)
            neighbours[i] = node + M;
        else if (i == 3 && (st - 1) > 0)
            neighbours[i] = node - M;
        else
            neighbours[i] = 0;
    }
}

void printPaths(graph G) {
    int i;
    printf("\n");
    for (i = 0; i < S; i++) {
        printf("Super: %d Citizen: %d\n", G->supermarkets[i]->n, G->supermarkets[i]->solution);
    }
}

int BFSVisitdosMercados(graph G, int node) {
    int i, j, s, av, st;
    int sol;
    queue *q;
    int *parent, *color, *suspects;
    int neighbours[4];

    color = malloc(((G->total) + 1) * sizeof(int));
    parent = malloc(((G->total) + 1) * sizeof(int));
    for (i = 0; i < (G->total) + 1; i++) {
        color[i] = 0;
        parent[i] = 0;
    }

    suspects = malloc(C * sizeof(int));
    for (i = 0; i < C; i++) {
        suspects[i] = 0;
    }

    q = malloc(sizeof(queue));
    initializeQueue(q);
    enQueue(q, node);

    s = 0;

    while(!isQueueEmpty(q)) {
        j = deQueue(q);
        getCoords(j, &av, &st);
        getNeighbours(j, neighbours);
        if (G->nodes[j]->supermarket == 1 && G->nodes[j]->citizen == 1) {
                parent[j] = 0;
                s = j;
        }
        if (s==0) {
            for (i = 0; i < 4; i++) {
                
                if (neighbours[i] && G->nodes[neighbours[i]]->visited != 1 && color[neighbours[i]] == 0) {
                    parent[neighbours[i]] = j;
                    if (G->nodes[neighbours[i]]->citizen) {
                        s = neighbours[i];
                        break;
                    }
                    color[neighbours[i]] = 1;
                    
                    enQueue(q, neighbours[i]);
                }
            }
        }

        if (s != 0) {
            
            
            sol = s;
            while (s != 0) {
                G->nodes[s]->visited = 1;
                G->nodes[s]->solution = node;
                s = parent[s];
            }
            G->nodes[node]->solution = sol;

            free(color);
            

            free(parent);
            free(suspects);
            free(q);

            return 1;
        }

    }
    
    return 0;
}

int BFSVisit(graph G, int node) {
    int i, j, s, av, st;
    int sol;
    queue *q;
    int *parent, *color, *suspects;
    int neighbours[4];

    color = malloc(((G->total) + 1) * sizeof(int));
    parent = malloc(((G->total) + 1) * sizeof(int));
    for (i = 0; i < (G->total) + 1; i++) {
        color[i] = 0;
        parent[i] = 0;
    }

    suspects = malloc(C * sizeof(int));
    for (i = 0; i < C; i++) {
        suspects[i] = 0;
    }

    q = malloc(sizeof(queue));
    initializeQueue(q);
    enQueue(q, node);

    s = 0;

    while(!isQueueEmpty(q)) {
        j = deQueue(q);
        getCoords(j, &av, &st);
        getNeighbours(j, neighbours);
        if (G->nodes[j]->supermarket == 1 && G->nodes[j]->citizen == 1 && G->nodes[j]->solution == 0) {
                parent[j] = 0;
                s = j;
        }
        if (s == 0) {
            for (i = 0; i < 4; i++) {
                if (neighbours[i] && G->nodes[neighbours[i]]->visited != 1 && color[neighbours[i]] == 0) {
                    parent[neighbours[i]] = j;
                    if (G->nodes[neighbours[i]]->supermarket) {
                        s = neighbours[i];
                        break;
                    }
                    color[neighbours[i]] = 1;
                    
                    enQueue(q, neighbours[i]);
                }
            }
        }

        if (s != 0) {
            
            
            sol = s;
            while (s != 0) {
                G->nodes[s]->visited = 1;
                G->nodes[s]->solution = node;
                s = parent[s];
            }
            G->nodes[node]->solution = sol;

            free(color);
            free(parent);
            free(suspects);
            free(q);

            return 1;
        }

    }
    
    return 0;
}

void BFSdosMercados(graph G) {
    int i;
    int paths = 0;

    for (i = 0; i < S; i++) {
        if (BFSVisitdosMercados(G, G->supermarkets[i]->n)) {
            paths++;
            if (paths == C)
                break;
        }
    }

    if (paths > pathsFinal)
        printf("%d\n", paths);
    else
        printf("%d\n", pathsFinal);
}

void BFS(graph G) {
    graph G1;
    int i;
    int paths = 0;

    for (i = 0; i < C; i++) {
        if (BFSVisit(G, G->citizens[i]->n)) {
            paths++;
            if (paths == S)
                break;
        }
    }
    if (paths == S)
        printf("%d\n", paths);
    else {
        pathsFinal = paths;
        G1 = initializeGraphdosMercados(G);
        BFSdosMercados(G1);
        /*printGrid(G1);
        printPaths(G1);*/
    }
}

int main() {
    graph G;

    scanf("%d %d", &M, &N);
    scanf("%d %d", &S, &C);

    G = initializeGraph();
    /*G1 = initializeGraphdosMercados(G);*/

    /*printGrid(G);*/

    BFS(G);


    return 0;
}