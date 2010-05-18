/*
 **********************************************
 // File:	Rt_iterator.h
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

#include "DsIterator.h"
#include "RadixTrie.h"

#ifndef KKURZ_RT_ITERATOR
#define KKURZ_RT_ITERATOR

/**
 * @class RadixTrieIterator
 * @brief a radix trie iterator class for use with various data structures and scoring models in the Open Word	Enumeration Framework.
 *
 * @author Kyle Kurz
 **/
class RadixTrieIterator : public DataStructureIterator
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************

		/**
		 * @brief standard constructor
		 */
		RadixTrieIterator();

		/**
		 * @brief initialized constructor
		 */
		RadixTrieIterator( OWEFArgs *from_input, DataStructure *structure, int wordlength );

		/**
		 * @brief standard destructor
		 */
		~RadixTrieIterator();

		/**
		 * @brief iterator information
		 */
		bool hasNext();
		std::string next();

	protected:
		RadixTrie *pointer;
		int wl, next_branch, words_reported;
		std::string last_word;
		RadixTrieNode *last_loc;

};

#endif
