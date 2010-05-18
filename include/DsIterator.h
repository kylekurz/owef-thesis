/*
 **********************************************
 // File:	Ds_iterator.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	10.28.09
 //
 // Purpose:	Define an abstract iterator class for use
 //		with various data structures and
 //		scoring models in the Open Word
 //		Enumeration Framework.
 **********************************************

 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Lee Nau, Xiaoyu Liang, Rami Alouran


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

#include "DataStructure.h"
#include "OWEFArgs.h"

#ifndef KKURZ_DS_ITERATOR
#define KKURZ_DS_ITERATOR

/**
 * @class ds_iterator
 * @brief Interface for data structure iterator.
 *
 * @author Kyle Kurz
 */
class DataStructureIterator
{
	public:

		//*************************l***********************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * standard constructor
		 */
		DataStructureIterator();

		/**
		 * initialized constructor
		 */
		DataStructureIterator( OWEFArgs *from_input, DataStructure *structure, int wordlength );

		/**
		 * standard destructor
		 */
		virtual ~DataStructureIterator()=0;

		/**
		 * iterator informations
		 */
		virtual bool hasNext()=0;
		virtual std::string next()=0;

		/**
		 * compute how many words are in the iterator.
		 */
		unsigned int getCount()
		{
			unsigned int ret = 0;
			while( hasNext())
			{
				next();
				++ret;
			}
			return ret;
		}

	protected:
		/**
		 * pointer to the list of input parameters
		 */
		OWEFArgs *list;
};

#endif
