/*
 // File:	Scores.cpp
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define a class for returning all
 //		scoring information about a word
 //		in the Open Word Enumeration
 //		Framework.

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

#include "Scores.h"

using namespace std;

Scores::Scores()
{
	//no-op
	palindrome = 0;
	revcomp_pres = 0;
	E = 0;
	E_s = 0;
	OE = 0;
	Oln = 0;
	Sln = 0;
	Var = 0;
	pval = 0;
	seqs = 0;
	count = 0;
	revcomp = "";
}

Scores::~Scores()
{
	//no-op
}
