/*
 **********************************************
 // File:	Cluster_method.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	4.22.09
 //
 // Purpose:	Define a class for use
 //		in clustering in the Open
 //		Word Enumeration Framework.
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

#ifndef KKURZ_CLUSTER_METHOD
#define KKURZ_CLUSTER_METHOD

/**
 * @class Cluster
 * @brief class for use	in clustering in the Open Word Enumeration Framework.
 *
 * @author Kyle Kurz
 */
class Cluster
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		Cluster();

		/**
		 * @brief standard destructor
		 */
		virtual ~Cluster()=0;

		//************************************************************
		//Modifiers
		//************************************************************

		virtual void buildClusters( DataStructure *structure, WordScoring *model )=0;

	protected:

		/**
		 * @brief create motif logos for clusters
		 */
		void createLogos( DataStructure *structure, WordScoring *model );
		//
		double scoreCluster( std::vector<std::string> cluster, DataStructure *structure, WordScoring *model );
		//
		void outputRegExp( std::ostream & out_file, const std::vector<std::vector<float> > & pwm );
		//
		void outputPwm( std::ostream & out_file, const std::vector<std::vector<float> > & pwm );
		//
		std::vector<std::vector<float> > computePwm( std::vector<std::string> cluster_data, DataStructure *structure );
		//
		void createMotifLogo( const std::vector<std::vector<float> > & pwm, const std::string & filename );
		//
		//bool desc_order(const pair<double, vector <word_data> > & op1, const pair<double, vector <word_data> > & op2);

		/**
		 * @brief pointer to the list of input parameters
		 */
		OWEFArgs *list;
		std::vector<std::vector<std::string> > clusters;
};

#endif
