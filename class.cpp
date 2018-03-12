
#ifndef CLASS_CPP__
#define CLASS_CPP__

#include "class.hpp"
#include <iostream>
#include <cstring>
#include <regex>

using namespace std;

char* convert(const string& str) {
    char* result = new char[str.length()+1];
    strcpy(result,str.c_str());
    return result;
}

void PrintVisitor::visit(NumberNode* numberNode){
    //cout << numberNode->getNumber() << " " ;
    completeResult += to_string(numberNode->getNumber());
    completeResult += " " ;
    //this->addToResult(numberNode->getNumber());
}
void PrintVisitor::visit(VariableNode* variableNode){
    //cout << variableNode->getVariableName() << " " ;
    

    completeResult += variableNode->getVariableName();
    completeResult += " " ;
    //this->addToResult(variableNode->getVariableName());
}
void PrintVisitor::visit(FunctionNode* functionNode){
    //cout << functionNode->getFunctionName() << " " ;
    completeResult += functionNode->getFunctionName();
    completeResult += " " ;
}
void PrintVisitor::visit(EqualNode* equalNode){
    equalNode->getLeftChild()->accept(this);
    //cout << equalNode->getOperation() << " ";
    completeResult += equalNode->getOperation();
    completeResult += " ";
    equalNode->getRightChild()->accept(this);
    
}
void PrintVisitor::visit(AddNode* addNode){
    addNode->getLeftChild()->accept(this);
    //cout << "+ ";
    completeResult += "+ ";
    addNode->getRightChild()->accept(this);
}
void PrintVisitor::visit(MinusNode* minusNode){
    minusNode->getLeftChild()->accept(this);
    //cout << "- ";
    completeResult += "- ";
    minusNode->getRightChild()->accept(this);
}
void PrintVisitor::visit(MultiplicationNode* multNode){
    multNode->getLeftChild()->accept(this);
    //cout << "* ";
    completeResult += "* ";
    multNode->getRightChild()->accept(this);
}
void PrintVisitor::visit(DivisionNode* divNode){
    divNode->getLeftChild()->accept(this);
    //cout << "/ ";
    completeResult += "/ ";
    divNode->getRightChild()->accept(this);
}
void PrintVisitor::visit(ExponentialNode* exponentialNode){
    
    completeResult += "pow(";
    exponentialNode->getLeftChild()->accept(this);
    //cout << "^ ";
    completeResult += ", ";
    exponentialNode->getRightChild()->accept(this);
    completeResult +=")";
}
void PrintVisitor::visit(ExpNode* expNode){
    //cout << " exp(";
    completeResult += "exp(";
    expNode->getChild()->accept(this);
    //cout << ") ";
    completeResult += ") ";
}
void PrintVisitor::visit(LnNode* lnNode){
    //cout << " ln(";
    completeResult += "log(";
    lnNode->getChild()->accept(this);
    //cout << ") ";
    completeResult += ")";
}
void PrintVisitor::visit(BracketNode* bracketNode){
    //cout << " (";
    completeResult += " (";
    bracketNode->getChild()->accept(this);
    //cout << ") ";
    completeResult += ") ";
}    



void DiffVisitor::handleResult(Node* child, Node* parent, int childType){
    if(childType != 0)
        parent->setChild(child, childType);
    else
        this->diffTreeRoot = child;
}

/*
int DiffVisitor::getParentType(BinaryOperationNode* parent){return 2;}
int DiffVisitor::getParentType(UnaryOperationNode* parent){return 3;}
int DiffVisitor::getParentType(NumberNode* parent){return 1;}
int DiffVisitor::getParentType(VariableNode* parent){return 1;}
int DiffVisitor::getParentType(FunctionNode* parent){return 1;}
*/
void DiffVisitor::visit(NumberNode* numberNode){
    #ifdef DO_DEBUG
        cout << "differentiation reached number node of value " << numberNode->getNumber() << endl;
    #endif
    
    Node* diffParent = this->getCurrentParent();
     //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=numberNode->getIsLeftKid();
        isRightKid=numberNode->getIsRightKid();
    }
   
    Node* diffNode = new NumberNode(0, diffParent, isLeftKid, isRightKid);
    
    //this->handleResult(diffNode, diffParent, diffNode->getParentRelation());
    
    
}
void DiffVisitor::visit(FunctionNode* functionNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=functionNode->getIsLeftKid();
        isRightKid=functionNode->getIsRightKid();
    }
    //newName = d + functionName + /d + this->diffVar
    string newName = "d";
    newName += functionNode->getFunctionName();
    newName += "_d";
    newName += this->getDiffName();

    Node* diffNode = new FunctionNode(convert(newName), newName.size(), diffParent, isLeftKid, isRightKid);
    
    #ifdef DO_DEBUG
        cout << "differentiation reached function node of name " << functionName << endl;
    #endif
    
    //this->handleResult(diffNode, diffParent, diffNode->getParentRelation());
        
}
void DiffVisitor::visit(VariableNode* variableNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=variableNode->getIsLeftKid();
        isRightKid=variableNode->getIsRightKid();
    }
    
    double value = 0;
    if(variableNode->getVariableName() == this->getDiffName()){
        value = 1;
    }
    
    Node* diffNode = new NumberNode(value, diffParent, isLeftKid, isRightKid);

    #ifdef DO_DEBUG
        cout << "differentiation reached VariableNode of the variable " << variableName << endl;
    #endif
    //this->handleResult(diffNode, diffParent, diffNode->getParentRelation());

}
void DiffVisitor::visit(AddNode* addNode){
    Node* diffParent = this->getCurrentParent();
        //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    //cout << "!!!!!!!!!!!!!!!!!" << endl << parentType << endl << "!!!!!!!!!!!!!!!!!!!!!!" << endl;
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=addNode->getIsLeftKid();
        isRightKid=addNode->getIsRightKid();
    }

    Node* diffNode = new AddNode(diffParent, isLeftKid, isRightKid);
    
    this->setCurrentParent(diffNode);
    addNode->getLeftChild()->accept(this);
    this->setCurrentParent(diffNode);
    addNode->getRightChild()->accept(this);
    
    //this->handleResult(diffNode, diffParent, diffNode->getParentRelation());
    
}
void DiffVisitor::visit(MinusNode* minusNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=minusNode->getIsLeftKid();
        isRightKid=minusNode->getIsRightKid();
    }
    
    Node* diffNode = new MinusNode(diffParent, isLeftKid, isRightKid);
    this->setCurrentParent(diffNode);
    //this->handleResult(diffNode, diffParent, diffNode->getParentRelation());
    minusNode->getLeftChild()->accept(this);
    
    Node* rightBracketNode = new BracketNode(diffNode, 0, 1);
    this->setCurrentParent(rightBracketNode);
    minusNode->getRightChild()->accept(this);
        
}
void DiffVisitor::visit(MultiplicationNode* multNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=multNode->getIsLeftKid();
        isRightKid=multNode->getIsRightKid();
    }
    Node* diffPlusNode = new AddNode(diffParent, isLeftKid, isRightKid);
    //Node* diffPlusNode = new AddNode(diffParent, multNode->getIsLeftKid(), multNode->getIsRightKid());
    
    Node* diffLeftMultNode = new MultiplicationNode(diffPlusNode, 1, 0);
    
    
    Node* leftCopy = multNode->getLeftChild()->copyNode(diffLeftMultNode, 1, 0);
    
    Node* diffRightMultNode = new MultiplicationNode(diffPlusNode, 0, 1);
    
    Node* rightCopy = multNode->getRightChild()->copyNode(diffRightMultNode, 0, 1);
    
    Node* leftBracketNode = new BracketNode(diffLeftMultNode, 0, 1);
    Node* rightBracketNode = new BracketNode(diffRightMultNode, 1, 0);
    
    this->setCurrentParent(leftBracketNode);    
    multNode->getRightChild()->accept(this);
    
    
    this->setCurrentParent(rightBracketNode);
    multNode->getLeftChild()->accept(this);

    this->handleResult(diffPlusNode, diffParent, diffPlusNode->getParentRelation());        
}
void DiffVisitor::visit(DivisionNode* divNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=divNode->getIsLeftKid();
        isRightKid=divNode->getIsRightKid();
    }
    
    Node* diffDivNode = new DivisionNode(diffParent, isLeftKid, isRightKid);
    #ifdef DO_DEBUG
        cout << endl << "created root division node for thisentiation" << endl;
    #endif
    
    //denominator of the division:
    
    Node* rightBracketNode = new BracketNode(diffDivNode, 0, 1);
    
    #ifdef DO_DEBUG
        cout << endl << "created root bracket node for thisentiation" << endl;
    #endif
    
    Node* multNode = new MultiplicationNode(rightBracketNode,1,1);
    Node* rightCopy1 = divNode->getRightChild()->copyNode(multNode, 0, 1);
    
    Node* rightCopy2 = divNode->getRightChild()->copyNode(multNode, 1,0);
    
    //enumerator of the division:
    Node* leftBracketNode = new BracketNode(diffDivNode, 1, 0);
    
    Node* diffMinusNode = new MinusNode(leftBracketNode, 1, 1);
    
    Node* diffLeftMultNode = new MultiplicationNode(diffMinusNode, 1, 0);
    Node* leftBracketNode2 = new BracketNode(diffLeftMultNode, 1, 0);
    Node* rightCopy = divNode->getRightChild()->copyNode(diffLeftMultNode, 0, 1);
    
    Node* diffRightMultNode = new MultiplicationNode(diffMinusNode, 0, 1);
    Node* rightBracketNode2 = new BracketNode(diffRightMultNode, 0, 1);
    Node* leftCopy = divNode->getLeftChild()->copyNode(diffRightMultNode, 1, 0);
    
    
    this->setCurrentParent(leftBracketNode2);    
    divNode->getLeftChild()->accept(this);
    
    
    this->setCurrentParent(rightBracketNode2);
    divNode->getRightChild()->accept(this);

    this->handleResult(diffDivNode, diffParent, diffDivNode->getParentRelation());    
        
}
void DiffVisitor::visit(ExponentialNode* exponentialNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=exponentialNode->getIsLeftKid();
        isRightKid=exponentialNode->getIsRightKid();
    }
    
    Node* diffMultNode = new MultiplicationNode(diffParent, isLeftKid, isRightKid);
    
    //right side of Multiplication - a copy of exponentialNode part of the tree:
    Node* bracketNode = new BracketNode(diffMultNode, 0, 1);
    Node* completeCopy = exponentialNode->copyNode(bracketNode, 1, 1);
    
    
    
    //left side of Multiplication - a new tree that still needs to be thisentiated
    Node* leftBracketNode = new BracketNode(diffMultNode, 1, 0);
        
    Node* diffPlusNode = new AddNode(leftBracketNode, 1, 1);
    
    Node* diffLeftMultNode = new MultiplicationNode(diffPlusNode, 1, 0);
    //Node* bracketNode1 = new BracketNode(diffLeftMultNode, 0, 1);
    Node* diffDivNode = new DivisionNode(diffLeftMultNode, 0, 1);//bracketNode1, 1, 1);
    //Node* bracketNode2 = new BracketNode(diffDivNode, 1, 0);
    Node* rightCopy = exponentialNode->getRightChild()->copyNode(diffDivNode, 1, 0);//bracketNode2, 1, 1);
    //Node* bracketNode3 = new BracketNode(diffDivNode, 0, 1);
    Node* leftCopy = exponentialNode->getLeftChild()->copyNode(diffDivNode, 0, 1);//bracketNode3, 1, 1);
    
    Node* diffRightMultNode = new MultiplicationNode(diffPlusNode, 0, 1);
    
    Node* diffLnNode = new LnNode(diffRightMultNode, 1, 0);
    Node* leftCopy2 = exponentialNode->getLeftChild()->copyNode(diffLnNode, 1, 1);
    
    
    //Building the tree is done, now we still need to thisentiate left and right side, however remember to give the right parents!
    
    Node* bracketNode4 = new BracketNode(diffLeftMultNode, 1, 0);
    this->setCurrentParent(bracketNode4);    
    exponentialNode->getLeftChild()->accept(this);
    
    Node* bracketNode5 = new BracketNode(diffRightMultNode, 0, 1);
    this->setCurrentParent(bracketNode5);
    exponentialNode->getRightChild()->accept(this);

    this->handleResult(diffMultNode, diffParent, diffMultNode->getParentRelation());    
     
}
void DiffVisitor::visit(EqualNode* equalNode){
    
    #ifdef DO_DEBUG
        cout << "accepted differentiation as EqualNode" << endl;
    #endif
    
    Node* diffParent = this->getCurrentParent();
    Node* diffNode = new EqualNode(diffParent, equalNode->getIsLeftKid(), equalNode->getIsRightKid());
    
    
    this->setCurrentParent(diffNode);
    equalNode->getLeftChild()->accept(this);
    
    this->setCurrentParent(diffNode);
    equalNode->getRightChild()->accept(this);
    
    this->handleResult(diffNode, diffParent, diffNode->getParentRelation());    
}
void DiffVisitor::visit(ExpNode* expNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=expNode->getIsLeftKid();
        isRightKid=expNode->getIsRightKid();
    }
    Node* multNode = new MultiplicationNode(diffParent, isLeftKid, isRightKid);
    Node* copy = expNode->copyNode(multNode, 1, 0);
    Node* diffBracketNode = new BracketNode(multNode, 0, 1);
    
    
    this->setCurrentParent(diffBracketNode);    
    expNode->getChild()->accept(this);
    

    this->handleResult(multNode, diffParent, multNode->getParentRelation());         
}
void DiffVisitor::visit(LnNode* lnNode){
     Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=lnNode->getIsLeftKid();
        isRightKid=lnNode->getIsRightKid();
    }
    Node* diffDivNode = new DivisionNode(diffParent, isLeftKid, isRightKid);
    Node* rightBracketNode = new BracketNode(diffDivNode, 0, 1);
    Node* copy = lnNode->copyNode(rightBracketNode, 1, 1);
    
    Node* leftBracketNode = new BracketNode(diffDivNode, 1, 0);
    
    
    this->setCurrentParent(leftBracketNode);    
    lnNode->getChild()->accept(this);
    

    this->handleResult(diffDivNode, diffParent, diffDivNode->getParentRelation());       
}
void DiffVisitor::visit(BracketNode* bracketNode){
    Node* diffParent = this->getCurrentParent();
    //include possibility, that the parent type changed from BinaryOperationNode to BracketNode (type 3)
    int parentType = diffParent->getParentType();
    int isLeftKid=1;
    int isRightKid=1;
    if(parentType != 3){
        isLeftKid=bracketNode->getIsLeftKid();
        isRightKid=bracketNode->getIsRightKid();
    }
    Node* diffBracketNode = new BracketNode(diffParent, isLeftKid, isRightKid);
    
    
    this->setCurrentParent(diffBracketNode);    
    bracketNode->getChild()->accept(this);
    

    this->handleResult(diffBracketNode, diffParent, diffBracketNode->getParentRelation());    
}   

void EvaluationVisitor::visit(NumberNode* numberNode){
    this->setTemporaryValue(numberNode->getNumber(), this->getCurrentChoice());
}
void EvaluationVisitor::visit(VariableNode* variableNode){
    if(this->getEvalVar() == variableNode->getVariableName())
        this->setTemporaryValue(this->getEvalVarValue(), this->getCurrentChoice());
    else
        this->setTemporaryValue(0, this->getCurrentChoice());
    
}
void EvaluationVisitor::visit(FunctionNode* functionNode){
        this->setTemporaryValue(0, this->getCurrentChoice());    
}
void EvaluationVisitor::visit(EqualNode* equalNode){
    this->setTempSingle(0);
    this->setTempRight(0);
    this->setTempLeft(0);
    setCurrentChoice(2);
    equalNode->getRightChild()->accept(this);
    this->handleResult(this->getTemporaryValue(2));
    
}
void EvaluationVisitor::visit(AddNode* addNode){
    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(1);
    addNode->getLeftChild()->accept(this);
    double temp = this->getTemporaryValue(1);
    
    this->setCurrentChoice(2);
    addNode->getRightChild()->accept(this);
    
    this->setTemporaryValue(temp + this->getTemporaryValue(2), myResultSpace);
}
void EvaluationVisitor::visit(MinusNode* minusNode){
    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(1);
    minusNode->getLeftChild()->accept(this);
    double temp = this->getTemporaryValue(1);
    
    this->setCurrentChoice(2);
    minusNode->getRightChild()->accept(this);
    
    this->setTemporaryValue(temp - this->getTemporaryValue(2), myResultSpace);
    
}
void EvaluationVisitor::visit(MultiplicationNode* multNode){

    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(1);
    multNode->getLeftChild()->accept(this);
    double temp = this->getTemporaryValue(1);
    
    this->setCurrentChoice(2);
    multNode->getRightChild()->accept(this);
    
    this->setTemporaryValue(temp * this->getTemporaryValue(2), myResultSpace);    
}
void EvaluationVisitor::visit(DivisionNode* divNode){

    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(1);
    divNode->getLeftChild()->accept(this);
    double temp = this->getTemporaryValue(1);
    
    this->setCurrentChoice(2);
    divNode->getRightChild()->accept(this);
    
    this->setTemporaryValue(temp / this->getTemporaryValue(2), myResultSpace);
}
void EvaluationVisitor::visit(ExponentialNode* exponentialNode){

    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(1);
    exponentialNode->getLeftChild()->accept(this);
    double temp = this->getTemporaryValue(1);
    
    this->setCurrentChoice(2);
    exponentialNode->getRightChild()->accept(this);
    
    this->setTemporaryValue(pow(temp,this->getTemporaryValue(2)), myResultSpace);
}
void EvaluationVisitor::visit(ExpNode* expNode){

    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(3);
    expNode->getChild()->accept(this);
    
    this->setTemporaryValue(exp(this->getTemporaryValue(3)), myResultSpace);
}
void EvaluationVisitor::visit(LnNode* lnNode){
    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(3);
    lnNode->getChild()->accept(this);
    
    this->setTemporaryValue(log(this->getTemporaryValue(3)), myResultSpace);    
}
void EvaluationVisitor::visit(BracketNode* bracketNode){
    int myResultSpace = this->getCurrentChoice();
    
    this->setCurrentChoice(3);
    bracketNode->getChild()->accept(this);
    
    this->setTemporaryValue(this->getTemporaryValue(3), myResultSpace);
}   

void Node::setParent(Node* pa, int isLeftKid, int isRightKid){
    parent = pa;
    if(isLeftKid !=0 && isRightKid == 0){
        isLeftKid = 1;
        isRightKid = 0;
    }else if(isLeftKid == 0 && isRightKid != 0){
        isLeftKid = 0;
        isRightKid = 1;
    }else if(isLeftKid != 0 && isRightKid !=0){
        isLeftKid = 1;
        isRightKid = 1;
    }
    
    isLeftChild = isLeftKid;
    isRightChild = isRightKid;
}

int Node::getParentRelation(){
    // if this returns 0, it is not a child, if it returns 1, it is a left child, if it returns 2, it is a right Child, 3: child of a UnaryNode
    return isLeftChild + isRightChild*2;
}


NumberNode::NumberNode(double value){
    #ifdef DO_DEBUG
        cout << "creating NumberNode with value " << value << endl;
        #endif
    number = value;
}
NumberNode::NumberNode(double value, Node * pa, int isLeftKid, int isRightKid){
    #ifdef DO_DEBUG
        cout << "creating NumberNode with parent and value " << value << endl;
    #endif
    number = value;
    setParent(pa, isLeftKid, isRightKid);
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}
NumberNode::~NumberNode(){
    #ifdef DO_DEBUG
        cout << "deleting NumberNode with value " << number << endl;
    #endif
    
}
void NumberNode::setChild(Node* child, int childType){
    
}


    
void NumberNode::accept(Visitor* v){
    v->visit(this);
}





FunctionNode::FunctionNode(char* _exp, int i){// _exp: full expression, i = position of end of function name

    char fName[90];
    if(i<90){
        strncpy(fName,_exp,i);
        fName[i] = '\0';
        #ifdef DO_DEBUG
            cout << "creating FunctionNode with functionName " << functionName << endl;
        #endif
    }else{
        cout << "function name is too long, setting it to f" << endl;
        fName[0] = 'f';
        fName[1] = '\0';
    }
    functionName = "";
    functionName += fName;
    
}
FunctionNode::FunctionNode(char* _exp, int i, Node * pa, int isLeftKid, int isRightKid){
    char fName[90];
    if(i<90){
        strncpy(fName,_exp,i);
        fName[i] = '\0';
        #ifdef DO_DEBUG
            cout << "creating FunctionNode with functionName " << functionName << " and parent" << endl;
        #endif
    }else{
        cout << "function name is too long, setting it to f" << endl;
        fName[0] = 'f';
        fName[1] = '\0';
    }
    functionName = "";
    functionName += fName;
    setParent(pa, isLeftKid, isRightKid);
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
       
}
FunctionNode::~FunctionNode(){
    #ifdef DO_DEBUG
        cout << "deleting FunctionNode with functionName " << functionName << endl;
    #endif
    
}


void FunctionNode::accept(Visitor* v){
    v->visit(this);
}

void FunctionNode::setChild(Node* child, int childType){
    
}



VariableNode::VariableNode(char* _exp, int size){// _exp: variableName
    char vName[50];


    if(size<50){
        #ifdef DO_DEBUG
            cout << "creating VariableNode with variableName " << _exp << endl;
#endif
        strncpy(vName,_exp, size);
        vName[size] = '\0';     
    }else{
        cout << "variable name is too long, setting it to x" << endl;
        vName[0] = 'x';
        vName[1] = '\0';
    }
    variableName = "";
    variableName += vName;
}
VariableNode::VariableNode(char* _exp, int size, Node* pa, int isLeftKid, int isRightKid){// _exp: variableName
    char vName[50];


    if(size<50){
        #ifdef DO_DEBUG
            cout << "creating VariableNode with variableName " << _exp << " and parent" << endl;
#endif
        strncpy(vName,_exp, size);
        vName[size] = '\0';     
    }else{
        cout << "variable name is too long, setting it to x" << endl;
        vName[0] = 'x';
        vName[1] = '\0';
    }
    variableName = "";
    variableName += vName;
    setParent(pa, isLeftKid, isRightKid);
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());

} 
VariableNode::~VariableNode(){
    #ifdef DO_DEBUG
        cout << "deleting VariableNode with variableName " << variableName << endl;
    #endif
    
}

void VariableNode::accept(Visitor* v){
    v->visit(this);
}

void VariableNode::setChild(Node* child, int childType){
    
}



BinaryOperationNode::BinaryOperationNode(char op){
    operation = op;        
}
BinaryOperationNode::BinaryOperationNode(char op, Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid){
    operation = op;      
    left = lkid;
    right = rkid;

    setParent(pa, isLeftKid, isRightKid);
    
}    
BinaryOperationNode::BinaryOperationNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid){     
    left = lkid;
    right = rkid;

    setParent(pa, isLeftKid, isRightKid);
    

    
}    
BinaryOperationNode::BinaryOperationNode(char op, Node * lkid, Node * rkid){
    operation = op;      
    left = lkid;
    right = rkid;
}

BinaryOperationNode::BinaryOperationNode(Node * lkid, Node * rkid){      
    left = lkid;
    right = rkid;
}

BinaryOperationNode::BinaryOperationNode(char op, Node * pa, int isLeftKid, int isRightKid){
    operation = op;      

    setParent(pa, isLeftKid, isRightKid);
    #ifdef DO_DEBUG
        cout << "\t\t\t BinaryOperationNode " << operation << " is child of type " << getParentRelation() << endl << endl;
    #endif
    
}

BinaryOperationNode::BinaryOperationNode(Node * lkid){     
    left = lkid;
}

BinaryOperationNode::~BinaryOperationNode(){ 
    #ifdef DO_DEBUG
        cout << "deleting BinaryOperationNode of operation " << operation << endl;   
    #endif
    delete left;
    delete right;
}

void BinaryOperationNode::setChild(Node * kid, int childType){
    if(childType == 1){
        left = kid;
        #ifdef DO_DEBUG
            cout << "Setting left kid for BinaryOperationNode of operation " << operation << endl;
        #endif
        
    }
    else if(childType == 2){
        right = kid;
        #ifdef DO_DEBUG
            cout << "Setting right kid for BinaryOperationNode of operation " << operation << endl;
        #endif
        
    }
}

void BinaryOperationNode::setLeftChild(Node * lkid){
    left = lkid;
    lkid->setParent(this,1,0);
}

void BinaryOperationNode::setRightChild(Node * rkid){
    right = rkid;
    rkid->setParent(this,0,1);
}



void BinaryOperationNode::setOperation(char op){
    operation = op;
}


AddNode::AddNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('+', lkid, rkid, pa, isLeftKid, isRightKid){  
    #ifdef DO_DEBUG
        cout << "creating AddNode with kids and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

AddNode::AddNode(Node * lkid, Node * rkid):BinaryOperationNode('+',lkid, rkid){      
    #ifdef DO_DEBUG
        cout << "creating AddNode with kids\n";
    #endif
    
}

AddNode::AddNode(Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('+',pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating AddNode with parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

AddNode::AddNode():BinaryOperationNode('+'){
    #ifdef DO_DEBUG
        cout << "creating AddNode without kids or parent\n";        
    #endif
}

void AddNode::accept(Visitor* v){
    v->visit(this);
}




MinusNode::MinusNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('-', lkid, rkid, pa, isLeftKid, isRightKid){  
    #ifdef DO_DEBUG
        cout << "creating MinusNode with kids and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

MinusNode::MinusNode(Node * lkid, Node * rkid):BinaryOperationNode('-',lkid, rkid){      
    #ifdef DO_DEBUG
        cout << "creating MinusNode with kids\n";
    #endif
    
}

MinusNode::MinusNode(Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('-',pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating MinusNode with parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

MinusNode::MinusNode():BinaryOperationNode('-'){
    #ifdef DO_DEBUG
        cout << "creating MinusNode without kids or parent\n";     
    #endif
}    


void MinusNode::accept(Visitor* v){
    v->visit(this);
}




MultiplicationNode::MultiplicationNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('*', lkid, rkid, pa, isLeftKid, isRightKid){  
    #ifdef DO_DEBUG
        cout << "creating MultiplicationNode with kids and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());

}

MultiplicationNode::MultiplicationNode(Node * lkid, Node * rkid):BinaryOperationNode('*',lkid, rkid){      
    #ifdef DO_DEBUG
        cout << "creating MultiplicationNode with kids\n";
    #endif
    
}

MultiplicationNode::MultiplicationNode(Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('*',pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating MultiplicationNode with parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

MultiplicationNode::MultiplicationNode():BinaryOperationNode('*'){
    #ifdef DO_DEBUG
        cout << "creating MultiplicationNode without kids or parent\n";   
    #endif
}


void MultiplicationNode::accept(Visitor* v){
    v->visit(this);
}




DivisionNode::DivisionNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('/', lkid, rkid, pa, isLeftKid, isRightKid){  
    #ifdef DO_DEBUG
        cout << "creating DivisionNode with kids and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

DivisionNode::DivisionNode(Node * lkid, Node * rkid):BinaryOperationNode('/',lkid, rkid){      
    #ifdef DO_DEBUG
        cout << "creating DivisionNode with kids\n";
    #endif
    
}

DivisionNode::DivisionNode(Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('/',pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating DivisionNode with parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

DivisionNode::DivisionNode():BinaryOperationNode('/'){
    #ifdef DO_DEBUG
        cout << "creating DivisionNode without kids or parent\n";     
    #endif
}


void DivisionNode::accept(Visitor* v){
    v->visit(this);
}



ExponentialNode::ExponentialNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('^', lkid, rkid, pa, isLeftKid, isRightKid){  
    #ifdef DO_DEBUG
        cout << "creating ExponentialNode with kids and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

ExponentialNode::ExponentialNode(Node * lkid, Node * rkid):BinaryOperationNode('^',lkid, rkid){      
    #ifdef DO_DEBUG
        cout << "creating ExponentialNode with kids\n";
    #endif
    
}

ExponentialNode::ExponentialNode(Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('^',pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating ExponentialNode with parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

ExponentialNode::ExponentialNode():BinaryOperationNode('^'){
    #ifdef DO_DEBUG
        cout << "creating ExponentialNode without kids or parent\n";    
    #endif
}   


void ExponentialNode::accept(Visitor* v){
    v->visit(this);
}
   


EqualNode::EqualNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('=', lkid, rkid, pa, isLeftKid, isRightKid){  
    #ifdef DO_DEBUG
        cout << "creating EqualNode with kids and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

EqualNode::EqualNode(Node * lkid, Node * rkid):BinaryOperationNode('=',lkid, rkid){      
    #ifdef DO_DEBUG
        cout << "creating EqualNode with kids\n";
    #endif
    
}

EqualNode::EqualNode(Node * pa, int isLeftKid, int isRightKid):BinaryOperationNode('=',pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating EqualNode with parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

EqualNode::EqualNode():BinaryOperationNode('='){
    #ifdef DO_DEBUG
        cout << "creating EqualNode without kids or parent\n";      
    #endif
}


void EqualNode::accept(Visitor* v){
    v->visit(this);
}

UnaryOperationNode::UnaryOperationNode(){}


UnaryOperationNode::UnaryOperationNode(Node * pa, int isLeftKid, int isRightKid){
    
    setParent(pa, isLeftKid, isRightKid);
}


UnaryOperationNode::UnaryOperationNode(Node * kid, Node * pa, int isLeftKid, int isRightKid){
    child = kid;
    setParent(pa, isLeftKid, isRightKid);
}

UnaryOperationNode::~UnaryOperationNode(){
    
    #ifdef DO_DEBUG
        cout << "deleting UnaryOperationNode of operation " << operation << endl;
    #endif
    delete child;
}
void UnaryOperationNode::setChild(Node * kid, int childType){
    if(childType==3)
        child = kid;
}

Node* UnaryOperationNode::getChild(){
    return child;
}


 

ExpNode::ExpNode(Node * kid, Node * pa, int isLeftKid, int isRightKid):UnaryOperationNode(kid, pa, isLeftKid, isRightKid){      
    #ifdef DO_DEBUG
        cout << "creating ExpNode with kid and parent\n";
    #endif
    
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

ExpNode::ExpNode(Node * pa, int isLeftKid, int isRightKid):UnaryOperationNode(pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating ExpNode with parent\n";
    #endif
    
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

ExpNode::ExpNode():UnaryOperationNode(){
    
}

ExpNode::~ExpNode(){
#ifdef DO_DEBUG
    cout << "destroying ExpNode \n"
#endif
}


void ExpNode::accept(Visitor* v){
    v->visit(this);
}



LnNode::LnNode(Node * kid, Node * pa, int isLeftKid, int isRightKid):UnaryOperationNode(kid, pa, isLeftKid, isRightKid){      
    #ifdef DO_DEBUG
        cout << "creating LnNode with kid and parent\n";
    #endif
    
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

LnNode::LnNode(Node * pa, int isLeftKid, int isRightKid):UnaryOperationNode(pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating LnNode with parent\n";
    #endif
    
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

LnNode::LnNode():UnaryOperationNode(){
    
}

LnNode::~LnNode(){
#ifdef DO_DEBUG
    cout << "destroying LnNode \n"
#endif    
}

void LnNode::accept(Visitor* v){
    v->visit(this);
}

BracketNode::BracketNode(Node * kid, Node * pa, int isLeftKid, int isRightKid):UnaryOperationNode(kid, pa, isLeftKid, isRightKid){      
    #ifdef DO_DEBUG
        cout << "creating BracketNode with kid and parent\n";
    #endif
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
    
}

BracketNode::BracketNode(Node * pa, int isLeftKid, int isRightKid):UnaryOperationNode(pa, isLeftKid, isRightKid){     
    #ifdef DO_DEBUG
        cout << "creating BracketNode with parent\n";
    #endif
    
    if(this->getParentRelation() != 0)
        pa->setChild(this, this->getParentRelation());
}

BracketNode::BracketNode():UnaryOperationNode(){
    
}

BracketNode::~BracketNode(){
#ifdef DO_DEBUG
    cout << "destroying BracketNode \n"
#endif
    
}


void BracketNode::accept(Visitor* v){
    v->visit(this);
}


#endif //CLASS_CPP__
