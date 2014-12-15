/*******************************************************************************
 Title          :   main.cpp
 Author         :   Anna Cristina Karingal
 Created on     :   Nov 9, 2014
 
 Description    :   A calculator that evaluates lines of ordinary infix
                    expressions with positive, fixed-decimal operands and
                    operators +, -, *, / and ^ as well as parentheses.
                    Note that the operator ^ is left associative in this
                    program.
 Purpose        :   To demonstrate usage of the stl::stack template class and
                    C++ exception handling
 
 Usage          :   ./calculator myFile.txt command2>error
                                OR
                    ./calculator command2>error
 
                    Where myFile.txt is a valid text file of infix expressions, 
                    with one expression on each line and error is the name of
                    the error log file. If no file name is given, user enters
                    infix expressions via the command line
 
 Build with     :   g++ -o calculator main.cpp calculator.cpp
 
 Last modified  :   Nov 18, 2014
 
 *******************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

#include "calculator.h"
using namespace std;

/******************************************************************************
                                MAIN PROGRAM
 ******************************************************************************/

int main(int argc, const char * argv[]) {
    
 if (argc == 3) { // Input file given as argument in command line
        
        string fName = argv[1];
        ifstream readf;
        
        try {
            // Create new calculator instance from input file
            // Reads and evaluates all expressions in put file
            calculator calc(fName.c_str(), readf);
            
            // Print valid expressions and their results to command line
            cout << "\nRESULTS: " << endl;
            cout << "==============================================================="<< endl;
            cout << calc;
        }
        catch (const invalid_argument& e ) {
            
            // Unable to open file. Exit with errors
            cerr << "Unable to open file " << e.what() << endl;
            exit(1);
        }
        

    }
    else if (argc < 3){ // No input file given
        string e;
        calculator calc;
        
        // Get user input from command line until end of file char is reached
        while(!getline(cin,e).eof()) {
            //getline(cin, e);
            
            try {
                // Add each expression to the calculator class and evaluate
                calc.add_new(e);
            }
            catch(exception &except) {
                // If an expression is invalid, output to error stream instead
                cerr << e << endl;
            }
        }
        
        // Prints all valid expressions and their results to command line
        cout << "\nRESULTS: " << endl;
        cout << "==============================================================="<< endl;
        cout << calc;
     
    }
    else { // Error: Too many arguments. Exit with errors
        cerr << "ERROR: Too many arguments passed." << endl;
        exit(1);
    }
    
    return 0;
}
