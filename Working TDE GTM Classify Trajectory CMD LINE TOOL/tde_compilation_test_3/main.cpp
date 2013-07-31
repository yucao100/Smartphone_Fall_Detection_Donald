#define STR_LEN 100
#include <iostream>
#include <fstream>
#include "BuildTree.h"
#include "TDEModel.h"
#include "Classifier.h"
#include "ClassifyTrajectory.cpp"
#include "Utils.h"
#include <vector>
#include <string>
#include <sstream>
#include "ConfusionMatrix.h"
#include <stdlib.h>

using namespace std;

bool isIndeterminate(const double pV)
{
    return (pV != pV);
} 

void print_matrix( CvMat* matrix ) {

	//cout << "inside print_matrix" << endl;

	int i,j;
	
	double tmp;

	for ( i = 0; i < matrix->rows; i++ ) {

		if ( isIndeterminate( CV_MAT_ELEM( *matrix, ANNcoord, i, 0 ) ) ){

			cout << " " << 0;
		}

		else{
		
				cout << CV_MAT_ELEM( *matrix, ANNcoord, i, 0 );
		}

		for ( j = 1; j < matrix->cols; j++ ) {
		
			if ( isIndeterminate( CV_MAT_ELEM( *matrix, ANNcoord, i, j ) ) ){

				cout << " " << 0;
			}

			else{

				cout << " " << CV_MAT_ELEM( *matrix, ANNcoord, i, j );
			}
		}

	cout << endl;
	
	}
}

//Seems to be sufficiently debugged and is creating models.
void createModelFiles(string modelFile){

	Settings settings = { 75, 0, 0xff, 3, 1, 2, 1, NULL, NULL, NULL, 0 };
	
	TDEModel *model;

	ifstream dataSetsIn("adjustedDataSets.dat");

	string fileName = "";

	char  newFileName[200];
	char newOutFileName[200];

	while(dataSetsIn >> fileName){
		
		Settings *settingsPtr = new Settings;
		*settingsPtr = settings;

		//cout << &settings << endl;
		//system("pause");

		//strcpy(newFileName, "\dataSets\\");

		//cout << "newFIleName = " << newFileName << endl;

		strcpy(newFileName, fileName.c_str());
		
		//cout << "then after strcat newFN = " << newFileName << endl;
		strcat(newFileName, ".dat");

		//cout << "newFileName = " << newFileName << endl;
		settingsPtr->infile = newFileName;

		strcpy(newOutFileName, fileName.c_str());
		strcat(newOutFileName, "-model.dat");

		//cout << "newFileName = " << newOutFileName << endl;

		//cout << "outFile = " << newOutFileName << endl;
		settingsPtr->outfile = newOutFileName;

		model = new TDEModel(settingsPtr);

		model->DumpTree(settingsPtr->outfile);

		//system("pause");
		//cout << "end of main while loop #####" << endl;
	}

	dataSetsIn.close();
}

int main( int argc, char *argv[ ] ){

	//classifyTrajectory( argv[ 4 ], 

	//Settings settings = { 200, 0, 0xff, 3, 1, 2, 1, NULL, NULL, NULL, 0 };
	
	//TDEModel *model;


	//settings.embdim = atoi( argv[ 1 ] );	
	//settings.pcaembdim = atoi( argv[ 2 ] );
	//settings.delay = atoi( argv[ 3 ] );
	//settings.infile = argv[ 4 ];

	//createModelFiles( "models.dat" );

	//cout << "after models" << endl;

	vector<NamedModel*> *models = new vector<NamedModel*>;

	loadModels("models.ini", models, 2, 16 );

	//cout << "before print_matrix" << endl;

	//cout << "before print_matrix" << endl;
	//cout << argv[ 4 ] << endl;
	print_matrix( classifyTrajectory( argv[ 4 ] ));
}


