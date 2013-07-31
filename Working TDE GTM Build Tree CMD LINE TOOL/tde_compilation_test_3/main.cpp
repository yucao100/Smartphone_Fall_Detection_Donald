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
#include <stdlib.h>
#include "ConfusionMatrix.h"

using namespace std;

int main( int argc, char *argv[ ] ){

	Settings settings = { 50, 0, 0xff, 3, 1, 2, 1, NULL, NULL, NULL, 0 };
	
	TDEModel *model;

	//cout << "Before cmd line arguments" << endl;

	settings.embdim = atoi( argv[ 1 ] );	
	settings.pcaembdim = atoi( argv[ 2 ] );
	settings.delay = atoi( argv[ 3 ] );
	settings.infile = argv[ 4 ];

	//cout << "after command line";
	//cout << settings.infile << endl;

	model = new TDEModel( &settings );

	string outFileName = string( settings.infile ).substr( 0, ( string( settings.infile ) ).size( ) - 4 ) + ".dat.dat";

	//cout << outFileName << endl;

	model->DumpTree( const_cast<char *>( outFileName.c_str( ) ) );
}


