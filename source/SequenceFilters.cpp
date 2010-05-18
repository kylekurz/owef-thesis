#include "SequenceFilters.h"
#include "Utilities.h"

std::vector<Sequence*> SequenceFilters::filter1xM( const Sequence & sequence, bool(*delimeterFunc)( char ) )
{
	using namespace std;
	vector<Sequence*> sequences;
	const Sequence * s_seq = &sequence;
	Sequence * curr_seq = NULL;
	unsigned int k = 0;
	bool N_DAYS = false;
	for( unsigned int j = 0; j < s_seq->getBodyLength(); ++j )
	{
		if( delimeterFunc( (*s_seq)[j] ) )
		{
			if( curr_seq != NULL && !N_DAYS )
			{
				curr_seq->body_length = j - k;
				sequences.push_back( curr_seq );
			}
			k = j;
			N_DAYS = true;
			continue;
		}

		if( curr_seq == NULL || N_DAYS )
		{
			if( N_DAYS )
			{
				++k;
				N_DAYS = false;
			}
			curr_seq = new Sequence( s_seq->getText(), k + s_seq->getOffset(), 0 );
			curr_seq->FASTA_id = sequence.FASTA_id;
		}
	}
	if( !N_DAYS && curr_seq != NULL )
	{
		curr_seq->body_length = s_seq->getBodyLength() - k;
		sequences.push_back( curr_seq );
	}
	return sequences;
}

std::vector<Sequence*> SequenceFilters::filterNxM( const std::vector<Sequence*> & sequences, bool(*delimeterFunc)( char ) )
{
	using namespace std;
	vector<Sequence*> ret;
	for( unsigned int i = 0; i < sequences.size(); ++i )
	{
		vector<Sequence*> t = SequenceFilters::filter1xM( *(sequences[i]), delimeterFunc );
		append_vector( ret, t );
	}
	return ret;
}

unsigned int SequenceFilters::countFASTASequences( const std::vector<Sequence*> & v )
{
	using namespace std;
	set<unsigned int> fasta_sequences;
	for( unsigned int i = 0; i < v.size(); ++i )
		fasta_sequences.insert( v[i]->getFASTAID() );
	return fasta_sequences.size();
}
