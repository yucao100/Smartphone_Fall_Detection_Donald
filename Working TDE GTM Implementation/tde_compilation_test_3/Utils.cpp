/*
 * Copyright (c) 2010 Jordan Frank, HumanSense Project, McGill University
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 * See LICENSE for more information
 *
 * Utils.cpp
 */

#include <stdlib.h>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include "ANNx.h"
#include <cxcore.h>
#include "Utils.h"
using namespace std;
void get_embedding(Settings* settings, ANNcoord* &data, int &length) {
	//cout << "inside get_embedding!" << endl;
	long i;
	int j, k;
	int alldim, maxemb, emb, rundel, delsum;
	int *inddelay;
	int *formatlist;
	double** series;
	//cout << "before check_alloc inside get_embedding" << endl;
	check_alloc(formatlist = (int*) malloc(sizeof(int) * settings->indim));
	//cout << "after check_alloc inside get_embedding" << endl;
	for (i = 0; i < settings->indim; i++) {
		formatlist[i] = settings->embdim / settings->indim;
	}
	alldim = 0;
	for (i = 0; i < settings->indim; i++)
		alldim += formatlist[i];
	check_alloc(inddelay = (int*) malloc(sizeof(int) * alldim));

	rundel = 0;
	for (i = 0; i < settings->indim; i++) {
		delsum = 0;
		inddelay[rundel++] = delsum;
		for (j = 1; j < formatlist[i]; j++) {
			delsum += settings->delay;
			inddelay[rundel++] = delsum;
		}
	}
	maxemb = 0;
	for (i = 0; i < alldim; i++)
		maxemb = (maxemb < inddelay[i]) ? inddelay[i] : maxemb;

	if (settings->column == NULL) {
		series = get_multi_series(settings->infile, &settings->length,
				settings->exclude, &settings->indim, (char*) "",
				1, settings->verbosity);
	} else {
		series = get_multi_series(settings->infile, &settings->length,
				settings->exclude, &settings->indim, settings->column,
				1, settings->verbosity);
	}
	//_android_log_print(ANDROID_LOG_DEBUG, "HUMANSENSE",

	check_alloc(data = (ANNcoord*) calloc((settings->length - maxemb)
			* settings->embdim, sizeof(ANNcoord)));
	uint step = settings->embdim;
	for (i = maxemb; i < settings->length; i++) {
		rundel = 0;
		for (j = 0; j < settings->indim; j++) {
			emb = formatlist[j];
			for (k = 0; k < emb; k++)
				data[(i - maxemb) * step + (emb - k - 1)]
						= (ANNcoord) series[j][i - inddelay[rundel++]];
		}
	}
	length = settings->length - maxemb;

	for (j = 0; j < settings->indim; j++) {
		free(series[j]);
	}

	//cout << "before free() " << endl;

	free(series);
	free(formatlist);
	free(inddelay);

}

void get_ann_points(ANNpointArray &dataPts, ANNcoord* series,
		long rows, int cols) {
	long k = 0;
	dataPts = annAllocPts(rows, cols);
	for (long i = 0; i < rows; i++) {
		for (long j = 0; j < cols; j++) {
			dataPts[i][j] = series[k++];
		}
	}
}

void convert_to_ann_points(ANNpointArray &dataPts, ANNcoord* series, int rows,
		int cols) {
	int k = 0, i, j;

	//cout << "inside convert_to_ann" << endl;
	// Assume Allocated
	for (i = 0; i < rows; i++) {

		//cout << "inside first for, convert_to_ann" << endl;

		for (j = 0; j < cols; j++) {

			//cout << "inside for, convert_to_ann" << endl;

			if(dataPts == NULL){

				cout << "dataPts is NULL" << endl;
				system("pause");
			}

			if(series == NULL){

				cout << "series is NULL" << endl;
			}

			//cout << "i = " << i << " k = " << k << " j = " << j << endl;
			dataPts[i][j] = series[k++];

			//cout << "after dataPts use convert_to_ann" << endl;
		}
	}

	//cout << "outside convert_to_ann for loops" << endl;
}

void print_matrix(CvMat* matrix, FILE *f) {
	int i, j;
	for (i = 0; i < matrix->rows; i++) {
		fprintf(f, FLOAT_OUT, CV_MAT_ELEM(*matrix, ANNcoord, i, 0));
		for (j = 1; j < matrix->cols; j++) {
			fprintf(f, " " FLOAT_OUT, CV_MAT_ELEM(*matrix, ANNcoord, i, j));
		}
		fprintf(f,"\n");
	}
}

void check_alloc(void *pointer) {
	if (pointer == NULL) {
		//_android_log_print(ANDROID_LOG_DEBUG, "HUMANSENSE",
		//		"check_alloc: Couldn't allocate enough memory. Exiting\n");
		cout << "not enough memory inside check_alloc" << endl;
		system("pause");
		exit( CHECK_ALLOC_NOT_ENOUGH_MEMORY);
	}
}

const int SIZE_STEP = 1000;

double **get_multi_series(char *name, long *l, long ex,
		int *col, char *which, char colfix, int verbosity) {
	//cout << "getting_multi_series" << endl;
	char *input, **format;
	int i, j;
	int *hcol, maxcol = 0, colcount = 0;
	long count, max_size = SIZE_STEP, hl, allcount;
	int input_size = INPUT_SIZE;
	double **x;
	FILE *fin;

//	cout << "after multi_series initialization" << endl;

	if (strlen(which) > 0) {
		colcount = 1;
		for (i = 0; i < strlen(which) - 1; i++) {
			if (!isdigit((int) which[i]) && (which[i] != ',')) {
				//_android_log_print(ANDROID_LOG_ERROR, "HUMANSENSE",
						//"Wrong format in the column string. Has to be num,num,num,...,num");
          		cout << "wrong format stuff dog" << endl;
				return NULL;
			}
			if (which[i] == ',') {
				colcount++;
				which[i] = ' ';
			}
		}
		if (!isdigit((int) which[strlen(which) - 1])) {
			//_android_log_print(ANDROID_LOG_ERROR, "HUMANSENSE",
					//"Wrong format in the column string. Has to be num,num,num,...,num");
			return NULL;
		}
	}
	if (!colfix && (*col < colcount)) {
		*col = colcount;
	}


	//cout << "before double check_alloc" << endl;
	check_alloc(input = (char*) calloc((size_t) input_size, (size_t) 1));
	check_alloc(hcol = (int*) malloc(sizeof(int) * *col));
	//cout << "after double check_alloc" << endl;
  
	while ((int) (*which) && isspace((int) (*which))) {
		which++;
	}
  
  //cout << "after while loop stuff inside Utils" << endl;
  
	if (*which) {
		for (i = 0; i < *col - 1; i++) {
			sscanf(which, "%u", &hcol[i]);
			if (hcol[i] > maxcol) {
				maxcol = hcol[i];
			}
			while ((int) (*which) && !isspace((int) (*which))) {
				which++;
			}
			while ((int) (*which) && isspace((int) (*which))) {
				which++;
			}
			if (!((int) (*which))) {
				break;
			}
		}
	}
  

  
	else {
		i = -1;
	//	cout << "inside else, after while, inside Utils" << endl;
	}

	if (*which) {
		//cout << "inside if after the else inside Utils" << endl;
		sscanf(which, "%u", &hcol[i]);
	}
	else {
   // cout << "inside else after the if for *which inside utils" << endl;
		for (j = i + 1; j < *col; j++) {
			hcol[j] = ++maxcol;
		}
	}
	check_alloc(format = (char**) malloc(sizeof(char*) * *col));
  
  //cout << "after check_alloc inside Utils after the else" << endl;
  
	for (i = 0; i < *col; i++) {
  
		//cout << "inside for loop before check_alloc" << endl;
    
		check_alloc(format[i] = (char*) calloc((size_t)(4 * hcol[i]), (size_t) 1));
		strcpy(format[i], "");
		for (j = 1; j < hcol[i]; j++) {
			strcat(format[i], "%*lf");
		}
		strcat(format[i], "%lf");
		
		//cout << "end of for loop" << endl;
	}

	//cout << "outside for loop befor free" << endl;
//
	free(hcol);

	//cout << "after free after for loop" << endl;

	check_alloc(x = (double**) malloc(sizeof(double*) * *col));

	//cout << "after check_alloc after free" << endl;

	for (i = 0; i < *col; i++) {
		check_alloc(x[i] = (double*) malloc(sizeof(double) * max_size));
	}
	hl = *l;

	count = 0;
	allcount = 0;

	//cout << "before if name==NULL" << endl;

	if (name == NULL) {

	//	cout << "inside if name==NULL" << endl;

		for (i = 0; i < ex; i++) {
			if ((input = getline(input, &input_size, stdin, verbosity)) == NULL) {
				break;
			}
		}
		while ((count < hl) && ((input = getline(input, &input_size, stdin,
				verbosity)) != NULL)) {
			if (count == max_size) {
				max_size += SIZE_STEP;
				for (i = 0; i < *col; i++) {
					check_alloc(x[i] = (double*) realloc(x[i], sizeof(double)
							* max_size));
				}
			}
			allcount++;
			for (i = 0; i < *col; i++) {
				if (sscanf(input, format[i], &x[i][count]) != 1) {
					break;
				}
			}
			if (i == *col) {
				count++;
			}
		}
	} else {

		//cout << "inside else inside Utils" << endl;

		fin = fopen(name, "r");

		//cout << "fileName = " << name << endl;

    if(fin == NULL){
    
     // cout << "file in is null" << endl;
    }
    
		for (i = 0; i < ex; i++) {
			if ((input = getline(input, &input_size, fin, verbosity)) == NULL) {
				break;
			}
		}

		//cout << "after for loop inside else" << endl;

		while ((count < hl) && ((input = getline(input, &input_size, fin,
				verbosity)) != NULL)) {

			//cout << "inside while after the for loop inside Utils" << endl;
			
			if (count == max_size) {
				max_size += SIZE_STEP;
				for (i = 0; i < *col; i++)
					check_alloc(x[i] = (double*) realloc(x[i], sizeof(double)
							* max_size));
			//	cout << "after check_alloc for loop" << endl;
			}
			allcount++;
			for (i = 0; i < *col; i++) {
				if (sscanf(input, format[i], &x[i][count]) != 1) {
					break;
				}
			}
			if (i == *col) {
				count++;
			}
		}

		//cout << "before fclose inside utils" << endl;

		fclose(fin);
	}

	for (i = 0; i < *col; i++) {
		//cout << "inside this for loop" << endl;
		free(format[i]);
	}
	free(format);
	free(input);

	*l = count;
	if (*l == 0) {
		//_android_log_print(ANDROID_LOG_ERROR, "HUMANSENSE", "0 lines read.");
		return NULL;
	}

	//cout << "before last check_alloc inside Utils" << endl;

	if (max_size > count) {
		for (i = 0; i < *col; i++) {
			check_alloc(x[i] = (double*) realloc(x[i], sizeof(double) * count));
		}

		//cout << "after lastcheck_alloc inside if utils" << endl;
	}

	//cout << "done getting multi-series in utils" << endl;
	//system("pause");
  
	return x;
}
char* getline(char *str, int *size, FILE *fin, int verbosity) {
	char *ret;
	char *hstr = NULL;
	char last;

	ret = fgets(str, *size, fin);
	if (ret == NULL)
		return NULL;

	last = str[strlen(str) - 1];

	while (last != '\n') {
		// If the line is too long, increase the length and read more data.
		*size += INPUT_SIZE;
		check_alloc(hstr = (char*) calloc((size_t) INPUT_SIZE, (size_t) 1));
		check_alloc(str = (char*) realloc(str, (size_t) * size));
		ret = fgets(hstr, INPUT_SIZE, fin);
		strcat(str, hstr);
		last = str[strlen(str) - 1];
		free(hstr);
	}
	return str;
}

#undef SIZE_STEP
