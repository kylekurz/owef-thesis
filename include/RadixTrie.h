//**********************************************
// File:	Radix_trie.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a radix trie class for use
//		with the WordSeeker pipeline.
//**********************************************
/*
 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran
 */

#ifdef KKURZ_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#endif

#include "DataStructure.h"
#include "RadixTrieLeaf.h"
#include "Sequence.h"
#include "SequenceFile.h"
#include "SequenceFilters.h"

#ifndef KKURZ_RADIX_TRIE
#define KKURZ_RADIX_TRIE

/**
 * @class RadixTrie
 * @brief a radix trie class for use with the WordSeeker pipeline.
 *
 * @author Kyle Kurz
 */
class RadixTrie : public DataStructure
{
		friend class RadixTrieIterator;

	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		RadixTrie();

		/**
		 * @brief empty constructor
		 */
		RadixTrie( OWEFArgs *from_input, int x );

		/**
		 * @brief initialized constructor
		 */
		RadixTrie( OWEFArgs *from_input, SequenceFile *in );

		/**
		 * @brief standard destructor
		 */
		~RadixTrie();
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************

		/**
		 * @brief function to get word count
		 */
		int getCount( std::string & motif );
		/**
		 * @brief function to get sequence count
		 */
		int getSeqs( std::string & motif );
		/**
		 * @brief function to get the statistics stored in a leaf node from the trie
		 */
		Scores* getStats( std::string & motif );
		/**
		 * @brief function to return all words that match a regular expression
		 */
		void getRegexMatches( std::vector<std::string> &matches, std::string & regex );
		/**
		 * @brief function to get all of the sequences from the input file
		 */
		void getSeqFile( std::vector<std::string> &seqs );
		/**
		 * @brief function to get a block of words
		 */
		void getNextWordBlock( std::vector<std::string> &block, int length, int count );
		/**
		 * @brief function to iterate through the words contained in the data structure
		 */
		void getNextWord( std::string &motif, int length );
		/**
		 * @brief function to iterate through the words contained in the data structure from a temporary root
		 */
		std::string getNextWord( RadixTrieNode *temp_root, int length );
		/**
		 * @brief increment the bit vector to show coverage of a word across the sequences
		 */
		void getLocs(std::vector<int> &bit, std::string &motif );
		/**
		 * @brief function to output information
		 */
		void output();
		/**
		 * @brief function to reset last_word variables
		 */
		//void reset();
		/**
		 * @brief function to reset last_iterator variables
		 */
		void resetIterator( int thread );

		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.
		//************************************************************
		/**
		 * @brief function to set count of a string
		 */
		int setCount( std::string motif, int count );
		/**
		 * @brief function to set the count in a trie member
		 */
		int trieSet( RadixTrieNode *node, char *s, int length, int count );
		/**
		 * @brief function to increment count
		 */
		int incCount( char *motif, int length );
		/**
		 * @brief function to set the statistics of a word in the trie
		 */
		int setStats( std::string & motif, Scores *new_stats );
		/**
		 * @brief function to set the statistics of a word in the trie
		 */
		int trieStats( RadixTrieNode *node, char *s, int length, Scores *new_stats );

	private:

		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implmented
		//for each data structure incorporated.
		//************************************************************


		/**
		 * @brief function to increment seqs
		 */
		int incSeqs( char *motif, int length );
		/**
		 * @brief function to count all the words (called from the constructor)
		 */
		void countWords( SequenceFile *input );
		/**
		 * @brief function to mutate a string to the next physical permutation
		 */
		void mutate( std::string &previous_word );

		//************************************************************
		//Radix Trie functions
		//These functions are specific to this implementation of the
		//WordSeeker data structure
		//************************************************************

		/**
		 * @brief add a node to the trie
		 */
		void trieAdd( RadixTrieNode * &node, char *s, int length, int level );

		/**
		 * @brief get the information about what is stored at a node
		 */
		int trieGet( RadixTrieNode * &node, char *s, int length, int level );

		/**
		 * @brief get the stats from a leaf
		 */
		Scores* trieFindStats( RadixTrieNode * &node, char *s, int length );

		/**
		 * @brief find a word in the trie
		 */
		int trieFind( RadixTrieNode * &node, char *s, int length );

		/**
		 * @brief function to get the sequence count of a word in the radix trie (no increment)
		 */
		int trieFindS( RadixTrieNode * &node, char *s, int length );

		/**
		 * @brief delete the trie
		 */
		void trieClear( RadixTrieNode * root );

		//output to a file so we can use it in the next step
		//void output(ofstream &ofile, RadixTrieNode *ptr, string word, int length);

		/**
		 * @brief when missing words and enumerate are true, enumerate all words that include 'X' character
		 */
		void enumerateString( std::ofstream &ofile, std::vector<std::string> words );

		/**
		 * @brief search for missing words in the dataset
		 */
		void missSearch( RadixTrieNode *root, std::ofstream &ofile, int length_rem, std::string &word );

		//convert strings to uppercase
		//string convert(string check);
		//string unconvert(string check);
		std::string toUppercase( const std::string & word );

		/**
		 * @brief functions to get the thread information from the system, regardless of whether we have OMP or not
		 */
		int threadNum();
		int numProcs();

		//***********************************************************
		//Radix Trie variables
		//These variables are specific to this implementation of the
		//WordSeeker data structure
		//***********************************************************

		/**
		 * @brief root of a trie. used to access all of the words.
		 */
		RadixTrieNode *root;

		/**
		 * @brief queue for use during getNextWord search
		 */
		std::vector<std::vector<std::string> > breadth_search;

		/**
		 * @brief store the last reported word
		 */
		std::vector<std::string> last_word;
		std::vector<RadixTrieNode *> last_loc;
		std::vector<std::vector<std::string> > last_ext;
		std::vector<std::vector<RadixTrieNode *> > last_ext_loc;
		int next_branch;
		std::vector<int> next_branch_iterator;
		bool flag;
		long double rt_size;
};

#endif
