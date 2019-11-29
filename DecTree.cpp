#include "DecTree.h"


DecTree::DecTree(string trainingSet, string validationSet, string testSet,
			string toPrint, string toPrune, string heuristic){	
	this->setBools(toPrint, toPrune);	
	this->setHeuristic(heuristic);	
	this->trainingFile = trainingSet;
	this->validationFile = validationSet;
	this->testFile = testSet;
	this->setAttributeList();
	this->setTrainingSet();
	this->setValidationSet();
	this->setTestSet();	
	bool * initAttributeList = this->initAttributeLeft();	
	this->head = new Node();
	this->head->parent = nullptr;	
	this->head->attributeDecision = -1;
	
	
	if(this->h1){
		this->makeH1Tree(&this->trainingSet, initAttributeList, this->head, 
											this->numAttributes-1);
	} else{	// perfrom h2
		this->makeH2Tree(&this->trainingSet, initAttributeList, this->head, 
											this->numAttributes-1);
	}
	this->trainingResults=this->runOnMadeTree(&this->trainingSet);
	
	this->validationResults=this->runOnMadeTree(&this->validationSet);

	if(this->toPrune){
		// prune on validation 
		this->prune();
	}
	
	this->testResults=this->runOnMadeTree(&this->testSet);		

	if(this->toPrint){
		this->printTree(this->head, 0, true);
	}
}




DecTree::~DecTree(){
	for(unsigned int i = 0; i < this->trainingSet.size(); i++){
		delete this->trainingSet[i];
	}
	for(unsigned int i = 0; i < this->validationSet.size(); i++){
		delete this->validationSet[i];
	}
	for(unsigned int i = 0; i < this->testSet.size(); i++){
		delete this->testSet[i];
	}
}

void DecTree::showResults(){
	string heuristic;
	string prune;	
	if(this->toPrune){
		prune = "P";
	}
	else{
		prune = "NP";
	}
	if(this->h1){
		heuristic = "H1";
	}
	else {
		heuristic = "H2";
	}
	cout << "---------------------------------------------------" << endl;	
	cout << heuristic << " " << prune << " Training " << this->trainingResults << endl;
	cout << heuristic << " " << prune << " Validation " << this->validationResults << endl;
	cout << heuristic << " " << prune << " Test " << this->testResults << endl;
	cout << "---------------------------------------------------" << endl;
}



void DecTree::compareValidation(){
	double onPrune = this->runOnMadeTree(&this->validationSet);
	cout << "first Validaiton " << this->validationResults << endl;
	cout << "after " << onPrune << endl;
}

void DecTree::postOrderPrint(Node * node){
	if(node==nullptr){
		return;
	}
	this->postOrderPrint(node->noChild);
	this->postOrderPrint(node->yesChild);
	cout <<  node->attribute << " " << node->resultOfPrune << " ";
	cout << node->isPruned << endl;
}

void DecTree::prune(){
	this->nodeToPrune=nullptr;
	this->bestSoFar=this->validationResults;	
	this->postOrderNodeRemoval(this->head);
	this->findBestNodeToRemove(this->head);

	while(this->nodeToPrune!=nullptr){		
		
		this->pruneToLeaf(this->nodeToPrune);
		this->nodeToPrune=nullptr;
		this->bestSoFar=this->validationResults;		
		this->postOrderNodeRemoval(this->head);
		this->findBestNodeToRemove(this->head);
	}
}

void DecTree::findBestNodeToRemove(Node * node){
	if(node==nullptr){
		return;
	}
	if(node->noChild==nullptr && node->yesChild==nullptr){
		return;
	}
	this->findBestNodeToRemove(node->noChild);
	this->findBestNodeToRemove(node->yesChild);
	if(node->resultOfPrune >= this->bestSoFar){
		this->bestSoFar=node->resultOfPrune;
		this->nodeToPrune = node;
	}
}

void DecTree::pruneToLeaf(Node * node){

	node->decision = node->prunedDecision;
	node->noChild=nullptr;
	node->yesChild=nullptr;
}

void DecTree::postOrderNodeRemoval(Node * node){
	
	if(node==nullptr){
		return;
	}
	if(node->noChild==nullptr && node->yesChild==nullptr){
		return;
	}		
	
	if(node->noChild!=nullptr && node->yesChild!=nullptr){	
		this->postOrderNodeRemoval(node->noChild);
		this->postOrderNodeRemoval(node->yesChild);
	}
	node->isPruned=true;
	
	vector<int*>posSet=
					this->split((this->numAttributes-1), 1, &(node->fullList));
	vector<int*>negSet=
					this->split((this->numAttributes-1), 0, &(node->fullList));
	int posSize = posSet.size();
	int negSize = negSet.size();
	if(posSize>negSize){
		node->prunedDecision=1;
	}else{
		node->prunedDecision=0;
	}
	node->resultOfPrune=this->runOnMadeTree(&this->validationSet);
	node->isPruned=false;
}

int DecTree::postOrderNodeCount(Node * node){
	if(node==nullptr){
		return 0;
	}	
	int count = 1;
	count+=this->postOrderNodeCount(node->noChild);
	count+=this->postOrderNodeCount(node->yesChild);
	return count;
}


double DecTree::runOnMadeTree(vector<int*> * list){
	int totalNum = (*list).size();	
	int numCorrect=0;
	Node * tmp;
	bool enter;	
	for(unsigned int i = 0; i < (*list).size(); i++){
		enter=true;		
		tmp = this->head;
		while(tmp->decision == -1){
			if(tmp->isPruned){			
				if(tmp->prunedDecision == (*list)[i][this->numAttributes-1]){
					numCorrect++;
				}
				enter=false;
				break;
			} 
			else{			
				if((*list)[i][tmp->attributePosition]==0){
					tmp = tmp->noChild;
				}
				else if((*list)[i][tmp->attributePosition]==1){
					tmp = tmp->yesChild;
				}
			}
		}
		if(enter){
			if(tmp->decision == (*list)[i][this->numAttributes-1]){
				numCorrect++;
			}
		}
	}
	return ((double)numCorrect)/((double)totalNum);
}


void DecTree::setHeuristic(string heuristic){
	if(heuristic == "H1"){
		this->h1=true;
	}
	else if(heuristic == "H2"){
		this->h1=false;
	}
	else{
		cout << "The heuristic invoked from the command line";
		cout << " was neither H1 or H2" << endl;
		exit(1);
	}
}

bool * DecTree::initAttributeLeft(){
	bool* ret = new bool[this->numAttributes];
	for(int i = 0; i < this->numAttributes; i++){
		ret[i] = true;
	}
	ret[this->numAttributes-1] = false; //this is the class/should not consider
	return ret;
}

bool * DecTree::makeNewAttributeLeft(bool* original, int idx){
	bool *ret = new bool[this->numAttributes];
	for(int i = 0; i < this->numAttributes; i++){
		if(original[i]){
			ret[i]=true;
		} else{
			ret[i]=false;
		}
	}
	ret[idx]=false;
	return ret;
}

// also sets numAttributes;
void DecTree::setAttributeList(){
	this->numAttributes = 0;	
	ifstream train(this->trainingFile);
	if(train.is_open()){
		string firstLine;
		if(!getline(train, firstLine)){
			cout << "error setting attribute list" << endl;
		}
		int lineLen = firstLine.length();
		int subStringLen = 0;
		int beginningIdx = 0;		

		string attribute;		
		for(int i = 0; i < lineLen; i++){
			if(firstLine[i] != ','){
				subStringLen++;
			}
			if((firstLine[i] == ',') || (i == (lineLen-1))){
				attribute = firstLine.substr(beginningIdx, subStringLen);
				beginningIdx = i + 1;
				subStringLen = 0;
				this->attributeList.push_back(attribute);
				this->numAttributes++;				
			}
		}
		train.close();

	}
	else{
		cout << "unable to open training file:" << trainingFile << endl;
		exit(1);
	}
}

// sets toPrint and toPrune
void DecTree::setBools(string toPrint, string toPrune){
	if(toPrint == "yes"){
		this->toPrint = true;
	} else if(toPrint == "no"){
		this->toPrint = false;
	} else{
		cout << "to-print was neither yes or no" << endl;
		exit(1);
	}
	if(toPrune == "yes"){
		this->toPrune = true;
	} else if(toPrune == "no"){
		this->toPrune = false;
	} else{
		cout << "to-prune was neither yes or no" << endl;
		exit(1);
	}
}

int* DecTree::parseLine(string line){
	int* instance = new int[this->numAttributes];
	int position = 0;
	int lineLen = line.length();
	int subStringLen = 0;
	int beginningIdx = 0;
	int tmp;	
	for(int i = 0; i < lineLen; i++){
		if(line[i] != ','){
			subStringLen++;
		}
		if((line[i] == ',') || ( i == (lineLen-1))){
			tmp = stoi(line.substr(beginningIdx, subStringLen));
			beginningIdx = i+1;
			subStringLen = 0;
			instance[position] = tmp;
			position++;
		
		}
	}
	assert(position == this->numAttributes);
	return instance;
}


void DecTree::setTrainingSet(){
	string line;	
	ifstream train(this->trainingFile);
	if(train.is_open()){
		string line;
		if(!getline(train, line)){
			cout << "error in setTrainingSet()" << endl;
			exit(1);
		}
		while(getline(train, line)){
			this->trainingSet.push_back(this->parseLine(line));
		}
	}
}


void DecTree::setValidationSet(){
	string line;	
	ifstream valid(this->validationFile);
	if(valid.is_open()){
		string line;
		if(!getline(valid, line)){
			cout << "error in setValidationSet()" << endl;
			exit(1);
		}
		while(getline(valid, line)){
			this->validationSet.push_back(this->parseLine(line));
		}
	}
}




void DecTree::setTestSet(){
	string line;	
	ifstream test(this->testFile);
	if(test.is_open()){
		string line;
		if(!getline(test, line)){
			cout << "error in setTestSet()" << endl;
			exit(1);
		}
		while(getline(test, line)){
			this->testSet.push_back(this->parseLine(line));
		}
	}
}



// value == 1 for positive split, value == 0 for negative split
vector<int*> DecTree::split(int position, int value, vector<int*> * list){
	vector<int*> result;
	for(unsigned int i = 0; i < (*list).size(); i++){
		if((*list)[i][position] == value){
			result.push_back((*list)[i]);
		}		
	}
	return result;
}

void DecTree::makeH1Tree(vector<int*> * list, bool* attributesLeft, 
												Node* node, int position){
	node->fullList = *list;	
	node->positiveSplit = this->split((this->numAttributes-1), 1, list);
	node->negativeSplit = this->split((this->numAttributes-1), 0, list);
	double entropy = this->entropy(this->numAttributes-1, list);
	if(entropy == 0){
		if(node->positiveSplit.size() == 0){
			node->decision=0;
		}
		else if(node->negativeSplit.size() == 0){
			node->decision=1;
		}
		node->attributePosition = position;
		node->attribute=this->attributeList[position];
		node->noChild=nullptr;
		node->yesChild=nullptr;
		return;
	}
	else{
		node->decision = -1;		
		int gainIdx = h1Gain(entropy, list, attributesLeft);
		if(gainIdx == -1){
			int posSize = node->positiveSplit.size();
			int negSize	= node->negativeSplit.size();
			if(posSize > negSize){
				node->decision = 1;
			}
			else{
				node->decision = 0;
			}
			node->attributePosition = position;
			node->attribute=this->attributeList[position];
			node->noChild=nullptr;
			node->yesChild=nullptr;	
		}		
		else{
			node->decision = -1;			
			node->attributePosition = gainIdx;		
			node->attribute=this->attributeList[gainIdx];
			node->fullList=(*list);
			node->positiveSplit=this->split(gainIdx, 1, list);
			node->negativeSplit=this->split(gainIdx, 0, list);
			node->noChild = new Node();
			node->noChild->parent = node;
			node->noChild->attributeDecision = 0;		
			node->yesChild = new Node();
			node->yesChild->parent = node;
			node->yesChild->attributeDecision = 1;
			bool * newAttributesLeft = this->makeNewAttributeLeft(
													attributesLeft, gainIdx);			
			makeH1Tree(&node->negativeSplit, newAttributesLeft, node->noChild, 
																	gainIdx);
			makeH1Tree(&node->positiveSplit, newAttributesLeft, node->yesChild, 
																	gainIdx);
		}
	}	
}

double DecTree::entropy(int position, vector<int*> * list){
	int numInstances = (*list).size();
	int numPositive = 0;
	int numNegative;	
	for(int i = 0; i < numInstances; i++){
		if((*list)[i][position] == 1){
			numPositive++;
		}
	}
	numNegative = numInstances - numPositive;
	double posTermFrac;
	double negTermFrac;

	if(numInstances){	
		posTermFrac = (double)numPositive/(double)numInstances;
		negTermFrac = (double)numNegative/(double)numInstances;
	}
	else{
		posTermFrac=0;
		negTermFrac=0;
	}
	double posTermResult;
	double negTermResult;
	if(posTermFrac){
		posTermResult = posTermFrac*(log2(posTermFrac));
	}
	else{
		posTermResult =0;
	}
	if(negTermFrac){
		negTermResult = negTermFrac*(log2(negTermFrac));	
	}
	else{
		negTermResult = 0;
	}
	double retVal = 0;
	retVal-=posTermResult;
	retVal-=negTermResult;	
	return retVal;
}

int DecTree::h1Gain(double entropy, vector<int*> *list, bool *attributesLeft){
	double tmpResult;	
	double posTerm;
	double negTerm;	
	int sizeTotal = (*list).size();	
	double entropyPos; 
	double entropyNeg;
	int sizePos;
	int sizeNeg;
	double* ret = new double[this->numAttributes];	
	for(int i = 0; i < this->numAttributes; i++){
		if(attributesLeft[i] == true){
			// calculate gain
			tmpResult=entropy;				
			vector<int*> pos = this->split(i, 1, list);
			vector<int*> neg = this->split(i, 0, list);
			
			sizePos=pos.size();
			sizeNeg=neg.size();
			entropyPos=this->entropy((this->numAttributes-1), &pos);
			entropyNeg=this->entropy((this->numAttributes-1), &neg);

			posTerm = (((double)sizePos/(double)sizeTotal)*(double)entropyPos);
			negTerm = (((double)sizeNeg/(double)sizeTotal)*(double)entropyNeg);
			tmpResult-=posTerm;
			tmpResult-=negTerm;
			ret[i] = tmpResult;

		}
		else if(attributesLeft[i] == false){
			ret[i] = -1;
		} 
	}
	int idx = -1;
	double tmpGain = 0;	
	for(int i = 0; i < this->numAttributes; i++){
		if(ret[i] > tmpGain){
			tmpGain = ret[i];
			idx = i;
		}
	}

	return idx;
}


void DecTree::makeH2Tree(vector<int*> * list, bool* attributesLeft, 
												Node* node, int position){
	node->fullList = *list;	
	node->positiveSplit = this->split((this->numAttributes-1), 1, list);
	node->negativeSplit = this->split((this->numAttributes-1), 0, list);
	double impurity = this->varianceImpurity(this->numAttributes-1, list);	

	if(impurity == 0){
		if(node->positiveSplit.size() == 0){
			node->decision=0;
		}
		else if(node->negativeSplit.size() == 0){
			node->decision=1;
		}
		node->attributePosition = position;
		node->attribute=this->attributeList[position];
		node->noChild=nullptr;
		node->yesChild=nullptr;
		return;
	}
	else{
		node->decision = -1;		
		int gainIdx = h2Gain(impurity, list, attributesLeft);
		if(gainIdx == -1){
			int posSize = node->positiveSplit.size();
			int negSize	= node->negativeSplit.size();
			if(posSize > negSize){
				node->decision = 1;
				
			}
			else{
				node->decision = 0;
			}
			node->attributePosition = position;
			node->attribute=this->attributeList[position];
			node->noChild=nullptr;
			node->yesChild=nullptr;	
		}		
		else{
			node->decision = -1;			
			node->attributePosition = gainIdx;		
			node->attribute=this->attributeList[gainIdx];
			node->fullList=(*list);
			node->positiveSplit=this->split(gainIdx, 1, list);
			node->negativeSplit=this->split(gainIdx, 0, list);
			node->noChild = new Node();
			node->noChild->parent = node;
			node->noChild->attributeDecision = 0;		
			node->yesChild = new Node();
			node->yesChild->parent = node;
			node->yesChild->attributeDecision = 1;
			bool * newAttributesLeft = this->makeNewAttributeLeft(
													attributesLeft, gainIdx);			
			makeH2Tree(&node->negativeSplit, newAttributesLeft, node->noChild, 
																	gainIdx);
			makeH2Tree(&node->positiveSplit, newAttributesLeft, node->yesChild, 
																	gainIdx);
		}
	}	
}

double DecTree::varianceImpurity(int position, vector<int*> * list){
	int numInstances = (*list).size();
	int numPositive = 0;
	int numNegative;	
	for(int i = 0; i < numInstances; i++){
		if((*list)[i][position] == 1){
			numPositive++;
		}
	}
	numNegative = numInstances - numPositive;
	double numerator = ((double)numPositive)*((double)numNegative);
	double denominator = ((double)numInstances)*((double)numInstances);

	if(denominator){
		return numerator/denominator;
	}else{
		return 0;
	}
}
// returns the index of the attribute with the greatest gain
int DecTree::h2Gain(double impurity, vector<int*> *list, 
													bool *attributesLeft){
	double tmpResult;	
	double posTerm;
	double negTerm;	
	int sizeTotal = (*list).size();	
	double varImpPos; 
	double varImpNeg;
	int sizePos;
	int sizeNeg;
	double* ret = new double[this->numAttributes];	
	for(int i = 0; i < this->numAttributes; i++){
		if(attributesLeft[i] == true){
			tmpResult=impurity;				
			vector<int*> pos = this->split(i, 1, list);
			vector<int*> neg = this->split(i, 0, list);		
			sizePos=pos.size();
			sizeNeg=neg.size();
			varImpPos=this->varianceImpurity((this->numAttributes-1), &pos);
			varImpNeg=this->varianceImpurity((this->numAttributes-1), &neg);
			posTerm = (((double)sizePos/(double)sizeTotal)*(double)varImpPos);
			negTerm = (((double)sizeNeg/(double)sizeTotal)*(double)varImpNeg);
			tmpResult-=posTerm;
			tmpResult-=negTerm;
			ret[i] = tmpResult;
		}
		else if(attributesLeft[i] == false){
			ret[i] = -1;
		} 
	}
	int idx = -1;
	double tmpGain = 0;	
	for(int i = 0; i < this->numAttributes; i++){
		if(ret[i] > tmpGain){
			tmpGain = ret[i];
			idx = i;
		}
	}
	return idx;
}



void DecTree::printTree(Node *node, int numTabs, bool isHead){
	int localNumTabs=numTabs;
	if(isHead){
		if(node->noChild == nullptr && node->yesChild == nullptr){
			if(node->decision == -1){
				cout << "problem in singleton where decision should be reached";
				cout << endl;
				exit(1);
			}			
			cout << "tree is of one node where all instances are";
			cout << "predicted to be: " << node->decision << endl;
			return;
		}
		else{
			if(node->noChild){		
				this->printTree(node->noChild, localNumTabs, false);
			}
			if(node->yesChild){
				this->printTree(node->yesChild, localNumTabs, false);
			}
		}
	}
	else{
		if(node != nullptr){
			if(numTabs){		
				for(int i = 0; i < numTabs; i+=4){
					 cout << "|   ";
				}			
			}
			cout << node->parent->attribute << " = " << node->attributeDecision;
			cout << " : ";
			if(node->decision != -1){
				cout << node->decision << endl;
			}
			else{
				cout << endl;
			}
			localNumTabs+=4;
			if(node->noChild){		
				this->printTree(node->noChild, localNumTabs, false);
			}
			if(node->yesChild){
				this->printTree(node->yesChild, localNumTabs, false);
			}
		}
	}
}

//debug func

void DecTree::_showFileNames(){
	cout << "----------------------------------------" << endl;
	cout << "Training: " << this->trainingFile << endl;
	cout << "Validation: " << this->validationFile << endl;
	cout << "Test: " << this->testFile << endl;
	cout << "----------------------------------------" << endl;
}


void DecTree::_showAttributeList(){
	int len = this->attributeList.size();
	cout << "attributeList---------------------------" << endl;	
	for(int i = 0; i < len; i++){
		cout << this->attributeList[i] << endl;
	}
	cout << "----------------------------------------" << endl;	 
}

void DecTree::_showSet(vector<int*> set){
	int len = set.size();
	cout << "------------------------------" << endl;	
	for(int i = 0; i < len; i++){
		for(int j = 0; j < this->numAttributes; j++){
			cout << set[i][j];
			if(j != (numAttributes-1)){
				cout << ",";
			}
		}
		cout << endl;
	}
	cout << "------------------------------" << endl;

}

