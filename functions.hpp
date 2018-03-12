 
#ifndef FUNCTIONS_HPP__
#define FUNCTIONS_HPP__

#include <iostream>
#include <cstring>
#include <regex>
#include <set>
#include "class.hpp"
//#define DO_DEBUG2


using namespace std;

void remove_spaces (char* str_trimmed, const char* str_untrimmed);

void setChild(Node* child, Node* parentToBe, int isLeftChild, int isRightChild, set<string>* setOfVariables);

void makeTreeLeaves(int start, int end, Node * parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables);

void buildSubTreePow(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables);

void buildSubTreeDiv(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables);

void buildSubTreeMult(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables);

void buildSubTreeMinus(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables);

void buildSubTreePlus(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables);

Node* start_tree_build(int start, int end, char *_exp, set<string>* setOfVariables, string* functionName);

void insertVariableIntoSet(string varName, set<string>* setOfVariables);

string generateCfunction(Node* tree, set<string>* setOfVariables, string functionName);

string generateCJacobian(Node* tree, set<string>* setOfVariables, string functionName);

//#include "functions.cpp"
#endif //FUNCTIONS_HPP__