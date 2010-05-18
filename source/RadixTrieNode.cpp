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
/*
 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran
 */

#include "RadixTrieNode.h"

using namespace std;

RadixTrieNode::RadixTrieNode()
{
	data = 0;
	num_seq = 0;
	last_seq = 0;
	branch = NULL;
}

RadixTrieNode **RadixTrieNode::newPointerArray( void )
{
	RadixTrieNode **new_node;
	new_node = (RadixTrieNode **) malloc( sizeof(RadixTrieNode *) * ALPH );
	return new_node;
}
