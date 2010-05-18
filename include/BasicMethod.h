/*
 **********************************************
 // File:		BasicMethod.h
 //
 // Author:	Kyle Kurz
 //
 // Date:		4.22.10
 //
 // Purpose:	Define an basic clustering class
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

#include "ClusterMethod.h"

#ifndef KKURZ_BASIC_METHOD
#define KKURZ_BASIC_METHOD

/**
 * @class BasicMethod
 * @brief BasicMethod class.
 *
 * @author Kyle Kurz
 */
class BasicMethod : public Cluster
{
	public:
	
		//************************************************************
		//Constructors/Destructors
		//************************************************************
	
		/**
		 * @brief standard constructor
		 */
		BasicMethod();
	
		/**
		 * @brief initialized constructor
		 */
		BasicMethod( OWEFArgs *from_input, DataStructure *structure, WordScoring *model );
	
		/**
		 * @brief standard destructor
		 */
		~BasicMethod();
	
	private:
	
		//***********************************************************
		//Modifiers
		//***********************************************************
	
		/**
		 * @brief create the clusters from the word list
		 */
		void buildClusters( DataStructure *structure, WordScoring *model ){return;}
};

#endif
