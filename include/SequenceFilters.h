#ifndef _LEV_SEQUENCE_FILTERS_H
#define _LEV_SEQUENCE_FILTERS_H

#include <vector>
#include <set>
#include "Sequence.h"

/**
 * @class SequenceFilters
 * @brief General 1xM filter.
 *
 * This filter takes a function that evaluates a single char to true or false.
 * This filter breaks up input sequence by evaluating each char using this function.
 * If function evaluates to true then it means that that particular char is a delimeter.
 *
 * @author Lev A Neiman - lev.neiman@gmail.com
 */
class SequenceFilters
{
	public:
		/**
		 * Filter one sequence using specified delimeter function.
		 */
		static std::vector<Sequence*> filter1xM( const Sequence & sequence, bool(*delimeterFunc)( char ) );

		/**
		 * filter many sequences.
		 */
		static std::vector<Sequence*> filterNxM( const std::vector<Sequence*> & sequences, bool(*delimeterFunc)( char ) );

		/**
		 * bool function for N Filtering.
		 */
		static bool NFilterDelimiterFunc( char c )
		{
			return c == 'N' || c == 'n';
		}

		/**
		 * bool function for N Filtering.
		 */
		static bool RadixFilterDelimiterFunc( char c )
		{
			return c != 'A' && c != 'a' && c != 'C' && c != 'c' && c != 'G' && c != 'g' && c != 'T' && c != 't';
		}

		/**
		 * bool function for Ancestor Filtering.
		 */
		static bool AncestorFilterDelimiterFunc(char c)
		{
			return c >= 97 && c <= 122; // check if its lower case.
		}

		/**
		 * Return the number of unique FASTA Sequences that are parents of sequences in the vector.
		 */
		static unsigned int countFASTASequences(const std::vector<Sequence*> & v );

};

#endif
