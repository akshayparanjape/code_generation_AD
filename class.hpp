 
#ifndef CLASS_HPP__
#define CLASS_HPP__

//#define DO_DEBUG

#include <iostream>
#include <cstring>
#include <string>
#include <regex>
#include <cmath>
//#include <stdlib.h>
//#include <files>
using namespace std;
//int DEBUG = 0;

char* convert(const string& str);
class Visitor;
class Node;

class Node
{
private:
    Node *parent = nullptr;
    int isLeftChild = 0;
    int isRightChild = 0;
    
public:

    virtual ~Node(){
        int isChild = getParentRelation();
        if(isChild > 0){
            Node * nullNode = nullptr;
            parent->setChild(nullNode, isChild);
        }
#ifdef DO_DEBUG
            cout << "deleting Child of type " << isChild << endl;
#endif
    }
    void sayHello(){
        cout << "Hello World! " << endl;
    }

    virtual void accept(Visitor* v)=0;
    
    void setParent(Node* pa, int isLeftKid, int isRightKid);
    virtual Node* getRightChild(){return nullptr;}
    virtual Node* getLeftChild(){return nullptr;}
    virtual void setChild(Node* child, int childType){}
    Node* getParent(){
        return parent;
    }
    int getIsLeftKid(){
        return isLeftChild;
    }
    int getIsRightKid(){
        return isRightChild;
    }



    int getParentRelation();
    virtual int getParentType()=0;
    virtual Node* copyNode(Node* newParent, int isLeftKid, int isRightKid)=0;
    /* data */
};


class NumberNode: public Node
{
private:
    double number;
public:
    NumberNode(double value);
    NumberNode(double value, Node * pa, int isLeftKid, int isRightKid);
    ~NumberNode();
    void accept(Visitor* v);
    double getNumber(){
        return number;
    }
    
    
    void setChild(Node* child, int childType);
    int getParentType(){
        return 1;
    }
    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new NumberNode(number, newParent, isLeftKid, isRightKid);
        return copy;
    }

};

class FunctionNode: public Node
{
private:
    //char functionName[90];
    string functionName;
public:
    FunctionNode(char* _exp, int i);    
    FunctionNode(char* _exp, int i, Node * pa, int isLeftKid, int isRightKid);
    ~FunctionNode();
    void accept(Visitor* v);
    string getFunctionName(){
        return functionName;
    }
    
    
    void setChild(Node* child, int childType);
    int getParentType(){
        return 1;
    }
    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new FunctionNode(convert(functionName), functionName.size(), newParent, isLeftKid, isRightKid);
        return copy;
    }


};

class VariableNode: public Node
{
private:
    
    string variableName;
public:
    VariableNode(char* _exp, int size);
    VariableNode(char* _exp, int size, Node* pa, int isLeftKid, int isRightKid);
    ~VariableNode();
    string getVariableName(){
        return variableName;
    }
    void accept(Visitor* v);
        
    
    void setChild(Node* child, int childType);
    int getParentType(){
        return 1;
    }
    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new VariableNode(convert(variableName), variableName.size(), newParent, isLeftKid, isRightKid);
        return copy;
    }


};

class BinaryOperationNode: public Node
{
private:
    Node * left = nullptr;
    Node * right = nullptr;
    char operation;
public:
    BinaryOperationNode(char op);
    BinaryOperationNode(char op, Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    BinaryOperationNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    BinaryOperationNode(char op, Node * lkid, Node * rkid);
    BinaryOperationNode(Node * lkid, Node * rkid);
    BinaryOperationNode(char op, Node * pa, int isLeftKid, int isRightKid);
    BinaryOperationNode(Node * lkid);
    
    ~BinaryOperationNode();
    
    char getOperation(){
        return operation;
    }
    
    Node* getRightChild(){
        return right;
    }
    Node* getLeftChild(){
        return left;
    }
    
    void setChild(Node* child, int childType);
    void setLeftChild(Node * lkid);
    void setRightChild(Node * rkid);

    void setRightOrLeftChild(int isLeftKid, int isRightKid);
    
    void setOperation(char op);
    virtual void accept(Visitor* v){}
    
    
    
    virtual Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new BinaryOperationNode(operation, newParent, isLeftKid, isRightKid);
        
        if(copy->getParentRelation() != 0)
            newParent->setChild(copy, copy->getParentRelation());
    
        Node* leftKid = nullptr, *rightKid=nullptr;
        if(left != nullptr)
            leftKid = left->copyNode(copy, 1, 0);
        
        if(right != nullptr)
            rightKid = right->copyNode(copy, 0, 1);
        
        
        return copy;
    }
    int getParentType(){
        return 2;
    }
};

class AddNode : public BinaryOperationNode
{
public:
    AddNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    AddNode(Node * lkid, Node * rkid);
    AddNode(Node * pa, int isLeftKid, int isRightKid);
    AddNode();
    void accept(Visitor* v);
    
    
    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new AddNode(newParent, isLeftKid, isRightKid);
        
    
        Node* leftKid = nullptr, *rightKid=nullptr;
        if(this->getLeftChild() != nullptr)
            leftKid = this->getLeftChild()->copyNode(copy, 1, 0);
        
        if(this->getRightChild() != nullptr)
            rightKid = this->getRightChild()->copyNode(copy, 0, 1);
        
        
        return copy;
    }

    
};

class MinusNode : public BinaryOperationNode
{
public:
    MinusNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    MinusNode(Node * lkid, Node * rkid);
    MinusNode(Node * pa, int isLeftKid, int isRightKid);
    MinusNode();
    void accept(Visitor* v);
};

class MultiplicationNode : public BinaryOperationNode
{
public:
    MultiplicationNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    MultiplicationNode(Node * lkid, Node * rkid);
    MultiplicationNode(Node * pa, int isLeftKid, int isRightKid);
    MultiplicationNode();
    
    
    void accept(Visitor* v);
    
    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new MultiplicationNode(newParent, isLeftKid, isRightKid);
        
    
        Node* leftKid = nullptr, *rightKid=nullptr;
        if(this->getLeftChild() != nullptr)
            leftKid = this->getLeftChild()->copyNode(copy, 1, 0);
        
        if(this->getRightChild() != nullptr)
            rightKid = this->getRightChild()->copyNode(copy, 0, 1);
        
        
        return copy;
    }
};

class DivisionNode : public BinaryOperationNode
{
public:
    DivisionNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    DivisionNode(Node * lkid, Node * rkid);
    DivisionNode(Node * pa, int isLeftKid, int isRightKid);
    DivisionNode();
    
    void accept(Visitor* v);

    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new DivisionNode(newParent, isLeftKid, isRightKid);
        
    
        Node* leftKid = nullptr, *rightKid=nullptr;
        if(this->getLeftChild() != nullptr)
            leftKid = this->getLeftChild()->copyNode(copy, 1, 0);
        
        if(this->getRightChild() != nullptr)
            rightKid = this->getRightChild()->copyNode(copy, 0, 1);
        
        
        return copy;
    }

};

class ExponentialNode : public BinaryOperationNode
{
public:

    ExponentialNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    ExponentialNode(Node * lkid, Node * rkid);
    ExponentialNode(Node * pa, int isLeftKid, int isRightKid);   
    ExponentialNode();
    
    void accept(Visitor* v);

    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new ExponentialNode(newParent, isLeftKid, isRightKid);
        
    
        Node* leftKid = nullptr, *rightKid=nullptr;
        if(this->getLeftChild() != nullptr)
            leftKid = this->getLeftChild()->copyNode(copy, 1, 0);
        
        if(this->getRightChild() != nullptr)
            rightKid = this->getRightChild()->copyNode(copy, 0, 1);
        
        
        return copy;
    }

};


class EqualNode : public BinaryOperationNode
{
public:
    EqualNode(Node * lkid, Node * rkid, Node * pa, int isLeftKid, int isRightKid);
    EqualNode(Node * lkid, Node * rkid);
    EqualNode(Node * pa, int isLeftKid, int isRightKid);
    EqualNode();
    
    
    void accept(Visitor* v);

    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        Node* copy = new EqualNode(newParent, isLeftKid, isRightKid);
        
    
        Node* leftKid = nullptr, *rightKid=nullptr;
        if(this->getLeftChild() != nullptr)
            leftKid = this->getLeftChild()->copyNode(copy, 1, 0);
        
        if(this->getRightChild() != nullptr)
            rightKid = this->getRightChild()->copyNode(copy, 0, 1);
        
        
        return copy;
    }

};


class UnaryOperationNode: public Node
{
private:
    Node * child=nullptr;
    string operation = "";
public:
    UnaryOperationNode();
    
    UnaryOperationNode(Node * pa, int isLeftKid, int isRightKid);
    
    UnaryOperationNode(Node * kid, Node * pa, int isLeftKid, int isRightKid);
    
    virtual ~UnaryOperationNode();
    
    virtual void accept(Visitor* v){}
    void setChild(Node * kid, int childType);
    Node* getChild();
    
    
    int getParentType(){
        return 3;
    }
    virtual Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        
        Node* copy = new UnaryOperationNode(newParent, isLeftKid, isRightKid);
        Node* singleKid = nullptr;
        if(child != nullptr)
            singleKid = child->copyNode(copy, 1, 1);
        
        copy->setChild(singleKid, 3);
        
        return copy;
    }
    
};

class ExpNode : public UnaryOperationNode
{
    private:
        string text = "exp";
        
    public:
    ExpNode(Node * kid, Node * pa, int isLeftKid, int isRightKid);
    
    ExpNode(Node * pa, int isLeftKid, int isRightKid);
    
    ExpNode();
    ~ExpNode();
    string getOperation(){
        return text;
    }
    
    
    void accept(Visitor* v);
    
    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        
        Node* copy = new ExpNode(newParent, isLeftKid, isRightKid);
        Node* singleKid = nullptr;
        if(this->getChild() != nullptr)
            singleKid = this->getChild()->copyNode(copy, 1, 1);
        
        return copy;
    }
    
};


class LnNode : public UnaryOperationNode
{
    private:
        string text = "ln";
        
    public:
    LnNode(Node * kid, Node * pa, int isLeftKid, int isRightKid);
    
    LnNode(Node * pa, int isLeftKid, int isRightKid);
    
    LnNode();
    ~LnNode();
    string getOperation(){
        return text;
    }
    
    void accept(Visitor* v);

    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        
        Node* copy = new LnNode(newParent, isLeftKid, isRightKid);
        Node* singleKid = nullptr;
        if(this->getChild() != nullptr)
            singleKid = this->getChild()->copyNode(copy, 1, 1);
        
        return copy;
    }};


class BracketNode : public UnaryOperationNode
{
    private:
        string text = " ";
        
    public:
    BracketNode(Node * kid, Node * pa, int isLeftKid, int isRightKid);
    
    BracketNode(Node * pa, int isLeftKid, int isRightKid);
    
    BracketNode();
    ~BracketNode();
    string getOperation(){
        return text;
    }
    
    void accept(Visitor* v);

    Node* copyNode(Node* newParent, int isLeftKid, int isRightKid){
        
        Node* copy = new BracketNode(newParent, isLeftKid, isRightKid);
        Node* singleKid = nullptr;
        if(this->getChild() != nullptr)
            singleKid = this->getChild()->copyNode(copy, 1, 1);
        
        return copy;
    }
};



class Visitor
{
private:
    Node* currentParent = nullptr;
public:
    Visitor(){
        //cout << endl << endl << "Visit started" << endl << endl;
        
    }
    virtual ~Visitor(){
       // cout << endl << endl << "Visit finished" << endl;
    }
    
    virtual void visit(NumberNode* numberNode) = 0;
    virtual void visit(VariableNode* variableNode)=0;
    virtual void visit(FunctionNode* functionNode)=0;
    virtual void visit(EqualNode* equalNode)=0;
    virtual void visit(AddNode* addNode)=0;
    virtual void visit(MinusNode* minusNode)=0;
    virtual void visit(MultiplicationNode* multNode)=0;
    virtual void visit(DivisionNode* divNode)=0;
    virtual void visit(ExponentialNode* exponentialNode)=0;
    virtual void visit(ExpNode* expNode)=0;
    virtual void visit(LnNode* lnNode)=0;
    virtual void visit(BracketNode* bracketNode)=0;

    ///*

    //*/
    virtual string getDiffName(){return "";}
    virtual Node* getDiffTree(){return nullptr;}
    
    void setCurrentParent(Node* parent){
        currentParent = parent;
    }
    
    Node* getCurrentParent(){
        return currentParent;
        
    }
    
    
    void sayHello(){
        cout << "Hello World! " << endl;
    }

    /* data */
};


class PrintVisitor: public Visitor{
    string completeResult="";
public:
    PrintVisitor(){
    }
    ~PrintVisitor(){
    }

    
    string getResult(){
        return completeResult;
    }
    
    void addToResult(string moreText){
        completeResult += moreText;
    }
    
    
    void visit(NumberNode* numberNode) override;
    void visit(VariableNode* variableNode) override;
    void visit(FunctionNode* functionNode) override;
    void visit(EqualNode* equalNode) override;
    void visit(AddNode* addNode) override;
    void visit(MinusNode* minusNode) override;
    void visit(MultiplicationNode* multNode) override;
    void visit(DivisionNode* divNode) override;
    void visit(ExponentialNode* exponentialNode) override;
    void visit(ExpNode* expNode) override;
    void visit(LnNode* lnNode) override;
    void visit(BracketNode* bracketNode) override;    
};

class DiffVisitor: public Visitor{
    
    string diffVar;
    
    Node* diffTreeRoot = nullptr;
    
public:
    
    DiffVisitor(string varName): diffVar(varName){
#ifdef DO_DEBUG
        cout << "Starting differentiation of Tree after Variable " << diffVar << endl;
#endif
    }
    ~DiffVisitor(){
#ifdef DO_DEBUG
        cout << endl << endl << "DiffVisit finished" << endl;
#endif
    }
    
    void handleResult(Node* child, Node* parent, int childType);
    
    
    Node* getDiffTree(){
        return diffTreeRoot;
    }
    
    string getDiffName(){
        return diffVar;
    }
    /*
    int getParentType(BinaryOperationNode* parent);
    int getParentType(UnaryOperationNode* parent);
    int getParentType(NumberNode* parent);
    int getParentType(VariableNode* parent);
    int getParentType(FunctionNode* parent);
    */
    void visit(NumberNode* numberNode);
    void visit(VariableNode* variableNode);
    void visit(FunctionNode* functionNode);
    void visit(EqualNode* equalNode);
    void visit(AddNode* addNode);
    void visit(MinusNode* minusNode);
    void visit(MultiplicationNode* multNode);
    void visit(DivisionNode* divNode);
    void visit(ExponentialNode* exponentialNode);
    void visit(ExpNode* expNode);
    void visit(LnNode* lnNode);
    void visit(BracketNode* bracketNode);    
};
/*
class DiffVisitor: public Visitor{
    
    string diffVar;
    
    Node* diffTreeRoot = nullptr;
    
public:
    
    DiffVisitor(string varName): diffVar(varName){
        cout << "Starting differentiation of Tree after Variable " << diffVar << endl;
    }
    
    
    void handleResult(Node* child, Node* parent, int childType);
    
    
    Node* getDiffTree(){
        return diffTreeRoot;
    }
    
    ~DiffVisitor(){
        cout << "Destroying DiffVisitor" << endl;
    }
    
    string getDiffName(){
        return diffVar;
    }
};
*/
class EvaluationVisitor: public Visitor{
    double result=0, tempSingle, tempLeft, tempRight;
    double* currentValueStorage;
    int currentChoice;
    double evalVarValue;
    string evalVar;
    
public:
    EvaluationVisitor(string varName, double value): evalVar(varName), evalVarValue(value){
        //cout << "Starting evaluation of Tree with Variable " << evalVar << endl;
    }
    ~EvaluationVisitor(){
        //cout << endl << endl << "EvaluationVisit finished" << endl;
    }
    void handleResult(double value){
        result += value;
    }
    
    double getResult(){
        return result;
    }
    
    void setCurrentValueStorage(int choice){
        if(choice == 1)
            currentValueStorage = &tempLeft;
        if(choice == 2)
            currentValueStorage = &tempRight;
        if(choice == 3)
            currentValueStorage = &tempSingle;
    }
    
    double getEvalVarValue(){
        return evalVarValue;
    }
    
    double* getCurrentValueStorage(){
        return currentValueStorage;
    }
    
    void setTemporaryValue(double value, int choice){
        if(choice == 1)
            tempLeft=value;
        if(choice == 2)
            tempRight=value;
        if(choice == 3)
            tempSingle=value;        
    }
    
    void setCurrentChoice(int choice){
         if(choice == 1 || choice == 2 || choice == 3)
            currentChoice=choice;
         /*
        if(choice == 2)
            tempRight=value;
        if(choice == 3)
            tempSingle=value;
        */
    }
    
    int getCurrentChoice(){
        return currentChoice;
    }
    
    double getTemporaryValue(int choice){
        double value = 0;
         if(choice == 1)
            value=tempLeft;
        if(choice == 2)
            value=tempRight;
        if(choice == 3)
            value=tempSingle;   
        return value;
    }
    
    double getEvaluationResult(){
        return result;
    }
    
    string getEvalVar(){
        return evalVar;
    }
    
    double getTempSingle(){
        return tempSingle;
    }
    double getTempLeft(){
        return tempLeft;
    }
    double getTempRight(){
        return tempRight;
    }
    
    void setTempSingle(double value){
        tempSingle = value;
    }
    void setTempLeft(double value){
        tempLeft = value;
    }
    void setTempRight(double value){
        tempRight = value;
    }
    
    void visit(NumberNode* numberNode);
    void visit(VariableNode* variableNode);
    void visit(FunctionNode* functionNode);
    void visit(EqualNode* equalNode);
    void visit(AddNode* addNode);
    void visit(MinusNode* minusNode);
    void visit(MultiplicationNode* multNode);
    void visit(DivisionNode* divNode);
    void visit(ExponentialNode* exponentialNode);
    void visit(ExpNode* expNode);
    void visit(LnNode* lnNode);
    void visit(BracketNode* bracketNode);    
    
};


//#include "class.cpp"

#endif /* __CLASS_HPP__ */