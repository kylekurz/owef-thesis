/*
 **********************************************
 // File:	Word_distribution.cpp
 //
 // Author:	Kyle Kurz
 //
 // Code contributions: 	Xiaoyu Liang, Josh Welch,
 //			Frank Drews, Jens Lichtenberg
 //
 // Date:	4.29.09
 //
 // Purpose:	Define a class for use
 //		in word distribution analysis
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

#include "WordDistribution.h"

#define MAX_VAL 100

using namespace std;

WordDistribution::WordDistribution()
{
	list = NULL;
}

WordDistribution::WordDistribution( OWEFArgs *from_input, DataStructure *structure, WordScoring *model, SequenceFile *input )
{
	cout << endl << "Computing word distributions... " << endl << endl;
	list = from_input;
	computeDistributions( structure, model, input );
}

WordDistribution::~WordDistribution()
{
	//no op
}

void WordDistribution::computeDistributions( DataStructure *structure, WordScoring *model, SequenceFile *input )
{
	int system_return;
	vector<int> distribution;
	vector<pair<string, Scores> > words = model->getSeeds();
	vector<Sequence*> seq_file;
	seq_file = input->getSequences();
	ofstream fout;
	string filename;
	stringstream stream;
	if( list->normalize )
		distribution.resize( MAX_VAL, 0 );
	else
		distribution.resize( seq_file.size(), 0);

	int num_dist=0;
	printf("sequences has size: %d\n", (int)seq_file.size());
	if(list->dist_count > static_cast<int>(words.size()))
		num_dist = words.size();
	else
		num_dist = list->dist_count;
	for( int i = 0; i < static_cast<int> ( num_dist ); ++i )
	{
		for( int j = 0; j < static_cast<int> ( seq_file.size() ); ++j )
		{
			//printf("computing j %d\n", j);
			if( list->normalize )
				countSequenceNormal( seq_file[ j ], words[ i ].first, distribution );
			else
				countSequence( seq_file[ j ], words[ i ].first, distribution );
		}

		stream << list->prefix << "_distribution_" << words[i].first << ".csv";
		stream >> filename;
		ofstream log( list->log_file.c_str(), ios::app );
		log << "Word Distribution file " << i + 1 << ": " << filename << endl;
		log.close();
		fout.open( filename.c_str() );
		outputWordDistribution( fout, distribution );
		fout.close();

		distribution.assign( distribution.size(), 0 );

		stream.clear();
		string cmd;
		stream << "./WordDist_Pl.sh " << list->prefix << "_distribution_" << words[i].first << ".csv " << list->prefix << "_distribution_" << words[i].first << ".png";
		getline( stream, cmd );
		system_return = system( cmd.c_str() );
		stream.clear();
	}

}

//store the locations of the words within the sequences
void WordDistribution::countSequence( const Sequence *seq, string & word, vector<int> & distribution )
{
	if( seq->getBodyLength() < word.length() )
		return;
				
	for( int i = 0; i < (static_cast<int> ( seq->getBodyLength() ))- (int)word.length() + 1; i++ )
	{
		const string *s_ptr = (*seq).getText();
		char *t = (char *)&(*s_ptr)[(*seq).getOffset() + i];
		
		if( static_cast<int> ( distribution.size() ) < (i + 1) )
                {
                        distribution.push_back( 0 );
                }

		if(match(t, &word[0], word.length()))
		{
			++distribution[ i ];
		}
	}
}

//store the locations of the words within the sequences with a percentage normalization
void WordDistribution::countSequenceNormal( const Sequence *seq, string & word, vector<int> & distribution )
{

	if( seq->getBodyLength() < word.length() )
		return;
				
	for( int i = 0; i < (static_cast<int> ( seq->getBodyLength() ))-(int)word.length() + 1; i++ )
	{
		const string *s_ptr = (*seq).getText();
		char *t = (char *)&(*s_ptr)[(*seq).getOffset() + i];
		if(match(t, &word[0], word.length()))
		{
			double percent = ((i / (double) seq->getBodyLength()) * MAX_VAL);
			int loc = (int) floor( percent );
			++distribution[ loc ];
		}
	}
}

void WordDistribution::outputWordDistribution( ofstream &fout, vector<int> distribution )
{
	if( list->normalize )
		fout << "Percent" << ',' << "Occurrences" << endl;
	else
		fout << "Position" << ',' << "Occurrences" << endl;

	for( int i = 0; i < static_cast<int> ( distribution.size() ); ++i )
	{
		if( list->normalize )
			fout << i << ',' << distribution[ i ] << endl;
		else
			fout << (i + 1) << ',' << distribution[ i ] << endl;
	}
}

//convert all characters in a word to upper_case
string WordDistribution::toUppercase( string check )
{
	for( int i = 0; i < static_cast<int> ( check.length() ); i++ )
	{
		switch( check[ i ] )
		{
			case 'a':
				check[ i ] = 'A';
				break;
			case 'c':
				check[ i ] = 'C';
				break;
			case 'g':
				check[ i ] = 'G';
				break;
			case 't':
				check[ i ] = 'T';
				break;
			case 'n':
				check[ i ] = 'N';
				break;
			case 'A':
				break;
			case 'C':
				check[ i ] = 'C';
				break;
			case 'G':
				check[ i ] = 'G';
				break;
			case 'T':
				check[ i ] = 'T';
				break;
			case 'N':
				check[ i ] = 'N';
				break;
			default:
				check[ i ] = 'N';
				break;
		}
	}
	return check;
}


bool WordDistribution::match(char *x, char *y, int length)
{
	for(int i=0; i<length; i++)
	{
		int res = (*x)-(*y);
		if(res != 0 && res != 32 && res != (-32))
		{
			//printf("false\n");
			return false;
		}
		x++;
		y++;
	} 
	return true;
}

