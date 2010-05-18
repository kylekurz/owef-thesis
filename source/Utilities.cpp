#include "Utilities.h"
#include <iostream>
#include <cstdlib>
#include <stdio.h>

char getRandCode()
{
	int t = rand() % 4;
	switch( t )
	{
		case 0:
			return 'A';
		case 1:
			return 'G';
		case 2:
			return 'T';
		case 3:
			return 'C';
	}
	return 'X';
}

int getTimeSpan( timeb & start, timeb & end )
{
	// if seconds are equal return the difference in milliseconds.
	if( start.time == end.time )
		return end.millitm - start.millitm;
	// otherwise do some hardcore maths :p
	return (end.time - (start.time + 1)) * 1000 + (1000 - start.millitm) + end.millitm;
}

unsigned int getFileSize( const char * file_name )
{
	struct stat result;
	if( stat( file_name, &result ) == 0 )
	{
		return result.st_size;
	}
	else
	{
		return -1;
	}
}
bool isLowerCase( char a )
{
	return a >= 97 && a <= 122;
}

unsigned int naiveCount( Sequence * sequence, const std::string & needle )
{
	unsigned int count = 0;
	for( unsigned int i = 0; i < sequence->getBodyLength() - needle.size() + 1; ++i )
	{
		bool ok = true;
		for( unsigned int j = 0; j < needle.size(); ++j )
		{
			if( (*sequence)[i + j] != needle[j] )
			{
				ok = false;
				break;
			}
		}
		if( ok )
			++count;
	}
	return count;
}

unsigned int naiveCount( const std::vector<Sequence*> & sequences, const std::string & needle )
{
	unsigned int count = 0;
	for( unsigned int seq_num = 0; seq_num < sequences.size(); ++seq_num )
	{
		count += naiveCount( sequences[seq_num], needle );
	}
	return count;
}

unsigned int naiveSeqCount( const std::vector<Sequence*> & sequences, const std::string & needle )
{
	unsigned int count = 0;
	for( unsigned int seq_num = 0; seq_num < sequences.size(); ++seq_num )
	{
		for( unsigned int i = 0; i < sequences[seq_num]->getBodyLength() - needle.size() + 1; ++i )
		{
			bool ok = true;
			for( unsigned int j = 0; j < needle.size(); ++j )
			{
				if( (*(sequences[seq_num]))[i + j] != needle[j] )
				{
					ok = false;
					break;
				}
			}
			if( ok )
			{
				++count;
				break;
			}
		}
	}
	return count;
}

void naiveWordEnumerator( const std::vector<Sequence*> & sequences, std::set<std::string> & words, unsigned int word_size )
{
	using namespace std;
	const string * text = sequences[0]->getText();
	for( unsigned int seq_num = 0; seq_num < sequences.size(); ++seq_num )
	{
		if( sequences[seq_num]->getBodyLength() < word_size )
			continue;
		for( unsigned int i = 0; i < sequences[seq_num]->getBodyLength() - word_size + 1; ++i )
		{
			words.insert( text->substr( i + sequences[seq_num]->getOffset(), word_size ) );
		}
	}
}

void naiveGetLocs( const std::vector<Sequence*> & sequences, const std::string & needle, std::vector<int> & locs )
{
	using namespace std;
	set<unsigned int> seqs;
	/*
	 for( unsigned int i = 0; i < sequences.size(); ++i )
	 {
	 Sequence * p = sequences[i];
	 unsigned int fasta_id = p->getFASTAID();
	 if( naiveCount( p, word ) > (unsigned int) 0 )
	 {
	 seqs.insert( fasta_id );
	 }
	 }
	 */

	for( unsigned int seq_num = 0; seq_num < sequences.size(); ++seq_num )
	{
		for( unsigned int i = 0; i <= sequences[seq_num]->getBodyLength() - needle.size(); ++i )
		{
			bool ok = true;
			for( unsigned int j = 0; j < needle.size(); ++j )
			{
				if( (*(sequences[seq_num]))[i + j] != needle[j] )
				{
					ok = false;
					break;
				}
			}
			if( ok )
			{
				seqs.insert( sequences[seq_num]->getFASTAID() );
				break;
			}
		}
	}

	for( set<unsigned int>::iterator i = seqs.begin(); i != seqs.end(); ++i )
	{
		++locs[*i];
	}
}

std::string generateRandomGeneticSequence( size_t length )
{
	using namespace std;
	string ret = "";
	for( size_t i = 0; i < length; ++i )
	{
		ret += getRandCode();
	}
	return ret;
}

std::vector<std::vector<double> > getPWM( std::string word )
{
	using namespace std;
	vector<vector<double> > ret;
	int last_loc = 0;
	while( last_loc < (int) word.length() )
	{
		vector<double> temp;
		temp.resize( ALPH - 1, 0 );
		//calculate the weights for this position in the string
		if( word[last_loc] != '[' && word[last_loc] != ']' )
		{
			temp[locateBranch( word[last_loc] )] = 1.0;
			last_loc++;
		}
		else if( word[last_loc] == '[' )
		{
			int start = last_loc + 1;
			int end = word.find_first_of( ']', last_loc );
			for( int j = start; j < end; j++ )
				temp[locateBranch( word[j] )] = (double) 1 / (end - start);
			last_loc = ++end;
		}
		ret.push_back( temp );
	}
	return ret;
}

// ljn added 10/5/2009
int locateBranch( char x )
{
	int branch_index = -1;
	x = toupper( x );
	branch_index = branch_array[x - 'A'];
	return branch_index;
}

