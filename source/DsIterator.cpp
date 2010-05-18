/*

 **********************************************
 // File:	Ds_iterator.cpp
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

 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran

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

#include "DsIterator.h"

//standard constructor
DataStructureIterator::DataStructureIterator()
{
	//no-op
}

//standard destructor
DataStructureIterator::~DataStructureIterator()
{
	list = NULL;
}
