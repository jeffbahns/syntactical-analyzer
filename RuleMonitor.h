#ifndef RULEMONITOR_H

#include "SyntacticalAnalyzer.h"
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
		void endNonterinal();
		string nonTerminalFromRule(int ruleNum);
		void addToken(token_type token);
		void printToFile();
	private:
		LexicalAnalyzer * lex;
		bool lexSet;
		std::stack<string> activeRules;
		std::stack<string> activeNonterminals;
		std::vector<string> sequences;
};


#endif 
