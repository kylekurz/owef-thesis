/*
 **********************************************
 // File:	Data_structure.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define an abstract class for use
 //		with various data structures and
 //		scoring models in the Open Word
 //		Enumeration Framework.
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
#include <omp.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>

#include "Scores.h"
#include "OWEFArgs.h"
#include "SequenceFile.h"
#include "Utilities.h"

#ifndef KKURZ_DATA_STRUCTURE
#define KKURZ_DATA_STRUCTURE

/**
 * @class DataStructure
 * @brief Interface for data structure implementations.
 *
 * @author Kyle Kurz
 */
class DataStructure
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		DataStructure();
		
		/**
		 * @brief initialized constructor
		 */
		DataStructure( OWEFArgs *from_input, SequenceFile *in );

		/**
		 * @brief standard destructor
		 */
		virtual ~DataStructure()=0;

		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************

		/**
		 * @brief get word count
		 */
		virtual int getCount( std::string & motif )=0;
		/**
		 * @brief  get sequence count
		 */
		virtual int getSeqs( std::string & motif )=0;
		/**
		 * @brief get the statistics for a word
		 */
		//virtual Scores* getStats( std::string & motif )=0;
		/**
		 * @brief return all words that match a regular expression
		 */
		//virtual void getRegexMatches( std::vector<std::string> &matches, std::string & regex )=0;
		//function to get total count of a regex
		//virtual pair<string, int> get_regex_counts(string motif)=0;
		/**
		 * @brief get the sequences from the input file
		 */
		//virtual void getSeqFile( std::vector<std::string> &sequences )=0;
		
		/**
		 * @brief increment the bit vector to show coverage of a word across the sequences
		 */
		virtual void getLocs(std::vector<int> &bit, std::string &motif )=0;

		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************

		/**
		 * @brief set the statistics of a word in the trie
		 */
		//virtual int setStats( std::string & motif, Scores *new_stats )=0;

		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implemented
		//for each data structure incorporated.
		//************************************************************

		/**
		 * @brief iterate through the words contained in the data structure
		 */
		//virtual void getNextWord( std::string &motif, int length )=0;
		/**
		 * @brief get a block of words to process (eliminate contention between threads
		 */
		//virtual void getNextWordBlock( std::vector<std::string> &block, int length, int count )=0;
		/**
		 * @brief output information
		 */
		//virtual void output()=0;
		/**
		 * @brief reset last_word variables
		 */
		//virtual void reset()=0;
	protected:
		/**
		 * pointer to the list of input parameters
		 */
		OWEFArgs *list;

		/**
		 * Sequence file with data.
		 */
		SequenceFile * input;
};

#endif
