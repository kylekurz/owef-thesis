/*
 * Utilities.h
 *
 * Header file for Utilities functions.
 *
 *      Author: Lev A Neiman - lev.neiman@gmail.com
 */

#ifndef _LEV_UTILITIES_H
#define _LEV_UTILITIES_H

#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <vector>
#include <set>
#include "Sequence.h"

#define ALPH 5

//change this array as we add characters to the supported alphabet
//use the branch_array as:  int = branch_array[input char - 'A'];
//use the reverse_branch as: char = 'A' + reverse_branch[int]; 
static const int branch_array[ 26 ] =
	{ 0, 4, 1, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4 };
static const int reverse_branch[ 26 ] =
	{ 0, 2, 6, 19, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13 };

/**
 * Compute number of milliseconds between end and start.
 */
int getTimeSpan( timeb & start, timeb & end );

/**
 * Retrieve file size
 */
unsigned int getFileSize( const char * file_name );

/**
 * return true if a is a latin letter in lower case.
 */
bool isLowerCase( char a );

/**
 * append vector b to end of vector a.
 */
template<class T>
inline void append_vector( std::vector<T> & a, std::vector<T>&b )
{
	a.insert( a.end(), b.begin(), b.end() );
}

unsigned int naiveCount( Sequence * sequence, const std::string & needle );

/**
 * count occurences of needle in vector of sequences.
 */
unsigned int naiveCount( const std::vector<Sequence*> & sequences, const std::string & needle );

/**
 * count how many sequences needle occurs in.
 */
unsigned int naiveSeqCount( const std::vector<Sequence*> & sequences, const std::string & needle );

/**
 * enumerate all unique words of given length and store them in set words.
 */
void naiveWordEnumerator( const std::vector<Sequence*> & sequences, std::set<std::string> & words, unsigned int word_size );

void naiveGetLocs( const std::vector<Sequence*>&sequences, const std::string & word, std::vector<int>&locs);

/**
 * Generate random genomic sequence of given length.
 */
std::string generateRandomGeneticSequence( size_t length );

/*
 * Generate random character - A, C, G or T
 */
char getRandCode();

/*
 * Convert a word to its specific PWM
 */
std::vector<std::vector<double> > getPWM(std::string word);

/*
 * Convert DNA characters to 0-4 (ACGTN = 01234)
 */
int locateBranch( char x );

#endif
