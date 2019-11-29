#include "Node.h"

Node::Node(){
	this->isPruned = false;
	this->prunedDecision = -1;
	this->resultOfPrune = -1;
}

Node::~Node(){
	
}
