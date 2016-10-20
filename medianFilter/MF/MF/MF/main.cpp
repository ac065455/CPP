#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class medianFilter{
private:
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMin;
    int newMax;
    int** mirrorFramedAry;
    int** tempAry;
    int* neighborAry;
    int* histogram;
    int** img;
    int** mirrorFramed;
    
    medianFilter(){
        
    }//constructor
    
    medianFilter(int numRows, int numCols){
        mirrorFramedAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            mirrorFramedAry[i] = new int[numCols]+2;
        }//for i
        
        tempAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            tempAry[i] = new int[numCols]+2;
        }//for i
        
    }//constructor

    void readImage(ifstream& inFile){
        int data;
        inFile >> data;
        numRows = data;
        cout << "numRows: " << numRows << "\n";
        inFile >> data;
        numCols = data;
        cout << "numCols: " << numCols << "\n";
        inFile >> data;
        minVal = data;
        cout << "minVal: " << minVal << "\n";
        inFile >> data;
        maxVal = data;
        cout << "maxVal: " << maxVal << "\n";
        
        histogram = new int[maxVal+1];
        for(int i=0; i<maxVal+1;i++){
            histogram[i]=0;
        }//for i
        
        mirrorFramedAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            mirrorFramedAry[i] = new int[numCols]+2;
        }//for i
        
        tempAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            tempAry[i] = new int[numCols]+2;
        }//for i
        
        img = new int*[numRows];
        for(int i = 0; i < numRows; ++i){
            img[i] = new int[numCols];
        }//for i
        
        int** mirrorFramed;
        mirrorFramed = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            mirrorFramed[i] = new int[numCols+2];
        }//for i
        
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                mirrorFramedAry[i][j] = 0;
                tempAry[i][j] = 0;
            }//for j
        }//for i
    }
    
    void loadImage(ifstream& inFile){
        int data;
        for(int i=0; i<numRows; i++){
            for(int j=0; j<numCols; j++){
                while(!inFile.eof()){
                    inFile >> data;
                    cout << data;
                    //computeHistogram(data);
                    img[i][j]=data;
                    //cout << numRows;
                    //cout << numCols;
                    break;
                }//while
            }//for j
        }//for i
    }//loadImage
    
    void computeHistogram(int data){
        histogram[data]++;
    }//computeHistogram
    
    void computeThreshold(int threshold){
        cout << numRows << " " << numCols << " " << minVal << " " << maxVal;
        for(int i=0; i<numRows; i++){
            cout << "\n" ;
            for(int j=0; j<numCols; j++){
                if(img[i][j] < threshold){
                    img[i][j]=0;
                    cout << "0";
                }//if
                else{
                    img[i][j]=1;
                    cout << "1";
                }//else
            }//for j
        }//for i
        cout << "\n";
    }//computeThreshold
    
//    void loadImage(){
//        for(int i=0; i<numRows+1; i++){
//            for(int j=0; j<numCols+1; j++){
//                mirrorFramedAry[i+1][j+1]=img[i][j];
//            }//for j
//        }//for i
//    }//loadImage
    
    void mirrorFrame(ofstream& outFile){
        int** mirrorFramed;
        mirrorFramed = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            mirrorFramed[i] = new int[numCols+2];
        }//for i

        for(int k=0; k<=numRows+1; k++){
            mirrorFramed[k][0] = mirrorFramedAry[k][1];
            mirrorFramed[k][numCols+1] = mirrorFramedAry[k][numCols+1];
        }//for k
        
        for(int l=0; l<=numCols+1; l++){
            mirrorFramed[0][l] = mirrorFramedAry[1][l];
            mirrorFramed[numRows+1][l] = mirrorFramedAry[numRows][l];
        }//for l
        
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                mirrorFramed[i][j] = mirrorFramedAry[i][j];
            }//for j
        }//for i
    }//mirrorFramed
    
    void getMedian(ofstream& outFile){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                int med = loadNeighbors(i,j);
                mirrorFramedAry[i][j] = med;
            }//for j
        }//for i
    }//getMedian
    
    int loadNeighbors(int i, int j){
        int a,b,c,d,x,f,g,h,k;
        
        a = mirrorFramedAry[i-1][j-1];
        b = mirrorFramedAry[i-1][j];
        c = mirrorFramedAry[i-1][j+1];
        d = mirrorFramedAry[i][j-1];
        x = mirrorFramedAry[i][j];
        f = mirrorFramedAry[i][j+1];
        g = mirrorFramedAry[i+1][j-1];
        h = mirrorFramedAry[i+1][j];
        k = mirrorFramedAry[i+1][j+1];
        
        int neighborAry[9] = {a,b,c,d,x,f,g,h,k};
        selectionSort(neighborAry, 9);
        
        tempAry[i][j] = neighborAry[4];
        
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                tempAry[i][j] = neighborAry[4];
                if (tempAry[i][j] > newMax){
                    newMax = tempAry[i][j];
                }//if
                if (tempAry[i][j] < newMin){
                    newMin = tempAry[i][j];
                }//if
            }//for j
        }//for i
        
        return neighborAry[4];
    }//loadNeighbors

    void selectionSort(int neighborAry[], int size){
        int min, temp;
        for(int i=0; i<size-1; i++){
            min = i;
            for(int j=1+1; j<size; j++){
                if(neighborAry[j] < neighborAry[min]){
                    min = j;
                }//if
            }//for j
            if(min != i){
                temp = neighborAry[i];
                neighborAry[i] = neighborAry[min];
                neighborAry[min] = temp;
            }//if
        }//for i
    }//selectionSort

    
};//class

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile;
    
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    
    int thr_value;
    cout << "enter threshold value: ";
    cin >> thr_value;
    
    medianFilter MF;
    MF.readImage(inFile);
    //MF.computeThreshold(thr_value);
    MF.loadImage(inFile);
    MF.mirrorFrame(outFile);
    
    return 0;
}
