#ifndef RULEMONITOR_H
#define RULEMONITOR_H

#include "LexicalAnalyzer.h"
#include <iostream>
#include <stack>
#include <vector>

class RuleMonitor{
	public:
		RuleMonitor();
		static string rules[72];
		void setLex(LexicalAnalyzer * lex);
		void startNonterminal(int ruleNum);
		void endNonterminal();
		void addNonterminal(string newPiece);
		void addToken(token_type newPiece);
		void printToFile();
	private:
		LexicalAnalyzer * lex;
		bool lexSet;
		std::stack<int> activeRules;
		std::stack<string> activeNonterminals;
		std::vector<string> sequences;
		string nonTerminalFromRule(int ruleNum);
};


#endif 
