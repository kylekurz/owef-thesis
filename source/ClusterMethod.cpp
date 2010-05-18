/*
 **********************************************
 // File:	Cluster_method.cpp
 //
 // Author:	Kyle Kurz
 //
 // Date:	4.22.09
 //
 // Purpose:	Define a class for use
 //		in clustering in the Open
 //		Word Enumeration Framework.
 **********************************************
 Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

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

#include "ClusterMethod.h"

using namespace std;

const int A_IND = 0; //Index of row in pwm where A counts are stored
const int C_IND = 1;
const int G_IND = 2;
const int T_IND = 3;
const int ALPHA_SIZE = 4; //Number of characters in alphabet

//standard constructor
Cluster::Cluster()
{
	//no-op
}

//standard destructor
Cluster::~Cluster()
{
	//no-op
}

void Cluster::createLogos( DataStructure *structure, WordScoring *model )
{
	cout << "Creating motif logos..." << endl << endl;
	stringstream stream, stream2, stream3, stream4;
	stream << list->prefix << "_" << list->maxlength << "_" << list->order << "_clusters";
	stream2 << list->prefix << "_" << list->maxlength << "_" << list->order << "_clusters";
	stream3 << list->prefix << "_" << list->maxlength << "_" << list->order << "_clusters";
	stream4 << list->prefix << "_" << list->maxlength << "_" << list->order << "_clusterscores";
#ifdef KKURZ_MPI
	stream << "_" << list->pref;
#endif
	stream << ".csv";
	stream2 << ".csv";
	stream3 << ".xml";
	stream4 << ".csv";
	
	string filename, filename2, filename3, filename4;
	stream >> filename;
	stream2 >> filename2;
	stream3 >> filename3;
	stream4 >> filename4;
	
#ifdef DEBUG
	printf("output file: %s\n", filename2.c_str());
#endif
	ofstream log( list->log_file.c_str(), ios::app );
	log << "Cluster file: " << filename2 << endl;
	log.close();
	ofstream ofile( filename.c_str() );
	ofstream CSVSCORE( filename4.c_str() );
	ofstream XML( filename3.c_str() );

	CSVSCORE << "Seed Word, Probability Score," << endl;
	/*ofile << "#Word,S,E_s,O,E,O/E,O*ln(O/E),S*ln(S/E_s)";
	if( list->revcomp )
		ofile << ",RevComp,Present,Palindrome";
	if( list->pval )
		ofile << ",Pval";
	ofile << endl << endl;*/
	Scores *word = new Scores;
	for( int i = 0; i < static_cast<int> ( clusters.size() ); i++ )
	{
		ofile << "Cluster " << clusters[i][0] << endl;
		CSVSCORE << clusters[i][0] << ",";
		XML << "<Cluster id=\"" << clusters[i][0] << "\">" << endl;
		XML << "\t<Seed>" << clusters[i][0] << "</Seed>" << endl;
		//stuff for motif logos
		double cluster_score = 0;
		cluster_score = scoreCluster( clusters[ i ], structure, model );
		vector<vector<float> > pwm;
		pwm = computePwm( clusters[ i ], structure );
		if( list->regex )
			outputRegExp( ofile, pwm );
		if( list->pwm )
			outputPwm( ofile, pwm );

		ofile << "Cluster Score: " << cluster_score << endl;
		XML << "\t<ProbabilityScore>" << cluster_score << "</ProbabilityScore>" << endl;
		CSVSCORE << cluster_score << ",";
		
		//create motif logo
		if( list->logos )
		{
			string logo_name( "motif" );
			stringstream logo_stream;
			logo_stream << list->prefix << "_" << logo_name << "_" << clusters[i][0] << ".png";
			logo_stream >> logo_name;
			createMotifLogo( pwm, logo_name );
		}

		for( int j = 0; j < static_cast<int> ( clusters[ i ].size() ); j++ )
		{
			model->computeScores( word, clusters[ i ][ j ], structure, list->order );
			//output the word and it's Scores
			string converted = clusters[ i ][ j ];
			ofile << converted;
			ofile << "," << word->seqs << "," << word->E_s << "," << word->count << "," << word->E << "," << word->OE << "," << word->Oln << "," << word->Sln;
			if( list->revcomp )
			{
				ofile << "," << word->revcomp;
				if( word->revcomp_pres )
					ofile << "," << "Yes";
				else
					ofile << "," << "No";
				if( word->palindrome )
					ofile << "," << "Yes";
				else
					ofile << "," << "No";
			}
			if( list->pval )
			{
				ofile << "," << word->pval;
			}
			ofile << endl;
		}
		ofile << endl;
		XML << "</Cluster>" << endl;
		CSVSCORE << endl;
	}
	ofile.close();
	XML.close();
	CSVSCORE.close();

	return;
}

double Cluster::scoreCluster( vector<string> cluster, DataStructure *structure, WordScoring *model )
{
	double score = 0.0;

	return score;
}

void Cluster::outputRegExp( ostream & out_file, const vector<vector<float> > & pwm )
{
	string letters;
	for( int j = 0; j < static_cast<int> ( pwm[ 0 ].size() ); ++j )
	{
		letters.clear();
		if( pwm[ A_IND ][ j ] > 0 )
			letters += "A";
		if( pwm[ C_IND ][ j ] > 0 )
			letters += "C";
		if( pwm[ G_IND ][ j ] > 0 )
			letters += "G";
		if( pwm[ T_IND ][ j ] > 0 )
			letters += "T";

		if( letters.length() > 1 )
		{
			out_file << "[" << letters << "]";
		}
		else
		{
			out_file << letters;
		}
	}
	out_file << endl;
}

void Cluster::outputPwm( ostream & out_file, const vector<vector<float> > & pwm )
{
	for( int i = 0; i < ALPHA_SIZE; ++i )
	{
		//out_file << "-";
		for( int j = 0; j < static_cast<int> ( pwm[ i ].size() ); ++j )
		{
			out_file << pwm[ i ][ j ];
			if( j < static_cast<int> ( pwm[ i ].size() ) - 1 )
			{
				out_file << ',';
			}
		}
		out_file << endl;
	}
	out_file << endl;
}

vector<vector<float> > Cluster::computePwm( vector<string> cluster_data, DataStructure *structure )
{

#ifdef KKURZ_MPI
	MPI_Status status;
	int rc, rec;
#endif
	vector<vector<float> > pwm;
	for( int i = 0; i < ALPHA_SIZE; ++i )
	{
		pwm.push_back( vector<float> ( cluster_data[ 0 ].length() ) );
	}

	string word;

	for( int i = 0; i < static_cast<int> ( cluster_data.size() ); ++i )
	{
		word = cluster_data[ i ];

		for( int j = 0; j < static_cast<int> ( word.length() ); ++j )
		{
			switch( word[ j ] )
			{
				case 'A':
#ifdef KKURZ_MPI
					rc = MPI_Send(&cluster_data[i][0], cluster_data[i].length(), MPI_CHAR, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD);
					rc = MPI_Recv(&rec, 1, MPI_INT, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD, &status);
					pwm[A_IND][j] += rec;
#else
					pwm[ A_IND ][ j ] += structure->getCount( cluster_data[ i ] );
#endif
					break;
				case 'C':
#ifdef KKURZ_MPI
					rc = MPI_Send(&cluster_data[i][0], cluster_data[i].length(), MPI_CHAR, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD);
					rc = MPI_Recv(&rec, 1, MPI_INT, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD, &status);
					pwm[C_IND][j] += rec;
#else
					pwm[ C_IND ][ j ] += structure->getCount( cluster_data[ i ] );
#endif
					break;
				case 'G':
#ifdef KKURZ_MPI
					rc = MPI_Send(&cluster_data[i][0], cluster_data[i].length(), MPI_CHAR, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD);
					rc = MPI_Recv(&rec, 1, MPI_INT, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD, &status);
					pwm[G_IND][j] += rec;
#else
					pwm[ G_IND ][ j ] += structure->getCount( cluster_data[ i ] );
#endif
					break;
				case 'T':
#ifdef KKURZ_MPI
					rc = MPI_Send(&cluster_data[i][0], cluster_data[i].length(), MPI_CHAR, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD);
					rc = MPI_Recv(&rec, 1, MPI_INT, list->hosts.host_array[branch_array[cluster_data[i][0] - 'A']][0], MPI_COUNT_TAG, MPI_COMM_WORLD, &status);
					pwm[T_IND][j] += rec;
#else
					pwm[ T_IND ][ j ] += structure->getCount( cluster_data[ i ] );
#endif
					break;
				default:
					cout << "Character other than <ACGT> in function compute_pwm! Ignoring character!\n";
			}
		}
	}

	float col_sum = 0; //sum of values down any column of pwm

	for( int i = 0; i < ALPHA_SIZE; ++i )
	{
		col_sum += pwm[ i ][ 0 ];
	}

	for( int i = 0; i < static_cast<int> ( pwm[ 0 ].size() ); ++i )
	{
		for( int j = 0; j < ALPHA_SIZE; ++j )
		{
			pwm[ j ][ i ] /= col_sum;
		}
	}
	return pwm;
}

void Cluster::createMotifLogo( const vector<vector<float> > & pwm, const string & filename )
{
	string cmd;
	stringstream stream;
	stream << '[';

	for( int i = 0; i < static_cast<int> ( pwm.size() ); ++i )
	{
		stream << '[';
		for( int j = 0; j < static_cast<int> ( pwm[ i ].size() ) - 1; ++j )
		{
			stream << pwm[ i ][ j ] << ',';
		}
		stream << pwm[ i ][ pwm[ i ].size() - 1 ] << ']' << ',';
	}

	int system_return = 0;
	stream >> cmd;
	cmd[ cmd.length() - 1 ] = ']';
	cmd.insert( 0, "perl ./TFBS.pl " );
	cmd += " ";
	cmd += filename;
	cmd += " 2>> " + list->prefix + "_TFBS_log_" + list->pref + ".txt";

	system_return = system( cmd.c_str() ); //invoke perl module to create logo
	//cout<<"system retun:"<<system_return<<"cmd:"<<cmd<<endl;
}
