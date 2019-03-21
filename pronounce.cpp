/*
Author: Wen Bo
Course: CSCI-135
Instructor: Genady Maryash
Assignment: Project 2

The program asks user input a word W, then print the pronunciation of the word; 
other words from the dictionary with the same pronunciation; 
words that can be obtained from W by adding one phoneme; 
words that can be obtained from W by removing one phoneme; 
words that can be obtained from W by replacing one phoneme.
*/

#include<iostream>
#include<string>
#include<fstream>
#include<cstdlib>
#include<cctype>
using namespace std;

void splitOnSpace(string s, string & before, string & after);
string Identical(string word, string ordword);
int countphoneme(string pronunciation);
string getNthphoneme(string phoneme, int num);
string addphon(string phoneme);
string remove(string phoneme);
string replace(string phoneme);
bool comparePhonemes(string word, string pron1);
int checkReplaceOrdering(string word, string pron1);
int checkRemoveOrdering(string word, string pron1);
int checkAddOrdering(string word, string pron1);
bool checkOutputWord(string word);


int main(){
	string w;
	string upperw;
	char newc;
	string pron,readline,beforestr,afterstr,space;
	string ident,rephon, addp, replacep;
	bool done = false;
	bool incluword = false;

	cout << "> ";
	cin >> w;

	for(int i = 0; i < w.length();i++){
		//ignore all words that contain non-alphabetic characters,
		//ignore all alternative pronunciations, 
		//The only non-letter character that is allowed in a word is apostrophe 
		if(w[i] >= 'a' && w[i] <= 'z'){
			newc = (char)((int)w[i] - 32);
			upperw += newc;
		}
		else if(w[i] >= 'A' && w[i] <= 'Z'){
			upperw += w[i];
		}
		else if((int)w[i] == 39){
			upperw += w[i];
		}
		else{
			done = true;
			cout << "Not found" << endl;
			break;
		}	
	}


	ifstream infile;
	infile.open("cmudict.0.7a");

	while(getline(infile,readline)){		
		splitOnSpace(readline, beforestr, afterstr);
		if(beforestr == upperw){
			splitOnSpace(afterstr, space, pron);
			cout << "Pronunciation :" << pron << endl;
			done = true;
			incluword = true;
		}
		
	}
	if(done == false){
		cout << "Not found" << endl;
	}

	infile.close();


	if(incluword){
		ident = Identical(pron, upperw);
		cout << endl;
		cout << "Identical: " << ident << endl;

		addp = addphon(pron);
		cout << "Add phoneme: " << addp << endl;

		rephon = remove(pron);
		cout << "Remove phoneme: " << rephon << endl;

		replacep = replace(pron);
		cout << "Replace phoneme: " << replacep << endl;		
	}
	return 0;
}


void splitOnSpace(string s, string & before, string & after) {
  // reset strings
 	before = ""; 
	after = "";
  // accumulate before space
	int i = 0;
	while (i < s.size() && not isspace(s[i])) { 
		before += s[i]; 
		i++;
	}
  // skip the space
	i++;
  // accumulate after space
	while (i < s.size()) { 
		after += s[i]; 
		i++; 
	}
}

string Identical(string word, string ordword){
	string readline, beforestr, afterstr, space, pron1;
	string sameword;
	ifstream infile;
	infile.open("cmudict.0.7a");

	while(getline(infile, readline)){
		
		splitOnSpace(readline, beforestr, afterstr);
		splitOnSpace(afterstr, space, pron1);
		//compare  pronunciations from the input word and dictionary word
		//if same, add this dictionary word in the return string
		if(pron1 == word && !(beforestr == ordword)){
			sameword += beforestr;
			sameword += " ";
			//add space after each word
		}
	}

	infile.close();
	return sameword;
}

int countphoneme(string pronunciation){
	//count the number of phoneme in the pronunciation string
	int num = 0;
	for(int i = 0; i < pronunciation.length(); i++){
		if(isspace(pronunciation[i])){
			num++;
		}
	}
	num += 1;
	return num;
}

string getNthphoneme(string phoneme, int num){
	//return the exact position's phoneme
	string outphon;
	string beforestr, afterstr;

	for(int i = 0; i <= num; i++){
		splitOnSpace(phoneme, beforestr, afterstr);
		outphon = beforestr;
		phoneme = afterstr;
	}
	return outphon;
}

string remove(string phoneme){
	string removeword, word, fword;
	string readline, beforestr, afterstr, space, pron1;
	ifstream infile;
	int curr_phoneme_count;
	infile.open("cmudict.0.7a");

	int in_phoneme_count = countphoneme(phoneme);
	
	while(getline(infile, readline)){
		int test = 0;
		splitOnSpace(readline, beforestr, afterstr);
		splitOnSpace(afterstr, space, pron1); 
		int curr_phoneme_count = countphoneme(pron1);

		//the number of phoneme from input word is one less than
		//the number of phoneme from dictionary word
		if(in_phoneme_count - 1 == curr_phoneme_count){
			for(int i = 0; i < in_phoneme_count; i++){
				word = getNthphoneme(phoneme, i);

				//check if the phoneme from input word is in the dictionary
				//word's phoneme string 
				if (comparePhonemes(word, pron1)) {
					test += 1;
				}
			}
		}

		//store all dictionary words which have the number of same phoneme 
		// one less than the number of input word's phoneme
		if(test == in_phoneme_count - 1){
			int samenum = checkRemoveOrdering(phoneme, pron1);
			if(samenum == in_phoneme_count - 1){
				if(checkOutputWord(beforestr)){
					removeword += beforestr + " ";	
				}
			}
			
		}
	}
	infile.close();
	return removeword;
}

string replace(string phoneme){
	string reword, word, fword;
	string readline, beforestr, afterstr, space, pron1;
	ifstream infile;
	int curr_phoneme_count;
	infile.open("cmudict.0.7a");

	int in_phoneme_count = countphoneme(phoneme);
	
	while(getline(infile, readline)){
		int test = 0;
		splitOnSpace(readline, beforestr, afterstr);
		splitOnSpace(afterstr, space, pron1); 
		int curr_phoneme_count = countphoneme(pron1);

		//the number of phoneme from input word is equal to
		//the number of phoneme from dictionary word
		if(in_phoneme_count == curr_phoneme_count){
			for(int i = 0; i < in_phoneme_count; i++){
				word = getNthphoneme(phoneme, i);
				if (comparePhonemes(word, pron1)) {
					test += 1;
				}
			}
		}

		//store all dictionary words which have the number of same phoneme 
		// equal to the number of input word's phoneme
		if(test == in_phoneme_count - 1){
			int samenum = checkReplaceOrdering(phoneme, pron1);
			if(samenum == in_phoneme_count - 1){
				if(checkOutputWord(beforestr)){
					reword += beforestr + " ";	
				}
			}			
		}
	}
	infile.close();
	return reword;
}


string addphon(string phoneme){
	string reword, word, fword;
	string readline, beforestr, afterstr, space, pron1;
	ifstream infile;
	int curr_phoneme_count;
	infile.open("cmudict.0.7a");

	int in_phoneme_count = countphoneme(phoneme);
	
	while(getline(infile, readline)){
		int test = 0;
		splitOnSpace(readline, beforestr, afterstr);
		splitOnSpace(afterstr, space, pron1); 
		int curr_phoneme_count = countphoneme(pron1);

		//the number of phoneme from input word is one greater than
		//the number of phoneme from dictionary word
		if(in_phoneme_count + 1 == curr_phoneme_count){
			for(int i = 0; i < in_phoneme_count; i++){
				word = getNthphoneme(phoneme, i);
				if (comparePhonemes(word, pron1)) {
					test += 1;
				}
			}
		}
	
		//store all dictionary words which have the number of same phoneme 
		// equal to the number of input word's phoneme
		if(test == in_phoneme_count){
			int samenum = checkAddOrdering(phoneme, pron1);
			if(samenum ==in_phoneme_count){
				if(checkOutputWord(beforestr)){
					reword += beforestr + " ";	
				}
			}
		}
	}
	
	infile.close();
	return reword;
}

bool comparePhonemes(string word, string pron1){
	//check if each phoneme of word is in the pron1
	int curr_phoneme_count = countphoneme(pron1); 

	for(int j = 0; j < curr_phoneme_count; j++){
		string fword = getNthphoneme(pron1, j);					
		if(word == fword){
			return true;
		}
	}
	return false;
}

int checkReplaceOrdering(string word, string pron1){
	int samenum = 0;
	string in_phoneme, curr_phoneme;
	int i;
	int in_phoneme_count = countphoneme(word);

	for(i = 0; i < in_phoneme_count; i++ ){
		in_phoneme = getNthphoneme(word, i);
		curr_phoneme = getNthphoneme(pron1, i);

		if(in_phoneme == curr_phoneme){
			samenum += 1;
		}
	}
	return samenum;
}

int checkAddOrdering(string word, string pron1){
	int samenum = 0;
	string in_phoneme, curr_phoneme;
	int i;
	int in_phoneme_count = countphoneme(word);

	in_phoneme = getNthphoneme(word, 1);
	curr_phoneme = getNthphoneme(pron1, 1);

	for(i = 0; i < in_phoneme_count; i++){
		in_phoneme = getNthphoneme(word, i);
		curr_phoneme = getNthphoneme(pron1, i);
		if(in_phoneme == curr_phoneme){
			samenum += 1;
		}
		else{
			curr_phoneme = getNthphoneme(pron1, i + 1);
			if(in_phoneme == curr_phoneme){
				samenum += 1;
			}
		}
	}
	return samenum;
}

int checkRemoveOrdering(string word, string pron1){
	int samenum = 0;
	string in_phoneme, curr_phoneme;
	int i;
	int in_phoneme_count = countphoneme(word);

	for(i = 0; i < in_phoneme_count - 1; i++){
		in_phoneme = getNthphoneme(word, i);
		curr_phoneme = getNthphoneme(pron1, i);
		if(in_phoneme == curr_phoneme){
			samenum += 1;
		}
		else{
			in_phoneme = getNthphoneme(word, i + 1);
			if(in_phoneme == curr_phoneme){
				samenum += 1;
			}
		}
	}
	return samenum;
}

bool checkOutputWord(string word){
	bool correctForm = false;

	for(int i = 0; i < word.length(); i++){

		if(word[i] >= 'A' && word[i] <= 'Z'){
			correctForm = true;
		}
		else if((int)word[i] == 39){
			correctForm = true;
		}
		else{
			correctForm = false;
		}
	}
	return correctForm;
}
