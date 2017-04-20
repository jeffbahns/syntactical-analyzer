/********************************************************************************/
/* Project: 									*/
/* Author: 									*/
/* Date: 									*/
/* Description:									*/
/********************************************************************************/

#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <fstream>
#include "LexicalAnalyzer.h"
#include "RuleMonitor.h"

using namespace std;

class SyntacticalAnalyzer 
{
    public:
	SyntacticalAnalyzer (char * filename);
	~SyntacticalAnalyzer ();
    private:
	LexicalAnalyzer * lex;
	ofstream p2file;
	ofstream lstfile;
	token_type token;
	int program();
	int define();
	int more_defines();
	int stmt_list();
	int stmt();
	int literal();
	int quoted_lit();
	int more_tokens();
	int param_list();
	int else_part();
	int action();
	int any_other_token();
	int GetRule(int, token_type);
	void print(string, token_type, int);
	void ending(string, token_type, int);
        int enforce(token_type &token, vector<int>expected_vector);
	int runNonterminal(string n);
	token_type NextToken();
	string lstOutput;
};
	
#endif
