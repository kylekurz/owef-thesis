/*
 **********************************************
 // File:	Word_distribution.h
 //
 // Author:	Kyle Kurz
 //
 // Code contributions: 	Xiaoyu Liang, Josh Welch,
 //			Frank Drews, Jens Lichtenberg
 //
 // Date:	4.29.09
 //
 // Purpose:	Define a class for use
 //		in word distribution analysis
 //		in the Open Word Enumeration
 //		Framework.
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
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>

#include "DataStructure.h"
#include "OWEFArgs.h"
#include "Scores.h"
#include "WordScoring.h"

#ifndef KKURZ_WORD_DISTRIBUTION
#define KKURZ_WORD_DISTRIBUTION

/**
 * @class WordDistribution
 * @brief  a class for use in word distribution analysis in the Open Word Enumeration Framework.
 *
 * @author Kyle Kurz
 */
class WordDistribution
{
	public:
		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * standard constructor
		 */
		WordDistribution();

		/**
		 * initialized constructor
		 */
		WordDistribution( OWEFArgs *from_input, DataStructure *structure, WordScoring *model, SequenceFile *input );

		/**
		 * standard destructor
		 */
		~WordDistribution();

	private:

		/**
		 * compute the word distributions
		 */
		void computeDistributions( DataStructure *structure, WordScoring *model, SequenceFile *input );

		/**
		 * store the locations of the words within the sequences
		 */
		void countSequence( const Sequence *seq, std::string & word, std::vector<int> & distribution );

		/**
		 * store the locations of the words within the sequences with a percentage normalization
		 */
		void countSequenceNormal( const Sequence *seq, std::string & word, std::vector<int> & distribution );

		/**
		 * output the distribution information
		 */
		void outputWordDistribution( std::ofstream &fout, std::vector<int> distribution );

		/**
		 * convert words to uppercase
		 */
		std::string toUppercase( std::string check );
		
		/**
		 * do a match check on two words, given their start character pointers
		 */
		bool match(char *x, char *y, int length);

		/**
		 * list of job parameters
		 */
		OWEFArgs *list;
};

#endif
