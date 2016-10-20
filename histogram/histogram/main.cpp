#include <iostream>
#include <fstream>
using namespace std;

class histogram{
private:
public:
    int numRows, numCols, minVal, maxVal;
    int* histogramAry;
    int** img;
    
    histogram(int numRows, int numCols, int minVal, int maxVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        
        //dynamically allocate histogramAry
        histogramAry = new int[maxVal];
        for(int i=0; i<maxVal; i++){
            histogramAry[i] = 0; //initalize to 0
        }//for i
        
        img = new int*[numRows];
        for(int i=0 ;i<numCols; i++){
            img[i] = new int[numCols];
        }//for i
        
        for(int i=0; i<numRows; i++){
            for(int j=0 ;j<numCols; j++){
                img[i][j] = 0;
            }//for j
        }//for i
    }//histogram
    
    void loadImage(ifstream& inFile){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                while(inFile >> data){
                    img[i][j] = data;
                    computeHistogram(data);
                    break;
                }//while
            }//for j
        }//for i
        inFile.close();
    }//loadImage
    
    void computeHistogram(int data){
        histogramAry[data]++;
    }//computeHistogram
    
    void printAry(ofstream& outFile){
        for(int i=0; i<maxVal; i++){
            if(histogramAry[i] >= 80){
                outFile << "(" << i << "): " << "80 +'s";
            }//if
            else{
                outFile << "(" << i << "): " << histogramAry[i];
                for(int count=0; count<histogramAry[i]; count++){
                    outFile << "+";
                }//for
            }//else
            outFile << "\n";
        }//for i
    }//printAry
};//class

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile;
    
    int numRows, numCols, minVal, maxVal;
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    
    //step1 read image header
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    
    histogram hist(numRows, numCols, minVal, maxVal);
    hist.loadImage(inFile);
    hist.printAry(outFile);
    
    outFile.close();
    return 0;
}//main
