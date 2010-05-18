/*
**********************************************
// File:	Word_scoring.cpp
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define an abstract class for use
//		with various word scoring models
//		in the Open Word Enumeration
//		Framework
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
 
#include "WordScoring.h"

using namespace std;

WordScoring::WordScoring()
{
	//no-op
}

WordScoring::~WordScoring()
{
	//no-op
}

//give the seed list to anyone who needs it
vector<pair<string, Scores> > WordScoring::getSeeds()
{
	vector<pair<string, Scores> > temp;
	switch( list->sort )
	{
		case 'r':
			if(top_words[0].size() == 0)
				return temp;
			return top_words[ 0 ];
			break;
		case 'o':
			if(top_words[1].size() == 0)
				return temp;
			return top_words[ 1 ];
			break;
		case 's':
			if(top_words[2].size() == 0)
				return temp;
			return top_words[ 2 ];
			break;
		default:
			if(top_words[2].size() == 0)
				return temp;
			return top_words[ 2 ];
			break;
	}
	return temp;
}

