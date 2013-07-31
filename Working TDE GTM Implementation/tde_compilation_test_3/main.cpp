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

using namespace std;


//All comments are from Donald Buhl-Brown
//This code is extremely messy as many
//different techniques were attempted as I tried to learn the best
//way to use the TDE.

//Writes out one data set out as K data sets. Going to redo in python
void FileSeparator(int dataSubsections, string fileNameToSeparate, int numberOfDataSets){

	int dataFileIncrementer = 0; //Increments through the array of ifstreams for writing out data.
	int dataPointIncrement = 0; //A second iterator through the ifstream array for writing out the data points.

	int numberOfModelFiles = dataSubsections * numberOfDataSets;

	ifstream filesToSeparate(fileNameToSeparate); //A stream for reading in the data sets
	ifstream *fileToSeparate; //A stream for reading the data to write to the split files

	//cout << "MODEL FILES = " << numberOfModelFiles << endl;

	ofstream *fileStreams = new ofstream[numberOfModelFiles + 1]; //A stream for writing out the split files
	
	//cout << "after fileStreams" << endl;

	ofstream modelListStream("models.dat"); //A file representing the models for classificiation
	ofstream adjustedDataSetsStream("adjustedDataSets.dat");

	string fileToOpen;
	string currentFileName;
	string tmp;

	stringstream ss;

	while(filesToSeparate >> currentFileName){ 

		//cout << "inside while" << endl;

		for(int i = 0; i < dataSubsections; i++){

		//	cout << "inside for" << endl;

			ss << currentFileName + "-" << i << ".dat";
		
		//	cout << "after ss " << endl;

			fileToOpen = ss.str();

			fileStreams[dataFileIncrementer].open(fileToOpen);

			//cout << "after incrementer file open" << endl;

		//	cout << ss.str().substr(0, ss.str().size() - 4) << endl;
		//	system("pause");

			adjustedDataSetsStream << ss.str().substr(0, ss.str().size() - 4) + " ";

			//cout << ss.str() << endl;
			//system("pause");

			ss.str("");

			dataFileIncrementer++;
		}
	}

	ifstream dataSetsNameStream("dataSets.txt");
	string fileName;

	while(dataSetsNameStream >> fileName){


	}

	modelListStream.close();
	adjustedDataSetsStream.close();
}

//Seems to be sufficiently debugged and is creating models.
void createModelFiles( ){

	Settings settings = { 200, 0, 0xff, 3, 1, 2, 1, NULL, NULL, NULL, 0 };
	
	TDEModel *model;

	ifstream dataSetsIn("adjustedDataSets.dat"); //Holds the names of the files

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

vector<NamedModel*>* loadModelsInsideMain(string file_name ){

    NamedModel *model;
    FILE* models_file;
    FILE* model_file;
    char buf[STR_LEN+1];
    int n = STR_LEN;
    vector<NamedModel*> *models = new std::vector<NamedModel*>();

    ////HS_2("Using models from: %s", ms_file);

    // Read the models.ini file.
    cout << file_name << endl;
    //system("pause");

    models_file = fopen(file_name.c_str(), "r"); // models_file.open(ms_file);

    //std::cout << "after file open inside load models" << std::endl;

    while(fgets(buf, STR_LEN + 1, models_file) != NULL) {

	//std::cout << "inside while" << std::endl;
	//system("pause");

    	if (strlen(buf) > 0) {

		//std::cout << "inside the if" << std::endl;

    		check_alloc(model=(NamedModel*)calloc(1,sizeof(NamedModel)));

		//std::cout << "after first alloc" << std::endl;

    		check_alloc(model->name=(char*)calloc(strlen(buf),sizeof(char)));

		//std::cout << "after double check_alloc" << std::endl;

    		strncpy(model->name,buf, strlen(buf));
    		model->name[strlen(buf) -1] = '\0';
    		model_file = fopen(model->name, "r");

    		if (model_file == NULL) {
			
			//std::cout << "model is null inside loadModels" << std::endl;
			cout << "model name is " << model->name << endl;
		//	system("pause");
    			return models;
    		}

		//std::cout << "before models are created inside loadModels" << std::endl;
		//system("pause");

    		model->model = new TDEModel(model_file);

		//std::cout << "after model is created inside loadModels" << std::endl;

    		fclose(model_file);
    		models->push_back(model);

		if(model == NULL){

			cout << "MODEL IS NULL INSIDE LOAD" << endl;
		//	system("pause");
		}

		//std::cout << "after if within while" << std::endl;
    	}
    }

    return models;
}

int main(){

	ANNcoord * dataToClassify; //The array of data to attempt to classify

	vector<NamedModel*> *models; //Vectir that holds a reference to nameModels which contains a TDEModel

	string dataFiles = "dataSets.dat"; 	//represents the filename in which the names of the data sets are stored
	
	string dataFileNameForClassification = "DataSetsForClassification.dat"; //Used only for the confusionMatrix code

	string tmp;

	int dataToClassifyArraySize = 0;
	int dataSetArraySize = 0;
	int dataSubsections = 0; //The number of subsections to break the data into for classification

	ifstream stream("bike-test.dat");
	ifstream dataSetStream("dataSets.dat");

	FILE  *outputs = fopen("data.txt","w");

	//Simply computes the number of lines in the file
	while(stream >> tmp){
		//cout << "inside first while" << endl;
		dataToClassifyArraySize++;
	}



	while(dataSetStream >> tmp){

		dataSetArraySize++;
	}

	dataToClassify = new ANNcoord[dataToClassifyArraySize];

	stream.close();

	ifstream newStream("bike.dat");

	//Reads the data from the files into the dataToClassify array which is just an array of ANNcoord's for use with classification
	
	for(int i = 0; i < dataToClassifyArraySize; i++){

		newStream >> dataToClassify[i];
	}
	
	//Temporary variables for use with the subsection code
	string answer;
	string fileName;
	//Breaks the data into K-subsections for use with the cross validation
	//cout << "Would you like the break the data into subsections for classification (Y/N)" << endl;
	//cin >> answer;

	/*
	if(answer == "Y"){

		cout << "How many subsections? " << endl;
		cin >> dataSubsections;

		FileSeparator(dataSubsections, dataFiles, dataSetArraySize);	
	}
	*/

	//system("pause");
	newStream.close();

	//cout << "before createModels" << endl;
	//system("pause");
	//Eventially make this work with more than one file at a time


	string response;
	cout << "Would you like to run the default classifier (Y/N) or T for Testing " << endl;
	cin >> response;

	if(response == "Y"){

		createModelFiles();

		models = loadModelsInsideMain("models.dat");

		if(models != NULL){

			cout << "Models is not null" << endl;
			//system("pause");

			cout << models->size() << endl;

			cout << "after ->size()" << endl;

			Classifier *classifier = new Classifier(models);

			cout << "after classifier is initialized" << endl;

			classifier->classifyAndSave(&dataToClassify,dataToClassifyArraySize,outputs);

			//system("pause");

		} else {

			cout << "models is null" << endl;
			//system("pause");
		}
	}

	//Answer N to the default classification question to generate the confusion matrix
	//This code is NOT FUNCTIONAL
	else if(response == "N"){
		
		ConfusionMatrix matrixMaker;

		createModelFiles(); //Creates a model from each file listed in adjustedDataSets.dat
		models = loadModelsInsideMain("models.dat");

		//Debugging messages
		cout << "outside loadModels" << endl;
		cout << models->size() << endl;
		//system("pause");

		matrixMaker.models = models;
		matrixMaker.dataToClassifyFileName = dataFileNameForClassification;

		cout << "before GenerateConfusionMatrix" << endl;

		matrixMaker.GenerateConfusionMatrix( );

		cout << "outside matrixMaker" << endl;


	//Outputs the results of the classifications to the classificationResults folder
	} else if (response == "T"){ //trying the classify trajectory code

		createModelFiles( ); //Creates a model from each file listed in adjustedDataSets.dat
		
		vector<NamedModel*> *models = new vector<NamedModel*>;

		loadModels("models.dat", models, 2, 16 ); //Loads the models into the models vector

		//This works correctly

		//The file representing which data sets to classify
		ifstream dataSetsStream( "DataSetsForClassification.dat" );

		string fileName;

		while( dataSetsStream >> fileName ){
			classifyTrajectory( ( "dataSets\\" + fileName + ".dat" ).c_str( ), ( "classificationResults\\" + fileName + "-classification.dat" ).c_str( ) );
		}

	}
}


