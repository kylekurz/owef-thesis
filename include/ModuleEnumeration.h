/*
 **********************************************
 // File:	Module_discovery.cpp
 //
 // Author:	Kyle Kurz and Xiaoyu Liang
 //
 // Date:	7.20.09
 //
 // Purpose:	Define a class for use
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

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <list>
#include <ctime>
#include <cmath>
#include <iomanip>

#include "ModuleDiscovery.h"

#define COMMENT '>'
#define DELIM ","

#ifndef KKURZ_MODULE_ENUMERATION
#define KKURZ_MODULE_ENUMERATION

/**
 * @class ModuleEnumeration
 * @brief class for use with various module discovery models in the Open Word Enumeration Framework
 *
 * @author Kyle Kurz
 * @author Xiaoyu Liang
 */
class ModuleEnumeration : public ModuleDiscovery
{
	public:
		//constructors
		ModuleEnumeration();
		ModuleEnumeration( OWEFArgs *from_input );
		ModuleEnumeration( OWEFArgs *from_input, DataStructure *structure, WordScoring *model, SequenceFile *input );

		/**
		 * @brief destructor
		 */
		~ModuleEnumeration();

		//general purpose module discovery functions
		void reportModules( DataStructure *structure, WordScoring *model );
		void findPosition(const std::vector<Sequence*> & seqs, const std::vector<std::pair<std::string, Scores> > & words, std::vector<std::vector<std::vector<int> > >& positions, int fixed );

	private:

		//general purpose private functions
		//void read_seq(istream &seq_file, vector<string> &seqs, int &seq_no);
		void replace( std::string& str, char former, char latter );
		//void readWords( std::istream &words_file, std::vector<std::pair<std::string, Scores> > &words, std::vector<double> &prob );
		void makeNCombinations( std::vector<std::vector<int> > & combinations, const std::vector<int> & input, int n, int s );
		bool check(std::vector<int> &sub_map,std::vector<int> &each_combs, const std::vector<std::pair<std::string, Scores> > &words );
		void posComb( const std::vector<int> &each_combs, const std::vector<std::vector<int> > &each_seq_pos, std::vector<std::vector<int> >&line );
		void createMap(const std::vector<std::vector<int> >&combs, const std::vector<std::vector<std::vector<int> > >&positions, std::vector<std::vector<std::vector<std::vector<int> > > >&map);
		void createNewMap( std::vector<std::vector<std::vector<std::vector<int> > > >&map, std::vector<std::vector<int> >&combs,	std::vector<std::pair<std::string, Scores> > &words);
		void distDens( const std::vector<std::vector<int> > &combs, const std::vector<std::vector<std::vector<std::vector<int> > > > &map, std::vector<std::vector<double> > &den,
				    const std::vector<std::pair<std::string, Scores> > &words, std::vector<std::vector<int> > &dist );
		double convertProb( double E, std::vector<int> seqs_length );
	
		//private variable
		SequenceFile *into;
};

#endif
