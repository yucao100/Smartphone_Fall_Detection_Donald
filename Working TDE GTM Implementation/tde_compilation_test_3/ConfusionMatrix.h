#ifndef CONFUSION_MATRIX_H
#define CONFUSION_MATRIX_H

#include <iostream>
#include <fstream>
#include "BuildTree.h"
#include "TDEModel.h"
#include "ClassifyTrajectory.h"
#include "Classifier.h"
#include "Utils.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;


struct ArrayPosition{

	int i;
	int j;
};

//A class which represents the result of a classification
class ClassificationResult{

public:
	ClassificationResult( );
	ClassificationResult( double classificationScore, string dataSetAnnotation ); 

	string dataSetAnnotation; //An annotation for use with building the confusion matrix
						 //The annotation will be extracted from the dataSets name i.e R,S,B
	double classificationScore; //Represents the total score for this result
};

class ConfusionMatrix{

public:

ConfusionMatrix( );
ConfusionMatrix( string adjustedDataSetsFileName, vector<NamedModel*> *models );

float ComputeTotalClassificationScore( CvMat *data ); //Performs a summation of the data inside the matrix

ClassificationResult * classifyDataSet( vector< CvMat* > ); //Returns a classification result which indicates what data set had the highest score
								    //The highest score indicates that this is what the data set was classified as.

void GetClassificationMatrices( ANNcoord * ); //Returns the classification matrices associated with that data set
											   //This class will utilize the Classifier classes classify() method.

void GenerateConfusionMatrix( ); //The method which will interface the rest of the class specifically creating the confusion matrix
void PrintConfusionMatrix( ); //Performs the algorithm to write out the confusion matrix in basic format

void setupDataSetNames( );
void CountNumberOfDataPoints( string fileName );
ArrayPosition ReturnArrayPosition( string trueClassName, string classifiedClassName );


vector< CvMat* > classificationMatrices;
vector< string > dataSetNames; //Holds the data set file names
vector< NamedModel* > *models;

string dataToClassifyFileName;
string dataCurrentlyBeingClassifiedAnnotation;

int currentIndexBeingClassified; //Used to keep the class from computing the classification score for the current data
int numberOfModels; //Calculated from the number of lines in the dataSetsArray
int numberOfDataPoints; //Calculated when dataCurrentlyBeingClassified is updated

ANNcoord *dataCurrentlyBeingClassified; //An array to hold the data we are trying to classify

/*

The columns indicate which class the data truly belonged to i.e 0 = Running data, 1 = Standing data, 2 = Biking data.
The rows indicate how many times the data type from the columns was classified as either running, standing, or biking.

An example result
  R S B 
R 5 1 0
S 3 0 7
B 0 9 0

For example, the 5 under [R,R] or [0,0] indicates that there were 5 successful classifications of running data.

*/

int confusionMatrix[ 3 ] [ 3 ]; 

};


#endif