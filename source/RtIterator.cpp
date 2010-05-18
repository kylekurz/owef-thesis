/*
 **********************************************
 // File:	Rt_iterator.cpp
 //
 // Author:	Kyle Kurz
 //
 // Date:	11.02.09
 //
 // Purpose:	Define a radix trie iterator class for use
 //		with various data structures and
 //		scoring models in the Open Word
 //		Enumeration Framework.
 **********************************************

 Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang, Rami Alouran


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

#include "RtIterator.h"

using namespace std;

//************************************************************
//Constructors/Destructors
//************************************************************

//standard constructor
RadixTrieIterator::RadixTrieIterator()
{
	//no-op
	pointer = NULL;
	list = NULL;
	next_branch = 0;
	last_word = "";
	last_loc = NULL;
}

RadixTrieIterator::RadixTrieIterator( OWEFArgs *from_input, DataStructure *in_structure, int wordlength )
{
	list = from_input;
	wl = wordlength;
	pointer = (RadixTrie *) in_structure;
	next_branch = 0;
	last_word = "";
	last_loc = NULL;
	words_reported = 0;
}

//standard destructor
RadixTrieIterator::~RadixTrieIterator()
{
	list = NULL;
	delete list;
	pointer = NULL;
	delete pointer;
	last_loc = NULL;
	delete last_loc;
}

//iterator information
bool RadixTrieIterator::hasNext()
{
	//if we haven't reported all the words, we can give more
	if( words_reported < list->num_words[ wl - 1 ] )
		return 1;
	//if we have, we need to reset so the next person to use the iterator can get the first word again
	else
	{
		next_branch = 0;
		last_word = "";
		last_loc = NULL;
		words_reported = 0;
		return 0;
	}
}

string RadixTrieIterator::next()
{
	string ret_word = "";
	RadixTrieNode *node = NULL;
	//if we haven't returned anything yet...
	if( last_loc == NULL && last_word.compare( "" ) == 0 )
	{
		//if we have a trie here...
		if( pointer->root && pointer->root->branch )
		{
			node = pointer->root;
			//hunt until we find a word long enough to be the "first"
			while( static_cast<int> ( ret_word.length() ) < wl )
			{
				//if we can keep following this branch
				if( node && node->branch && node->branch[ next_branch ] )
				{
					///char x = conversion[next_branch];
					char x = 'A' + reverse_branch[ next_branch ]; // ljn 10/5/2009
					ret_word += x;
					node = node->branch[ next_branch ];
					next_branch = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else
				{
					next_branch++;
					if( next_branch >= ALPH )
					{
						while( next_branch >= ALPH )
						{
							char branch_id = ret_word[ ret_word.length() - 1 ];
							ret_word = ret_word.substr( 0, ret_word.length() - 1 );
							next_branch = (locateBranch( branch_id )) + 1;
							node = pointer->root;
							for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
							{
								if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
								{
									node = node->branch[ locateBranch( ret_word[ i ] ) ];
								}
							}
							if( ret_word.length() == 0 && next_branch >= ALPH )
							{
								last_loc = NULL;
								last_word.clear();
								last_word = "";
								return "";
							}
						}
					}
				}
			}
			last_loc = node;
			last_word = ret_word;
			words_reported++;
			return ret_word;
		}
	}
	//otherwise we have something and can start from there...
	else
	{
		char branch_id = last_word[ last_word.length() - 1 ];
		ret_word = last_word.substr( 0, last_word.length() - 1 );
		next_branch = locateBranch( branch_id ) + 1;
		node = pointer->root;
		for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
		{
			if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
			{
				node = node->branch[ locateBranch( ret_word[ i ] ) ];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while( static_cast<int> ( ret_word.length() ) < wl )
		{
			//if we can keep following this branch
			if( node && node->branch && node->branch[ next_branch ] )
			{
				///char x = conversion[next_branch];
				char x = 'A' + reverse_branch[ next_branch ]; // ljn 10/5/2009
				ret_word += x;
				node = node->branch[ next_branch ];
				next_branch = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else
			{
				next_branch++;
				if( next_branch >= ALPH )
				{
					char branch_id;
					while( next_branch >= ALPH )
					{
						branch_id = ret_word[ ret_word.length() - 1 ];
						ret_word = ret_word.substr( 0, ret_word.length() - 1 );
						next_branch = locateBranch( branch_id ) + 1;
						node = pointer->root;
						for( int i = 0; i < static_cast<int> ( ret_word.length() ); i++ )
						{
							if( node && node->branch && node->branch[ locateBranch( ret_word[ i ] ) ] )
							{
								node = node->branch[ locateBranch( ret_word[ i ] ) ];
							}
						}
						if( ret_word.compare( "" ) == 0 && (next_branch >= ALPH || next_branch < 0) )
						{
							last_loc = NULL;
							last_word.clear();
							last_word = "";
							return "";
						}
					}
				}
			}
		}
	}
	last_loc = node;
	last_word = ret_word;
	words_reported++;
	return ret_word;
}

