#pragma once
#include <bits/stdc++.h>

using namespace std;

class Expression
{
    public:
        Expression* rhs = nullptr;

        virtual double evaluate() = 0;
        virtual int priorityLookup() = 0;

        Expression* right(Expression* rhs = nullptr) 
        {
            Expression* oldRHS = this->rhs;
            if(rhs) //If you pass an argument, assume you want to set rhs, else just get rhs
                if(oldRHS) //If rhs has already been set, set its rhs recursively, stopping at the first unset rhs, else just set rhs
                    this->rhs->right(rhs);
                else
                    this->rhs = rhs;
            
            return oldRHS;
        }

        bool operator>(Expression* rhs)
        {
            return this->priorityLookup() > rhs->priorityLookup(); 
        }
        bool operator>=(Expression* rhs)
        {
            return this->priorityLookup() >= rhs->priorityLookup(); 
        }
        bool operator<(Expression* rhs)
        {
            return this->priorityLookup() < rhs->priorityLookup(); 
        }
        bool operator<=(Expression* rhs)
        {
            return this->priorityLookup() <= rhs->priorityLookup(); 
        }
        bool operator==(Expression* rhs)
        {
            return this->priorityLookup() == rhs->priorityLookup(); 
        }

        virtual ~Expression() 
        {
            delete rhs;
        }
};

class BinaryExpression : public Expression 
{
    public:
        Expression* lhs = nullptr;

        virtual Expression* left(Expression* lhs=nullptr)
        {
            Expression *oldLHS = this->lhs;
            if (lhs)        //If you pass an argument, assume you want to set lhs, else just get lhs
                this->lhs = lhs;

            return oldLHS;
        }

        ~BinaryExpression()
        {
            delete lhs;
        }
};

class AdditionExpression : public BinaryExpression
{
    public:
        AdditionExpression(Expression* lhs= nullptr, Expression* rhs= nullptr)
        {
            this->lhs = lhs;
            this->rhs = rhs;
        }

        int priorityLookup()
        {
            return 1;
        }

        double evaluate()
        {
            cout << "Evaluating Addition" << endl;
            return lhs->evaluate() + rhs->evaluate();
        }
};

class SubtractionExpression : public BinaryExpression
{
    public:
        SubtractionExpression(Expression* lhs=nullptr, Expression* rhs= nullptr)
        {
            this->lhs = lhs;
            this->rhs = rhs;
        }

        int priorityLookup()
        {
            return 1;
        }

        double evaluate() 
        {
            cout << "Evaluating Subtraction" << endl;
            return lhs->evaluate() - rhs->evaluate();
        }
};

class MultiplicationExpression : public BinaryExpression
{
    public:
        MultiplicationExpression(Expression* lhs=nullptr, Expression* rhs= nullptr)
        {
            this->lhs = lhs;
            this->rhs = rhs;
        }

        int priorityLookup()
        {
            return 2;
        }

        double evaluate() 
        {
            cout << "Evaluating Multiplication" << endl;
            return lhs->evaluate() * rhs->evaluate();
        }
};

class DivisionExpression : public BinaryExpression
{
    public:
        DivisionExpression(Expression* lhs=nullptr, Expression* rhs= nullptr)
        {
            this->lhs = lhs;
            this->rhs = rhs;
        }

        Expression* right(Expression* rhs= nullptr) 
        {
            if(rhs)
                this->rhs = rhs;

            return this->rhs;
        }

        int priorityLookup()
        {
            return 2;
        }

        double evaluate() 
        {
            cout << "Evaluating Division" << endl;
            return lhs->evaluate() / rhs->evaluate();
        }
};

class ExponentiationExpression : public BinaryExpression
{
    public:
        ExponentiationExpression(Expression* lhs=nullptr, Expression* rhs= nullptr)
        {
            this->lhs = lhs;
            this->rhs = rhs;
        }

        int priorityLookup()
        {
            return 3;
        }

        double evaluate() 
        {
            cout << "Evaluating Exponentiation" << endl;
            return pow(lhs->evaluate(),  rhs->evaluate());
        }
};

class ParentheticalExpression : public Expression
{
    public:
        ParentheticalExpression(Expression* inside = nullptr)
        {
            rhs = inside;
        }

        int priorityLookup()
        {
            return 4;
        }

        double evaluate() 
        {
            cout << "Evaluating Parenthetical" << endl;
            return rhs->evaluate();
        }
};

class ConstantExpression : public Expression
{
    public:
        double value;

        ConstantExpression(double value=0.0) 
        {
            this->value = value;
        }

        int priorityLookup()
        {
            return 5;
        }

        double evaluate() 
        {
            cout << "Evaluating Constant" << endl;
            return value;
        }
};