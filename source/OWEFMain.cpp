/*
 **********************************************
 // File:	OWEF_Main.cpp
 //
 // Author:	Kyle Kurz
 //
 // Date:	2.20.09
 //
 // Purpose:	Define a main function for use
 //		in the Open Word
 //		Enumeration Framework.
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
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <sstream>
#include <string.h>
#include <vector>

#include "OWEF.h"

using namespace std;

#define EXIT_SUCCESS 0

int main( int argc, char *argv[] )
{
	map<string, pair<int, int> > tester;
	//counting stage variables
	bool count;
	string structure_type = "rt";
	string seq_file = "";
	int word_length = 0;
	int min_length = 0;
	int min_seqs = 1;
	int min_O = 1;
	bool ancestral_filter = 0;
	bool enumerate = 0;
	bool missing = 0;
	bool N_filter = 0;
	bool record_seqs = 0;

	//scoring stage variables
	bool score = 0;
	string model_type = "mm";
	int order = 0;
	bool revcomp = 0;
	bool pval = 0;
	bool pthr = 0;
	double pthresh = 0.05;

	//clustering stage variables
	string cluster_type = "cm";
	bool cluster = 0;
	int seeds = 25;
	string sort;
	string type;
	int distance = 1;
	bool motif_score = 0;
	bool pwm = 0;
	bool logos = 0;
	bool regex = 0;

	//CoOccurrence stage variables
	bool cooccur = 0;
	bool map = 0;
	string mout;
	bool cdist = 0;
	string dout;
	int group_size = 2;
	bool self = 0;
	bool upper_lower = 0;

	//word distribution stage variables
	bool word_distribution = 0;
	int dist_count = 5;
	bool normalize = 0;

	//scatter plot variables
	bool scatter = 0;

	//sequence clustering variables
	bool sequence_clustering = 0;

	//conservation anaylsis variables

	//module discovery variables
	bool discovery = 0;
	bool fixed = 0;
	int dimensions = 0;
	bool self_compare = 0;
	bool distributed = 0;
	bool preserve_order = 0;
	bool density = 0;
	bool overlap_words = 0;
	
	//word family stage variables
	int no_n = 0;

	//visualization variables
	bool visualization = 0;
	bool cgr = 0;

	//job variables
	string directory = "";
	string prefix = "";
	bool parallel = 0;

	//if we don't get command line options, prompt for user input
	if( argc == 1 )
	{
		cout << "Enter a job ID: ";
		cin >> prefix;
		cout << "Perform counting? (enter 0 for no, 1 for yes): ";
		cin >> count;
		if( count )
		{
			cout << "Data structure type: ";
			cin >> structure_type;
			cout << "Sequence file name: ";
			cin >> seq_file;
			cout << "Word length: ";
			cin >> word_length;
			cout << "Minimun word length: ";
			cin >> min_length;
			cout << "Minimum number of sequence occurrences: ";
			cin >> min_seqs;
			cout << "Minimum number of occurrences: ";
			cin >> min_O;
			cout << "Ancestral filter (enter 0 for no, 1 for yes): ";
			cin >> ancestral_filter;
			cout << "N filter (enter 0 for no, 1 for yes): ";
			cin >> N_filter;
			cout << "List missing words? (enter 0 for no, 1 for yes): ";
			cin >> missing;
			cout << "Enumerate missing words? (enter 0 for no, 1 for yes): ";
			cin >> enumerate;
			cout << "Record sets of sequences for word occurrence? (enter 0 for no, 1 for yes): ";
			cin >> record_seqs;
		}
		cout << "Perform scoring? (enter 0 for no, 1 for yes): ";
		cin >> score;
		if( score )
		{
			//****************************************************
			//Developers, add parameters for your algorithms here.
			//****************************************************
		}
		cout << "Perform clustering? (enter 0 for no, 1 for yes): ";
		cin >> cluster;
		if( cluster )
		{
			//****************************************************
			//Developers, add parameters for your algorithms here.
			//****************************************************
		}
		
		cout << "Perform word distribution analysis? (enter 0 for no, 1 for yes): ";
		cin >> word_distribution;
		if( word_distribution )
		{
			cout << "Number of words to analyze: ";
			cin >> dist_count;
			cout << "Normalize distributions? (enter 0 for no, 1 for yes): ";
			cin >> normalize;
		}
		
		cout << "Perform sequence clustering? (enter 0 for no, 1 for yes): ";
		cin >> sequence_clustering;
		cout << "Perform visualizations? (enter 0 for no, 1 for yes): ";
		cin >> visualization;
		if(visualization)
		{
			//****************************************************
			//Developers, add parameters for your algorithms here.
			//****************************************************
			cout << "Create CGR for job? (enter 0 for no, 1 for yes): ";
			cin >> cgr;
		}
	}
	//otherwise read the options and set flags
	else
	{
		vector<string> flags;
		flags.push_back( "--count" );
		flags.push_back( "-i" );
		flags.push_back( "-l" );
		flags.push_back( "-ml" );
		flags.push_back( "-ms" );
		flags.push_back( "-mo" );
		flags.push_back( "-a" );
		flags.push_back( "-n" );
		flags.push_back( "-m" );
		flags.push_back( "-e" );
		flags.push_back( "-rs" );
		flags.push_back( "--score" );
		flags.push_back( "-o" );
		flags.push_back( "-r" );
		flags.push_back( "-p" );
		flags.push_back( "-pt" );
		flags.push_back( "--cluster" );
		flags.push_back( "-s" );
		flags.push_back( "-c" );
		flags.push_back( "-t" );
		flags.push_back( "-d" );
		flags.push_back( "-sm" );
		flags.push_back( "-pwm" );
		flags.push_back( "-logos" );
		flags.push_back( "-regex" );
		flags.push_back( "--scatter" );
		flags.push_back( "--distribution" );
		flags.push_back( "-dc" );
		flags.push_back( "-dn" );
		flags.push_back( "--cooccurrence" );
		flags.push_back( "-cm" );
		flags.push_back( "-cd" );
		flags.push_back( "-cn" );
		flags.push_back( "-cc" );
		flags.push_back( "-cs" );
		flags.push_back( "--sequence" );
		flags.push_back( "--modules" );
		flags.push_back( "-mef" );
		flags.push_back( "-med" );
		flags.push_back( "-mes" );
		flags.push_back( "-medd" );
		flags.push_back( "-mepo" );
		flags.push_back( "-mede" );
		flags.push_back( "-meow" );
		flags.push_back( "--prefix" );
		flags.push_back( "--parallel" );
		flags.push_back( "-st" );
		flags.push_back( "-mt" );
		flags.push_back( "-ct" );
		flags.push_back( "-no" );
		flags.push_back( "--visualization" );
		flags.push_back( "-cgr");
		flags.push_back( "-h" );

		for( int i = 1; i < argc; i++ )
		{
			int loc = -1;
			for( int j = 0; j < static_cast<int> ( flags.size() ); j++ )
			{
				if( strcmp( flags[ j ].c_str(), argv[ i ] ) == 0 )
					loc = j;
			}
			switch( loc )
			{
				case 0:
					count = true;
					break;
				case 1:
					seq_file = argv[ ++i ];
					break;
				case 2:
					word_length = atoi( argv[ ++i ] );
					break;
				case 3:
					min_length = atoi( argv[ ++i ] );
					break;
				case 4:
					min_seqs = atoi( argv[ ++i ] );
					break;
				case 5:
					min_O = atoi( argv[ ++i ] );
					break;
				case 6:
					ancestral_filter = true;
					break;
				case 7:
					N_filter = true;
					break;
				case 8:
					missing = true;
					break;
				case 9:
					enumerate = true;
					break;
				case 10:
					record_seqs = true;
					break;
				case 11:
					score = true;
					break;
				case 12:
					order = atoi( argv[ ++i ] );
					break;
				case 13:
					revcomp = true;
					break;
				case 14:
					pval = true;
					break;
				case 15:
					pthr = true;
					pthresh = atof( argv[ ++i ] );
					break;
				case 16:
					cluster = true;
					break;
				case 17:
					seeds = atoi( argv[ ++i ] );
					break;
				case 18:
					sort = argv[ ++i ];
					break;
				case 19:
					type = argv[ ++i ];
					break;
				case 20:
					distance = atoi( argv[ ++i ] );
					break;
				case 21:
					motif_score = true;
					break;
				case 22:
					pwm = true;
					break;
				case 23:
					logos = true;
					break;
				case 24:
					regex = true;
					break;
				case 25:
					scatter = true;
					break;
				case 26:
					word_distribution = true;
					break;
				case 27:
					dist_count = atoi( argv[ ++i ] );
					break;
				case 28:
					normalize = true;
					break;
				case 29:
					cooccur = true;
					break;
				case 30:
					map = true;
					mout = argv[ ++i ];
					break;
				case 31:
					cdist = true;
					dout = argv[ ++i ];
					break;
				case 32:
					group_size = atoi( argv[ ++i ] );
					break;
				case 33:
					self = true;
					break;
				case 34:
					upper_lower = true;
					break;
				case 35:
					sequence_clustering = true;
					break;
				case 36:
					discovery = true;
					break;
				case 37:
					fixed = true;
					break;
				case 38:
					dimensions = atoi( argv[ ++i ] );
					break;
				case 39:
					self_compare = true;
					break;
				case 40:
					distributed = true;
					break;
				case 41:
					preserve_order = true;
					break;
				case 42:
					density = true;
					break;
				case 43:
					overlap_words = true;
					break;
				case 44:
					prefix = argv[ ++i ];
					break;
				case 45:
					parallel = true;
					break;
				case 46:
					structure_type = argv[ ++i ];
					break;
				case 47:
					model_type = argv[ ++i ];
					break;
				case 48:
					cluster_type = argv[ ++i ];
					break;
				case 49:
					no_n = atoi(argv[ ++i ]);
					break;
				case 50:
					visualization = true;
					break;
				case 51:
					cgr = true;
					break;
				case 52:
					cout << endl
							<< "Command options format: \n./OWEFexec [--count -i sequence_file -l word_length -ml minimum_length -ms minimum_sequences -mo minimum_occurrences <-a> <-n> <-m> <-e>] [--score] [--cluster] [--scatter] [--distribution <-dc number> <-dn>] [--prefix JobID]"
							<< endl;
					cout << endl << "Counting stage options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --count : perform the counting stage" << endl;
					cout << "	-i filename (-i indicates input file, filename is the name of the file on your system)" << endl;
					cout << "	-l wordlength (-l indicates maximum word length, wordlength is an integer)" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "	-ml mimimum_length (-ml indicates the minimum wordlength (range), minimum_length is an integer)" << endl;
					cout << "	-ms minimum_sequences (-ms indicates minimum number of sequences, minimum_sequences is an integer)" << endl;
					cout << "	-mo minimum_occurrences (-mo indicates minimum_number of occurrences, minimum_occurrences is an integer)" << endl;
					cout << "	-a (-a indicates that the input is marked for ancestral repeats and should be filtered accordingly)" << endl;
					cout << "	-n (-n indicates that arbitrary nucleotides be filtered from the input)" << endl;
					cout << "	-m (-m causes the framework to list out words NOT found in the input file)" << endl;
					cout << "	-e (-e fully enumerates any missing word listed)" << endl;
					cout << endl << "Scoring stage options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --score : perform the scoring stage" << endl;
					cout << endl << "Clustering stage options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --cluster : perform the clustering stage" << endl;
					cout << endl << "Scatter plot options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --scatter" << endl;
					cout << endl << "Word distribution options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --distribution" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "	-dc number (-dc indicates the number of top words used for computing distributions, number is an integer)" << endl;
					cout << "	-dn (-dc normalizes the word distributions to a percentage through the sequences)" << endl;
					cout << endl << "Sequence clustering options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --sequence" << endl;
					cout << endl << "Visualization options:" << endl;
					cout << "    --visualization" << endl;
					cout << "	-cgr (creates a CGR for the input file)" << endl;
					cout << endl << "General job options:" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "    --prefix JobID (--prefix places a prefix in front of every file created during the job, JobID is a string)" << endl << endl;
					cout << "    --parallel (run the job with parallelizations active)" << endl << endl;
					exit( 0 );
					break;
				default:
					cout
							<< "Command options format: \n./OWEFexec [--count -i sequence_file -l word_length -ml minimum_length -ms minimum_sequences -mo minimum_occurrences <-a> <-n> <-m> <-e> <-rs> <-st structure_type>] [--score -o markov_order <-r> <-p> <-pt threshold>] [--cluster <-s #_of_clusters> <-c sort_column> <-t cluster_type> <-d distance> <-sm> <-pwm> <-logos> <-regex>] [--scatter] [--cooccurrence <-cm map_name> <-cd dist_name> <-cn number> <-cc> <-cs>] [--sequence] [--modules <-mef> <-med dimensions> <-mes> <-medd> <-mepo> <-mede> <-meow>][--visualization <-cgr>][--prefix JobID] [--parallel]"
							<< endl;
					exit( -1 );
					break;
			}
		}
	}
	//set our prefix up to be the input filename without the extension (if we don't get a prefix on the command line)
	if( prefix == "" )
	{
		prefix = seq_file;
		if( prefix.find( "." ) != string::npos )
			prefix.erase( prefix.find( "." ), prefix.length() - prefix.find( "." ) );
	}

	if( min_length == 0 )
		min_length = word_length;

	//set working directory to be the same as the prefix (or job id)
	directory = prefix;
	string job_log = prefix + "_output_files.txt";

	//*******************************************************************
	//create a radix trie based on all the options we just set up
	//developers should replace this with a constructor to the dataset
	//they are implementing
	//*******************************************************************
	OWEFArgs *from_input;
	from_input = new OWEFArgs( count, structure_type, seq_file, word_length, min_length, min_seqs, min_O, ancestral_filter, N_filter, missing, enumerate, record_seqs, score, model_type, order, revcomp, pval, pthr, pthresh, cluster, seeds, sort[ 0 ], type[ 0 ], distance, motif_score, pwm, logos, regex, word_distribution, dist_count, normalize, cooccur, map, mout, cdist, dout, group_size, self, upper_lower, sequence_clustering, discovery, fixed, dimensions, self_compare, distributed, preserve_order, density, overlap_words, cluster_type, no_n, visualization, cgr, prefix, parallel, job_log );

cout << "Starting MPI" << endl;

#ifdef KKURZ_MPI

	//*******************************************************************
	//initialize the mpi code to create the proper communication 
	//capabilities
	//*******************************************************************
	cout << "setting up mpi" << endl;
	int rc = MPI_Init(&argc, &argv);
	if(rc != MPI_SUCCESS)
	{
		printf("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Status status;
	int req, rec;
	char hostname[HOSTNAMELEN];
	int recv_array[NUMHOSTS*NUMPERHOST];
	if (rc != MPI_SUCCESS)
	{
		printf ("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}

	MPI_Comm_size(MPI_COMM_WORLD,&from_input->numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&from_input->rank);

	if(from_input->rank == 0)
	{
		std::map<std::string, pair<int, int> > host_map;
		MPI_Status status;
		string offset;
		req = 1;
		gethostname(hostname, HOSTNAMELEN);
		host_map[hostname] = make_pair(from_input->rank, -1);

#ifdef DEBUG
		printf ("hostname: %s rank: %d\n", hostname, from_input->rank);
		cout << hostname << " " << host_map[hostname].first << " " << host_map[hostname].second << endl;
#endif

		for(int dest=1; dest<from_input->numtasks; dest++)
		{
			int rc = MPI_Send(&req,1, MPI_INT, dest, 1, MPI_COMM_WORLD);
			rc = MPI_Recv(&hostname, HOSTNAMELEN, MPI_CHAR, dest, 1, MPI_COMM_WORLD, &status);
			pair<int, int> temp = host_map[hostname];
			if(temp.first == 0 && temp.second == 0)
			{
				host_map[hostname] = make_pair(dest, -1);
			}
			else if(temp.first == -1)
			{
				temp.first = dest;
				if(temp.first > temp.second)
				{
					int t = temp.first;
					temp.first = temp.second;
					temp.second = t;
				}
				host_map[hostname] = temp;
			}
			else if(temp.second == -1)
			{
				temp.second = dest;
				if(temp.first > temp.second)
				{
					int t = temp.first;
					temp.first = temp.second;
					temp.second = t;
				}
				host_map[hostname] = temp;
			}

#ifdef DEBUG
			printf("hostname: %s rank: %d\n", hostname, dest);
			cout << hostname << " " << host_map[hostname].first << " " << host_map[hostname].second << endl;
#endif
		}
		std::map<string, pair<int,int> >::iterator it;
		it = host_map.begin();
		pair<int, int> lowest = make_pair(INT_MAX, INT_MAX);
		for(int i=0; i<NUMHOSTS; i++)
		{
			std::map<string, pair<int,int> >::iterator it;
			std::map<string, pair<int,int> >::iterator to_erase;
			it = host_map.begin();
			pair<int, int> lowest = make_pair(INT_MAX, INT_MAX);
			while(it != host_map.end())
			{
				if((*it).second.first < lowest.first)
				{
					lowest.first = (*it).second.first;
					lowest.second = (*it).second.second;
					to_erase = it;
				}
				it++;
			}
			from_input->hosts.host_array[i][0] = lowest.first;
			from_input->hosts.host_array[i][1] = lowest.second;
			host_map.erase(to_erase);
		}

		//print the stuff if we wanna see it
#ifdef DEBUG
		for(int i=0; i<NUMHOSTS; i++)
		{
			for(int j=0; j<NUMPERHOST; j++)
			{
				cout << from_input->hosts.host_array[i][j] << " ";
			}
			cout << endl;
		}
#endif

		//send the information as one big long array
		int trans_array[NUMHOSTS*NUMPERHOST];
		for(int i=0; i<NUMHOSTS; i++)
		{
			for(int j=0; j<NUMPERHOST; j++)
			{
				trans_array[2*i+j] = from_input->hosts.host_array[i][j];
			}
		}

		MPI_Datatype columntype;
		MPI_Type_contiguous(NUMHOSTS*NUMPERHOST, MPI_INT, &columntype);
		MPI_Type_commit(&columntype);

		//transmit it to everyone
		if(from_input->rank == 0)
		{
			for (int i=1; i<from_input->numtasks; i++)
			{
				rc = MPI_Send(&trans_array[0], 1, columntype, i, 2, MPI_COMM_WORLD);
			}
		}
		MPI_Type_free(&columntype);
	}
	else
	{
		char to_count[HOSTNAMELEN];

		//get request for hostname
		rc = MPI_Recv(&rec, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		string test = to_count;
		gethostname(hostname, HOSTNAMELEN);

		//send hostname
		rc = MPI_Send(&hostname, HOSTNAMELEN, MPI_CHAR, 0, 1, MPI_COMM_WORLD);

		//get the host map from the master
		rc = MPI_Recv(recv_array, NUMHOSTS*NUMPERHOST, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);

#ifdef DEBUG
		//print the received array
		//for(int i=0; i<NUMHOSTS*NUMPERHOST; i++)
		//	printf("rank: %d i: %d recv_array: %d\n", from_input->rank, i, recv_array[i]);
#endif

		//setup the host map
		for(int i=0; i<NUMHOSTS; i++)
		{
			for(int j=0; j<NUMPERHOST; j++)
			{
				from_input->hosts.host_array[i][j] = recv_array[(2*i)+j];
			}
		}

#ifdef DEBUG
		//print the map to verify it worked
		/*if(from_input->rank == 1)
		 {
		 for(int i=0; i<NUMHOSTS; i++)
		 {
		 for(int j=0; j<NUMPERHOST; j++)
		 {
		 printf("%d ", hosts.host_array[i][j]);
		 }
		 printf("\n");
		 }
		 }*/
#endif
	}

	/*if(from_input->rank == 0)
	 {
	 //tell everyone they can quit
	 req = 0;
	 for(int dest=1; dest<from_input->numtasks; dest++)
	 {
	 rc = MPI_Send(&req, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
	 }
	 MPI_Finalize();
	 }
	 exit(0);
	 */
#endif

	from_input->writeLogs();

	OWEF *job;
	//this is a silly way to do this, but it works... constructor calls all sub-processes
	job = new OWEF( from_input );
	//don't need it once it returns
	delete job;

	//used to issue commands to the system
	string cmd;
	stringstream stream;
	int system_return = 0;

	//if we want scatter plots...
	if( scatter )
	{
		ofstream log( from_input->log_file.c_str(), ios::app );
		log << "Scatter Plot: " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << "_OlnOE.png" << endl;
		log << "Scatter Plot: " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << "_SlnSE.png" << endl;
		log.close();
		//run the scatter plots
		stream.clear();
		stream << "./bin/OlnOE_scatter_plot.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << ".csv " << from_input->prefix << "_" << from_input->maxlength
				<< "_" << from_input->order;
		getline( stream, cmd );
		system_return = system( cmd.c_str() );

		if( from_input->missing && from_input->enumerate )
		{
			stream.clear();
			stream << "./bin/missing_scatter.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << "_missing_scored.csv " << from_input->prefix << "_"
					<< from_input->maxlength << "_" << from_input->order;
			getline( stream, cmd );
			system_return = system( cmd.c_str() );
		}

		stream.clear();
		stream << "./bin/SlnSE_scatter_plot.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << ".csv " << from_input->prefix << "_" << from_input->maxlength
				<< "_" << from_input->order;
		getline( stream, cmd );
		system_return = system( cmd.c_str() );

		if( pval )
		{
			stream.clear();
			stream << "./bin/pval_scatter.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << ".csv " << from_input->prefix << "_" << from_input->maxlength
					<< "_" << from_input->order;
			getline( stream, cmd );
			system_return = system( cmd.c_str() );
		}
	}

#ifdef KKURZ_MPI
	if(from_input->rank == from_input->hosts.host_array[branch_array[from_input->pref - 'A']][0])
	{
#endif

	cout << "Creating results directory...\n\n";
	//Create results directory:
	stream.clear();
	stream << directory;
	if( from_input->count )
		stream << "_" << from_input->maxlength;
	if( from_input->score )
		stream << "_" << from_input->order;
#ifdef KKURZ_MPI
	//stream << "_" << from_input->pref;
#endif
	stream >> directory;

	stream.clear();
	stream << "mkdir " << directory;
	getline( stream, cmd );
	cout << cmd << endl;

	system_return = system( cmd.c_str() );

	//move everything over at once
	stream.clear();
	stream << "mv " << prefix << "*.* ./" << directory << "/";
	getline( stream, cmd );
	system_return = system( cmd.c_str() );

	cout << "Results stored in " << directory << "/\n";
	cout << "\nRun successful!\n";

#ifdef KKURZ_MPI
}

//if we are the scoring thread on the master host, wait for everyone else to send that they are done
if(from_input->rank == from_input->hosts.host_array[0][1])
{
#ifdef DEBUG
	printf("\nwaiting for everyone to finish scoring\n");
#endif
	for(int i=1; i<from_input->numtasks/2; i++)
	{
		int stat;
		rc = MPI_Recv(&stat, 1, MPI_INT, from_input->hosts.host_array[i][1], MPI_ANY_TAG, MPI_COMM_WORLD, &status);

		//test to see if we can figure out what's going on...
		/*char *buf;
		 buf = (char *) malloc ( sizeof(char)*(from_input->maxlength) );
		 sprintf(buf, "%d", from_input->maxlength);
		 printf("buf: %s length: %d to %d\n", buf, from_input->maxlength, from_input->hosts.host_array[i][0]);
		 rc = MPI_Send(&buf, from_input->maxlength, MPI_CHAR, from_input->hosts.host_array[i][0], MPI_NUMWORDS_TAG, MPI_COMM_WORLD);
		 rc = MPI_Recv(&rec, 1, MPI_INT, from_input->hosts.host_array[i][0], MPI_NUMWORDS_TAG, MPI_COMM_WORLD, &status);*/
	}
#ifdef DEBUG
	printf("\nall done\n");
	//then send to all the waiting slave threads so they can quit
	printf("\nkilling the slave threads\n");
#endif
	req = 0;
	for(int i=0; i<from_input->numtasks/2; i++)
	rc = MPI_Send(&req,1, MPI_INT, from_input->hosts.host_array[i][0], MPI_DONE_TAG, MPI_COMM_WORLD);
}
else
rc = MPI_Send(&req,1, MPI_INT, from_input->hosts.host_array[0][1], 1, MPI_COMM_WORLD);

MPI_Barrier(MPI_COMM_WORLD);
MPI_Finalize();
if(from_input->rank !=0)
exit(0);
#endif
	return EXIT_SUCCESS;
}
