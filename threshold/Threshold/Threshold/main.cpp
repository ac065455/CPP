#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

class threshold{
private:
public:
    int numRows, numCols, minVal, maxVal, thr_value;
    int** img;
    
    threshold(int numRows, int numCols, int minVal, int maxVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this-> maxVal = maxVal;
        
        img = new int*[numRows];
        for(int i=0; i<numRows; i++){
            img[i] = new int[numCols];
        }//for i
        
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                img[i][j] = 0;
            }//for j
        }//for i
    }//constructor
    
    void loadImage(ifstream& inFile, int thr_value){
        this->thr_value = thr_value;
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                while(inFile >> data){
                    computeThreshold(data, thr_value, i, j);
                    break;
                }//while
            }//for j
        }//for i
        inFile.close();
    }//loadImage
    
    void computeThreshold(int data, int value, int i, int j){
        if(data < value){
            img[i][j] = 0;
        }//if
        else{
            img[i][j] = 1;
        }//else
    }//computeThreshold
    
    void printAry(ofstream& outFile){
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                outFile << img[i][j] << " ";
            }//for j
            outFile << "\n";
        }//for i
    }//printAry
    
};//class

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile;
    
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    
    int numRows, numCols, minVal, maxVal, thr_value;
    
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    
    cout << "enter threshold value: " << "\n";
    cin >> thr_value;
    
    threshold thres(numRows, numCols, minVal, maxVal);
    thres.loadImage(inFile, thr_value);
    thres.printAry(outFile);
    
    outFile.close();
    return 0;
}//main
