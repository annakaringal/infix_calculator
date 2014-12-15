
/*****************************************************************************
 Title:             calculator.cpp
 Author:            Anna Cristina Karingal
 Created on:        Nov 9, 2014
 Description:       Calculator Class Implementation
 
 Last Modified:     Nov 18, 2014
 
 *****************************************************************************/

#include "calculator.h"

/* Calculator constructor that takes no parameters. Initializes empty vector
 for all_expressions.
 */
calculator::calculator(){ }

/* Calculator constructor that attempts to initialize all_expressions vector
 from user given file. Reads expression, tries to calculate expression result. 
 If successful, stores both expression and result in vector. If fails and
 expression is invalid, prints only expression to error stream.
 */
calculator::calculator(string fName, ifstream &readf, ostream &err) throw(invalid_argument){
    
    readf.open(fName.c_str());
    
    // Throw an exception if file is invalid and cannot open
    if (readf.fail()){
        throw invalid_argument(fName);
    }
    
    if (readf.is_open()) {
        while (!readf.eof()) {
            // Read line
            evaluated_expression ee;
            getline(readf, ee.exp);
            
            try {
                // Attempt to evaluate expression
                ee.result = evaluate(ee.exp);
                
                // No exception thrown, add to vector
                all_expressions.push_back(ee);
            }
            catch(...) {
                // Failed to evaluate result, print to error stream instead
                err << ee.exp << endl;
            }
            
        }
    }
    
    // Close file
    readf.close();
    
}

/* Returns infix expression at exp_id */
string calculator::get_expression(int exp_id) const{
    return all_expressions[exp_id].exp;
}

/* Returns result of infix expression a exp_id */
float calculator::get_result(int exp_id) const {
    return all_expressions[exp_id].result;
}

/* Adds a given to the all_expressions vector as the last element of the vector.
 Reads expression, tries to calculate expression result. If successful, stores
 both expression and result in vector. If fails and expression is invalid,
 prints only expression to error stream and all_expressions is unchanged.
 */
void calculator::add_new(string exp, ostream &err) {
    evaluated_expression ee;
    
    ee.exp = exp;
    ee.result = evaluate(ee.exp);
    all_expressions.push_back(ee);
}

/* Compares character to list of known operators, returns true if ch == operator
 else returns false .
 */
bool calculator::is_operator(char ch) {
    
    if (ch == '+' || ch == '-' || ch == '/' || ch == '*' || ch == '^') {
        return true;
    }
    else {
        return false;
    }
}


/* Returns assigned precedence value of a given operator character. Throws
 exception if character is not a known operator.
 */
int calculator::precedence(char op) throw(invalid_argument){
    
    if (op == '(') {
        return 0;
    }
    else if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    else if (op == '^') {
        return 3;
    }
    else { // Character is not an operator
        throw invalid_argument(&op);
    }
}

/* Pops the top two values off the value stack v, as operand2 and operand1 (in
 that order) and the top value off the operator stack o as op and return the
 result of the operation operand1 op operand2.
 */
float calculator::execute(stack<float> &v, stack<char> &o) throw(underflow_error, invalid_argument) {
    
    float operand1, operand2;
    char op;
    
    // If no operand to pop, throw exception
    if (v.empty()) { throw underflow_error("Trying to pop an empty stack"); }
    else {
        operand2 = v.top();
        v.pop();
    }
    
    // If no operands or operators to pop, throw exception
    if (v.empty() || o.empty()) { throw underflow_error("Trying to pop an empty stack"); }
    else {
        operand1 = v.top();
        v.pop();
        op = o.top();
        o.pop();
    }
    
    // Perform the operation operand1 op operand2
    switch (op) {
        case '+':
            return operand1 + operand2;
        case '-':
            return operand1 - operand2;
        case '*':
            return operand1 * operand2;
        case '/':
            // Divide by zero error
            if (operand2 == 0){
                throw invalid_argument("Attempt to divide by zero");
            }
            else { return operand1 / operand2; }
        case '^':
            return pow(operand1, operand2);
        default:
            // invalid operator, throw exception
            throw invalid_argument("Operator character expected");
    }
    
}

/* Takes an infix expression as a string exp, checks it for validity (e.g.
 checks for matching parentheses, correct number of operands vs. operators,
 division by zero errors) and returns the result of its evaluation. Does this
 using two stacks: one for operand values and one for operators. Reads each
 character of the string and determines whether it is a decimal value (or part
 of one), a parenthesis or an operator '+', '-', '*', '/', '^'.
    If a set of characters is a float number containing only digits and no more
 than one decimal point, it is pushed to the value stack.
    If a character is an operator, it is pushed to the operator stack if and
 only if the character already on the top of the operator stack is of lesser
 precedence. If the character on the top of the stack is of higher precedence,
 an operation is "executed", i.e. performed with this character on the top of
 the operator stack and the top two values of the value stack and all three
 elements are popped off their respective stacks.
    If a character is a right parenthesis, it is popped onto the operator stack.
 If a character is a left parenthesis, operations are executed until the top of
 the operator stack contains a rigth parenthesis, in which case it is popped and
 we continue reading the rest of the string.
    Once the string is read, executions are performed until the operator stack 
 is empty and the value stack contains only a single value. This value is what
 is returned by the function.
 */
float calculator::evaluate(string exp, ostream &err){
    // Use two stacks: one for valid operators, one for valid float values
    stack<char> opStack;
    stack<float> valStack;
    
    // For each character in exp
    for (int i = 0; i<exp.length(); i++) {
        
        // If char is an operand,
        // Push digits and decimal point to value stack as a single float
        if (isdigit(exp[i])){
            
            stringstream opss;
            opss << exp[i];
            
            int j = 1;
            int count_decimal = 0;
            
            // Check next characters to see if they are part of the single float
            while (isdigit(exp[i+j]) || exp[i+j] == '.'){
                opss << exp[i+j];
                
                // VALIDITY CHECK: make sure only one decimal point in float
                if (exp[i+j] == '.') {
                    count_decimal ++;
                    if (count_decimal > 1) {
                        throw invalid_argument("Too many decimal points");
                    }
                }
                
                j++;
            }
            
            // Push valid float to stack
            float operand;
            opss >> operand;
            try {
                valStack.push(operand); }
            catch (...) {
                // If push to stack fails, exit with errors
                err << "Unable to allocate memory" << endl;
                exit(-1);
            }
            
            // Increment i so next iteration checks next character after float
            i = i + j-1 ;
        }
        
        // If char is a right parenthesis, push to operator stack
        else if (exp[i] == '(') {
            try {
                opStack.push(exp[i]);
            }
            catch (...) {
                // If push to stack fails, exit with errors
                err << "Unable to allocate memory" << endl;
                exit(-1);
            }

        }
        
        // If char is an operator (non-parentheses)
        else if (is_operator(exp[i])){
            
            // If operator stack is empty, push operator on top
            if (opStack.empty()) {
                
                try {
                    opStack.push(exp[i]);
                }
                catch (...) {
                    // If push to stack fails, exit with errors
                    err << "Unable to allocate memory" << endl;
                    exit(-1);
                }

            }
            
            // If stack is non-empty, if operator has greater precedence
            // than top of stack, push operator on top
            else if (precedence(exp[i]) > precedence(opStack.top())){
                
                try {
                    opStack.push(exp[i]);
                }
                catch (...) {
                    // If push to stack fails, exit with errors
                    err << "Unable to allocate memory" << endl;
                    exit(-1);
                }

            }
            
            // If operator has lower precedence, perform operations with higher
            // precedence until operator has highest precedence
            else {
                while (!opStack.empty() && precedence(exp[i]) <= precedence(opStack.top())){
                    float result = execute(valStack, opStack);
                    
                    try {
                        valStack.push(result);
                    }
                    catch (...) {
                        // If push to stack fails, exit with errors
                        err << "Unable to allocate memory" << endl;
                        exit(-1);
                    }
                }
                
                // Push operator onto stack
                try {
                    opStack.push(exp[i]);
                }
                catch (...) {
                    // If push to stack fails, exit with errors
                    err << "Unable to allocate memory" << endl;
                    exit(-1);
                }

            }
        }
        
        // If char is a left parenthesis, perform operation with top two values
        // in value stack and top operator in operator stack, pop those off
        // stacks and push result back to value stack.
        // Do this until matching right parenthesis is reached.
        
        else if (exp[i] == ')'){
            while (!opStack.empty() && opStack.top()!= '('){
                float result = execute(valStack, opStack);
                try {
                    valStack.push(result);
                }
                catch (...) {
                    // If push to stack fails, exit with errors
                    err << "Unable to allocate memory" << endl;
                    exit(-1);
                }
            }
            // If end of stack reached and no matching right parenthesis is
            // found, expression is invalid.
            if (opStack.empty()) { throw invalid_argument("No matching parenthesis"); }
            else {
                // Top of opStack is a right parenthesis
                // Pop right parenthesis off stack
                opStack.pop();
            }
        }
        
        // White space is a valid character, but does nothing
        else if (exp[i] == ' ') { }
        
        // Invalid character, i.e. nonoperand and nonoperator, throw exception
        else {
            throw invalid_argument("Invalid character");
        }
        
    }
    
    // Finished reading entire exp string
    // Pop operator and top two operands off stacks and perform operands
    // Push result back onto value stack
    // Do this until we have no more operators to use
    while (!opStack.empty()){
        float result = execute(valStack, opStack);
        try {
            valStack.push(result);
        }
        catch (...) {
            // If push to stack fails, exit with errors
            err << "Unable to allocate memory" << endl;
            exit(-1);
        }
    }
    
    // Top of the value stack is the result of all executions
    // Check to see if it's the last value in the stack, else throw exception
    float final_result;
    if (valStack.size()!=1) { throw invalid_argument("Invalid number of operands"); }
    else {
        final_result = valStack.top();
        valStack.pop(); // Stack is now empty
    }
    
    // Everything looks good, return final result.
    return final_result;
    
}


/* Friend function to the class that displays all expressions and their results
 in a formatted, user-friendly manner to the console by manipulating the output
 stream. No expressions and their results are changed.
 */

ostream &operator << (ostream &os, const calculator &c){
    
    for (int i=0; i<c.all_expressions.size(); i++) {
        os << fixed << setprecision(2)  << c.get_result(i) << " = " << c.get_expression(i) << endl;
    }
    
    return os;
}