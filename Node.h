#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <cassert>
#include <iomanip>
#include <cmath>

using namespace std;

class Node {
	public:
		Node();
		~Node();
	
		string attribute;
		int attributePosition;
		
		int decision; // -1 if not a leaf node		
		
		int attributeDecision; // -1 if root				
		
		Node * parent;
		Node * noChild;
		Node * yesChild;
		
		vector<int*> fullList;		
		vector<int*> positiveSplit; // it is split on class
		vector<int*> negativeSplit;	// if it is a leaf otherwise split on 
									// attribute
		bool isPruned;
		int prunedDecision;
		double resultOfPrune;

};									
