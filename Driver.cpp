#include <iostream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include "DecTree.h"

using namespace std;

int main(int argc, char *argv[]){
	if(argc != 7){
		cout << "must invoke as ./program <training>.csv <validation>.csv ";
		cout << "<test>.csv <print?>{yes, no} <prune?>{yes, no} ";
		cout << "<Heuristic>{H1, H2}" << endl;
		return -1;
	}	
	
	istringstream buf1(argv[1]);	
	istringstream buf2(argv[2]);
	istringstream buf3(argv[3]);
	istringstream buf4(argv[4]);
	istringstream buf5(argv[5]);
	istringstream buf6(argv[6]);

	string trainingFile;
	string validationFile;
	string testFile;
	string toPrint;
	string toPrune;
	string heuristic;
	
	buf1 >> trainingFile;
	buf2 >> validationFile;
	buf3 >> testFile;
	buf4 >> toPrint;
	buf5 >> toPrune;
	buf6 >> heuristic;

	DecTree * decTree = new DecTree(trainingFile, validationFile, testFile,
			toPrint, toPrune, heuristic);	
	
	
	decTree->showResults();
	return 0;
}
