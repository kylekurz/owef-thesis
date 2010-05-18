/*
 **********************************************
 // File:	Visualization.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	4.6.10
 //
 // Purpose:	Define an abstract class for use
 //		with various word scoring models
 //		in the Open Word Enumeration
 //		Framework
 **********************************************

 Copyright (C) 2010  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

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
#include "WordScoring.h"

#ifndef KKURZ_VISUALIZATION
#define KKURZ_VISUALIZATION

/**
 * @class WordScoring
 * @brief an abstract class for use with various word scoring models in the Open Word Enumeration Framework
 *
 * @author Kyle Kurz
 */
class Visualization
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * standard constructor
		 */
		Visualization();

		/**
		 * initialized constructor
		 */
		Visualization(OWEFArgs *from_input, DataStructure *structure, WordScoring *model);

		/**
		 * standard destructor
		 */
		~Visualization();

		/**
		 * Create CGR visualizations
		 */
		void createCGR(DataStructure *structure, WordScoring *model);
		
	private:
		/**
		 * pointer to the list of input parameters
		 */
		OWEFArgs *list;
};

#endif
