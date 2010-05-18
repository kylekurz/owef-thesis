/*
 * SequenceFile.h
 *
 * Header file for SequenceFile class
 *
 *      Author: Lev A Neiman - lev.neiman@gmail.com
 */

#ifndef _LEV_SEQ_FILE_H
#define _LEV_SEQ_FILE_H

#include <string>
#include <vector>
#include <iostream>

#include "Sequence.h"
#include "Utilities.h"
/**
 * @class SequenceFile
 * @brief Container for sequences extracted from a FASTAFile.
 *
 * @author Lev A Neiman - lev.neiman@gmail.com
 */
class SequenceFile
{
	public:
		SequenceFile( const char * file_name );
		SequenceFile(std::istream & input_stream );

		virtual ~SequenceFile()
		{
			//Sequence::clear_all();
		}

		/**
		 * return const reference to vector of sequences.
		 */
		std::vector<Sequence*> getSequences() const
		{
			return sequences;
		}

		/**
		 * return number of sequences in this FASTA file.
		 */
		unsigned int getNumSequences()
		{
			return sequences.size();
		}

		/**
		 * return const reference to Sequence under index i.
		 */
		const Sequence & getSequence( unsigned int i )
		{
			return *sequences[i];
		}

		const char * getText()
		{
			return text.c_str();
		}

		std::string toString() const;

	public:
		const char * file_name;
		std::string text;
		std::vector<Sequence*> sequences;

		void readFile();
		void readStream(std::istream & input_stream);

};

#endif
