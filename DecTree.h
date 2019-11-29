
#include "Node.h"

using namespace std;

class DecTree {
	public:
		DecTree(string trainingSet, string validationSet, string testSet,
			string toPrint, string toPrune, string heuristic);
		~DecTree();		
		
		void showResults();
		void compareValidation();		


		void postOrderPrint(Node* node);		


		void prune();	
	
		void findBestNodeToRemove(Node * node);
		void pruneToLeaf(Node * node);		


		void postOrderNodeRemoval(Node * node);
		
		int postOrderNodeCount(Node * node);		
				
		double runOnMadeTree(vector<int*> * list);


		void setHeuristic(string heuristic);		

		bool *initAttributeLeft();		
		bool *makeNewAttributeLeft(bool* original, int idx);		
		
		void setAttributeList();
		void setBools(string toPrint, string toPrune);	
	
		int* parseLine(string line);	
			
		void setTrainingSet();
		void setValidationSet();		
		void setTestSet();

		vector<int*> split(int position, int value, vector<int*> * list);		

		void makeH1Tree(vector<int*> * list, bool* attributesLeft, 
												Node* node, int position);
		double entropy(int position, vector<int*> * list);
		int h1Gain(double entropy, vector<int*> *list, 
												bool *attributesLeft);
		
		void makeH2Tree(vector<int*> * list, bool* attributesLeft, Node* node,
																int position);
		double varianceImpurity(int position, vector<int*> * list);
		int h2Gain(double impurity, vector<int*> *list, 
												bool *attributesLeft);

		void printTree(Node *node, int numTabs, bool isHead);

		//debug func
		void _showFileNames();
		void _showAttributeList();
		void _showSet(vector<int*> set);
	private:
		int numAttributes;
		bool toPrint;
		bool toPrune;
		bool h1; // true if h1 false if h2	
		
		string trainingFile;
		string validationFile;
		string testFile;		
		
		vector<string> attributeList;		
		
		vector<int*> trainingSet;
		vector<int*> validationSet;
		vector<int*> testSet;

		double trainingResults;
		double validationResults;
		double testResults;
	
		Node* head;
		
		// these are tool for pruning
		double bestSoFar;
		Node* nodeToPrune;

};
