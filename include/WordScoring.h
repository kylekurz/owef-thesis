/*
 **********************************************
 // File:	Word_scoring.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define an abstract class for use
 //		with various word scoring models
 //		in the Open Word Enumeration
 //		Framework
 **********************************************

 Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "DataStructure.h"
#include "DsIterator.h"
#include "RtIterator.h"
#include "Scores.h"
#include "OWEFArgs.h"

#ifndef KKURZ_WORD_SCORING
#define KKURZ_WORD_SCORING

/**
 * @class WordScoring
 * @brief an abstract class for use with various word scoring models in the Open Word Enumeration Framework
 *
 * @author Kyle Kurz
 */
class WordScoring
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * standard constructor
		 */
		WordScoring();

		/**
		 * standard destructor
		 */
		virtual ~WordScoring()=0;

		/**
		 * function to compute all Scores of a word
		 */
		virtual void computeScores( Scores *word, std::string &motif, DataStructure *structure, int &order )=0;
		/**
		 * function to compute the E value of a motif
		 */
		//virtual double computeE( std::string &motif, DataStructure *structure, int &order )=0;
		/**
		 * function to compute the Es value of a motif
		 */
		//virtual double computeEs( std::string &motif, DataStructure *structure, int &order )=0;
		/**
		 * function to compute the Oln value of a motif
		 */
		//virtual double computeOln( std::string &motif, DataStructure *structure, int &order )=0;
		/**
		 * function to compute the Sln value of a motif
		 */
		//virtual double computeSln( std::string &motif, DataStructure *structure, int &order )=0;
		/**
		 * function to compute the pval value of a motif
		 */
		//virtual double computePval( std::string &motif, DataStructure *structure )=0;
		/**
		 * function to compute the rank value of a motif
		 */
		//virtual int computeRank( std::string &motif, DataStructure *structure )=0;
		/**
		 * function to compute the count value of a motif
		 */
		//virtual int computeCount( std::string &motif, DataStructure *structure )=0;
		/**
		 * function to compute the seqs value of a motif
		 */
		//virtual int computeSeqs( std::string &motif, DataStructure *structure )=0;
		/**
		 * funtion to provide clustering with seeds
		 */
		std::vector<std::pair<std::string, Scores> > getSeeds();

	protected:
		/**
		 * pointer to the list of input parameters
		 */
		OWEFArgs *list;
		std::vector<std::pair<std::string, Scores> > top_words[ 3 ];
};

#endif
