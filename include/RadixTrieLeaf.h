//******************************************************
// File:	Radix_trie_leaf.h
//
// Author:	Kyle Kurz
//
// Date:	10.15.09
//
// Purpose:	Define a radix trie leaf structure
//		for use with the WordSeeker pipeline.
//*****************************************************
/*
Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran
*/

#include "RadixTrieNode.h"
#include "Scores.h"

#ifndef KKURZ_RADIX_TRIE_LEAF
#define KKURZ_RADIX_TRIE_LEAF

/**
 * @class RadixTrieLeaf
 * @brief a radix trie leaf structure for use with the WordSeeker pipeline.
 *
 *	class for a node (has 1 NULL pointer if leaf, possibly 4 NULL pointers if branch)
 *
 * @author Kyle Kurz
 */
class RadixTrieLeaf : public RadixTrieNode
{
	public:
		RadixTrieLeaf();
		Scores *storage;
};

#endif
