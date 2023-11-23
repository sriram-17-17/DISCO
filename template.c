/**
 * Group ID - 
 * Member 1 Name - Sriram Sudheer Hebbale
 * Member 1 BITS ID - 2022A7PS0147P
 * Member 2 Name - Siddhartha Gotur
 * Member 2 BITS ID - 2022A7PS0070P
 * Member 3 Name - Granth Jain
 * Member 3 BITS ID - 2022A7PS0172P
*/

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Graph {
    int n; // Size of the graph
    int** adj; // Adjacency matrix
    char** station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase 
 * and creates the Graph structure from it.
*/
Graph* create_graph(char input_file_path[]) {
    FILE* f = fopen(input_file_path, "r");
    Graph* g = (Graph*)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char**)malloc(g->n * sizeof(char*));
    for(int i = 0; i < g->n; i++) {
        g->station_names[i] = (char*)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for(int i = 0; i < edges; i++) {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1 
 * Return the number of junctions.
*/
int find_junctions(Graph* g) {
    int junctions = 0;
    int station_count = 0;
    for(int i = 0; i<g->n; i++){
        station_count = 0;
        for(int j = 0; j<g->n; j++){
            if ((i != j) && (g -> adj[i][j])) {
                station_count++;
            }
        }
        if(station_count >= 4){
            junctions+=1;
        }
    }

    return junctions;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this 
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved. 
 * If false, then question 2(b) must be solved.
*/
bool sheldons_tour(Graph* g, bool SAME_STATION) {
    int** connections = g -> adj; 
    int station_count = 0;
    if (SAME_STATION){
        for(int i = 0; i < g-> n; i++){
            for (int j = 0; j < g-> n; j++){
                if(connections[i][j]){
                    station_count++;
                }
            }
            if (station_count % 2 != 0){
                return false;
            }

            station_count = 0;
        }

        return true;
    }
    else{
        int odd_count= 0;
        for(int i = 0; i < g -> n; i++){
            for(int j = 0; j < g-> n ; j++){
                if (connections[i][j]){
                    station_count++;
                }
            }

            if (station_count % 2 != 0){
                odd_count++;
            }

            station_count = 0;
        }

        if (odd_count == 2){
            return true;
        }
        else {
            return false;
        }
    }
}
/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required. 
*/
int** warshall(Graph* g) {
    // Do not modify or delete pre-filled code!
    int** closure = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        closure[i] = calloc(g->n, sizeof(int));
    }

    // Code goes here

    //Populating closure

    for(int i = 0; i < g -> n; i++){
        for (int j = 0; j < g-> n; j++){
            closure[i][j] = g -> adj[i][j];
        }
    }
    
    // Appplying warshall's algorithm on closure to obtain the transitive closure of the adjacency matrix
    for(int k = 0; k < g-> n; k++){
        for(int j = 0; j < g-> n; j++){
            for(int i = 0; i < g->n; i++){
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);

            }
        }
    }
    
    return closure; // Do not modify
}

/**
 * Q.3
 * Return the number of impossible pairs.
*/
int find_impossible_pairs(Graph* g) {
    int** closure = warshall(g); // Do not modify

    int impossible_pairs = 0;

    for (int i = 0; i < g->n; i++){
        for(int j = 0; j < g->n; j++){
            if(closure[i][j] == 0 && (i > j)){
                impossible_pairs++;
            }
        } 
    }

    return impossible_pairs;
}

/**
 * Q.4
 * Return the number of vital train tracks.
*/
int find_vital_train_tracks(Graph* g) {
    int** closure = warshall(g); // Do not modify
    int** removedEdge;
    int pairs = 0;

    for(int i = 0; i<g->n; i++){
        for(int j = 0; j<g->n; j++){
            if(g->adj[i][j]==1 && i>j){
                g->adj[i][j]=0;
                g->adj[j][i]=0;
                removedEdge = warshall(g);

                int same = 1;
                for(int k=0; ((k < g -> n) && same);k++){    
                    for(int z = 0; (z < (g -> n) && same);z++){
                        if(removedEdge[k][z] != closure[k][z]){
                            pairs++;
                            same=0;
                        }
                    }
                }
                g->adj[i][j]=1;
                g->adj[j][i]=1;

            }
            
        }
    }
    return pairs;
}
/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
*/
int* upgrade_railway_stations(Graph* g) {
    int* upgrades = calloc(g->n, sizeof(int)); // Do not modify
    int colored[g->n][g->n];

    //intializing colored to an invalid color
    for(int i = 0; i<g->n; i++){
        upgrades[i] = -1;
    }

    // Code goes here
    upgrades[0] = 0;

    int valid = 1;

    for(int i = 1; i < g -> n; i++){
        for(int j = 0; j < g -> n; j++){
            //checking validity
            if(g -> adj[i][j]){
                if(upgrades[i] == -1)
                    upgrades[i] = !upgrades[j];

                else if(upgrades[i] == upgrades[j])
                    valid = 0;
            }
        }
    }
    
    if(!valid){
        for(int i = 0; i<g->n; i++){
        upgrades[i] = -1;
        }
    }

    return upgrades; // Do not modify
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
*/
int distance(Graph* g, int city_x, int city_y) {

    int** distance = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        distance[i] = calloc(g->n, sizeof(int));
    }

    //distance= W_k and prev= W_(k-1)
    int** prev = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g-> n; i++) {
        prev[i] = calloc(g->n, sizeof(int));
    }

    //initializing adjacency values in the distance
    for(int i = 0; i<g->n; i++){
        for(int j = 0; j<g->n; j++){
            if(g -> adj[i][j]){
                distance[i][j] = g-> adj[i][j];
                prev[i][j] = g-> adj[i][j];
            }
            else{
                distance[i][j] = -1;
                prev[i][j] = -1;               
            }

        }
    }

    //warshall's algorithm
    for(int k = 0; k<g->n;k++){
        for(int i = 0; i<g->n; i++){
            for(int j = 0; j<g->n; j++){
                int dist1=prev[i][j];
                int dist2;

                if (prev[i][k] == -1 || prev[k][j] == -1){
                    distance[i][j] = dist1;
                }
                else if (dist1 == -1){
                    dist2 = prev[i][k] + prev[k][j];
                    distance[i][j] = dist2;
                }
                else{
                    dist2 = prev[i][k] + prev[k][j];
                    if (dist1 <= dist2) {
                        distance[i][j] = dist1;
                    }
                    else {
                        distance[i][j] = dist2;
                    }
                }
            }
        }

        for(int i = 0; i<g->n; i++){
            for(int j = 0; j<g->n; j++){
                prev[i][j]=distance[i][j];
            }
        }


    }

    return distance[city_x][city_y];
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
*/
int railway_capital(Graph* g) {

    int** distance = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g->n; i++) {
        distance[i] = calloc(g->n, sizeof(int));
    }

    //distance= W_k and prev= W_(k-1)
    int** prev = (int**)malloc(g->n * sizeof(int*));
    for(int i = 0; i < g-> n; i++) {
        prev[i] = calloc(g->n, sizeof(int));
    }

    //initializing adjacency values in the distance
    for(int i = 0; i<g->n; i++){
        for(int j = 0; j<g->n; j++){
            if(g -> adj[i][j]){
                distance[i][j] = g-> adj[i][j];
                prev[i][j] = g-> adj[i][j];
            }
            else{
                distance[i][j] = -1;
                prev[i][j] = -1;               
            }

        }
    }

    //warshall's algorithm
    for(int k = 0; k<g->n;k++){
        for(int i = 0; i<g->n; i++){
            for(int j = 0; j<g->n; j++){
                int dist1=prev[i][j];
                int dist2;

                if (prev[i][k] == -1 || prev[k][j] == -1){
                    distance[i][j] = dist1;
                }
                else if (dist1 == -1){
                    dist2 = prev[i][k] + prev[k][j];
                    distance[i][j] = dist2;
                }
                else{
                    dist2 = prev[i][k] + prev[k][j];
                    if (dist1 <= dist2) {
                        distance[i][j] = dist1;
                    }
                    else {
                        distance[i][j] = dist2;
                    }
                }
            }
        }

        for(int i = 0; i<g->n; i++){
            for(int j = 0; j<g->n; j++){
                prev[i][j]=distance[i][j];
            }
        }


    }

    int sumOfDistances[g->n];
    for(int k = 0; k < g-> n; k++){
        sumOfDistances[k] = 0;
        for(int h = 0; h < g -> n; h++){
            if (distance[k][h] != -1){
                sumOfDistances[k] += distance[k][h];
            }
        }
    } 

    // Finding the minimum distance from the sumOfDistance array

    int capital = 0;

    for(int i = 0; i < g-> n; i++){
        if(sumOfDistances[capital] > sumOfDistances[i]){
            capital = i;
        }
    }

    for(int i = 0; i < g-> n; i++){
        printf("%d\t", sumOfDistances[i]);
    }

    return capital;
}
    


/**
 * Helper function for Q.8
*/
bool maharaja_express_tour(Graph* g, int source, int current_city, int previous_city, int* visited) {

    visited[current_city] = 1;

    if (g -> adj[current_city][source] && previous_city != source){
        return true;
    }
    else {
        for(int j = 0; j < g-> n; j++){
            if (g -> adj[current_city][j] && j!= previous_city && !visited[j]){
               if(maharaja_express_tour(g, source, j, current_city, visited)){
                    return true;
               }
               else {
                    visited[j] = 0;
               }
            }   
        }
        return false;
    }
     
}   

/**
 * Q.8 
 * Return true if it is possible to have a Maharaja Express tour 
 * starting from source city, else return false. 
*/
bool maharaja_express(Graph* g, int source) {
    int* visited = (int*)malloc(g->n * sizeof(int)); 
    for(int i = 0; i < g->n; i++) {
        visited[i] = 0;
    }

    // Hint: Call the helper function and pass the visited array created here.

    return maharaja_express_tour(g, source, source, source , visited);
}

int main() {
    char input_file_path[100] = "testcase_1.txt"; // Can be modified
    Graph* g = create_graph(input_file_path); // Do not modify
    return 0;
}
