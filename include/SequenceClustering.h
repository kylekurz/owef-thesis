/*
 **********************************************
 // File:	Sequence_clustering.h
 //
 // Author:	Kyle Kurz
 //
 // Code contributions: 	Xiaoyu Liang, Josh Welch,
 //			Frank Drews, Jens Lichtenberg
 //
 // Date:	4.30.09
 //
 // Purpose:	Define a class for use
 //		in sequence clustering analysis
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
#include <iomanip>
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

#ifndef KKURZ_SEQUENCE_CLUSTERING
#define KKURZ_SEQUENCE_CLUSTERING

using namespace std;

/**
 * @class SequenceClustering
 * @brief a class for use in sequence clustering analysis in the Open Word Enumeration Framework.
 *
 * @author Kyle Kurz
 */
class SequenceClustering
{
	public:
		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		SequenceClustering();

		/**
		 * @brief initialized constructor
		 */
		SequenceClustering( OWEFArgs *from_input, DataStructure *structure, WordScoring *model );

		/**
		 * @brief standard destructor
		 */
		~SequenceClustering();
	private:
		OWEFArgs *list;
};

#endif
