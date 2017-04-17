#include "RuleMonitor.h"
#include "LexicalAnalyzer.h"
#include <iostream>

using namespace std;

string RuleMonitor::rules[72] = {
" 1. <program> -> <define> <more_defines> EOF_T",
" 2. <define> -> LPAREN_T DEFINE_T LPAREN_T IDENT_T <param_list> RPAREN_T <stmt> <stmt_list> RPAREN_T",
" 3. <more_defines> -> <define> <more_defines>",
" 4. <more_defines> -> {}",
" 5. <stmt_list> -> <stmt> <stmt_list>",
" 6. <stmt_list> -> {}",
" 7. <stmt> -> <literal>",
" 8. <stmt> -> IDENT_T",
" 9. <stmt> -> LPAREN_T <action> RPAREN_T",
"10. <literal> -> NUMLIT_T",
"11. <literal> -> QUOTE_T <quoted_lit>",
"12. <quoted_lit> -> <any_other_token>",
"13. <more_tokens> -> <any_other_token> <more_tokens>",
"14. <more_tokens> -> {}",
"15. <param_list> -> IDENT_T <param_list>",
"16. <param_list> -> {}",
"17. <else_part> -> <stmt>",
"18. <else_part> -> {}",
"19. <action> -> IF_T <stmt> <stmt> <else_part>",
"20. <action> -> LISTOP_T <stmt>",
"21. <action> -> CONS_T <stmt> <stmt>",
"22. <action> -> AND_T <stmt_list>",
"23. <action> -> OR_T <stmt_list>",
"24. <action> -> NOT_T <stmt>",
"25. <action> -> NUMBERP_T <stmt>",
"26. <action> -> SYMBOLP_T <stmt>",
"27. <action> -> LISTP_T <stmt>",
"28. <action> -> ZEROP_T <stmt>",
"29. <action> -> NULLP_T <stmt>",
"30. <action> -> CHARP_T <stmt>",
"31. <action> -> STRINGP_T <stmt>",
"32. <action> -> PLUS_T <stmt_list>",
"33. <action> -> MINUS_T <stmt> <stmt_list>",
"34. <action> -> DIV_T <stmt> <stmt_list>",
"35. <action> -> MULT_T <stmt_list>",
"36. <action> -> EQUALTO_T <stmt_list>",
"37. <action> -> GT_T <stmt_list>",
"38. <action> -> LT_T <stmt_list>",
"39. <action> -> GTE_T <stmt_list>",
"40. <action> -> LTE_T <stmt_list>",
"41. <action> -> IDENT_T <stmt_list>",
"42. <action> -> DISPLAY_T <stmt>",
"43. <action> -> NEWLINE_T",
"44. <any_other_token> -> LPAREN_T <more_tokens> RPAREN_T",
"45. <any_other_token> -> IDENT_T",
"46. <any_other_token> -> NUMLIT_T",
"47. <any_other_token> -> CONS_T",
"48. <any_other_token> -> IF_T",
"49. <any_other_token> -> DISPLAY_T",
"50. <any_other_token> -> NEWLINE_T",
"51. <any_other_token> -> LISTOP_T",
"52. <any_other_token> -> AND_T",
"53. <any_other_token> -> OR_T",
"54. <any_other_token> -> NOT_T",
"55. <any_other_token> -> DEFINE_T",
"56. <any_other_token> -> NUMBERP_T",
"57. <any_other_token> -> SYMBOLP_T",
"58. <any_other_token> -> LISTP_T",
"59. <any_other_token> -> ZEROP_T",
"60. <any_other_token> -> NULLP_T",
"61. <any_other_token> -> CHARP_T",
"62. <any_other_token> -> STRINGP_T",
"63. <any_other_token> -> PLUS_T",
"64. <any_other_token> -> MINUS_T",
"65. <any_other_token> -> DIV_T",
"66. <any_other_token> -> MULT_T",
"67. <any_other_token> -> EQUALTO_T",
"68. <any_other_token> -> GT_T",
"69. <any_other_token> -> LT_T",
"70. <any_other_token> -> GTE_T",
"71. <any_other_token> -> LTE_T",
"72. <any_other_token> -> QUOTE_T"
};

RuleMonitor::RuleMonitor(){
	lexSet = false;
}

void RuleMonitor::setLex(LexicalAnalyzer * _lex){
	lex = _lex;
	lexSet = true;
}

void RuleMonitor::startNonterminal(int ruleNum){
    activeRules.push(ruleNum+1);
    activeNonterminals.push("    <" + nonTerminalFromRule(ruleNum) + "> ->");
}


void RuleMonitor::endNonterminal(){
	string outputString = "";
	string spacing = "";
	int numSpaces = activeRules.size();
	for(int i = 0; i < numSpaces; i++){
		spacing += "| ";
	}
	if(activeRules.top() == -1){
		outputString += spacing + "Exp: "
			+ nonTerminalFromRule(activeRules.top()) + " -> ??? "
			+ "(Unable to identify a specific rule)\n";
	} else{
		outputString += spacing + "Exp: " + rules[activeRules.top()-1] + "\n";
	}
	outputString += spacing + "Rec: " + activeNonterminals.top() + "\n";
	
	activeRules.pop();
	activeNonterminals.pop();
	
	sequences.push_back(outputString);
}


string RuleMonitor::nonTerminalFromRule(int ruleNum){
	if(ruleNum == 1)
		return "program";
	if(ruleNum == 2)
		return "define";
	if(ruleNum == 3 || ruleNum == 4)
		return "more_defines";
	if(ruleNum == 5 || ruleNum == 6)
		return "stmt_list";
	if(ruleNum >= 7 && ruleNum <= 9)
		return "stmt";
	if(ruleNum == 10 || ruleNum == 11)
		return "literal";
	if(ruleNum == 12)
		return "quoted_list";
	if(ruleNum == 13 || ruleNum == 14)
		return "more_tokens";
	if(ruleNum == 15 || ruleNum == 16)
		return "param_list";
	if(ruleNum == 17 || ruleNum == 18)
		return "else_part";
	if(ruleNum >= 19 && ruleNum <= 43)
		return "action";
	if(ruleNum >= 44 && ruleNum <= 72)
		return "any_other_token";
}


void RuleMonitor::addNonterminal(string newPiece){
	if(activeNonterminals.top() != ""){
		activeNonterminals.top() += " ";
	}
	activeNonterminals.top() += "<" + newPiece + ">";
}

void RuleMonitor::addToken(token_type newPiece){
	if(activeNonterminals.top() != ""){
		activeNonterminals.top() += " ";
	}
	activeNonterminals.top() += lex->GetTokenName(newPiece);
}


void RuleMonitor::printToFile(){
	lex->debug << "\nThe structure of the source file is listed below. Lines beginning\n"
		   << "with 'Exp' are the expected structure of the file, and lines\n"
		   << "beginning with 'Rec' are the lines that were parsed.\n\n";
	
	int numLines = sequences.size();
	for(int i = numLines - 1; i >= 0; i--){
		lex->debug << sequences[i];
	}
}