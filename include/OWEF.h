/*
**********************************************
// File:	OWEF.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a class for use
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
#include <limits.h>
#include <math.h>

#ifdef MPI
#include <mpi.h>
#endif

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "BasicMethod.h"
#include "BasicModel.h"
#include "ClusterMethod.h"
#include "DataStructure.h"
#include "DsIterator.h"
#include "OWEFArgs.h"
#include "RadixTrie.h"
#include "RtIterator.h"
#include "Scores.h"
#include "Sequence.h"
#include "SequenceFile.h"
#include "SequenceFilters.h"
#include "SequenceClustering.h"
#include "WordDistribution.h"
#include "WordScoring.h"

#ifndef KKURZ_OWEF
#define KKURZ_OWEF

/**
 * @class OWEF
 * @brief a class for use with various data structures and scoring models in the Open Word Enumeration Framework.
 *
 * @author Kyle Kurz
 */
class OWEF
{
	public:
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		/**
		 * @brief standard constructor
		 */
		OWEF();
		
		/**
		 * @brief initialized constructor
		 */
		OWEF(OWEFArgs *from_input);
		
		/**
		 * @brief standard destructor
		 */
		~OWEF();
		
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.  If the data structure
		//does not store the information, throw an error with (-1)
		//************************************************************
		
		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************
		
		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implmented
		//for each data structure incorporated.
		//************************************************************
		/**
		 * @brief iterate through the words contained in the data structure
		 */
		//string getNextWord(int length);
		
		/**
		 * @brief output all of the information currently stored about the input
		 */
		//void output();
		
		/**
		 * @brief process the threads of execution
		 */
		void *process(void *foo);
		
	private:
		Cluster *method;
		DataStructure *structure;
		OWEFArgs *list;
		SequenceClustering *tool;
		WordDistribution *type;
		WordScoring *model;
};

#endif
