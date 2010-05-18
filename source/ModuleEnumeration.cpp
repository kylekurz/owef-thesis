/*
 **********************************************
 // File:	Module_discovery.cpp
 //
 // Author:	Kyle Kurz and Xiaoyu Liang
 //
 // Date:	7.20.09
 //
 // Purpose:	Define a class for use
 //		with various module discovery models
 //		in the Open Word Enumeration
 //		Framework
 **********************************************

 Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang

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

#include "ModuleEnumeration.h"

using namespace std;

//standard constructor
ModuleEnumeration::ModuleEnumeration()
{
	//no-op
}

//standard destructor
ModuleEnumeration::~ModuleEnumeration()
{
	//no-op
}

//constructor for module operation
ModuleEnumeration::ModuleEnumeration( OWEFArgs *from_input )
{
	list = from_input;
}

//constructor for module operation
ModuleEnumeration::ModuleEnumeration( OWEFArgs *from_input, DataStructure *structure, WordScoring *model, SequenceFile *input )
{
	list = from_input;
	into = input;
	cout << "Creating modules..." << endl;
	reportModules( structure, model );
}

/*void ModuleEnumeration::read_seq(istream &seq_file, vector<string> &seqs, int &seq_no)
 {
 string tmp, temp, line;
 //char c;
 int index = 0;
 while (!seq_file.eof())
 {
 char c = seq_file.peek();
 if((c == COMMENT)||(c == '\n')||(c == ' '))
 {
 if(index > 0)
 {
 seqs.push_back(line);
 seq_no++;
 line.clear();
 index = 0;
 }
 getline(seq_file,tmp);
 }else{
 getline(seq_file,temp);
 line+=temp;
 temp.clear();
 index++;
 }
 }
 seqs.push_back(line);
 seq_no++;
 }*/

void ModuleEnumeration::replace( std::string& str, char former, char latter )
{
	for( int i = 0; str[ i ] != '\0'; i++ )
	{
		if( str[ i ] == former )
		{
			str[ i ] = latter;
		}
	}
}

/*void ModuleEnumeration::readWords( istream &words_file, vector<string> &words, vector<double> &prob )
{
	string tmp, line, val, str_P;
	double P;
	//double temp;
	int pos_prev_delim;
	int pos_next_delim;
	//int occu_prev_delim;
	//int occu_next_delim;

	getline( words_file, tmp );

	while( !words_file.eof() )
	{
		getline( words_file, line );
		if( words_file.eof() )
		{
			break;
		}
		pos_prev_delim = line.find( DELIM, 0 );
		pos_next_delim = line.find( DELIM, pos_prev_delim + 1 );

		//for (int i = 0; i < 3; i++)
		{
			pos_prev_delim = line.find( DELIM, pos_prev_delim + 1 );
			pos_next_delim = line.find( DELIM, pos_prev_delim + 1 );
		}

		str_P = line.substr( pos_prev_delim + 1, pos_next_delim - pos_prev_delim );
		//cout<<"str_P--"<<str_P<<endl;
		P = atof( str_P.c_str() );
		//cout<<"P: "<<P<<endl;

		prob.push_back( P );

		istringstream iss;
		replace( line, ',', ' ' );
		iss.str( line );
		iss >> val;
		words.push_back( val );
	}
}*/

void ModuleEnumeration::findPosition( const vector<Sequence*> & seqs, const vector<pair<string, Scores> > & words, vector<vector<vector<int> > >& positions, int fixed)
{
	vector<vector<int> >pos;
	string temp;
	
	int word_length = 0;
	if(list->fixed == 1){
		word_length = words[0].first.length();
	}
	for(int k = 0; k<(int)seqs.size(); k++){		
		pos.resize(words.size());
		pos.assign(pos.size(),vector<int> ());
		
		for (int j = 0; j < (int)words.size(); ++j){
			if(fixed != 1){
				word_length = words[j].first.length();
			}
			for (int i = 0; i < ((int)seqs[k]->getBodyLength() - word_length + 1); i++){
				//if (upper_lower) // adjust the upper or lower case of the sequence and word files.
				//{
				
				const string *s_ptr = (*seqs[k]).getText();
				//cout << *s_ptr << endl;
				char *t = (char *)&(*s_ptr)[(*seqs[k]).getOffset() + i];
				//cout << *t << " " << k << endl;
				//cout << words[j].first << words[j].second.E << endl;
				
				int dist=0;
				for(int l=0; l<word_length; l++)
				{
					if(*t != words[j].first[l])
						dist++;
					t++;
				}
				if(dist == 0)
					pos[j].push_back(i);	
			}
		}
		positions.push_back(pos);
	}
}

void ModuleEnumeration::makeNCombinations( vector<vector<int> > & combinations, const vector<int> & input, int n, int s )
{
	if ((int)input.size() < n || n < 0){
		std::cerr << "Bad input in function make_n_combinations! Returning!" << std::endl;
		std::cerr << "Info:\ninput size = " << input.size() << " n = " << n << std::endl;
		return;
	}
	
	if (n == (int)input.size()){
  		combinations.push_back(input);
  		return;
 	}
	
	if (n == 1){
		vector<int> temp;
		for (int i = 0; i < (int)input.size(); ++i)
		{
			temp.push_back(input[i]);
			combinations.push_back(temp);
			temp.pop_back();
		}
		return;
	}
	
	if (n == 0){
		return;
 	}
	
 	vector<int> input_copy(input);
 	vector<vector<int> > smaller_combs;
 	for (int i = 0; i <= (int)input.size() - n; ++i){
 		int val_erased = input_copy.back();
 		if (s == 0){ 
			input_copy.pop_back();
		}
 		makeNCombinations(smaller_combs, input_copy, n - 1, s); 
		for (int j = 0; j < (int)smaller_combs.size(); ++j){
			smaller_combs[j].insert(smaller_combs[j].begin(), val_erased);
			combinations.push_back(smaller_combs[j]);
		}
		smaller_combs.clear();
		if(s == 1){
			input_copy.pop_back();
		}
 	}
}

void ModuleEnumeration::posComb( const vector<int> &each_combs, const vector<vector<int> > &each_seq_pos, vector<vector<int> >&line )
{
	vector<int> each_combs_copy (each_combs); 
	if(each_combs.size()<2){
		std::cerr << "BAD INPUT, the dimention is smaller than 2" << std::endl;
		return;
	}
	
	if(each_combs.size()==2){
		vector<int> temp;
		for(int posa=0;posa<(int)each_seq_pos[each_combs[0]].size();posa++){
			for(int posb=0; posb<(int)each_seq_pos[each_combs[1]].size();posb++){
				temp.push_back(each_seq_pos[each_combs[0]][posa]);
				temp.push_back(each_seq_pos[each_combs[1]][posb]);
				line.push_back(temp);
				temp.clear();
			}
		}
	}
	if(each_combs.size()>2){
		for(int num=0; num<(int)each_combs_copy.size()-2;num++){
			int erased = each_combs_copy.back();
			each_combs_copy.pop_back();
			vector<vector<int> >less_line;
			posComb(each_combs_copy,each_seq_pos,less_line);
			for(int pos=0; pos<(int)each_seq_pos[erased].size();pos++){
				for(int m=0; m<(int)less_line.size();m++){
					less_line[m].push_back(each_seq_pos[erased][pos]);
					line.push_back(less_line[m]);
					less_line[m].pop_back();
				}
			}
		}
	}
}

void ModuleEnumeration::createMap( const vector<vector<int> >&combs, const vector<vector<vector<int> > >&positions, vector<vector<vector<vector<int> > > >&map )
{
	for(int j=0; j<(int)combs.size();j++){
		vector<vector<vector<int> > >seq_tmp;
		for(int seq=0; seq<(int)positions.size();seq++){
			vector<vector<int> >line;
			posComb(combs[j],positions[seq],line);
			seq_tmp.push_back(line);
			line.clear();
		}
		map.push_back(seq_tmp);
		seq_tmp.clear();
	}
}

bool ModuleEnumeration::check(vector<int> &sub_map,vector<int> &each_combs, const vector<pair<string, Scores> > &words ){
	for(int pos_f=0; pos_f<(int)sub_map.size()-1;pos_f++){
		for(int pos_s=pos_f+1;pos_s<(int)sub_map.size();pos_s++){
			//cout<<"1:"<<sub_map[pos_f]<<" 2:"<<sub_map[pos_s]<<" length:"<<words[each_combs[pos_f]].length()<<endl;
			if(((sub_map[pos_f]<sub_map[pos_s])&&((sub_map[pos_f]+(int)words[each_combs[pos_f]].first.length()-1)<sub_map[pos_s]))
			   ||((sub_map[pos_s]<sub_map[pos_f])&&((sub_map[pos_s]+(int)words[each_combs[pos_s]].first.length()-1)<sub_map[pos_f]))){
				return true;
				//cout<<"return true"<<endl;
			}else{
				//cout<<"return flase"<<endl;
				return false;
			}
		}
	}
	return false;
}

void ModuleEnumeration::createNewMap( vector<vector<vector<vector<int> > > >&map, vector<vector<int> >&combs,	vector<pair<string, Scores> > &words)
{
	vector<vector<int> >tmp_line;
	vector<vector<vector<int> > >seq_tmp;
	int count;
	for(int i =0; i<(int)combs.size();i++){
		count = 0;
		for(int seq=0; seq<(int)map[i].size();seq++){
			for(int each = 0; each<(int)map[i][seq].size();each++){
				bool flag = check(map[i][seq][each],combs[i],words);
				if(!flag){
					if(each != (int)map[i][seq].size()-1){
						bool last_item=false;
						vector<int> tmp;
						bool flag = false;
						for(int last=map[i][seq].size()-1;last>each;last--){
							tmp = map[i][seq].back();
							last_item=check (tmp,combs[i],words);
							map[i][seq].pop_back();
							if(last_item){
								flag =true;
								break;
							}
						}
						if(flag){
							map[i][seq][each]=tmp;
						}else{
							map[i][seq].pop_back();
						}
					}else{
						map[i][seq].pop_back();
					}
				}
			}
		}
	}
}

void ModuleEnumeration::distDens( const vector<vector<int> > &combs, const vector<vector<vector<vector<int> > > > &map, vector<vector<double> > &den,
						   const vector<pair<string, Scores> > &words, vector<vector<int> > &dist )
{

	for(int i=0; i<(int)combs.size();i++){
		double number=0;
		double total_density =0;
		double total_distance = 0;
		double range;
		double average;
		double average_dist;
		vector<double> tmp_line;
		vector<int> dist_tmp_line;
		int number1 =0;
		int number2=0;
		int number3=0;
		int number4=0;
		int number5=0;
		int number6=0;
		double total;
		int dist_number1=0;
		int dist_number2=0;
		int dist_number3=0;
		int dist_number4=0;
		int dist_number5=0;
		int dist_number6=0;
		int dist_min=1000;
		int dist_max=0;
		
		vector<double>stand_den;
		vector<double>stand_dis;
		double standard_den;
		double standard_dis;
		
		for(int seq=0; seq<(int)map[i].size();seq++){
			for(int comb=0; comb<(int)map[i][seq].size(); comb++){
				range = 0;
				double density;
				int tmp;
				int dist_tmp;
				total =0;
				int local_min = 10000;
				int local_max = 0;
				for(int p1=0; p1<(int)map[i][seq][comb].size()-1;p1++){
					total+=words[combs[i][p1]].first.length();
					for(int p2=p1+1; p2<(int)map[i][seq][comb].size();p2++){
						if(map[i][seq][comb][p2]>map[i][seq][comb][p1]){
							tmp = map[i][seq][comb][p2] + words[combs[i][p2]].first.length() - map[i][seq][comb][p1];
							dist_tmp = map[i][seq][comb][p2]-words[combs[i][p1]].first.length() - map[i][seq][comb][p1];
						}else{
							tmp = map[i][seq][comb][p1] + words[combs[i][p1]].first.length() - map[i][seq][comb][p2];
							dist_tmp = map[i][seq][comb][p1]-words[combs[i][p2]].first.length() - map[i][seq][comb][p2];
						}
						
						if(range<tmp){
							range = tmp;
						}
						if(local_min > dist_tmp){
							local_min = dist_tmp;
						}
						if(local_max < dist_tmp){
							local_max = dist_tmp;
						}
					}
				}
				total += words[combs[i][map[i][seq][comb].size()-1]].first.length();
				density = total/range;
				total_density += density;
				stand_den.push_back(density);
				if(density<0.1){
					number1++;
				}else if(density < 0.2){
					number2++;
				}else if(density < 0.3){
					number3++;
				}else if(density <0.4){
					number4++;
				}else if(density <0.5){
					number5++;
				}else{
					number6++;
				}
				
				stand_dis.push_back(local_min);
				if(local_min < 20){
					dist_number1++;
				}else if(local_min < 40){
					dist_number2++;
				}else if(local_min < 60){
					dist_number3++;
				}else if(local_min < 80){
					dist_number4++;
				}else if(local_min < 100){
					dist_number5++;
				}else{
					dist_number6++;
				}
				
				total_distance += local_min;
				if(dist_min > local_min){
					dist_min = local_min;
				}
				if(dist_max < local_max){
					dist_max = local_max;
				}
			}	
		}
		number = number1+number2+number3+number4+number5+number6;
		average = total_density/number;
		double sum_dens=0;
		for(int s=0; s<(int)stand_den.size();s++){
			double tmp = pow(stand_den[s]-average,2);
			sum_dens +=tmp;
		}
		double linshi =sum_dens/number;
		standard_den= sqrt(linshi);
		tmp_line.push_back(number1);
		tmp_line.push_back(number2);
		tmp_line.push_back(number3);
		tmp_line.push_back(number4);
		tmp_line.push_back(number5);
		tmp_line.push_back(number6);
		tmp_line.push_back(average);
		tmp_line.push_back(standard_den);
		den.push_back(tmp_line);
		tmp_line.clear();
		
		double sum_dis=0;
		average_dist = total_distance/number;
		for(int d=0; d<(int)stand_dis.size();d++){
			double tmp = pow(stand_dis[d]-average_dist,2);
			sum_dis +=tmp;
		}
		double zanshi=sum_dis/number;
		standard_dis = sqrt(zanshi);
		dist_tmp_line.push_back(dist_number1);
		dist_tmp_line.push_back(dist_number2);
		dist_tmp_line.push_back(dist_number3);
		dist_tmp_line.push_back(dist_number4);
		dist_tmp_line.push_back(dist_number5);
		dist_tmp_line.push_back(dist_number6);
		dist_tmp_line.push_back(dist_min);
		dist_tmp_line.push_back(dist_max);
		dist_tmp_line.push_back((int)average_dist);
		dist_tmp_line.push_back((int)standard_dis);
		dist.push_back(dist_tmp_line);
		dist_tmp_line.clear();
	}
	
}

double ModuleEnumeration::convertProb( double E, vector<int> seqs_length )
{
	double prob = 1;
	int sum = 0;
	for( int i = 0; i < static_cast<int> ( seqs_length.size() ); i++ )
	{
		sum += seqs_length[ i ] - list->maxlength + 1;
	}
	prob = E / sum;
	return prob;
}

void ModuleEnumeration::reportModules( DataStructure *structure, WordScoring *model )
{
	//ifstream words_file(argv[1]);	
	
	time_t start, end;
	time(&start);
	/*ifstream seq_file(argv[2]);
	int fixed (atoi(argv[3]));
	int no_dimentions (atoi(argv[4]));
	int self (atoi(argv[5]));
	int dis (atoi(argv[6]));
	int dens(atoi(argv[7]));
	int overlap (atoi(argv[8]));
	int seq_occ (atoi(argv[9]));*/
	char dist_file[100];
	char dens_file[100];
	if (list->distributed==1){
		cout<<"Please Input the Distance Distribution's Result File Name:"<<endl;
		cin>>dist_file;
	}
	if(list->density==1){
		cout <<"Please Input the Density Distribution's Result File Name:"<<endl;
		cin>>dens_file;
	}
	
	vector<Sequence*> seqs;
	seqs = into->getSequences();
	cout<<"reading sequence..."<<endl;
	//readSeq(seq_file, seqs);
	/*for(int i=0; i<seqs.size();i++){
	 cout<<"seqs "<<i<<": with length: "<<seqs[i].length()<<" :"<<seqs[i]<<endl;
	 }*/
	cout<<"reading words & probability..."<<endl;
	
	vector<pair<string, Scores> >words;
	words = model->getSeeds();
	for (int x=0; x<(int)words.size(); x++) {
		cout << words[x].first << " " << words[x].second.E << " " << words[x].second.OE << endl;
	}
	vector<double>prob;
	//read_words(words_file,words,prob);
	//readWords(words_file,words,expec);
	
	/*cout<<"words' expected value:"<<endl;
	 for(int k=0; k<expec.size();k++)
	 {
	 cout<<expec[k]<<"	";
	 }
	 cout<<endl;*/
	
	if(list->fixed == 1){
		int pos=0;
		int fixed_length = words[0].first.length();
		for(int i=0; i<(int)seqs.size();i++){
			pos+=seqs[i]->getBodyLength()-fixed_length+1;
		}
		for(int j=0; j<(int)words.size();j++){
			double tmp = words[j].second.E/pos;
			prob.push_back(tmp);
		}
	}else{
		int pos=0;
		for(int j=0; j<(int)words.size();j++){
			for(int i=0; i<(int)seqs.size();i++){
				pos += seqs[i]->getBodyLength()-words[j].first.length()+1;
			}
			double tmp=words[j].second.E/pos;
			prob.push_back(tmp);
		}
	}
	//expec.clear();
	
	/*for (int i=0; i<prob.size();i++){
	 cout<<words[i]<<": "<<prob[i]<<endl;
	 }*/
	
	
	cout<<"finding positions"<<endl;
	vector<vector< vector<int> > > positions; // seq -> words -> position
	findPosition(seqs, words, positions, list->fixed); 	//positions: seq->words->position
	/*for(int p=0; p<seqs.size();p++){
	 cout<<"in seq: "<<p<<endl;
	 for(int l=0; l<positions[p].size();l++){
	 if(positions[p][l].size()>0){
	 cout<<words[l];
	 cout<<" occur in: ";
	 for(int g=0;g<positions[p][l].size();g++){
	 cout<<positions[p][l][g]<<" ";
	 }
	 cout<<endl;
	 }
	 }
	 }*/
	
	cout<<"combinating..."<<endl;
	vector<vector<int> > combs;
	vector<int> input;
	for (int i = 0; i <= (int)words.size()-1; ++i){
		input.push_back(i);
	}
	makeNCombinations(combs,input,list->dimensions,list->self_compare);
	
	
	//	if(no_dimentions == 2 && order == 1){ //¥¶¿Ì“ª∂‘¥Œ£¨≤¢«“ «øº¬«orderµƒ«Èøˆ // order conservation is only working for pairs.
	//		vector<vector<int> >combs_copy (combs);
	//		for(int i=0; i<combs_copy.size();i++){
	//			vector<int> temp;
	//			for(int j=1; j>=0; j--){
	//				temp.push_back(combs_copy[i][j]);
	//			}
	//			combs.push_back(temp);
	//			temp.clear();
	//		}
	//		combs_copy.clear();
	//	}
	
	/*cout<<"combinations' result"<<endl;
	 for (int i=0; i<combs.size();i++)
	 {
	 for(int j=0; j<combs[i].size();j++)
	 {
	 cout<<"_"<<words[combs[i][j]]<<"_";
	 }
	 cout<<endl;
	 }*/	
	cout<<"create_map..."<<endl;
	//create a map to record every possible combination using the positions.
	vector<vector<vector<vector<int> > > >map; //combs->{seq}->{position combination}
	createMap(combs,positions,map);
	positions.clear();
	//if(order == 1 || overlap == 0){ //order is not functional so far.
	if(list->overlap_words == 0){
		cout<<"overlap =0"<<endl;
		createNewMap(map,combs,words);
	}
	
	/*cout<<"map:"<<endl;,
	 for(int c=0; c<map.size();c++){
	 cout<<"module: ";
	 for(int w=0; w<combs[c].size();w++){
	 cout<<words[combs[c][w]]<<"_";
	 }
	 cout<<endl;
	 for(int s=0; s<map[c].size();s++){
	 if(map[c][s].size()!=0){
	 cout<<"in seq"<<s<<":"<<endl;
	 for(int l=0; l<map[c][s].size();l++){
	 cout<<"	the "<<l<<"th possible:"<<endl;
	 for(int pp=0; pp<map[c][s][l].size();pp++){
	 cout<<map[c][s][l][pp]<<" ";
	 }
	 cout<<endl;
	 }
	 }
	 }
	 }*/
	
	/*cout<<"combinations' result after filtered"<<endl;
	 cout<<"map:"<<endl;
	 for(int c=0; c<map.size();c++){
	 cout<<"module: ";
	 for(int w=0; w<combs[c].size();w++){
	 cout<<words[combs[c][w]]<<"_";
	 }
	 cout<<endl;
	 for(int s=0; s<map[c].size();s++){
	 if(map[c][s].size()!=0){
	 cout<<"in seq"<<s<<":"<<endl;
	 for(int l=0; l<map[c][s].size();l++){
	 cout<<"	the "<<l<<"th possible:"<<endl;
	 for(int pp=0; pp<map[c][s][l].size();pp++){
	 cout<<map[c][s][l][pp]<<" ";
	 }
	 cout<<endl;
	 }
	 }
	 }
	 }*/
	
	cout<<"computing scores..."<<endl;
	vector<double>E_s;
	for (int k=0; k<(int)combs.size();k++){
		int power;
		double re=0;
		for(int j=0; j<(int)seqs.size();j++){
			double temp = 1;
			for(int i=0; i<(int)combs[k].size();i++){
				power = seqs[j]->getBodyLength()-words[combs[k][i]].first.length()+1;
				//cout<<"power: "<<power<<" prob: "<<prob[combs[k][i]]<<endl;
				temp *= (1- pow((1-prob[combs[k][i]]),power)); 
			}
			re += temp;
		}
		E_s.push_back(re);
	}
	
	/*cout<<"expected value of sequences: "<<endl;
	 for(int i=0; i<E_s.size(); i++){
	 cout<<"comb no "<<i<<" :"<<E_s[i]<<endl;
	 }*/
	cout<<words[1].first<<" E: "<<words[1].second.E<<" P: "<<prob[1]<<" E_s: "<<E_s[1]<<endl;
	
	vector<int> O_s;
	vector<vector<bool> > seq_occur; //could be used for generating the sequence occuring map
	for (int k=0;k<(int)combs.size();k++){
		int number =0;
		vector <bool> eachline;
		for(int s=0; s<(int)seqs.size();s++){
			if(map[k][s].size()>0){//À˘”–µƒ¥ ∂º‘⁄¥À–Ú¡–÷–≥ˆœ÷π˝
				number++;
				eachline.push_back(1);
			}else{
				eachline.push_back(0);
			}
		}
		O_s.push_back(number);
		//if(seq_occ==1){
		//	seq_occur.push_back(eachline);
		//}
	}
	
	/*cout<<"seqs hits"<<endl;
	 for(int i=0; i<O_s.size();i++)
	 {
	 cout<<O_s[i]<<endl;
	 }
	 cout<<"seq_hits map"<<endl;
	 for(int i=0; i<seq_occur.size();i++)
	 {
	 for(int j=0; j<seq_occur[i].size();j++)
	 {
	 cout<<seq_occur[i][j]<<" ";
	 }
	 cout<<endl;
	 }*/
	vector<vector<int> >dist;
	vector<vector<double> >den;
	if((list->density==1)||(list->distributed ==1)){
		cout<<"distance/density distribution...."<<endl;
		distDens(combs, map, den, words, dist);
	}
	map.clear();
	cout<<"output result..."<<endl;
	ofstream fout;
	stringstream stream;
	stream << list->prefix;
	stream << "_modules.csv";
	string filename;
	stream >> filename;
	stream.clear();
	//      ofstream fout("module.csv");
	fout.open( filename.c_str() );
	fout<<"Module, O_s, E_s, O_s/E_s, Sln(O_s/E_s), (O-E)^2/E"<<endl;
	for(int i=0; i<(int)combs.size(); i++){
		if(O_s[i]>0){
			for(int j=0; j<(int)combs[i].size();j++){
				fout<<words[combs[i][j]].first<<"_";
			}
			fout<<DELIM;
			fout<< O_s[i]<<DELIM<<E_s[i]<<DELIM<<O_s[i]/E_s[i]<<DELIM;
			double temp = O_s[i]/E_s[i];
			fout<< O_s[i]*log(temp)<<DELIM<<(pow((O_s[i]-E_s[i]),2)/E_s[i])<<DELIM<<endl;
		}
	}
	
	if(list->distributed == 1){
		//ofstream dout("distance_distribution.csv");
		ofstream dout;
		dout.open(dist_file);
		dout<<"Module,<=20, 20~40, 40~60, 60~80, 80~100, >100,Minimum Distance, Maximum Distance, Average,standard_deviation,Sln(Os/Es)"<<endl;
		for(int i=0; i<(int)combs.size();i++){
			if(O_s[i]>0){
				for(int j=0; j<(int)combs[i].size();j++){
					dout<<words[combs[i][j]].first<<"_";
				}
				dout<<DELIM;
				for(int k=0; k<(int)dist[i].size();k++){
					dout<< dist[i][k]<<DELIM;
				}
				dout<<O_s[i]*log(O_s[i]/E_s[i])<<DELIM<<endl;
			}
		}
	}
	
	if(list->density == 1){
		//ofstream sout("density_distribution.csv");
		ofstream sout;
		sout.open(dens_file);
		sout<<"Module,<10%, 10%~20%, 20%~30%, 30%~40%, 40%~50%,>=50%,average,standard deviation,Sln(Os/Es)"<<endl;
		for(int i=0; i<(int)combs.size();i++){
			if(O_s[i]>0){
				for(int j=0; j<(int)combs[i].size();j++){
					sout<<words[combs[i][j]].first<<"_";
				}
				sout<<DELIM;
				for(int k=0; k<(int)den[i].size();k++){
					sout<< den[i][k]<<DELIM;
				}
				sout<<O_s[i]*log(O_s[i]/E_s[i])<<DELIM<<endl;
			}
		}
	}
	
	time (&end);
	cout<<"completed in "<< difftime(end, start)<<" seconds."<<endl;
	return;
}

