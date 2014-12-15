/*****************************************************************************
 Title:             calculator.h
 Author:            Anna Cristina Karingal
 Created on:        Nov 9, 2014
 Description:       Calculator Class Definition (Header File)
                        - Parses an infix expression
                        - Evaluates an infix expression
                        - Receives string that contains the expression,
                            evaluates it and returns its value to the calling
                            program.
 
 Last Modified:     Nov 18, 2014
 
 *****************************************************************************/


#ifndef ___calculator__
#define ___calculator__

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stack>
#include <sstream>
#include <math.h>
#include <ctype.h>
#include <vector>
#include <stdexcept>
#include <cstdlib>
using namespace std;

class calculator {
    
    // A structure to hold an infix expression and it's corresponding result
    struct evaluated_expression{
        string exp;
        float result;
    };
    
    // Vector to store all evaluated expressions
    vector<evaluated_expression> all_expressions;
    
public:
    
/******************************************************************************
    Constructors
 ******************************************************************************/
    
    /* calculator();
     Constructor for calculator that takes no parameters. Initializes
     all_expressions to an empty vector
     Precondition:      none
     Postcondition:     all_expressions is an initialized vector of 0 elements
     */
    calculator();
    
    /* calculator();
     Constructor for calculator that initializes all_expressions from user
     supplied file
        @param  string fName [in]       file name
        @param  ifstream &readf [in]    file stream to read file input from
        @param  ostream &err [out]      output stream to output any errors
     Precondition:      &readf and &err are open and initialized, fName is the
                        name and path of a valid input file of n infix
                        expressions. An evaluated_expression is a data structure
                        containing a single infix expression and its correctly
                        evaluated result.
     Postcondition:     all_expressions is an initialized vector of n
                        n evaluated_expressions. Any invalid infix expressions 
                        are left unevaluated and sent to &err
     */
    calculator(string fName, ifstream &readf, ostream &err=cerr) throw(invalid_argument);
    
/******************************************************************************
     Accessors
******************************************************************************/
    
    /* void add_new(string exp, ostream &err=cerr);
    Stores a given infix expression and its result as the last element in vector
    all_expressions
        @param  string exp[in/out]      infix expression
        @param  ostream &err[in/out]    output stream to output any errors
     Precondition:      &err is initialized and open. exp is non-empty and
                        initialized with a valid infix expression.
                        all_expressions is an initialized vector of n 
                        evaluated_expressions. An evaluated_expression is a data
                        structure containing a single infix expression and its
                        correctly evaluated result.
     Postcondition:     If exp is a valid infix expressions, its result is 
                        correctly evaluated and both are pushed back to 
                        all_expressions as the last element of the vector. 
                        all_expressions now contains n+1 evaluated_expressions. 
                        If exp is not a valid infix expression, an exception is
                        thrown and
     */
    void add_new(string exp, ostream &err=cerr);
    
    /* string get_expression(int exp_id) const;
     Returns the infix expression that is in the (exp_id)th position of the 
    all_expressions vector.
        @param  int exp_id[in]      position of the infix expression to retrieve
        @return  string[out]        infix expression at (exp_id)th position
     Precondition:       An evaluated_expression is a data structure containing
                        an infix expression and its correctly evaluated result.
                        all_expressions is a non-empty, initialized vector of
                        evaluated_expressions of size <= exp_id+1
    Postcondition:      all_expressions[i].exp is returned, all_expressions is
                        unchanged.
    */
    string get_expression(int exp_id) const;
    
    /* float get result (int exp_id) const;
     Returns the result of the infix expression that is in the (exp_id)th
     position of the all_expressions vector.
        @param  int exp_id [in]      position of the result to retrieve
        @return  float [out]         result at the (exp_id)th position
     Precondition:       An evaluated_expression is a data structure containing
                        an infix expression and its correctly evaluated result.
                        all_expressions is a non-empty, initialized vector of
                        evaluated_expressions of size <= exp_id+1
     Postcondition:     all_expressions[i].result is returned, all_expressions
                        is unchanged.
     */
    float get_result(int exp_id) const;
    
    /*  friend ostream &operator << (ostream &os, const calculator &c);
     Overloaded operator friend function that prints out all elements of the
     all_expressions vector with each element printed to the stream on a single
     line as "Result = Expression".
        @param  ostream &os [in/out]    outstream to print output to
        @param  calculator &c [in]      instance of calculator to print
     Precondition:      &os is open and initialized. &c is initialized.
     Postcondition:     Each element of c.all_expressions is printed out to 
                        &os in a single line as "Result = Expression". &c is 
                        unchanged.
     */
    friend ostream &operator << (ostream &os, const calculator &c);
    
/******************************************************************************
     Expression evaluation functions
******************************************************************************/
    
    /*  bool is_operator(char ch);
     Returns true if ch is an operator (i.e. +, -, /, * or ^), else returns
     false.
        @param  char ch [in]     character to evaluate
        @return bool [out]       returns true if ch is an operator, else
                                    returns false
     Precondition:      ch is non-empty and initialized character
     Postcondition:     returns true if ch is either +, -, /, * or ^, else 
                        returns false
     
     */
    bool is_operator(char ch);
    
    /*  int precedence(char op);
     Returns assigned precedence of a given operator
        @param  char ch [in]     character to evaluate
        @return int [out]        returns 1 if ch is + or -, 2 if * or /,
                                    3 if ^.
     Precondition:      ch is non-empty and initialized character of either
                        '+', '-', '*', '/', '(' or '^'
     Postcondition:     Returns assigned precedence value: 1 if ch is + or -,
                        2 if * or /, 3 if ^
     */
    int precedence(char op) throw(invalid_argument);
    
    /*  float execute(stack<float> &v, stack<char> &o);
     Pops the top two values off the value stack v, as operand2 and operand1 (in
     that order) and the top value off the operand stack o as op and returns the
     result of operand1 op operand2.
        @param  stack<float> &v [in/out]    stack of operands
        @param  stack<char> &o [in/out]     stack of operators
        @return float                       result of operation
     Precondition:      &o contains only characters of type '+', '-', '*', '/',
                        '(' or '^' and op can only be of type '+', '-', '*', '/'
                        or '^' (i.e. not a parenthesis). &v contains only valid
                        float values. &o and &v contain on >= 1 and vn >= 2
                        elements respectively. If top of &o=='/', top of &v!=0.
     Postcondition:     Returns result of operation operand1 op operand2 where
                        operand2 and operand1 were the top two values of &v in
                        that order and op was the top value of &o. operand2 and
                        operand1 are popped off &v and op is popped off &o so 
                        &v and &o contain vn-2 and on-1 elements respectively.
     */
    float execute(stack<float> &v, stack<char> &o) throw(underflow_error, invalid_argument);

    /* float evaluate(string exp); 
     Takes an infix expression as a string exp, checks it for validity (e.g. 
     checks for matching parentheses, correct number of operands vs. operators, 
     division by zero errors) and returns the result of its evaluation.
        @param  string exp [in]         string to validate and evaluate
        @param  ostream &err [in/out]   strema to output any errors to
        @return float [out]             result of evaluation of valid string
     Precondition:      exp is a valid infix expression containing only positive
                        decimal numbers, parentheses, white spaces and the
                        operators '+', '-', '*', '/' or '^'. 
     Postcondition:     returns the result of the infix expression if it is 
                        valid, else throws an exception.
     */
    float evaluate(string exp, ostream &err=cerr);
    
};

#endif