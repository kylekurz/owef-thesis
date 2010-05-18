/*
 **********************************************
 // File:	Scores.h
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define a class for returning all
 //		scoring information about a word
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
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#ifndef KKURZ_SCORES
#define KKURZ_SCORES

#define SCORES_EL 10

/**
 * @class Scores
 * @brief a class for returning all	scoring information about a word in the Open Word Enumeration Framework.
 *
 * @author Kyle Kurz
 */
class Scores
{
	public:
		/**
		 * @brief standard constructor
		 */
		Scores();
		/**
		 * @brief standard destructor
		 */
		~Scores();

		void zeroOut()
		{
			E = E_s = Var = OE = Oln = Sln = pval = expect = variance = 0.0;
			seqs = count = 0;

		}

		//public variables
		bool palindrome;
		bool revcomp_pres;
		double E;
		double E_s;
		double Var;
		double OE;
		double Oln;
		double Sln;
		double pval;
		int seqs;
		int count;
		std::string revcomp;
		//rmes variables
		double expect;
		double variance;
};
#endif
