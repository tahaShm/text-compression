#include <cstdlib>
#include <iostream>
#include <string>
#include <vector> 
#include <fstream>
#include <sstream>
#include <cassert>
#define ENCODE "encode"
#define DECODE "decode"
#define COMPRESS_ONLY "compress_only"
#define DECOMPRESS_ONLY "decompress_only"
#define LENGTH 256
using namespace std;
struct Characters{
	int frequency;
	char name;
	string huffman;
};
struct Node{
	int frequency;
	bool checked=0;
	Node  *left;
	Node  *right;
};

int power(int base , int pow);

bool compare_string_charStar(char* input , string str);

int true_int_to_char(int input);

string check_operation(int argc , char** argv, string & input_file_name ,string & output_file_name , int & caesar) ;

void primary_set_vector(vector <Characters>& inputs , vector <Node> & nodes);

void sort_node(vector <Node> & nodes);
void read_file(vector <Characters>& inputs , vector <Node> & nodes , string & input_file , string input_file_name);
void make_huffman(vector <Node> & nodes);
void set_huffman(vector <Characters>& inputs , Node & root , string & tree);

string compressing_and_making_output_file(vector <Characters>& inputs , string input_file , string output_file_name, int caesar);
void do_caesar(string &compressed , int caesar);
unsigned long long hashing( string compressed );

void just_read_file(string & input_file , string input_file_name);
bool check_valid_decode(string & compressed ,int & size,string output_file_name);
void make_huffman_via_file(vector <Characters>& inputs ,string input_file );
string separate_compress_from_input(string input_file);
void separate_hash_and_do_caesar(string & input_file , int caesar);
string compress_to_normal(string compress,int size);
int size_of_compress(string input_file);
void decompressing_and_making_output_file(vector <Characters>& inputs ,string normalized,string output_file_name, int caesar);

void do_compress_only(string input_file_name,string output_file_name ,vector <Characters>& inputs , vector <Node> & nodes ,string & tree);
void do_decompress_only(string input_file_name,string output_file_name ,vector <Characters>& inputs);
void do_encode(string input_file_name,string output_file_name ,vector <Characters>& inputs , vector <Node> & nodes ,string & tree,int caesar);
void do_decode(string input_file_name,string output_file_name ,vector <Characters>& inputs, int caesar);

void test_compare_string_charStar();
void test_true_int_to_char();
void test_primary_set_vector();
void test_sort_node();
void test_do_caecar();
void test_hashing();
void test_separate_compress_from_input();
void test_separate_hash_and_do_caesar();
void test_compress_to_normal();
void test_size_of_compress();

int main(int argc, char** argv) {
	test_compare_string_charStar();
	test_true_int_to_char();
	test_primary_set_vector();
	test_sort_node();
	test_do_caecar();
	test_hashing();
	test_separate_compress_from_input();
	test_separate_hash_and_do_caesar();
	test_compress_to_normal();
	test_size_of_compress();
	int caesar=0;
	vector <Characters> inputs(LENGTH);
	vector <Node> nodes(LENGTH);
	string tree , input_file , compressed; 
	primary_set_vector(inputs,nodes);
	string operation;
	string input_file_name;
	string output_file_name;
	operation=check_operation(argc ,argv, input_file_name,output_file_name,caesar);
	if (operation==ENCODE) do_encode(input_file_name,output_file_name,inputs,nodes,tree,caesar);
	else if (operation==DECODE) do_decode(input_file_name,output_file_name,inputs,caesar);
	else if (operation==COMPRESS_ONLY) do_compress_only(input_file_name,output_file_name,inputs,nodes,tree);
	else if (operation==DECOMPRESS_ONLY) do_decompress_only(input_file_name,output_file_name,inputs);
}
bool compare_string_charStar(char* input , string str){
	string ctos(input);
	if (ctos==str) return 1;
	else return 0;
}
string check_operation(int argc , char** argv, string & input_file_name ,string & output_file_name , int & caesar){
	int operation=0;
	string operate;
	for (int i = 0; i < argc; ++i) {
	    if (i==1){
	    	string op(argv[i]);
	    	operate=op;
	    	if (compare_string_charStar(argv[i],ENCODE))	operation=1;
	    	else if (compare_string_charStar(argv[i],DECODE))	operation=2;
	    	else if (compare_string_charStar(argv[i],COMPRESS_ONLY))	operation=3;
	    	else if (compare_string_charStar(argv[i],DECOMPRESS_ONLY))	operation=4;
	   	}
		if (i==2){
		  		string input(argv[i]);
		  		input_file_name=input;
		  	}
	   	if (i==3){
		   	string output(argv[i]);
		  	output_file_name=output;
		}
		if (i==4 && operation<3)	caesar=atoi(argv[i]);
	}
	if (operation==0){ 
		cout << "Invalid Operation" << endl;
		operate.clear();
	}
	return operate;
}
int power(int base , int pow){
	int ans=1;
	for (int i=0;i<pow;i++)	ans = ans * base;
	return ans;
}
void primary_set_vector(vector <Characters>& inputs , vector <Node> & nodes){
	for (int i=0;i<LENGTH;i++){
		inputs[i].name = (char)i ;
		nodes[i].left = NULL ;
		nodes[i].right = NULL ;
	}
}
void sort_node(vector <Node> & nodes){
	for (int i=0;i<nodes.size();i++){
		for (int j=i+1;j<nodes.size();j++){
			if ( nodes[i].frequency>nodes[j].frequency ){
				Node swap;

				swap.frequency=nodes[i].frequency;
				swap.left=nodes[i].left;
				swap.right=nodes[i].right;

				nodes[i].frequency=nodes[j].frequency;
				nodes[i].left=nodes[j].left;
				nodes[i].right=nodes[j].right;

				nodes[j].frequency=swap.frequency;
				nodes[j].left=swap.left;
				nodes[j].right=swap.right;
			}
		}
	}
}
void make_huffman(vector <Node> & nodes){
	sort_node(nodes);
	while (nodes.size()>1){
		int two_least_values = nodes[0].frequency + nodes[1].frequency;	
		Node new_node;
		Node* left;
		Node* right;
		left = new Node;
		right = new Node;
		new_node.frequency=two_least_values;
		(*left)=nodes[0];
		(*right)=nodes[1];
		new_node.left=left;
		new_node.right=right;
		if (nodes.size()>1){
			nodes.erase(nodes.begin() , nodes.begin()+2);
			nodes.insert(nodes.begin(),new_node);
			sort_node(nodes);
		}
	}
}
void read_file(vector <Characters>& inputs , vector <Node> & nodes , string & input_file , string input_file_name){
	fstream file_name;
	char c;
	file_name.open(input_file_name,ios::in);
	while (file_name.get(c)){
		inputs[(int)c].frequency++;
		input_file += c;
	}
	file_name.close();
	for (int i=0;i<LENGTH;i++)	nodes[i].frequency=inputs[i].frequency;
}
void just_read_file(string & input_file , string input_file_name){
	fstream file_name;
	char c;
	file_name.open(input_file_name,ios::in);
	while (file_name.get(c)){
		input_file += c;
	}
	file_name.close();
}
void separate_hash_and_do_caesar(string & input_file ,int caesar){
	bool is_hash_passed=0;
	string new_input_file;
	string all_hash;
	for (int i=0;i<input_file.size();i++){
		char c=input_file[i];
		if (c=='\n' && is_hash_passed==0){
			is_hash_passed=1;
			all_hash += c;
		}
		else if (is_hash_passed==1){
			new_input_file += c;
		}
		else all_hash +=c;
	}
	do_caesar(new_input_file,caesar);
	input_file.clear();
	input_file += all_hash;
	input_file += new_input_file;
}
void set_huffman(vector <Characters>& inputs , Node & root , string & tree){
	bool not_null=0;
	if (root.left!=NULL){
		tree.push_back('0');
		set_huffman(inputs,*(root.left),tree);
		tree.pop_back();
		not_null=1;
	}
	if (root.right!=NULL){
		tree.push_back('1');
		set_huffman(inputs,*(root.right),tree);
		tree.pop_back();
		not_null=1;
	}
	if (not_null==1) return;
	else {
		if (root.checked==0){
			root.checked=1;
			for (int i=0 ; i<LENGTH ; i++){
				if (root.frequency==inputs[i].frequency && inputs[i].huffman.size()==0){
					inputs[i].huffman = tree;
					break;
				}
			}
		}
	}
}
void do_caesar(string & file , int caesar){
	bool valid_number=1;
	for (int i=0;i<file.size();i++){
		if (caesar+(int)file[i]<-128) caesar+=256;
		else if (caesar+(int)file[i]>128) caesar-=256;
	}
	for (int i=0;i<file.size();i++){
		file[i] = (char)((int)(file[i]+caesar));
	}
}
string compressing_and_making_output_file(vector <Characters>& inputs , string input_file , string output_file_name, int caesar){
	string compressed;
	string all_characters;
	stringstream all_file;
	string file;
	fstream file_name;
	int ascii_value=0,size=0;
	file_name.open(output_file_name ,ios::out);
	for (int i=0;i<input_file.size();i++)
		all_characters += inputs[(int)input_file[i]].huffman;
	for (int i=0;i<inputs.size();i++){
		all_file << inputs[i].huffman;
		all_file <<' ';
	}
	int mod=all_characters.size() % 8;
	size=all_characters.size();
	all_file << size;
	all_file << '\n';
	if (mod!=0){
		for (int i=0;i<=8-mod;i++)	all_characters += "0";
	}
	for (int i=0;i<all_characters.size();i++){
		if (i % 8 == 0 && i!=0) {
			unsigned char int_to_char= ascii_value;
			compressed += int_to_char;
			all_file << int_to_char;
			ascii_value=0;
		}
		if (all_characters[i]=='1')	ascii_value += power(2, 7 - (i % 8));
	}
	file=all_file.str();
	do_caesar(file,caesar);
	file_name << hashing(compressed) << endl;
	file_name << file;
	file_name.close();
	return compressed;
}
unsigned long long hashing(string compressed){
	unsigned long long hash=0;
	for (int i=0;i<compressed.size();i++)	hash = compressed[i] + (hash << 6) + (hash << 16) - hash ;
	return hash;
}
bool check_valid_decode(string & compressed , int & size ,string output_file_name){
	unsigned long long check_hash=0;
	fstream file_name;
	char c;
	int i=0;
	int neg=1;
	bool firstline_cheched=0;
	bool secondline_cheched=0;
	file_name.open(output_file_name ,ios::in);
	while (file_name.get(c)){
		if (firstline_cheched==0){
			if (c=='-' && i==0) neg=-1;
			else if (c!='\n'){
				check_hash = check_hash * 10;
				check_hash += (int)(c - '0');
			}
			else{
				firstline_cheched=1;
				check_hash = check_hash *neg;
			}
		}
		i++;
	}
	file_name.close();
	if (hashing(compressed)==check_hash) return 1;
	else return 0;
}
int true_int_to_char(int input){
	if (input<0)	input += 256;
	return input;
	
}
void make_huffman_via_file(vector <Characters>& inputs ,string input_file ){
	bool is_started=0;
	int which_input=0;
	for (int i=0;i<input_file.size();i++){
		char c=input_file[i];
		if (c=='\n') is_started=1;
		if (is_started && which_input<LENGTH){
			if (c=='1' || c=='0') inputs[which_input].huffman += c;
			else if (c==' ')	which_input++;
		}
		else if (which_input >= LENGTH) break;
	}
}
string separate_compress_from_input(string input_file){
	int nums_of_enters=0;
	int checked=0;
	string compressed;
	for (int i=0;i<input_file.size();i++){
		char c=input_file[i];
		if (c=='\n') nums_of_enters++;
		if (nums_of_enters > 1){
			if (checked>0) compressed += c;
			checked++;
		}
	}
	return compressed;
}
int size_of_compress(string input_file){
	int size=0;
	int nums_of_enters=0;
	string compressed;
	for (int i=0;i<input_file.size();i++){
		char c=input_file[i];
		if (c=='\n') nums_of_enters++;
		if (nums_of_enters==2){
			for(int k=i-1;input_file[k]>='0' && input_file[k]<='9';k--){
				size += power(10,i-k-1)*((int)input_file[k]-'0');
			}
			break;
		}
	}
	return size;
}
string compress_to_normal(string compressed,int size){
	string normal;
	for (int i=0;i<compressed.size();i++){
		int ascii=true_int_to_char((int)compressed[i]);
		for(int k=7;k>=0;k--){
			if (((ascii >> k)%2)==1) normal += '1' ;
			if (((ascii >> k)%2)==0) normal += '0' ;
		}
	}
	while (normal.size()>size)	normal.pop_back();
	return normal;
}
void decompressing_and_making_output_file(vector <Characters>& inputs ,string normalized,string output_file_name, int caesar){
	fstream file_name;
	file_name.open(output_file_name ,ios::out);
	string check;
	string original;
	for (int i=0;i<normalized.size();i++){
		check += normalized[i];
		for (int j=0;j<inputs.size();j++){
			if (check == inputs[j].huffman){
				original += char(j);
				check.clear();
			}
		}
	}
	file_name << original;
	file_name.close();
}
void do_compress_only(string input_file_name,string output_file_name ,vector <Characters>& inputs , vector <Node> & nodes ,string & tree){
	string input_file , file;
	read_file(inputs,nodes,input_file,input_file_name);
	make_huffman(nodes);
	set_huffman(inputs,nodes[0],tree);
	file += compressing_and_making_output_file(inputs,input_file,output_file_name,0);
}
void do_decompress_only(string input_file_name,string output_file_name ,vector <Characters>& inputs ){
	int size=0;
	string input_file , compressed , normalized;
	just_read_file(input_file,input_file_name);
	make_huffman_via_file(inputs,input_file);
	compressed += separate_compress_from_input(input_file);
	size = size_of_compress(input_file);
	if (check_valid_decode(compressed,size,input_file_name)){
		normalized += compress_to_normal(compressed , size );
		decompressing_and_making_output_file(inputs,normalized,output_file_name,0);
	}
	else {
		cout << "File has changed " << endl;
	}
}
void do_encode(string input_file_name,string output_file_name ,vector <Characters>& inputs , vector <Node> & nodes ,string & tree,int caesar){
	string input_file , file;
	read_file(inputs,nodes,input_file,input_file_name);
	make_huffman(nodes);
	set_huffman(inputs,nodes[0],tree);
	file += compressing_and_making_output_file(inputs,input_file,output_file_name,caesar);
}
void do_decode(string input_file_name,string output_file_name ,vector <Characters>& inputs, int caesar){
	int size=0;
	string input_file , compressed , normalized;
	just_read_file(input_file,input_file_name);
	separate_hash_and_do_caesar(input_file , caesar);
	make_huffman_via_file(inputs,input_file);
	compressed += separate_compress_from_input(input_file);
	size = size_of_compress(input_file);
	if (check_valid_decode(compressed,size,input_file_name)){
		normalized += compress_to_normal(compressed , size );
		decompressing_and_making_output_file(inputs,normalized,output_file_name,0);
	}
	else {
		cout << "File has changed " << endl;
	}
}


void test_compare_string_charStar(){
	char * input=new char[10];
	for (int i=0;i<9;i++) input[i]='a';
	string s_input("aaaaaaaaa");
	string s2_input("aabcdsrra");
	assert(compare_string_charStar(input,s_input));
	assert(!compare_string_charStar(input,s2_input));
}
void test_true_int_to_char(){
	assert(true_int_to_char(128)==128);
	assert(true_int_to_char(-27)==-27+256);
	assert(true_int_to_char(44)==44);
}
void test_primary_set_vector(){
	vector <Characters> inputs (LENGTH) ;
	vector <Node>  nodes  (LENGTH);
	primary_set_vector(inputs,nodes);
	assert(inputs[65].name == 'A');
	assert(nodes[20].left == NULL);

}
void test_sort_node(){
	vector <Node>  nodes  (LENGTH);
	for (int i=0;i<LENGTH;i++) nodes[i].frequency=0;
	nodes[27].frequency=120;
	nodes[8].frequency=1000;
	nodes[100].frequency=200;
	nodes[30].frequency=400;
	nodes[77].frequency=5;
	sort_node(nodes);
	assert(nodes[100].frequency == 0);
	assert(nodes[LENGTH-1].frequency == 1000);
	assert(nodes[LENGTH-2].frequency == 400);
	assert(nodes[LENGTH-3].frequency == 200);
	assert(nodes[LENGTH-4].frequency == 120);
	assert(nodes[30].frequency == 0);
	
}
void test_do_caecar(){
	string input("abcdefg");
	int caesar=3;
	do_caesar(input,caesar);
	assert(input == "defghij");
}
void test_hashing(){
	string input("ab");
	assert (hashing(input)==6363201);
}
void test_separate_compress_from_input(){
	string input("abcdefg");
	string added("hijklmnop");
	string compressed("qrstuvwxyz");
	input += '\n';
	input += added;
	input += '\n';
	input += compressed;
	assert(separate_compress_from_input(input) == compressed );
}
void test_separate_hash_and_do_caesar(){
	int caesar=4;
	string input("-128134");
	string after_hash ("1001002121 1212");
	input += '\n';
	string after_caesar;
	after_caesar += input;
	input += after_hash;
	do_caesar(after_hash ,caesar);
	after_caesar += after_hash ;
	separate_hash_and_do_caesar(input , caesar);
	assert (input == after_caesar);
}
void test_compress_to_normal(){
	string compressed ( "AaBb" );
	int size = 30 ; 
	assert(compress_to_normal(compressed,size) == "010000010110000101000010011000");
}
void test_size_of_compress(){
	string input("123121");
	string after_hash("ceatrjfgs 12345499");
	input += '\n';
	input += after_hash;
	input +='\n';
	size_of_compress(input);
	assert(size_of_compress(input)== 12345499 );
}