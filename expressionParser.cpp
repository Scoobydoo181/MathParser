#include <bits/stdc++.h>

#include "./expressions.h"
#include "./parserTree.h"


using namespace std;

constexpr unsigned int strSwitch(const char *str, int h = 0)
{
    return !str[h] ? 5381 : (strSwitch(str, h + 1) * 33) ^ str[h];
}

bool isDigit(char ch)
{
    return int(ch) >= 48 && int(ch) <= 57 || ch == '.';
}

bool isBinOp(char ch)
{
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

bool isNumber(string input)
{
    for (char ch : input)
        if (!isDigit(ch))
            return false;

    return true;
}

string validateAdjacentOperators(char left, char right)
{
    //Invalid sequences: "( +",      "* -",        "^ )"
    if (isBinOp(left) && (isBinOp(right) || right == ')') || isBinOp(right) && (left == '('))
        throw invalid_argument("Invalid adjacent operators: " + string(1, left) + " and " + right);

    return string(1, right);
}

vector<string> tokenize(string input)
{
    vector<string> tokens;

    string number = "";
    for (char ch : input)
    {
        if (isDigit(ch)) //It's a digit, add it to the cache and move on
        {
            number.push_back(ch);
            continue;
        }

        if (number.size() > 0) //The number has reached the end, push and reset the cached number
        {
            tokens.push_back(number);
            number = "";
        }

        if (ch == ' ')
        {
            continue;
        }
        else if (!tokens.size()) //It's the first character and it's not a digit
        {
            if (ch == ')' || isBinOp(ch))
                throw invalid_argument("Cannot start expression with a " + ch);

            tokens.push_back(string(1, ch));
        }
        else //It's an operator or parenthesis
        {
            tokens.push_back( validateAdjacentOperators(tokens.back()[0], ch) );
        }
    }

    if (number.size() > 0) //If digit is last character, clean up cached number
    {
        tokens.push_back(number);
        number = "";
    }

    return tokens;
}

ParserTree* parseTokens(vector<string> tokens)
{
    ParserTree* expressions = new ParserTree();
    for (string token : tokens)
    {
        if (token == "(")
        {
            expressions->push(new ParentheticalExpression());
            expressions->incrementParenthesis();
        }
        else if (token == ")")
        {
            expressions->decrementParenthesis();
        }
        else if (isNumber(token))
        {
            expressions->push(new ConstantExpression(stod(token)));
        }
        else
        {
            switch (strSwitch(token.c_str()))
            {
            case strSwitch("+"):
                expressions->push(new AdditionExpression());
                break;
            case strSwitch("-"):
                expressions->push(new SubtractionExpression());
                break;
            case strSwitch("*"):
                expressions->push(new MultiplicationExpression());
                break;
            case strSwitch("/"):
                expressions->push(new DivisionExpression());
                break;
            case strSwitch("^"):
                expressions->push(new ExponentiationExpression());
                break;
            }
        }
    }
    return expressions;
}

double parse(string input) 
{
    vector<string> tokens = tokenize(input);

    ParserTree* tree = parseTokens(tokens);
    //cout << tree->root->display() << endl;
    double output = tree->root->evaluate();
    delete tree;

    return output;
}

int main()
{
    cout << "Testing " << "(0.5+0.5)= " << parse("(0.5+0.5)") << endl;
    cout << "Expected answer: 1\n" << endl;

    cout << "Testing " << "(2)+(2)= " << parse("(2)+(2)") << endl;
    cout << "Expected answer: 4\n" << endl;
    
    cout << "Testing " << "((2)+(2))= " << parse("((2)+(2))") << endl;
    cout << "Expected answer: 4\n" << endl;

    cout << "Testing " << "(2 + 2) * 3= " << parse("(2 + 2) * 3") << endl;
    cout << "Expected answer: 12\n" << endl;

    cout << "Testing " << "2+(5+9) =\n" << parse("2+(5+9)") << endl;
    cout << "Expected answer: 16\n" << endl;

    cout << "Testing " << "3 =\n" << parse("3") << endl;
    cout << "Expected answer: 3\n" << endl;

    cout << "Testing " << "3+2*4^2-4 =\n" << parse("3+2*4^2-4") << endl;
    cout << "Expected answer: 31\n" << endl;

    cout << "Testing " << "4^2*3+5/2 =\n" << parse("4^2*3+5/2") << endl;
    cout << "Expected answer: 50.5\n" << endl;

    cout << "Testing " << "3+2*4^2-4*2 =\n" << parse("3+2*4^2-4*2") << endl;
    cout << "Expected answer: 27\n" << endl;

    cout << "Testing " << "1+3-4/2*3+4*5 =\n" << parse("1+3-4/2*3+4*5") << endl;
    cout << "Expected answer: 18\n" << endl;

    cout << "Testing " << "4/5*6+6-3-25*14/(25+36*24)-24^2-4*24/2+31-9*45^3/14 =\n" << parse("4/5*6+6-3-25*14/(25+36*24)-24^2-4*24/2+31-9*45^3/14") << endl;
    cout << "Expected answer: -59165.9508436\n" << endl;

    cout << "Testing " << "2+(3*4)*5 =\n" << parse("2+(3*4)*5") << endl;
    cout << "Expected answer: 62\n" << endl;

    cout << "Testing " << "(2)*(3)/(4) =\n" << parse("(2)*(3)/(4)") << endl;
    cout << "Expected answer: 1.5\n" << endl;

    cout << "Testing " << "(16-3)*(9 + 48^3)/2 =\n" << parse("(16-3)*(9 + 48^3)/2") << endl;
    cout << "Expected answer: 718906.5\n" << endl;
    return 0;
}