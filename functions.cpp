
#ifndef FUNCTIONS_CPP__
#define FUNCTIONS_CPP__


#include <iostream>
#include <cstring>
#include <regex>
#include "functions.hpp"

#include "class.hpp"


using namespace std;

//int DEBUG2 = 0;
 
void remove_spaces (char* str_trimmed, const char* str_untrimmed)
{
  while (*str_untrimmed != '\0')
  {
    if(!isspace(*str_untrimmed))
    {
      *str_trimmed = *str_untrimmed;
      str_trimmed++;
    }
    str_untrimmed++;
  }
  *str_trimmed = '\0';
}

// only for expression with + and * of type    y = exp


void setChild(Node* child, Node* parentToBe){
    
    int childType = child->getParentRelation();
    #ifdef DO_DEBUG
        cout << "Received child of type " << childType << endl;
    #endif
    
    if(childType>0)
        parentToBe->setChild(child,childType);
}

void insertVariableIntoSet(string varName, set<string>* setOfVariables){
    setOfVariables->insert(varName);
}

string generateCfunction(Node* tree, set<string>* setOfVariables, string functionName){
    string result = "void SiSc_function(double &";//functionName is the name of the function of the tree

    result += functionName;
    for ( set<string>::iterator it=setOfVariables->begin(); it!=setOfVariables->end(); ++it){
	    
        result += ", const double ";
        result += *it;
        //cout << ' ' << *it;    
            
    }
    
    result += "){\n\t";
    
    PrintVisitor* printer = new PrintVisitor();
    //cout << result;
    //printer->getFunctionFile() << result;
    tree->accept(printer);
    

    //cout << ";\n}";
    
    //printer->getFunctionFile() << ";\n}";
    result += printer->getResult();
    
    result +=";\n}"; 
    //cout <<endl<< result << endl;
    delete printer;
    return result;
}

string generateCJacobian(Node* tree, set<string>* setOfVariables, string functionName){
    
    //void SiSc_Jacobian(double &dy_dx1, double &dy_dx2, const double x1, const double x2){
    //  dy_dx1 = x1;
    //  dy_dx2 = 2*x2;
    //}
      //cout << "generating C-function of Jacobian" << endl;  
    string result = "void SiSc_Jacobian(";
    int numberOfParameters = setOfVariables->size();
    if(numberOfParameters>0){
        // the first entry is always the string corresponding to functionName of the FunctionNode
        string diffFunctionName = "d";
        diffFunctionName += functionName;
        diffFunctionName += "_d";
        set<string>::iterator it=setOfVariables->begin();
        for ( ; it!=setOfVariables->end(); ++it){
            
            result += "double &";
            result += diffFunctionName;
            result += *it;
            result += ", ";
            //cout << ' ' << *it;    
                
        }
        
        it=setOfVariables->begin();
        // to switch the comma from after to before the parameter we need to do the first variable by hand:
        result += "const double ";
        result += *it;
        ++it;
        // Now, proceed with <comma> <parameter_type> <parameter_name>:
        for ( ; it!=setOfVariables->end(); ++it){
                
            result += ", const double ";
            result += *it;
            //cout << ' ' << *it;    
                
        }    
        result += "){\n";
        
        for(it=setOfVariables->begin(); it!=setOfVariables->end(); ++it){
            //cout << endl << *it << endl;
            DiffVisitor* differ = new DiffVisitor(*it);
            tree->accept(differ);
            Node* diffTree = differ->getDiffTree();
            PrintVisitor* printer = new PrintVisitor();
            diffTree->accept(printer);
            //cout << "\t" << *it << endl;
            result += "\t";
            result += printer->getResult();
            result +=";\n"; 
            
            delete differ;
            delete printer;
            delete diffTree;
        }    
        
        result += "}\n";
        
    }else{
        result += "){\n\t//Function does not depend on Variables!!!\n}\n";
    }
//for (std::set<std::string>::iterator it=setOfVariables->begin(); it!=setOfVariables->end(); ++it)
//	    std::cout << ' ' << *it;

    return result;
}

void makeTreeLeaves(int start, int end, Node * parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables){

    char myExp[50];
    strncpy(myExp,&_exp[start],end-start+1);
    myExp[end-start+1] = '\0';
    #ifdef DO_DEBUG2
        if(isLeftChild != 0 && isRightChild == 0)

            cout << "asked to make left tree leave from " << myExp << endl;    //Test for variableName
        else if(isLeftChild == 0 && isRightChild != 0)
            cout << "asked to make right tree leave from " << myExp << endl;
        else if(isLeftChild != 0 && isRightChild != 0)
            cout << "asked to make single tree leave from " << myExp << endl;
#endif        
    if (regex_match (myExp, std::regex("[A-Za-z]+[1-9]*"))){//[A-Za-z]{1,*}"))){
        Node* child = new VariableNode(myExp, end-start+1, parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);
        
        string varName = myExp;
        
        insertVariableIntoSet(varName, setOfVariables);
        
     //Test for number   
    }else if(regex_match (myExp, std::regex("[0-9]+(\\.[0-9]+)?"))){//"[0-9]{1,*}"))){
        double number = atof(myExp);
        Node* child = new NumberNode(number, parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);
    }else{
        #ifdef DO_DEBUG2
            cout << "\t Neither number nor node found from input " << myExp << endl;
#endif
    }

}


void buildSubTreePow(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables){		
    
    int flag = 0;
    int i;
    
    for(i = start;i<=end;i++){

        if (_exp[i] == '^' )
        {flag = 1;
        break;}
    }
    
    if (flag == 1)
    {	
        Node* child = new ExponentialNode(parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);

        buildSubTreePow(i+1, end,child,_exp, 0, 1, setOfVariables); //right side
        buildSubTreePow(start,i-1,child,_exp, 1, 0, setOfVariables); //left side
    }
    else if (flag == 0)
        makeTreeLeaves(start, end,parentToBe,_exp, isLeftChild, isRightChild, setOfVariables);
}


void buildSubTreeDiv(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables){		
    
    int flag = 0;
    int i;
    
    for(i = start;i<=end;i++){

        if (_exp[i] == '/' )
        {flag = 1;
        break;}
    }
    
    if (flag == 1)
    {	
        Node* child = new DivisionNode(parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);

        buildSubTreeDiv(i+1, end,child,_exp, 0, 1, setOfVariables); //right side
        buildSubTreeDiv(start,i-1,child,_exp, 1, 0, setOfVariables); //left side
    }
    else if (flag == 0)
        buildSubTreePow(start, end,parentToBe,_exp, isLeftChild, isRightChild, setOfVariables);
}



void buildSubTreeMult(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables){		
    
    int flag = 0;
    int i;
    
    for(i = start;i<=end;i++){

        if (_exp[i] == '*' )
        {flag = 1;
        break;}
    }
    
    if (flag == 1)
    {	
        Node* child = new MultiplicationNode(parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);

        buildSubTreeMult(i+1, end,child,_exp, 0, 1, setOfVariables); //right side
        buildSubTreeMult(start,i-1,child,_exp, 1, 0, setOfVariables); //left side
    }
    else if (flag == 0)
        buildSubTreeDiv(start, end,parentToBe,_exp, isLeftChild, isRightChild, setOfVariables);
}



void buildSubTreeMinus(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables){
    int flag = 0;
    int i;
    
    for(i = start;i<=end;i++){

        if (_exp[i] == '-')
        {flag = 1;
        break;}
    }
    
    if (flag == 1)
    {	
        Node* child = new MinusNode(parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);

        buildSubTreeMinus(i+1, end,child,_exp, 0, 1, setOfVariables); //right side
        buildSubTreeMinus(start,i-1,child,_exp, 1, 0, setOfVariables); //left side
    }
    else if (flag == 0)
        buildSubTreeMult(start, end,parentToBe,_exp, isLeftChild, isRightChild, setOfVariables);

}

void buildSubTreePlus(int start, int end, Node* parentToBe, char *_exp, int isLeftChild, int isRightChild, set<string>* setOfVariables){
    int flag = 0;
    int i;
    
    for(i = start;i<=end;i++){

        if (_exp[i] == '+' )//|| '-')
        {flag = 1;
        break;}
}
    
    if (flag == 1)
    {	
        Node* child = new AddNode(parentToBe, isLeftChild, isRightChild);
        setChild(child, parentToBe);
        buildSubTreePlus(i+1, end,child,_exp, 0, 1, setOfVariables); //right side
        buildSubTreePlus(start,i-1,child,_exp, 1, 0, setOfVariables); //left side
    }
    else if (flag == 0)
        buildSubTreeMinus(start, end,parentToBe,_exp, isLeftChild, isRightChild, setOfVariables);

}



Node* start_tree_build(int start, int end, char *_exp, set<string>* setOfVariables, string* functionName){
    int flag = 0;
    int i;
    
    for(i = start;i<=end;i++){
    
        if (_exp[i] == ':' && i<end)
            if(_exp[i+1] == '=')
            {
                flag = 1;
                break;
            }
    }
    
    if(flag ==1){
        
        Node* treeRoot = new EqualNode();
        Node* function = new FunctionNode(_exp, i, treeRoot, 1, 0);
        setChild(function, treeRoot);

        char fName[90];
        string protoFunctionName;
        if(i<90){
            strncpy(fName,_exp,i);
            fName[i] = '\0';
        }else{
            fName[0] = 'f';
            fName[1] = '\0';
        }
        protoFunctionName = "";
        protoFunctionName += fName;
        *functionName = protoFunctionName;
        //setOfVariables->insert(functionName);
    
        
        char rightExp[200];
        
        
        strcpy(rightExp,&_exp[i+2]);
        
        //cout << rightExp << endl;
        
        buildSubTreePlus(0, strlen(rightExp)-1, treeRoot, rightExp, 0, 1, setOfVariables);
        return treeRoot;
        
    }else{
        cout << "No Function found. \n";
        return nullptr;
    }
}


#endif //FUNCTIONS_CPP__
