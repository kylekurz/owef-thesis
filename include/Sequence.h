/*
 * Sequence.h
 *
 * Header file for Sequence class
 *
 *      Creator: Lev A Neiman - lev.neiman@gmail.com
 */

#ifndef _LEV_SEQUENCE_H
#define _LEV_SEQUENCE_H

#include <string>
#include <map>
#include <vector>

class SequenceFile;
class SequenceFilters;

/**
 * @class Sequence
 * @brief Genetic sequence
 *
 * Sequence class represents a sequence of genetic data.
 *
 * @author Lev A Neiman - lev.neiman@gmail.com
 */
class Sequence
{
		friend class SequenceFile;
		friend class SequenceFilters;

	private:
		/**
		 * internal counter for enumerating each sequence.
		 */
		static int TOTAL_SEQ;
		static const unsigned int NO_PARENT_ID = (unsigned int) ((long long) 1 << 32) - 1;

		/**
		 * Use this static method to allocate new Sequences.
		 */
		/*
		 static Sequence * createSequence( const std::string * text, unsigned int offset, unsigned int body_length )
		 {
		 Sequence * new_sequence = new Sequence( text, offset, body_length );
		 //all_sequences[new_sequence->getID()] = new_sequence;
		 all_sequences.push_back( new_sequence );
		 return new_sequence;
		 }
		 */

		/**
		 * stores all Sequences created by their ids.
		 */
		//static std::vector<Sequence*> all_sequences;

	public:

		/**
		 * Retrieve pointer to sequence by its id.
		 * @param id of the sequence to retrieve.
		 * @return pointer to const Sequence.
		 */
		/*
		 static const Sequence * getSequenceByID( unsigned int id )
		 {
		 return all_sequences[ id ];
		 }
		 */

		/**
		 * Inserts '$' char at the end of each sequence.  Modifies subsequent sequences's offset.
		 *
		 * This is sort of a dirty hack right now and this will mess up offsets in sequences not included in the vector passed!
		 */
		static void appendTerminatingCharacterToSequences( std::vector<Sequence*> & sequences );

		/**
		 * get ascii character
		 * @param index into sequence array.
		 */
		const char operator[]( unsigned int index ) const
		{
			return (*text)[ offset + index ];
		}

		bool operator <( const Sequence & seq ) const
		{
			return getOffset() < seq.getOffset();
		}

		bool operator ==( const Sequence & seq ) const
		{
			return this->text == seq.text && this->offset == seq.offset && this->body_length == seq.body_length;
		}

		/**
		 * Get the offset which is the position in the FASTA file where this sequence begins.
		 */
		unsigned int getOffset() const
		{
			return offset;
		}

		/**
		 * Get length of this sequence.
		 */
		unsigned int getBodyLength() const
		{
			return body_length;
		}

		/**
		 * Get header of this sequence.  If it was filtered then the header return is of its FASTA parent.
		 */
		/*
		 const std::string getHeader() const
		 {
		 unsigned int fid = getFASTAID();
		 unsigned int start, size;
		 if( fid == 0 )
		 start = 0;
		 else
		 start = all_sequences[ fid - 1 ]->getOffset() + all_sequences[ fid - 1 ]->getBodyLength();
		 size = all_sequences[ fid ]->getOffset() - start;
		 return text->substr( start, size );
		 }
		 */

		/**
		 * Return a pointer to const string which is the entire FASTA file in memory.
		 */
		const std::string * getText() const
		{
			return text;
		}

		// ljn added 3/4/2010
		/*const std::string * getSeqText() const
		{
			return (*text)[ offset ];
		}*/

		/**
		 * Get id of the parent.
		 */
		/*
		 unsigned int getParent() const
		 {
		 return parent_id;
		 }
		 */

		/**
		 * get whether or not this is a top Sequence.  That is Sequence in FASTA file.
		 */
		/*
		 bool hasParent() const
		 {
		 return parent_id != Sequence::NO_PARENT_ID;
		 }
		 */

		/**
		 * get unique ID of this sequence.
		 */
		unsigned int getID() const
		{
			return id;
		}

		/**
		 * get ID of the FASTA sequence from which this sequence was derived.  If called on FASTA sequence then its own id will be returned.
		 */
		unsigned int getFASTAID() const
		{
			/*
			 if( !hasParent() )
			 return getID();
			 return all_sequences[ getParent() ]->getFASTAID();
			 */
			return this->FASTA_id;
		}

		bool isFASTA() const
		{
			return getFASTAID() == getID();
		}

		/**
		 * return subsequence at given index in this sequence and of given length.
		 */
		Sequence subSequence(unsigned int index, unsigned int length )
		{
			return Sequence( this->text, offset+index, length, getFASTAID() );
		}

		/**
		 * @return brief summary of this sequence.
		 */
		std::string toString() const;

	protected:
		/**
		 * Default constructor.
		 *
		 * @param header - header for this sequence.
		 * @param text - string that contains entire source file.
		 * @param offset - offset of this sequence's data in the source file.
		 * @param body_length - number of characters in this sequence.
		 */
		Sequence( const std::string * text, unsigned int offset, unsigned int body_length ) :
			text( text ), offset( offset ), body_length( body_length )
		{
			//parent_id = Sequence::NO_PARENT_ID;
			FASTA_id = Sequence::NO_PARENT_ID;
			id = Sequence::TOTAL_SEQ++;
		}

		Sequence( const std::string * text, unsigned int offset, unsigned int body_length, unsigned int ID ):
			text( text ), offset( offset ), body_length( body_length ), id(ID)
		{
			FASTA_id = ID;
		}


		const std::string * text;
		unsigned int offset;
		//std::string header;
		unsigned int body_length;

		unsigned int id;
		unsigned int FASTA_id;
		//unsigned int parent_id;

};

#endif
