#include "ConfusionMatrix.h"

/*Beginning of ClassificationResultr class implementation*/
#define CV_MAT_ELEM( mat, elemtype, row, col )           \
    (*(elemtype*)CV_MAT_ELEM_PTR_FAST( mat, row, col, sizeof(elemtype)))


ClassificationResult::ClassificationResult( ){

}

ClassificationResult::ClassificationResult( double classificationScore, string dataSetAnnotation ){

	this->classificationScore = classificationScore;
	this->dataSetAnnotation = dataSetAnnotation;
}

/*Beginning of ConfusionMatrix class implementation*/

ConfusionMatrix::ConfusionMatrix( ){

	this->numberOfDataPoints = 0;

	for( int i = 0; i < 3; i++ ){

		for( int j = 0; j < 3; j++ ){

			confusionMatrix[ i ][ j ] = 0;
		}
	}
}

ConfusionMatrix::ConfusionMatrix( string dataToClassifyFileName, vector<NamedModel*> *models ){

	this->dataToClassifyFileName = dataToClassifyFileName;
	this->models = models;

	cout << "modelsSize = " << this->models->size() << endl;

	this->numberOfDataPoints = 0;

	system("pause");
}

float ConfusionMatrix::ComputeTotalClassificationScore( CvMat *data ){

	float totalScore = 0;

	cout << data->rows << " " << data->cols << endl;
	system("pause");

	for( int i = 0; i < data->rows; i++ ){
		
		//cout << "inside for" << endl;

		for(int j = 0; j < data->cols; j++ ){

			//cout << i << " " << j << endl;
			//cout << "rows/cols = " << data->rows << " " << data->cols << endl;

			//cout << "ELEMENT = " <<  CV_MAT_ELEM( *data, ANNcoord, i, j ) << endl;

			totalScore += CV_MAT_ELEM( *data, ANNcoord, i, j );
		}
	} 

	/*
	for (int i = 0; i < data->rows; i++) {
		
		totalScore += CV_MAT_ELEM(*data, ANNcoord, i, 0));
		for (int j = 1; j < data->cols; j++) {
			fprintf(f, " " FLOAT_OUT, CV_MAT_ELEM(*matrix, ANNcoord, i, j));
		}
		fprintf(f,"\n");
	}
	*/

	//Confirm the totalScores
	cout << "totalScore =  " << totalScore << endl;
	cout << "fileRead = " << dataCurrentlyBeingClassifiedAnnotation << endl;
//	system("pause");
	return ( ( 1.0 / (numberOfDataPoints - 2 ) ) * totalScore );
}

ClassificationResult * ConfusionMatrix::classifyDataSet( vector< CvMat* > classificationResults ){

	//ClassificationResult *currentTopResult = new ClassificationResult( ComputeTotalClassificationScore( classificationResults[ 0 ] ), dataSetNames[ 0 ] );

	ClassificationResult *currentTopResult = new ClassificationResult( 0, "" );

	cout << "classResults = " << classificationResults.size( );
	//system( "pause" );

	for( int i = 0; i < classificationResults.size( ); i++ ){

		FILE *f = new FILE;
		f = fopen( ( dataSetNames[ i ].substr( 0, dataSetNames[ i ].size( ) - 4 ) + "-classification.dat'" ).c_str( ),"w" );

		print_matrix( classificationResults[ i ], f ); 

		cout << "inside for before classifyDataSets" << endl;

		if( ComputeTotalClassificationScore( classificationResults[ i ] ) > currentTopResult->classificationScore ){

			currentTopResult->classificationScore = ComputeTotalClassificationScore( classificationResults[ i ] );
			
			//cout << "before dataSets notation" << endl;
			//cout << classificationResults.size( ) << endl;
			//cout << dataSetNames.size( ) << endl;
			//cout << "i = " << i << endl;
			currentTopResult->dataSetAnnotation = dataCurrentlyBeingClassifiedAnnotation;
			cout << currentTopResult->dataSetAnnotation << endl;
			cout << dataSetNames[ i ] << endl;
			//system("pause");
			//cout << "after DatasetsName" << endl;
		}
	}

	classificationResults.clear( );

	return currentTopResult;
}

void ConfusionMatrix::GetClassificationMatrices( ANNcoord *dataToClassify ){

	cout << "inside get matrices" << endl;

	Classifier *classifier = new Classifier( this->models );
	cout << this->models->size() << endl;
	//system("pause");
	cout << "after pause" << endl;
	for( int i = 0; i < models->size( ); i++ ){

		cout << "before push_back before matrices" << endl;
		classificationMatrices.push_back( classifier->classify( &dataToClassify, numberOfDataPoints, ( *models )[ i ]->model ) ); 
		cout << "after push_back before matrices" << endl;
	}
}

void ConfusionMatrix::GenerateConfusionMatrix( ){

	setupDataSetNames( );

	cout << "inside GenerateConfusionMatrix" << endl;
	cout << dataSetNames.size() << endl;
	//system( "pause" );

	for( int i = 0; i < dataSetNames.size( ); i++ ){

		cout << "inside for loop" << endl;

		ifstream dataStream( dataSetNames[ i ] );

		dataCurrentlyBeingClassifiedAnnotation = dataSetNames[ i ].substr( 0, 4 );

		cout << "after extracting data annotation" << endl;

		CountNumberOfDataPoints( dataSetNames[ i ] );

		cout << "after countNumberOfPoints" << endl;
		cout << "num of Points " << numberOfDataPoints << endl;

		dataCurrentlyBeingClassified = new ANNcoord[ this->numberOfDataPoints ];

		cout << "before for loop after ANNcoord allocation" << endl;

		for( int j = 0; j < this->numberOfDataPoints; j++ ){

			//cout << "reading data points" << endl;

			dataStream >> dataCurrentlyBeingClassified[ j ];
		}

		cout << "after data in for loop" << endl;

		GetClassificationMatrices( dataCurrentlyBeingClassified );

		ClassificationResult classificationResult = *classifyDataSet( classificationMatrices );

		classificationMatrices.clear( );

		//cout << classificationMatrices.size( ) << endl;
		//system("pause")

		//cout << "dataSetsAnno =" << dataSetNames[ i ].substr( 0, 4 ) << endl;
		//cout << "dataAnnotatio = " << classificationResult.dataSetAnnotation << endl;

		ArrayPosition position = ReturnArrayPosition( dataCurrentlyBeingClassifiedAnnotation, classificationResult.dataSetAnnotation );
		
		//cout << "setting confusionMatrix point" << endl;
		//cout << position.i << " " << position.j << endl;
		confusionMatrix[ position.i ][ position.j ] ++;

	}

	PrintConfusionMatrix( );
}

void ConfusionMatrix::PrintConfusionMatrix( ){

	cout << " R B S" << endl;

	char c;

	for( int i = 0; i < 3; i++ ){

		if( i == 0 ){

			c = 'R';

		} else if ( i == 1 ) {

			c = 'B';

		} else {

			c = 'S';
		}

		cout << c << " ";

		for( int j = 0; j < 3; j++ ){

			cout << confusionMatrix[ i ][ j ] << " ";
		}

		cout << endl;
	}

	system( "pause" );

	for( int i = 0; i < 3; i++ ){

		for( int j = 0; j < 3; j++ ){

			cout << confusionMatrix[ i ][ j ] << " ";
		}
	}

	system( "pause" );
}

ArrayPosition ConfusionMatrix::ReturnArrayPosition( string trueClassName, string classifiedClassName ){

	ArrayPosition arrayPosition;

	cout << "trueClassName = " << trueClassName << endl;
	cout << "classifiedClassName = " << classifiedClassName << endl;
	system("pause");

	if( trueClassName == "runn" ){

		arrayPosition.i = 0;

	} else if( trueClassName == "bike" ){

		arrayPosition.i = 1;

	} else {

		arrayPosition.i = 2;
	}

	if( classifiedClassName == "runn") {

		arrayPosition.j = 0;

	} else if( classifiedClassName == "bike" ){

		arrayPosition.j = 1;
	} else {

		arrayPosition.j = 2;
	}

	cout << "returning position" << endl;

	return arrayPosition;
}

void ConfusionMatrix::CountNumberOfDataPoints( string fileName ){

	cout << "fileName = " << fileName << endl;

	ifstream stream( "dataSets//" + fileName );

	string tmp;
	
	while( stream >> tmp ){

		this->numberOfDataPoints++;
	}
}

void ConfusionMatrix::setupDataSetNames( ){

	ifstream dataSetFiles( dataToClassifyFileName );

	string tmpFileName;

	while( dataSetFiles >> tmpFileName ){

		cout << tmpFileName << endl;

		dataSetNames.push_back( tmpFileName );

	}
}