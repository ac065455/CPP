#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class graphNode{
    friend class KruskalMST;
private:
public:
    int nodeID;
    graphNode* next;
    
    graphNode(int node){
        nodeID = node;
        next = NULL;
    }//graphNode constructor
    //A constructor to create a new graphNode of a given nodeID and next should be null.
    
    graphNode(int node, graphNode* n){
        nodeID = node;
        next = n;
    }//graphNode constructor
    
    ~graphNode(){
        delete next;
    }//A distractor to free a graphNode
    
    void Insert(int node){
        graphNode* current = this;
        while(current->next != NULL){
            current = current->next;
        }//while
        current->next = new graphNode(node);
    }//Insert function
    
    void Delete(){
        graphNode* current = this;
        while(current->next != NULL){
            if(current->next->next == NULL){
                current->next = NULL;
                continue;
            }//if
            else{
                current->next = current->next->next;
            }//else
            current = current->next;
        }//while
    }//Delete function
};

class undirectedEdge{
    friend class KruskalMST;
private:
public:
    int Ni;
    int Nj;
    int edgeCost;
    undirectedEdge* next;
    
    undirectedEdge(int Ni, int Nj, int edgeCost){
        this->Ni = Ni;
        this->Nj = Nj;
        this->edgeCost = edgeCost;
        next = NULL;
    }
    
    undirectedEdge(int Ni, int Nj, int edgeCost, undirectedEdge* next){
        this->Ni = Ni;
        this->Nj = Nj;
        this->edgeCost = edgeCost;
        this->next = next;
    }//undirectedEdge function
    
    void Insert(undirectedEdge* node){
        undirectedEdge* current = this;
        while(current->next != NULL){
            current = current->next;
        }//while
        current->next = node;
    }//Insert function
    
    void Delete(){
        undirectedEdge* current = this;
        while(current->next != NULL){
            if(current->next->next == NULL){
                current->next = NULL;
                continue;
            }//if
            else{
                current->next = current->next->next;
            }//else
            current = current->next;
        }//while
    }//Delete function
    
    void printEdge(ofstream& outFile1){
        outFile1 << Ni << " " << Nj << " " << edgeCost << "\n";
    }//printEdge function
};

class KruskalMST{
private:
public:
    int numNodes;//number of nodes in G
    int* inWhichSet;//  an int arrary of size = numNodes +1
    //to indicate which set each node belongs; dynamically allocated, and set to itself, initially.
    int numSets;// to indicate the remaining number of sets in the computation,  initially set to numNodes.
    int totalCost;// initially set to zero
    int min;
    int max;
    int index;
    undirectedEdge* edgeListHead;// the head of the  linked list of all edges, should point to a dummy node
    undirectedEdge* MSTofG;// the head of the linked list of MST, should point to a dummy node)
    
    KruskalMST(int n){
        numNodes = n;
        int* inWhichSet = new int [n+1];
        numSets = n;
        totalCost = 0;
        undirectedEdge* edgeListHead = new undirectedEdge(-9999,-9999,-9999);
        undirectedEdge* MSTofG = new undirectedEdge(-9999,-9999,-9999);
    }//constructor
    
    void printList(undirectedEdge* edgeListHead, ofstream& outFile2){
        undirectedEdge* spot = edgeListHead;
        int count = 0;
        outFile2 << "First 10 edges: " << "\n";
        while(spot != NULL){
            if(count <= 10){
                outFile2 << spot->Ni << spot->Nj << spot->edgeCost << "\n";
                spot = spot->next;
                count++;
            }//if
            else{
                break;
            }//else
            outFile2 << "\n";
        }//while
    }// print the given linked list, L,  from dummy node to the end
    
    void mergeTwoSets(int Ni, int Nj){
        if (inWhichSet[Ni] <= inWhichSet[Nj]){
            min = inWhichSet[Ni];
            max = inWhichSet[Nj];
        }//if
        else{
            min = inWhichSet[Nj];
            max = inWhichSet[Ni];
        }//else
        
        while(index <= numNodes){
            index = min;
            if(inWhichSet[index] == max){
                inWhichSet[index] = min;
                index++;
            }//if
        }//while
    }// merge the set of Ni and the set of Nj  // See algorithm below.
    
    bool isEmpty(undirectedEdge* head){
        if(head->next == NULL){
            return true;
        }//if
        return false;
    }//isEmpty function
    
    undirectedEdge* findSpot (undirectedEdge* listHead, int cost){
        undirectedEdge* spot = listHead;
        if (isEmpty(spot)){
            return spot;
        }//if
        while (spot->next!= NULL && spot->next->edgeCost <= cost){
            spot = spot->next;
        }//while
        return spot;
    }//findSpot
    
    void insertEdge(undirectedEdge* edge, undirectedEdge* listHead){
        undirectedEdge *spot = findSpot(listHead, edge->edgeCost);
        edge->next = spot->next;
        spot->next = edge;
    }//insertEdge
    
    void edgeListCreate(ifstream& inFile){
        int data;
        int Ni;
        int Nj;
        int cost;
        while (inFile >> data){
            inFile >> Ni >> Nj >> cost;
            undirectedEdge* edge = new undirectedEdge(Ni, Nj, cost);
            insertEdge(edge, edgeListHead);
        }//while
    }//edgeListCreate
    
    void printArray(ofstream& outfile2, int edgeArray[], int numNodes){
        for(int i=1; i < numNodes; i ++){
            outfile2 << i << " contains: " << edgeArray[i] << "\n";
        }//for i
    }//printArray
    
    void Kruskals(int n,ifstream& inFile,ofstream& outFile1, ofstream& outFile2){
        edgeListHead = new undirectedEdge(-9999, -9999, -9999);
        MSTofG = new undirectedEdge(-9999, -9999, -9999);
        numNodes = n;
        numSets = n;
        outFile1 << "*Minimum Spanning Tree" << "\n";
        outFile1 << "number Nodes in Graph = " << numNodes << "\n";
        numNodes++;
        //initialize the inWhichSet array with values of itself
        inWhichSet = new int [n+1];
        for (int i = 1; i < n+1; i++){
            inWhichSet[i] = i;
        }//for i
        edgeListCreate(inFile);
        printList(edgeListHead, outFile2);
        undirectedEdge* min;
        while(n>1){
//            while(inWhichSet[edgeListHead->Ni] == inWhichSet[edgeListHead->Nj]){
            n--;
//            totalCost += edgeListHead->edgeCost;
//            mergeTwoSets(edgeListHead->Ni, edgeListHead->Nj);
            printArray(outFile2, inWhichSet, numNodes);
        }//while
        printList(MSTofG, outFile1);
        outFile1 << "total cost = " << totalCost << "\n";
    }// the Kruskal’s algorithm
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
    int Ni;
    int Nj;
    int cost;
    
    inFile >> data;
    numNodes = data;
    KruskalMST Kruskal(numNodes);
    
    while(inFile >> data){
        inFile >> Ni;
        inFile >> Nj;
        inFile >> cost;
    }//while
   Kruskal.Kruskals(numNodes,inFile,outFile1,outFile2);
    
    return 0;
}//main
