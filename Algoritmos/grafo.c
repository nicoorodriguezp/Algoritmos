#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct Arista{
    struct Node *node;
    struct Arista *next;
    struct Arista *former;

};

struct  Node{
    char name;
    int  id;
    struct Node* next;
    struct Node* former;
    struct Arista*adjacent;

};

struct Node *firstNode = NULL;

int cant_nodes = 0;
int next_id = 0;
int cant_impares = 0;

//>>>>>>>>>>>>>>>>>>> Matrices <<<<<<<<<<<<<<<<<<<<<<<

void showMatrix(int matrix[cant_nodes][cant_nodes]){
    
    // showing the matrix
    for(int row = 0; row < cant_nodes; row ++){
        printf(" | ");
        for(int column = 0; column < cant_nodes; column ++){
            printf("%d", matrix[row][column]);
            printf(" | ");
        }
        printf("\n");
    }
}

void copyMatrix(int origen[cant_nodes][cant_nodes], int final[cant_nodes][cant_nodes]){
    
    for(int row = 0; row < cant_nodes; row ++){
        for(int column = 0; column < cant_nodes; column ++){
            final[row][column] = origen[row][column];
        }
    }
}

void sumMatrix(int resultingMatrix[cant_nodes][cant_nodes], int auxMatrix[cant_nodes][cant_nodes]){

    for(int i=0;i < cant_nodes;i++)
    {
        for(int j=0 ;j < cant_nodes;j++){
          resultingMatrix[i][j] = resultingMatrix[i][j] + auxMatrix[i][j];
        } 
    }

}

void multiplyMatrices(int adjacentMatrix[cant_nodes][cant_nodes], int aux[cant_nodes][cant_nodes], int result[cant_nodes][cant_nodes]) {

   // Initializing elements of matrix mult to 0.
   for (int i = 0; i < cant_nodes; ++i) {
      for (int j = 0; j < cant_nodes; ++j) {
         result[i][j] = 0;
      }
   }

   // Multiplying first and second matrices and storing it in result
   for (int i = 0; i < cant_nodes; ++i) {
      for (int j = 0; j < cant_nodes; ++j) {
         for (int k = 0; k < cant_nodes; ++k) {
            result[i][j] += adjacentMatrix[i][k] * aux[k][j];
         }
      }
   }
}



//>>>>>>>>>>>>>>>>>>> Nodes <<<<<<<<<<<<<<<<<<<<<<<

bool existsNode(char name){

    if(firstNode != NULL){

        struct Node *aux = firstNode;

        while(aux != NULL){
            if(aux->name == name){
                return true;
            }else{
                aux = aux->next;
            }
        }

    }

    return false;

}

void getNode(struct Node **node, char name){

    struct Node *aux = firstNode;

    while(aux != NULL){
        if(aux->name == name){
            *node = aux;
            break;
        }
        else if(aux->next != NULL){
           aux = aux->next; 
        }else{
            printf("El nodo %c no existe.\n", name);
            break;
        }
    }

}

void insertNode(){

    char name;

    fflush(stdin);
    printf("Ingrese identificacion de vertice(Ej: A): \n");
    scanf("%c",  &name);

    if(!existsNode(name)){
        struct Node* aux;
        struct Node* new = (struct Node* )malloc(sizeof(struct Node));
        // new node init
        new->id = next_id;
        new->name = name;
        new->next = NULL;
        new->former = NULL;
        new->adjacent = NULL;

        // if node start is null, replace it with the new one 
        if(firstNode==NULL){
            firstNode = new;
        }else{
            aux=firstNode;

            // Looking for the last node in the list of nodes
            while(aux->next != NULL){
                aux = aux->next;
            }

            new->former = aux; // the new one has now as former the last one in the list
            aux->next = new; // the last one has the next node as the new one
            

        }

        cant_nodes += 1;
        next_id += 1;
        printf("\n\n La cantidad de nodos en el grafo es de %d\n\n", cant_nodes); 
    }else{
        printf("\nYa existe un nodo con el nombre: '%c'\n\n", name);
    }
    

}


void deleteNode(){

    char name;
    fflush(stdin);

    printf("Ingrese identificacion de vertice(Ej: A): \n");
    scanf("%c", &name);

    struct Node*  aux;
    getNode(&aux, name);

    if(aux != NULL){

        struct Node *next =  aux->next;
        struct Node *former = aux->former;

        if(former == NULL){
                     
            if(next != NULL){
                next->former = NULL;
                firstNode = next;
            }else{
                firstNode = NULL;
            }

        }else{
            if(next != NULL){
                former->next = next;
                next->former = former;
            }
        }

        free(aux);
        printf("Se elimino el nodo %c y todas sus aristas.\n\n", name);

    }

}



//>>>>>>>>>>>>>>>>>>> Aristas <<<<<<<<<<<<<<<<<<<<<<<


void addAdjacent(struct Node* from, struct Node* to){

    struct Arista *newArista = (struct Arista*)malloc(sizeof(struct Arista));
    
    newArista->node = to;
    newArista->next = NULL;
    newArista->former = NULL;

    struct Arista *a;

    // if it does not have an adjacent node yet so this is the first one
    if(from->adjacent == NULL){
        from->adjacent=newArista;
    }else{
        a = from->adjacent;
        while(a->next != NULL){
            a = a->next;
        }

        newArista->former = a;
        a->next = newArista;
    }

}

void deleteAdjacent(struct Node *from, struct Node *to){

    struct Arista *a = from->adjacent;

    if(a != NULL){
        while(a != NULL){

            if(a->node == to){

                struct Arista *next =  a->next;
                struct Arista *former = a->former;
                
                if(former == NULL){
                     
                    if(next != NULL){
                        next->former = NULL;
                        from->adjacent = next;
                    }else{
                        from->adjacent = NULL;
                    }

                }else{
                    if(next != NULL){
                        former->next = next;
                        next->former = former;
                    }
                }

                free(a);
                

                printf("Se elimino la conexion de %c -> %c. \n", from->name, a->node->name);
                break;
            }else{
                a = a->next;
            }
            
        }       
    }else{
        printf("El nodo %c no tiene aristas asociadas.\n", from->name);
    }

    
    

}


void deleteArista(){

    char node1, node2;

    struct Node *aux, *aux2;

    if(firstNode == NULL){
        printf("El grafo esta vacio. No tiene vertices.\n");
        return;
    }

    fflush(stdin);
    printf("Ingrese los nodos que quiere desconectar. Ej: A B \n");
    scanf("%c %c", &node1, &node2);

    // Looking for the nodes gave by the user
    getNode(&aux, node1);
    getNode(&aux2, node2);

    if(aux != NULL && aux2 != NULL){

        // deleting the adjacent to each node
        deleteAdjacent(aux, aux2);
        deleteAdjacent(aux2, aux);


    }

}

bool alreadyConnected(struct Node *from, struct Node *to){

    struct Arista *a = from->adjacent;

    if(a != NULL){
        while(a != NULL){
            if(a->node == to){
                printf("Los nodos %c y %c ya estan conectados.\n\n", from->name, to->name);
                return true;
            }
            a = a->next;
        }
    }else{
        return false;
    }

    return false;
}

void insertArista(){

    char node1, node2;

    struct Node *aux, *aux2;

    if(firstNode == NULL){
        printf("El grafo esta vacio. No tiene vertices.\n");
        return;
    }

    fflush(stdin);
    printf("Ingrese los nodos que quiere conectar. Ej: A B \n");
    scanf("%c %c", &node1, &node2);

    // Looking for the nodes gave by the user
    getNode(&aux, node1);
    getNode(&aux2, node2);

    if(aux != NULL && aux2 != NULL){

        if(!alreadyConnected(aux, aux2)){
            printf("aca \n\n");
            // adding the adjacent to each node
            addAdjacent(aux, aux2);
            addAdjacent(aux2, aux);
        }



    }else{
        printf("No se puedo crear una conexion a los vertices ingresados.\n");
    }

}

//>>>>>>>>>>>>>>>>>>> Eulerian Path <<<<<<<<<<<<<<<<<<<<<<<
bool isConexo(){

    struct Node* aux = firstNode;

    printf("\nLa matriz de Adyacencia es de %cx%c\n", cant_nodes, cant_nodes);
    int adjacentMatrix[cant_nodes][cant_nodes];

    int row, column;

    // initialize the matrix
    for(row = 0; row < cant_nodes; row ++){
        for(column = 0; column < cant_nodes; column ++){
            adjacentMatrix[row][column] = 0; 
        }
    }


    while(aux != NULL){

        struct Arista *a = aux->adjacent;

        row = aux->id;

        // scroll through the adjacency list to update the matrix.
        while(a != NULL){
            column = a->node->id;
            adjacentMatrix[row][column] = 1;

            a = a->next;
        }

        aux = aux->next;
    }


    printf("\n\n La matriz de adjacencia es:\n\n");
    showMatrix(adjacentMatrix);
    printf("\n\n");


    printf("\n\n La matriz resultante de hacer la sumatoria de potencias de la matriz adjacencia hasta A ^ (n-1):\n\n");
    int resultingMatrix[cant_nodes][cant_nodes];
    int auxMatrix[cant_nodes][cant_nodes];

    // copio la matriz
    copyMatrix(adjacentMatrix, resultingMatrix);
    copyMatrix(adjacentMatrix, auxMatrix);

    int result[cant_nodes][cant_nodes];

    // Si la matriz resultante de la sumatoria de adjacentMatrix ^ (cant_nodos-1) da distinto de cero excepto la diagonal, el grafo es conexo.
    for(int n = 1; n < cant_nodes-1; n++){
        multiplyMatrices(adjacentMatrix, auxMatrix, result);
        copyMatrix(result, auxMatrix);
        sumMatrix(resultingMatrix, auxMatrix);
    }



    // showing the resulting matrix

    showMatrix(resultingMatrix);
    printf("\n\n");

    for(row = 0; row < cant_nodes; row ++){
        for(column = 0; column < cant_nodes; column ++){
            if(resultingMatrix[row][column] == 0){
                return false;
            } 
        }
    }

    return true;

}

bool isEulerianOpenPath(){
    // Un grafo G diremos que es Un camino abierto euleriano en un grafo es un camino abierto que recorre todas las aristas del grafo solo una vez
    // Propiedad: Un grafo conexo tiene un camino abierto euleriano si y solo si tiene exactamente dos vertices de grado impar

    if(cant_impares == 2){
        return true;
    }

    return false;
}


bool statisfiesEulerTheorem(){

    // Todos los vertices tienen que ser grado par o a lo sumo tiene que haber 2 con grado impar.
    struct Node* aux = firstNode;
    cant_impares = 0;

    while(aux != NULL){
        struct Arista *a = aux->adjacent;
        int grade = 0;

        while(a != NULL){
            grade++;
            a = a->next;
        }

        // Si el resto es != de 0 ==> es impar
        if( grade % 2 != 0){
            cant_impares++;
        }

        aux = aux->next;

    }

    if(isEulerianOpenPath()){
        printf("El grafo admite un camino abierto euleriano.\n");
    }else{
        printf("El grafo NO admite un camino abierto euleriano.\n");
    }

    if(cant_impares == 0){
        return true;
    }

    return false;

}

bool isEulerianCircuit(){
    
    // Un grafo G diremos que es Euleriano si existe un circuito (camino cerrado) que recorre todas las aristas sin repetir ninguna. 
    // A dicho circuito lo llamaremos circuito euleriano.

    // Propiedad: Un grafo conexo es euleriano si y solo si todo vertice tiene grado par
    // - Grafo Conexo
    // - Todos los vertices grado par

    if(isConexo()){

        printf("El grafo es conexo.\n");

        if(statisfiesEulerTheorem()){
            return true;
        }
        
    }else{
        printf("El grafo NO es conexo, por lo tanto no admite ni circuito ni camino abierto euleriano.\n");
    }
    return false;

}


void showAristas(){

    struct Node *aux = firstNode;

    printf("\n\n");

    while(aux != NULL){

        printf("---------------------------------------------------\n ");
        printf("El nodo %c esta conectado con los siguientes nodos:\n ", aux->name);
        struct Arista *arista = aux->adjacent;

        while(arista != NULL){

            //printf(arista);
            char name = arista->node->name;
           
            printf("- %c\n", name); 
    
            

            arista = arista -> next;
        } 

        aux = aux->next;
    }

    printf("\n\n");

}


// >>>>>>>>>>>>>>>>>>> MAIN <<<<<<<<<<<<<<<<<<<<<<<

int main(int argc, char *argv[]){


    int opt= -1;
    while(opt!=0){

        printf("--------------------------------------\n");
        printf("Ingrese la opcion que desee hacer: \n");
        printf("1 - Agregar nodo.\n");
        printf("2 - Agregar arista entre dos nodos.\n");
        printf("3 - Eliminar arista entre dos nodos.\n");
        printf("4 - Eliminar nodo y sus aristas.\n");
        printf("5 - Mostrar Conexiones.\n");
        printf("6 - Verificar si el grafo tiene circuito y/o camino abierto euleriano.\n");
        printf("0 - Salir\n");
        printf("--------------------------------------\n");

        scanf("%d", &opt);

        switch(opt){
            case 0:
                return 0;
                break;
            case 1:
                insertNode();
                break;
            case 2:
                insertArista();
                break;
            case 3:
                deleteArista();
                break;
            case 4:
                deleteNode();
                break;
            case 5:
                showAristas();
                break;
            case 6:
                
                if(cant_nodes > 1){
                  if(isEulerianCircuit()){
                    printf("El grafo admite un circuito euleriano.\n\n");
                  }else{
                    printf("El grafo NO admite un circuito euleriano.\n\n");
                  } 
                }else{
                    printf("No hay nodos suficientes. Cantidad de nodos: %d\n", cant_nodes);
                }
                
                break;
            
        }

    }
}