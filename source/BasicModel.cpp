//**********************************************
// File:		BasicModel.cpp
//
// Author:	Kyle Kurz
//
// Date:		4.17.10
//
// Purpose:	Define a class for computing based
//			on Markov models in the WordSeeker 
//			pipeline.
//**********************************************

#include "BasicModel.h"

using namespace std;

BasicModel::BasicModel(){
	list = NULL;
}

BasicModel::BasicModel(OWEFArgs *from_input){
	list = from_input;
}

BasicModel::BasicModel(OWEFArgs *from_input, DataStructure *structure){
	list = from_input;
}

BasicModel::~BasicModel(){
	list = NULL;
}