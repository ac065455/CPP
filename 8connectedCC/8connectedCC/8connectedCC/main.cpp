#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <iomanip>
using namespace std;

class connectedCC{
private:
public:
    int numRows, numCols, minVal, maxVal, newMin, newMax, newLabel = 0, NLU = 0;
    int** zeroFramedAry;
    int* EQAry;
    int minRow, minCol, maxRow, maxCol, count;
    int neighborAry[4];
    
    connectedCC(int numRows, int numCols, int minVal, int maxVal){
        this->numRows = numRows;
        this->numCols = numCols;
        this->minVal = minVal;
        this->maxVal = maxVal;
        
        zeroFramedAry = new int*[numRows+2];
        for(int i=0; i<numRows+2; i++){
            zeroFramedAry[i] = new int[numCols];
        }//for j
        
        //initialize zeroFramedAry to 0
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                zeroFramedAry[i][j] = 0;
            }//for j
        }//for i
        
        //initialize neighborAry
        for(int i=0; i<4; i++){
            neighborAry[i] = 0;
        }//for i
        
        //initialize EQAry to 0
        int size = (numRows*numCols)/4;
        EQAry = new int[size];
        for(int i=0; i<size; i++){
            EQAry[i] = i;
        }//for i
    }//constructor
    
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
    
    void connectedCC_Pass1(){
        int topLeft, top, topRight, left, newMin = 999, newMax = 0, min, max;
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                    neighborAry[0] = topLeft = zeroFramedAry[i-1][j-1];
                    neighborAry[1] = top = zeroFramedAry[i-1][j];
                    neighborAry[2] = topRight = zeroFramedAry[i-1][j+1];
                    neighborAry[3] = left = zeroFramedAry[i][j-1];
                    //case 1
                    if(topLeft == 0 && top == 0 && topRight == 0 && left == 0){
                        newLabel++;
                        zeroFramedAry[i][j] = newLabel;
                    }//if
                    //case2
                    else{
                        max = findMax(neighborAry, 1, 0);
                        min = findMin(max, neighborAry, 1, max);
                        if(min==max){
                            zeroFramedAry[i][j] = min;
                        }//if
                        //case 3
                        else{
                            zeroFramedAry[i][j] = min;
                            updateEQAry(neighborAry, min);
                        }//else
                    }//else
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
        //propertyFile(outFile2);
    }//connectedCC_Pass1

    void connectedCC_Pass2(){
        int botRight, bot, botLeft, right, min, max, newMax = 0, newMin = 999;
        for(int i=numRows; i>0; i--){
            for(int j=numCols; j>0; j--){
                if(zeroFramedAry[i][j] > 0){
                    neighborAry[0] = botRight = zeroFramedAry[i+1][j+1];
                    neighborAry[1] = bot = zeroFramedAry[i+1][j];
                    neighborAry[2] = botLeft = zeroFramedAry[i+1][j-1];
                    neighborAry[3] = right = zeroFramedAry[i][j+1];
                    max = findMax(neighborAry, 2, zeroFramedAry[i][j]);
                    min = findMin(max, neighborAry, 2, zeroFramedAry[i][j]);
                    if(min!=max){
                        zeroFramedAry[i][j] = min;
                        updateEQAry(neighborAry, min);
                    }//if
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
    }//connectedCC_Pass2
    
    void connectedCC_pass3(ofstream& outFile2){
        int newMax = 0, newMin = 999;
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                if(zeroFramedAry[i][j] > 0){
                    zeroFramedAry[i][j] = EQAry[zeroFramedAry[i][j]];
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
        propertyFile(outFile2);
    }//connectedCC_pass3
    
    void updateEQAry(int neighborAry[], int min){
        for(int i=0; i<4; i++){
            if(neighborAry[i] != 0){
                EQAry[neighborAry[i]] = min;
            }//if
        }//for i
    }//updateEQAry
    
    void manageEQAry(){
        NLU = 1;
        for(int i=1; i<=newLabel; i++){
            if(EQAry[i]==i){
                EQAry[i] = NLU++;
            }//if
            else{
                EQAry[i] = EQAry[EQAry[i]];
            }//else
        }//for i
    }//updateEQAry
    
    int findMax(int neighborAry[], int passNum, int currentValue){
        int max=0;
        for(int i=0 ; i<4; i++){
            if(neighborAry[i] > max){
                max = neighborAry[i];
            }//if
        }//for i
        
        if(passNum==2){
            if(currentValue > max){
                return currentValue;
            }//if
            else{
                return max;
            }//else
        }//if
        else{
            return max;
        }//else
    }//findMax
    
    int findMin(int max, int neighborAry[], int passNum, int currentValue){
        int min = max;
        for(int i=0; i<4; i++){
            if(neighborAry[i] < min && neighborAry[i] != 0){
                min = neighborAry[i];
            }//if
        }//for i
        
        if(passNum==1){
            return min;
        }//if
        else if(currentValue < min){
            return currentValue;
        }//else if
        else{
            return min;
        }//else
    }//findMin
    
    void print(ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << " \n";
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                    outFile << zeroFramedAry[i][j] << " ";
            }//for j
            outFile << "\n";
        }//for i
    }//prettyPrint
    
    void prettyPrint(ofstream& outFile){
        outFile << numRows << " " << numCols << " " << newMin << " " << newMax << " \n";
        for(int i=0; i<numRows+2; i++){
            for(int j=0; j<numCols+2; j++){
                if(zeroFramedAry[i][j] > 0){
                    outFile << zeroFramedAry[i][j];
                }//if
                else{
                    outFile << " ";
                }//else
            }//for j
            outFile << "\n";
        }//for i
    }//prettyPrint
    
    void printEQAry(ofstream& out){
        out << "\n";
        out << "EQAry: ";
        for(int i=0; i<=newLabel; i++){
            out << EQAry[i] << " ";
        }//for i
        out << "\n";
        out << "\n";
    }//printEQAry
    
    void propertyFile(ofstream& out){
        findProperties(out);
    }//propertyFile
    
    void findProperties(ofstream& out){
        int minCol = 999;
        int minRow = 999;
        for(int label=1; label<=newLabel; label++){
            for(int i=1; i<numRows+1; i++){
                for(int j=1; j<numCols+1; j++){
                    if(zeroFramedAry[i][j] == label){
                        count++;
                        if(j<minCol){
                            minCol = j;
                        }//if
                        if(i<minRow){
                            minRow = i;
                        }//if
                        if(i>maxRow){
                            maxRow = i;
                        }//if
                        if(j>maxCol){
                            maxCol = j;
                        }//if
                    }//if
                }//for j
            }//for i
            if(count == 0){
                return;
            }//if
            else{
                out << label << " " << count << " " << minRow-1 << " " << minCol-1 << " " << maxRow-1 << " " <<maxCol-1;
                out << "\n";
                minCol = 999;
                minRow = 999;
                maxRow = 0;
                maxCol = 0;
                count = 0;
            }//else
        }//for label
    }//minCol
    
};//class
int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile1;
    ofstream outFile2;
    
    int numRows, numCols, minVal, maxVal;
    
    inFile.open(argv[1]);
    
    inFile >> numRows;
    inFile >> numCols;
    inFile >> minVal;
    inFile >> maxVal;
    
    outFile1.open(argv[2]);
    outFile2.open(argv[3]);
    
    connectedCC cc(numRows,numCols,minVal,maxVal);
    cc.loadImage(inFile);
    cc.connectedCC_Pass1();
    
    cc.connectedCC_Pass2();
    cc.manageEQAry();
    
    cc.connectedCC_pass3(outFile2);
    cc.print(outFile1);
    
    outFile1.close();
    outFile2.close();
    
    return 0;
}//main
