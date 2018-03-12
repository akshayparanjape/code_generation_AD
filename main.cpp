#include <iostream>
#include <fstream>
#include <cstring>
#include <regex>
#include "class.hpp"
#include "functions.hpp"

#include<set>

using namespace std;




// TODO : Different types of pointers -  shared and unique. Get to know them

// TODO : Implement Andrea 's regex here

	
int main(int argc, char **argv)
{
	
	char *_expression =  argv[1];

        
        ofstream myfile;
        myfile.open ("modelicaModel.cpp", ios::trunc);
        myfile << "#include <iostream>\n#include <fstream>\n#include <cstring>\n#include <stdlib.h>\n#include <cmath>\n\nusing namespace std; \n\n";
        

    
	
	char cut_expression[200];
        
        set<string> setOfVariables;
        
        remove_spaces(cut_expression,_expression);
	int size = strlen(cut_expression);
	string functionName;
	cout<< "Received input " << cut_expression << " of size " << size<<endl;
	Node* tree = start_tree_build(0,size-1,cut_expression, &setOfVariables, &functionName);
        
        //cout << "Building the Tree is finished!\n";
        
        //Visitor * printer = new PrintVisitor();
        
        //tree->accept(printer);
        /*
        string vName = "x";
        Visitor* differ = new DiffVisitor(vName);
        tree->accept(differ);
        
        Node* diffTree = differ->getDiffTree();
        cout << endl << endl;
        diffTree->accept(printer);
        cout << endl << endl << endl << endl;
        
        Visitor* secondDiffer = new DiffVisitor(vName);
        diffTree->accept(secondDiffer);
        Node* diffDiffTree = secondDiffer->getDiffTree();
        diffDiffTree->accept(printer);
        
        cout << endl << endl << endl << endl;
        
        
        Node* kid = tree->getRightChild();
        
        //TESTING: exp node:
        
        Node* equal = new EqualNode();
        Node* f = tree->getLeftChild()->copyNode(equal, 1, 0);
        Node* exp = new ExpNode(equal, 0, 1);        
        buildSubTreePlus(4,size-1, exp, cut_expression, 1, 1, &setOfVariables);
        
        equal->acceptPrintVisitor(printer);
        
        cout << endl << endl;
        equal->accept(differ);
        diffTree = differ->getDiffTree();
        diffTree->accept(printer);
        */
        cout << endl << endl;
        
        EvaluationVisitor* evaluator = new EvaluationVisitor("x",2.0);
        
        //tree->accept(printer);
        tree->accept(evaluator);
        
        //cout << endl;
        //cout << evaluator->getResult();
        //cout << endl;
        
        
        string jacobianFunction = generateCJacobian(tree, &setOfVariables, functionName);
        cout << "\n==================================================\n\n";
        string cFunction = generateCfunction(tree, &setOfVariables, functionName);
        
        cout << cFunction;
        
        cout << "\n==================================================\n";
        cout << jacobianFunction;
        
        myfile << cFunction << endl;        
        myfile << jacobianFunction << endl;

        myfile << "int main(int argc, char **argv)\n{";
       
        myfile << "\tchar *myNumber =  argv[1];" << endl;
        
        myfile << "\tstring number = \"\";" << endl;
        myfile << "\tnumber += myNumber;" << endl;
        myfile << "\tdouble value = atof(myNumber);" << endl ;
        
        string inputVariables1 = "";
        string outputVariables = "";
        string coutVariables = "";
        int i=0; size = setOfVariables.size()-1;
        for ( set<string>::iterator it=setOfVariables.begin(); it!=setOfVariables.end(); ++it){
	    myfile << "\tdouble result" << i << ";\n"; 
            outputVariables += "result";
            outputVariables += to_string(i);
            coutVariables += "result";
            coutVariables += to_string(i);
            if(i < size){
                outputVariables += ", ";
                coutVariables += "<< endl << ";
            }
            i++;
            inputVariables1 += ", value ";  
            
            
        }
    
        
        myfile << "\tSiSc_function(result0" << inputVariables1 << ");" << endl << "\tcout << result0 << endl;" << endl;
        myfile << "\tSiSc_Jacobian(" << outputVariables << inputVariables1 << ");" << endl << "\tcout <<" << coutVariables << "<< endl;" << endl;
        
        myfile << "\treturn 0; \n}" << endl;
        
         myfile.close();       
        //delete printer;
        delete tree;
        //delete diffTree;
        //delete diffDiffTree;
        
	return 0;
}
