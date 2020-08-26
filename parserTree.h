#pragma once
#include <bits/stdc++.h>

#include "./expressions.h"

using namespace std;

class ParserTree
{
    Expression* getParent(Expression* target)
    {
        Expression* cur = root;

        if (!cur)
            return cur;

        while(Expression* next = cur->right())
        {
            if (next == target)
                return cur;
            cur = next;
        }

        return nullptr;
    }

    Expression* getLeafNode(Expression* newOp)
    {
        //Precondition: root != nullptr
        //The tree should be structured as a hierarchy of precedence with the lowest priority operators closest to the root and
        //the highest priority operators closeset to the leaves
        //Use pushLowerPriority() to append an operator to the end of the chain of equal or higher priority operators, the lowest one available
        //Use pushHigherPriority() to insert an operator between operators of different precedence
        //Returns the node to replace in the tree. It's either the node with equal precedence or higher precedence
        
        function<bool (Expression *)> isParenthesis = [](Expression* node) { return dynamic_cast<ParentheticalExpression *>(node); };

        Expression *prev = nullptr;
        Expression *node = root;

        int ctr = parenthesisCount;
        
        //Traverse the tree until there are no more parenthesis
        while (ctr && node->right())
        {
            if( isParenthesis(node) )
                --ctr;
            prev = node;
            node = node->right();
        }
        //Postcondition of loop: ctr == 0

        bool foundEqual = false;
        while( node->right() )
        {
            if( isParenthesis(node) )
                break;

            if(*newOp == node)
            {   
                foundEqual = true; 
                break;
            }
            prev = node;
            node = node->right();
        }

        if(foundEqual)
            return node;
        else
            return prev = node;
    }

    public:
        Expression* root = nullptr;
        int parenthesisCount = 0;

        void push(Expression *newOp)
        {
            if (!root)
            {
                //Insert first expression
                root = newOp;  
            }               
            else if ( dynamic_cast<ConstantExpression*>(newOp) || dynamic_cast<ParentheticalExpression*>(newOp) )
            {
                //Insert a new constant or parenthesis recursively
                root->right(newOp);
            }
            else
            {
                Expression* lhs = getLeafNode(newOp);
                BinaryExpression* binOp = dynamic_cast<BinaryExpression*>(newOp);

                insertNode(lhs, binOp); 
            }
        }

        void insertNode(Expression *lhs, BinaryExpression *newOp)
        {
            //Precondition: lhs is a constant inside parenthesis, closed parenthesis, or a full binary expression
            //Stick entire subtree into newOp and add it to the bottom of the rest of the tree
            
            //pushHigherPriority(parent, newOp) is the same thing as pushLowerPriority(child, newOp)

            newOp->left(lhs);   //Stick entire old tree into left of newOp

            if(lhs == root)
                root = newOp;
            else
            {
                Expression* parent = getParent(lhs);   
                parent->rhs = nullptr;
                parent->right(newOp);
            }
        }

        void incrementParenthesis()
        {
            parenthesisCount += 1;
        }

        void decrementParenthesis()
        {
            if(parenthesisCount == 0)
                throw invalid_argument("Invalid parenthesis placement");
            parenthesisCount -= 1;            
        }

        ~ParserTree()
        {
            delete root;
        }
};