//******************************************************
// File:	Radix_Trie_Node.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a radix trie node structure
//		for use with the WordSeeker pipeline.
//*****************************************************

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#ifndef KKURZ_RADIX_TRIE_NODE
#define KKURZ_RADIX_TRIE_NODE

#define ALPH 5

//
/**
 * @class RadixTrieNode
 * @brief a radix trie node structure for use with the WordSeeker pipeline.
 *
 *	class for a node (has 1 NULL pointer if leaf, possibly 4 NULL pointers if branch)
 *
 * @author Kyle Kurz
 */
class RadixTrieNode
{
	public:
		/**
		 * @brief standard constructor
		 */
		RadixTrieNode();
		/**
		 * @brief function to set branch pointer to something else
		 */
		RadixTrieNode **newPointerArray (void);
		//public variables
		int data;
		int num_seq;
		int last_seq;
		RadixTrieNode **branch;	// we only need 4 elements: A, C, G, T
								// actually need 5, including 'n'
};

#endif
