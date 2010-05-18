//**********************************************
// File:		BasicModel.h
//
// Author:	Kyle Kurz
//
// Date:		4.17.10
//
// Purpose:	Define a class for computing based
//			on Markov models in the WordSeeker 
//			pipeline.
//**********************************************

#include "WordScoring.h"

#ifndef KKURZ_BASIC_MODEL
#define KKURZ_BASIC_MODEL

/**
 * @class BasicModel
 * @brief class for computing based on basic models in the WordSeeker pipeline.
 *
 * @author Kyle Kurz
 */
class BasicModel : public WordScoring
{
	public:
		//Constructors
		BasicModel();
		/**
		 *
		 */
		BasicModel( OWEFArgs *from_input );
	
		/**
		 * @brief initialized basic model
		 */
		BasicModel( OWEFArgs *from_input, DataStructure *structure );
	
		/**
		 * @brief standard destructor
		 */
		~BasicModel();
	
		//*******************************************************************
		//Computational functions
		//If implementation is incomplete or not provided, an exception is
		//thrown with a (-1)
		//*******************************************************************
	
		/**
		 * @brief compute all Scores for a motif
		 */
		void computeScores( Scores *word, std::string &motif, DataStructure *structure, int &order ){return;}
	
	private:
		OWEFArgs *list;
	
};

#endif
