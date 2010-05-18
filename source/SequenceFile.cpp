#include "SequenceFile.h"
#include <sys/stat.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>

bool isHeader( std::string & line )
{
	if( line.size() > 0 && (line[0] == '>' || line[0] == ';') )
		return true;
	return false;
}

SequenceFile::SequenceFile( const char * file_name )
{
	using namespace std;
	this->file_name = file_name;

	ifstream ifs( file_name );
	if( !ifs.good() )
	{
		cerr << "Couldnt open file " << file_name << endl;
		return;
	}

	readStream( ifs );

	ifs.close();
}

SequenceFile::SequenceFile(std::istream & ifs )
{
	this->file_name="no name";
	readStream(ifs);
}

void SequenceFile::readStream( std::istream & ifs )
{
	using namespace std;
	Sequence * curr_sequence = NULL;
	unsigned int curr_pos = 0;
	string line;

	unsigned int fasta_id = 0;

	while( getline( ifs, line ) )
	{
		curr_pos = text.size();
		if( isHeader( line ) )
		{
			if( curr_sequence != NULL )
			{
				curr_sequence->body_length = curr_pos - curr_sequence->offset;
				sequences.push_back( curr_sequence );
			}
			curr_sequence = new Sequence( &text, curr_pos + line.size(), -1 );
			//curr_sequence->FASTA_id = curr_sequence->id;
			curr_sequence->FASTA_id = fasta_id++;
		}
		text += line;
	}

	if( curr_sequence == NULL )
	{
		cerr << "Whoops something went wrong in reading the FASTA file " << file_name << " :(" << endl;
	}
	else
	{
		curr_sequence->body_length = text.size() - curr_sequence->offset;
		sequences.push_back( curr_sequence );
	}

}

std::string SequenceFile::toString() const
{
	using namespace std;
	stringstream s;
	s << "File name: " << this->file_name << endl;
	s << "Number of sequences: " << sequences.size() << endl;
	for( unsigned int i = 0; i < sequences.size(); ++i )
	{
		s << "=================" << endl << sequences[i]->toString() << endl;
	}

	return s.str();
}
