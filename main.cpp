#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cmath>
#include <vector>
#include <cstring>
/*Finding the smallest program that demonstrates the error" is a powerful debugging tool. - Bjarne Stroustrup */
using namespace std;

struct Node
{
    Node *prev;
    Node *next;
    char value;
    int precedenceNum;
};
/* Dont need to write our own getLength() function. 
int getLength(char *line[])
{
    std::cout << "in getLength()\n";
    char ptr;
    int i = 0;
    std:: cout << "Length: " << i << std::endl;
    return i;
}
*/
//precedence()
int precedence(Node *&top, Node *&bottom, char op)
{
    //return true if higher
    //return false if lower
    bool higher;
    int prec = 0;

    char symbol;
    /* Order of precedence (high to low) =>  1. ^   2. * or / 3. + or -  4. (  5. = */
    switch (op)
    {
    case '=':
        prec = 1;
        break;
    case '(':
        prec = 2;
        break;
    case '+':
        prec = 3;
        break;
    case '-':
        prec = 3;
        break;
    case '/':
        prec = 4;
        break;
    case '*':
        prec = 4;
        break;
    case '^':
        prec = 5;
    default:
        break;
    }
    return prec;
}

//comppare precedence
//check if stack is empty
void removeFromStack(Node* poppedNode) // DON'T NEED THIS ANYMORE, DELETE
/* delete because we allocated memory on heap for our stack nodes. Now that we popped our nodes from the stack
we want to delete them from memory. This is good practice and is functional for dynamic memory management.*/
{
	std::cout << "called removeFromStack() on: " << poppedNode->value << "\n";
	delete poppedNode; 
}
bool isEmpty(Node *&top)
{
	if(!top)
	{
		return true;
	}
	else return false;
}
void push(Node *&top, Node *&bottom, char character)/*int num*/
{
	std::cout << "Called push()\n";
	Node* newNode = new Node;
	Node* temp = new Node;
	//newNode->value = num;
    newNode->value = character;
	if(!top) // top == NULL
	{
		top = newNode;
		top->prev = NULL;
		top->next = NULL;
		bottom = newNode;
		bottom->prev = NULL;
		bottom->next = NULL;
	}
	else
	{
		temp = newNode;
		temp->prev = NULL;
		temp->next = top;
		top->prev = temp;
		top = temp;
	}
	
}
int getLengthOfStack(Node *&top)
{
	Node* current;
	int i = 0;
	for (current = top; current; current = current->next, i++){}
	std::cout << "Length of stack is :" << i << std::endl;
	return i;
}
char pop(Node *&top)
{
	Node* temp = new Node; // used to hold the value of top so we can then delete that node after top becomes top->next
    //int nodeValue = -999;
    char nodeData;
	if (getLengthOfStack(top) == 0)
	{
		std::cout << "Stack is empty! You are trying to pop a value from an empty stack!\n";
		return 0;
	}
	else
	{
		std::cout << top->value << " popped from stack." << std::endl;
		//nodeValue = top->value;
        nodeData = top->value;
		temp = top;
		top = top->next;
		delete temp;
		//return nodeValue;
        return nodeData;
	}
}

//push on stack

//pop from stack
int main()
{
    Node* top = NULL;
	Node* bottom = NULL;
    int newPrecedenceNum = 0;
    char line[1000];
    int number= 0;
    char symbol;
    //TODO: change our chars to ints using atoi()
    cout << "Enter your infix expression: ";
    scanf("%s", line);
    FILE* filePtr;
    filePtr = fopen("output.txt", "w"); //for writing postfix expression to file
    std::cout << "the length of line is " << strlen(line) << std::endl;
    for(int i = 0; i < strlen(line); i ++)
    {
        std::cout << "line[" << i << "]: " << line[i] <<std::endl;
        if (line[i] != '/' && line[i] != '*' && line[i] != '+' && line[i] != '-' && line[i] != '(' && line[i] != ')')
        {
            //it is a letter
            //cout the letter
            fprintf(filePtr, "%c", line[i]);// line[i] <<std::endl;
        }
        else if(line[i] != '/' && line[i] != '*' && line[i] != '+' && line[i] != '-')
        {
            if(isEmpty(top) == true)
            {
                // push on stack
                push(top, bottom, line[i]); // might need to change this to handle chars/string
                top->precedenceNum = precedence(top, bottom, line[i]);
            }
            else if(isEmpty(top) == false)
            {
                newPrecedenceNum = precedence(top, bottom, line[i]);
                //if precedenceNum >, <, = to what is sitting on stack then do either push, pop
                if(newPrecedenceNum > top->precedenceNum)
                //(current operator is higher in precedence 
                //than what is on top of the stack)
                {
                    // push on stack
                    push(top,bottom,line[i]);
                    top->precedenceNum = precedence(top,bottom,line[i]);
                }
                if(newPrecedenceNum <= top->precedenceNum)
                //current operator is lower or equal in 
                //precedence than what is on top of the stack
                {
                    while(isEmpty(top) == false)
                    {
                        // pop and display what is on stack. 
                        // Repeat process with hot potatoe operator 
                        // to resolve it
                        symbol = pop(top);
                        fprintf(filePtr, "%c", symbol);
                    }
                }
            }
        }
        else if(line[i] == '(')/*we get  an open parenthesis*/
        {
            // push on stack
            push(top, bottom, line[i]);
        }
        else if(line[i] == ')')/*we get a closed paranthesis)*/
        {
            // pop and display everything up to 
            // the open parenthesis.
            // pop the open parenthesis
            // do not display open parenthesis
            while(symbol != '(')
            {
                symbol = pop(top);
                fprintf(filePtr, "%c", symbol);
            }
            symbol = pop(top); // to get rid of the '('
        }
        else
        {
            //pop and display everything
            while(isEmpty(top) == false)
            {
                symbol = pop(top);
                fprintf(filePtr, "%c", symbol);
            }
        }
    }
    fclose(filePtr);
    return 0;
}