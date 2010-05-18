/*
 **********************************************
 // File:	Sequence_clustering.cpp
 //
 // Author:	Kyle Kurz
 //
 // Code contributions: 	Xiaoyu Liang, Josh Welch,
 //			Frank Drews, Jens Lichtenberg
 //
 // Date:	4.29.09
 //
 // Purpose:	Define a class for use
 //		in sequence clustering analysis
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

#include "SequenceClustering.h"

using namespace std;

SequenceClustering::SequenceClustering()
{
	list = NULL;
}

SequenceClustering::SequenceClustering( OWEFArgs *from_input, DataStructure *structure, WordScoring *model )
{
	cout << endl << "Clustering sequences..." << endl << endl;
	list = from_input;
	int retval;
	string cmd;
	stringstream stream;

	stream << list->prefix << "_" << list->maxlength << "_" << list->order << "_sequence_clusters.csv";
	string filename;
	stream >> filename;

	ofstream log( list->log_file.c_str(), ios::app );
	log << "Sequence clustering file: " << filename << endl;
	log.close();

	stream.clear();
	string score_index;
	switch( list->sort )
	{
		case 'r':
			score_index = "_scored_selected_r.csv ";
			break;
		case 'o':
			score_index = "_scored_selected_o.csv";
			break;
		case 's':
			score_index = "_scored_selected_s.csv";
			break;
		default:
			score_index = "_scored-selected_r.csv";
			break;

	}
	stream << "./bin/SequenceClustering " << list->seq_file << " " << list->prefix << score_index << " " << list->prefix;
	getline( stream, cmd );
	retval = system( cmd.c_str() );

	//analyze(structure, model);
}

SequenceClustering::~SequenceClustering()
{
	list = NULL;
	delete list;
}
