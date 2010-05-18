#include "Sequence.h"
#include <sstream>
#include <algorithm>

using namespace std;

int Sequence::TOTAL_SEQ = 0;

std::string Sequence::toString() const
{
	using namespace std;
	stringstream s;
	//s << "Sequence header: " << getHeader() << endl;
	s << "id: " << getID() << endl;
	s<<"FASTA id:" << getFASTAID()<<endl;
	s << "Number of characters: " << body_length << endl;
	s << "Offset in file: " << getOffset() << endl;
	//s<<"Pointer to text: "<<text<<endl;
	//s<<"Text size: "<<text->size()<<endl;
	for( unsigned int i = 0; i < getBodyLength(); ++i )
	{
		s << (*this)[ i ];
	}

	return s.str();
}

//std::vector<Sequence*> Sequence::all_sequences;

void Sequence::appendTerminatingCharacterToSequences( vector<Sequence*> & sequences )
{
	if( sequences.size() == 0 )
		return;
	//sort( sequences.begin(), sequences.end() );
	std::string * text = const_cast<string*> ( sequences[ 0 ]->text );

	for( unsigned int i = 0; i < sequences.size(); ++i )
	{
		Sequence * curr_sequence = sequences[ i ];
		text->insert( curr_sequence->getOffset() + curr_sequence->getBodyLength(), "$" );
		curr_sequence->body_length++;
		for( unsigned int j = i + 1; j < sequences.size(); ++j )
		{
			sequences[j]->offset += 1;
		}
	}
}
