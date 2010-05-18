//******************************************************
// File:	Radix_trie_leaf.h
//
// Author:	Kyle Kurz
//
// Date:	10.15.09
//
// Purpose:	Define a radix trie leaf structure
//		for use with the Open Motif framework.
//*****************************************************
/*
 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran
 */

#include "RadixTrieLeaf.h"

using namespace std;

RadixTrieLeaf::RadixTrieLeaf()
{
	data = 0;
	num_seq = 0;
	last_seq = 0;
	branch = NULL;
	storage = NULL;
}
