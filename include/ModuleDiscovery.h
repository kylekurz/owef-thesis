/*
 **********************************************
 // File:	Module_discovery.h
 //
 // Author:	Kyle Kurz and Xiaoyu Liang
 //
 // Date:	7.20.09
 //
 // Purpose:	Define an abstract class for use
 //		with various module discovery models
 //		in the Open Word Enumeration
 //		Framework
 **********************************************

 Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang

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
#include "Scores.h"
#include "OWEFArgs.h"
#include "Scores.h"
#include "Sequence.h"
#include "SequenceFile.h"
#include "SequenceFilters.h"
#include "WordScoring.h"

#ifndef KKURZ_MODULE_DISCOVERY
#define KKURZ_MODULE_DISCOVERY

/**
 * @class ModuleDiscovery
 * @brief  abstract class for use with various module discovery models in the Open Word Enumeration	Framework
 *
 * @author Kyle Kurz
 */
class ModuleDiscovery
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		ModuleDiscovery();

		/**
		 * @brief standard destructor
		 */
		virtual ~ModuleDiscovery()=0;

		virtual void reportModules( DataStructure *structure, WordScoring *model)=0;
		virtual void findPosition(const std::vector<Sequence*> & seqs, const std::vector<std::pair<std::string, Scores> > & words, std::vector<std::vector<std::vector<int> > >& positions, int fixed )=0;

	protected:
		/**
		 * @brief pointer to the list of input parameters
		 */
		OWEFArgs *list;
};

#endif
