#include <iostream>
#include <fstream>
using namespace std;

class DijkstraSSS{
    
private:
public:
    int numNodes; //number of nodes in G
    int sourceNode;
    int minNode;
    int currentNode;
    int newCost;
    int** costMatrix; //a 2-D cost matrix (integer array), size of N X N, should be dynamically allocated.
    // Initially, costMatrix[i][i] set to zero and all others set to infinity, 99999
    int* fatherAry; //a  1-D integer array, size of N, should be dynamically allocated.
    // initially set to itself, i.e., father[i] = i
    int* markedAry; //a 1-D integer array, size of N, should be dynamically allocated.
    // initially set to 0 (not marked)
    int* bestCostAry; //a  1-D integer array, size of N, should be dynamically allocated.
    // initially set to 9999 (infinity)
    
    DijkstraSSS(int n){
        numNodes = n;
        costMatrix = new int*[n+1];
        for(int i = 0; i < n+1 ; i++){
            costMatrix[i] = new int[n+1];
        }//for i
        
        fatherAry = new int [n+1];
        markedAry = new int [n+1];
        bestCostAry = new int [n+1];
        
        //initialize diagonal to 0
        for(int i = 0; i < n+1; i++){
            for(int j = 0; j < n+1; j++){
                if(i == j){
                    costMatrix[i][j] = 0;
                }//if
                else {
                    costMatrix[i][j] = 9999;
                }//else
            }//for j
        }//for i
        
        //initialize all 3 arrays
        for(int i = 0; i < n+1; i++){
            fatherAry[i] = i;
            markedAry[i] = 0;
            bestCostAry[i] = 9999;
        }//for i
        
        sourceNode = 1;
        bestCostAry[sourceNode] = 0;
    }//DijkstraSSS constructor
    
    void loadCostMatrix(int Ni, int Nj, int cost){
        costMatrix[Ni][Nj] = cost;
    }// read from input file and fill the costMatrix,
    
    void loadBestCostAry(int s){
        int n = numNodes;
        for(int i = 0; i < n; i++){
            bestCostAry[i] = s;
        }//for i
    }// copy the row of source node from costMatrix,
    
    void loadFatherAry(int s){
        int n = numNodes;
        for(int i = 0; i < n; i++){
            fatherAry[i] = s;
        }//for i
    }//set all to source
    
    //    void loadMarkedAry(){
    //        int n = numNodes;
    //        for(int i = 0; i < n; i++){
    //            markedAry[i] = 0;
    //        }//for i
    //    }// set all to 0,
    
    int computeCost(int minNode, int currentNode){
        return bestCostAry[minNode] + costMatrix[minNode][currentNode];
    }//it compute the currentNode’s cost ( bestCostArray[minNode] +  cost from minNode to currentNode, in costMatrix)
    
    void markMinNode(int minNode){
        markedAry[minNode] = 1;
    }// mark the minNode  in markedAry
    
    void changeFather(int node, int minNode){
        fatherAry[currentNode] = minNode;
    }// set node’s father be minNode in fatherAry
    
    void changeCost(int node, int newCost){
        bestCostAry[node] = newCost;
    }// set node’s best cost to newCost in bestCostAry
    
    void Dijkstra(ofstream& outFile1, ofstream& outFile2){
        int n = numNodes;
        int count = 1;
        while(count < n+1){
            findMinNode(outFile2);
            for(int i = 1; i < n+1; i++){
                if(markedAry[i] == 0){
                    expandMinNode(i,outFile2);
                }//if
            }//for i
            count++;
        }//while
        debugPrint(outFile2);
        outFile1 << "the graph has " << numNodes << " nodes" << "\n";
        outFile1 << "the source node is " << sourceNode << "\n";
        outFile1 << "\n";
        int s = 1;
        while(s <= numNodes){
            outFile1 << "the path from " << sourceNode << " to: " << s <<";";
            printShortestPath(s,outFile1);
            outFile1 << " cost = " << bestCostAry[s];
            outFile1 << "\n";
            s++;
        }//while
    }//Dijkstra
    
    void printShortestPath(int s, ofstream& outFile1){
        if(fatherAry[s] == sourceNode){
            outFile1 << " " << sourceNode << " is greater than " << s << ";";
            return;
        }//if
        else{
            printShortestPath(fatherAry[s],outFile1);
            outFile1 << " less than " << s << ";";
        }//else
    }//printShortestPath
    
    void expandMinNode(int unmarkedNode, ofstream& outFile2){
        currentNode = unmarkedNode;
        newCost = computeCost(minNode, currentNode);
        if(newCost < bestCostAry[currentNode]){
            changeFather(currentNode, minNode);
            changeCost(currentNode, newCost);
            debugPrint(outFile2);
        }//if
    }//expandMinNode
    
    void debugPrint(ofstream& outFile2){
        int n = numNodes;
        outFile2 << "Debug Print " << "\n";
        outFile2 << "\n";
        outFile2 << "sourceNode " << sourceNode << "\n";
        outFile2 << "\n";
        outFile2 << "fatherAry " << "\n";
        for(int i = 1; i < n+1; i++){
            outFile2 << fatherAry[i] << " ";
        }//for i
        outFile2 << "\n";
        outFile2 << "\n";
        outFile2 << "bestCostAry " << "\n";
        for(int i = 1; i < n+1; i++){
            outFile2 << bestCostAry[i] << " ";
        }//for i
        outFile2 << "\n";
        outFile2 << "\n";
        outFile2 << "markedAry " << "\n";
        for(int i = 1; i < n+1; i++){
            outFile2 << markedAry[i] << " ";
        }//for i
        outFile2 << "\n";
        outFile2 << "\n";
    }//debugPrint
    
    void findMinNode(ofstream& outFile1){
        int min = 0;
        int n = numNodes;
        for(int i = 0; i < n+1; i++){
            if(bestCostAry[i] < bestCostAry[min]){
                if(markedAry[i] == 0){
                    min = i;
                }//if
            }//if
        }//for
        minNode = min;
        markMinNode(minNode);
        outFile1 << "minNode " << minNode << "\n";
    }//findMinNode
    
    void nextSourceNode(int nextSourceNode){
        int n = numNodes;
        fatherAry = new int[n+1];
        markedAry = new int[n+1];
        bestCostAry = new int[n+1];
        for (int i = 0; i < n+1; i++) {
            fatherAry[i] = i;
            markedAry[i] = 0;
            bestCostAry[i] = 9999;
        }//for i
        sourceNode = nextSourceNode;
        bestCostAry[sourceNode] = 0;
    }
};

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile1;
    ofstream outFile2;
    
    inFile.open(argv[1]);
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    
    int data;
    int numNodes;
    int sourceNode;
    int Ni = 0;
    int Nj = 0;
    int cost = 0;
    int count = 1;
    
    inFile >> data;
    numNodes = data;
    DijkstraSSS Dijkstra(numNodes);
    
    while(inFile >> data){
        if(count == 1){
            Ni = data;
            count++;
        }//if
        else if(count == 2){
            Nj = data;
            count++;
        }//else if
        else if(count == 3){
            cost = data;
            sourceNode = Ni;
            Dijkstra.loadCostMatrix(Ni,Nj,cost);
            count = 1;
        }//else if
    }//while
    inFile.close();
    Dijkstra.debugPrint(outFile2);
    Dijkstra.Dijkstra(outFile1,outFile2);
    outFile1 << "\n";
    
    for (int i = 2; i < numNodes+1; i++) {
        sourceNode = 2;
        Dijkstra.nextSourceNode(i);
        Dijkstra.Dijkstra(outFile1,outFile2);
        outFile1 << "\n";
    }//for i
    
    outFile1.close();
    outFile2.close();
    return 0;
}//main
