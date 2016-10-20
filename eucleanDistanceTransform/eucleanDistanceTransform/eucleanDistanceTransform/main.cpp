#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <iomanip>
using namespace std;

class eucleanDistanceTransform{
private:
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newMin;
    int newMax;
    double ** zeroFramedAry;
    double neighborAry[5];
    
    eucleanDistanceTransform(int numRows, int numCols, int minVal, int maxVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        
        zeroFramedAry = new double*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            zeroFramedAry[i] = new double[numCols+2];
        }//for i
        
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                zeroFramedAry[i][j] = 0;
            }//for j
        }//for i
        
//        neighborAry = new double[5];
        for(int i=0; i<5; i++){
            neighborAry[i] = 0;
        }//for i
    }//constructor
    
    //load image into 2d array
    void loadImage(ifstream& inFile){
        int data;
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                while(!inFile.eof()){
                    inFile >> data;
                    zeroFramedAry[i][j] = data;
                    break;
                }//while
            }//for j
        }//for i
        inFile.close();
    }//loadImage
    
    void firstPassEuclean(){
        double topLeft, top, topRight, left, newMin = 999, newMax = 1;
        double euclean = sqrt(2);
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                    neighborAry[0] = topLeft = zeroFramedAry[i-1][j-1] + euclean;
                    neighborAry[1] = top = zeroFramedAry[i-1][j] + 1;
                    neighborAry[2] = topRight = zeroFramedAry[i-1][j+1] + euclean;
                    neighborAry[3] = left = zeroFramedAry[i][j-1] + 1;
                    double max = findMaxDistance(neighborAry);
                    double min = findMinDistance(max,neighborAry);
                    zeroFramedAry[i][j] = min;
                }//if
            }//for j
        }//for i
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                    if(zeroFramedAry[i][j] < newMin){
                        newMin = zeroFramedAry[i][j];
                    }//if
                    if(zeroFramedAry[i][j] > newMax){
                        newMax = zeroFramedAry[i][j];
                    }//else if
                }//if
            }//for j
        }//for i
        this->newMax = newMax;
        this->newMin = newMin;
    }//firstPassEuclean
    
    void secondPassEuclean(){
        double right, botLeft, bot, botRight, self, newMin = 999, newMax = 1;
        double euclean = sqrt(2);
        for(int i=numRows; i>0; i--){
            for(int j=numCols ;j>0; j--){
                if(zeroFramedAry[i][j] > 0){
                    neighborAry[0] = right = zeroFramedAry[i][j+1] + 1;
                    neighborAry[1] = botLeft = zeroFramedAry[i+1][j-1] + euclean;
                    neighborAry[2] = bot = zeroFramedAry[i+1][j] + 1;
                    neighborAry[3] = botRight = zeroFramedAry[i+1][j+1] + euclean;
                    neighborAry[4] = self = zeroFramedAry[i][j];
                    double max = findMaxDistance(neighborAry[4],findMaxDistance(neighborAry));
                    double min = findMinDistance(neighborAry[4],findMinDistance(max,neighborAry));
                    zeroFramedAry[i][j] = min;
                }//if
            }//for j
        }//for i
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                    if(zeroFramedAry[i][j] < newMin){
                        newMin = zeroFramedAry[i][j];
                    }//if
                    if(zeroFramedAry[i][j] > newMax){
                        newMax = zeroFramedAry[i][j];
                    }//else if
                }//if
            }//for j
        }//for i
        this->newMax = newMax;
        this->newMin = newMin;
    }//secondPassEuclean
    
    double findMaxDistance(double neighborAry[]){
        double max=0;
        for(int i=0; i<4; i++){
            if(neighborAry[i] > max){
                max = neighborAry[i];
            }//if
        }//for i
        return max;
    }//findMax
    
    double findMaxDistance(double self, double neighbors){
        if(self > neighbors){
            return self;
        }//if
        else{
            return neighbors;
        }//else
    }//findMinDistance
    
    double findMinDistance(double self, double neighbors){
        if(self < neighbors){
            return self;
        }//if
        else{
            return neighbors;
        }//else
    }//findMinDistance
    
    double findMinDistance(double max, double neighborAry[]){
        double min = max;
        for(int i=0; i<4; i++){
            if(neighborAry[i] < min){
                min = neighborAry[i];
            }//if
        }//for i
        return min;
    }//findMin
    
    void prettyPrint(ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << " \n";
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                if(zeroFramedAry[i][j] > 0){
                    outFile << int(zeroFramedAry[i][j] + .5);
                }//if
                else{
                    outFile << " ";
                }//else
            }//for j
            outFile << "\n";
        }//for i
    }//prettyPrint
    
    void prettyPrintDistance(ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << " \n";
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                outFile << setprecision(3) << right << setw(7) << zeroFramedAry[i][j];
            }//for j
            outFile << "\n";
        }//for i
    }//prettyPrint
    
};//class
int main(int argc, const char * argv[]) {
    ifstream inFile;
    inFile.open(argv[1]);
    
    ofstream outFile1;
    ofstream outFile2;
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    
    inFile >> numRows;
    cout << "numRows: " << numRows << "\n";
    inFile >> numCols;
    cout << "numCols: " << numCols << "\n";
    inFile >> minVal;
    cout << "minVal: " << minVal << "\n";
    inFile >> maxVal;
    cout << "maxVal: " << maxVal << "\n";
    
    eucleanDistanceTransform EDT(numRows, numCols, minVal, maxVal);
    EDT.loadImage(inFile);
    outFile2 << "result after pass 1: " << "\n";
    EDT.firstPassEuclean();
    EDT.prettyPrint(outFile2);
    
    outFile2 << "result after pass 2: " << "\n";
    outFile1 << "result after pass 2: " << "\n";
    EDT.secondPassEuclean();
    EDT.prettyPrintDistance(outFile1);
    EDT.prettyPrint(outFile2);
    outFile2.close();
    outFile1.close();

    return 0;
}//main
