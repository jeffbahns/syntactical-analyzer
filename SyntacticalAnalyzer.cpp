/********************************************************************************/
/* Project: 2, Syntactical Analyzer 	      					*/
/* Author: Jeff Bahns, Erich Bucher, Jordan Bergero		    		*/
/* Date: 4/16/2017							    	*/
/* Description:	A syntactical analyzer built on top of Project 1     	     	*/
/********************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "SyntacticalAnalyzer.h"
#include "RuleMonitor.h"

using namespace std;

int firstsTable[][33] =
    {{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1},
     {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, -1, -1/*-2*/},
     {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, 4},
     {0, 5, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6, 5, -1, -1},
     {0, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, -1/*-2*/, 7, -1, -1},
     {0, -1/*-2*/, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1/*-2*/, -1/*-2*/, 11, -1, -1},
     {0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, -1/*-2*/, 12, -1, -1},
     {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, -1, -1},
     {0, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1},
     {0, 17, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17, 18, 17, -1, -1},
     {0, 41, -1, 21, 19, 42, 43, 20, 22, 23, 24, -1, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, -1, -1/*-2*/, -1, -1, -1},
     {0, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 44, -1/*-2*/, 72, -1, -1}};

/**
 * constructor, takes filename as arg
 **/
SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    /********************************************************************************/
    /* This function will								*/
    /********************************************************************************/
    lex = new LexicalAnalyzer (filename);
    rules.setLex(lex);
    token = AND_T;
    int fnlength = strlen (filename);
    filename[fnlength-2] = 'p';
    filename[fnlength-1] = '2';
    p2file.open (filename);

    filename[fnlength-2] = 'l';
    filename[fnlength-1] = 's';
    std::string lstFilename(filename);
    lstFilename += 't';
    lstfile.open (lstFilename.c_str(), std::ofstream::app);

    token = NextToken();
    int errors = program ();
}

/**
 * destructor
 **/
SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
    p2file.close();
    lstfile.close();
    delete lex;
}

/**
 * called when non-terminating program() is reached
 **/
int SyntacticalAnalyzer::program (){
    /********************************************************************************/
    /* This function will								*/
    /********************************************************************************/
    int errors = 0;
	
    int rule = GetRule(0,token);
    string nonTerminal = "program";
    print(nonTerminal, token, rule);

    if(rule == -1){
	vector<int>expected_vector;
	expected_vector.push_back(LPAREN_T);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
            ending(nonTerminal, token, errors);
            return errors;
        }
        rule = GetRule(0,token);	  
    }

    rules.startNonterminal(rule);
    if (rule == 1){
	errors += runNonterminal("define");
	errors += runNonterminal("more_defines");
    }

    if (token != EOF_T){
	lex->ReportError ("Expected end of file; " + lex->GetLexeme ());
	errors++;
    }

    ending(nonTerminal, token, errors);
    rules.printToFile();
    cout << "Errors: " << errors << endl;
    return errors;
}      

/** 
 * called when non-terminating define() is reached
 **/
int SyntacticalAnalyzer::define(){
    int errors = 0;
	
    int rule = GetRule(1,token);
    string nonTerminal = "define";
    print(nonTerminal, token, rule);

    if(rule == -1){
	vector<int>expected_vector;
	expected_vector.push_back(LPAREN_T);
	errors += enforce(token,expected_vector);
	expected_vector.clear();

	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
            return errors;
        }

	rule = GetRule(1,token);
    }

    rules.startNonterminal(rule);
    if (rule == 2){
	rules.addToken(token);
	token = NextToken();
	vector<int>expected_vector;
	expected_vector.push_back(DEFINE_T);
	errors += enforce(token, expected_vector);
	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
	    return errors;
	}
	expected_vector.clear();
	  
	token = NextToken();
	expected_vector.push_back(LPAREN_T);	 
	errors += enforce(token, expected_vector);
	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
	    return errors;
	}
	expected_vector.clear();

	token = NextToken();
	expected_vector.push_back(IDENT_T);
	errors += enforce(token, expected_vector);
	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
	    return errors;
	}
	expected_vector.clear();

	token = NextToken();
	errors += runNonterminal("param_list");

	expected_vector.push_back(RPAREN_T);
	errors += enforce(token, expected_vector);
	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
	    return errors;
	}
	expected_vector.clear();
	  
	token = NextToken();
	errors += runNonterminal("stmt");

	errors += runNonterminal("stmt_list");

	expected_vector.push_back(RPAREN_T);
	errors += enforce(token, expected_vector);
	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
	    return errors;
	}
	expected_vector.clear();

	token = NextToken();	//Get one additional token
    }
    ending(nonTerminal, token, errors);	
    return errors;
}

/**
 *  called when non-terminating more_defines() is reached
 **/
int SyntacticalAnalyzer::more_defines(){
    int rule = GetRule(2, token);
    int errors = 0;
    string nonTerminal = "more_defines";
    print(nonTerminal, token, rule);
    if(rule == -1){
	int array[1] = {LPAREN_T};
	vector<int>expected_vector(array, array+1);
	errors += enforce(token,expected_vector);
	if(token == EOF_T) {
            ending(nonTerminal, token, errors);	
	    return errors;
	}
	rule = GetRule(2,token);
    }
    rules.startNonterminal(rule);
    if (rule == 3){
	errors += runNonterminal("define");
	errors += runNonterminal("more_defines");
    } else if (rule == 4){
        //Do nothing for lambda.
    }
    ending(nonTerminal, token, errors);
    return errors;
}

/**
 *  called when non-terminating stmt_list() is reached
 **/
int SyntacticalAnalyzer::stmt_list(){
    int rule = GetRule(3, token);
    int errors = 0;
    string nonTerminal = "stmt_list";
    print(nonTerminal, token, rule);

    if(rule == -1){
	int array[5] = {IDENT_T, NUMLIT_T, LPAREN_T, QUOTE_T, RPAREN_T};
	vector<int>expected_vector(array, array+5);
	errors += enforce(token,expected_vector);
	if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}
	rule = GetRule(3,token);
    }
    rules.startNonterminal(rule);
    if(rule == 5){
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt_list");
    } else if (rule == 6){
        //Do nothing for lambda.
    }
    ending(nonTerminal, token, errors);
    return errors;
}

/**
 * called when non-terminating stmt_() is reached
 */
int SyntacticalAnalyzer::stmt(){
    int rule = GetRule(4,token);
    int errors = 0;
    string nonTerminal = "stmt";
    print(nonTerminal, token, rule);

    if(rule == -1){
	int array[4] = {IDENT_T, NUMLIT_T, LPAREN_T, QUOTE_T};
	vector<int>expected_vector(array, array+4);
	errors += enforce(token,expected_vector);
	if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}
	rule = GetRule(4,token);
    }
    rules.startNonterminal(rule);
    if (rule == 7){
	errors += runNonterminal("literal");	
    } else if (rule == 8){
	rules.addToken(token);
	token = NextToken();	//Get one additional token
    } else if (rule == 9){
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("action");
		
	vector<int>expected_vector;
	expected_vector.push_back(RPAREN_T);
	errors += enforce(token, expected_vector);
	expected_vector.clear();
	if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
            return errors;
        }
		
	token = NextToken();	//Get one additional token
    }
    ending(nonTerminal, token, errors);
    return errors;
}

/**
 * called when non-terminating literal() is reached
 **/
int SyntacticalAnalyzer::literal(){
    int errors = 0;
	
    int rule = GetRule(5,token);
    string nonTerminal = "literal";
    print(nonTerminal, token, rule);

    if(rule == -1){
	int array[2] = {NUMLIT_T, QUOTE_T};
	vector<int>expected_vector(array, array+2);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(5,token);
    }
    rules.startNonterminal(rule);
    if (rule == 10) {
	rules.addToken(token);
	token = NextToken();	//Get one additional token
    } else if (rule == 11) {
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("quoted_lit");

    }

    ending(nonTerminal, token, errors);
    return errors;
}


/**
 * called when non-terminating literal() is reached
 **/
int SyntacticalAnalyzer::quoted_lit() {
    int errors = 0;
	
    int rule = GetRule(6,token);
    string nonTerminal = "quoted_lit";
    print(nonTerminal, token, rule);

    if (rule == -1) {
	int array[30] = {IDENT_T, NUMLIT_T, CONS_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP_T, AND_T, OR_T, NOT_T, DEFINE_T, NUMBERP_T, SYMBOLP_T, LISTP_T, ZEROP_T, NULLP_T, CHARP_T,
			 STRINGP_T, PLUS_T, MINUS_T, DIV_T ,MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, LPAREN_T, RPAREN_T, QUOTE_T};
	vector<int>expected_vector(array, array+30);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(6, token);
    }
    rules.startNonterminal(rule);
    if (rule == 12) {
	errors += runNonterminal("any_other_token");

    }

    ending("quoted_lit", token, errors);
    return errors;

}

/**
 * called when non-terminating more_tokens() is reached
 **/
int SyntacticalAnalyzer::more_tokens(){
    int errors = 0;
	
    int rule = GetRule(7,token);
    string nonTerminal = "more_tokens";
    print(nonTerminal, token, rule);

    if (rule == -1) {
	int array[30] = {IDENT_T, NUMLIT_T, CONS_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP_T, AND_T, OR_T, NOT_T, DEFINE_T, NUMBERP_T, SYMBOLP_T, LISTP_T, ZEROP_T, NULLP_T, CHARP_T,
			 STRINGP_T, PLUS_T, MINUS_T, DIV_T ,MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, LPAREN_T, RPAREN_T, QUOTE_T};
	vector<int>expected_vector(array, array+30);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(7,token);
    }
    rules.startNonterminal(rule);
    if (rule == 13) {
	errors += runNonterminal("any_other_token");
	errors += runNonterminal("more_tokens");
    } else if (rule == 14) {
        //Do nothing for lambda.
    }

    ending(nonTerminal, token, errors);
    return errors;
}

/** 
 * called when non-terminating param_list() is reached
 **/
int SyntacticalAnalyzer::param_list(){
    int errors = 0;
	
    int rule = GetRule(8,token);
    string nonTerminal = "param_list";
    print(nonTerminal, token, rule);

    if (rule == -1) {
	int array[2] = {IDENT_T, RPAREN_T};
	vector<int>expected_vector(array, array+2);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(8, token);
    }
    rules.startNonterminal(rule);
    if (rule == 15) {
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("param_list");

    } else if (rule == 16) {
	//Do nothing for lambda.
    }
    ending(nonTerminal, token, errors);
    return errors;
}

/**
 * called when non-terminating else_part() is reached
 **/
int SyntacticalAnalyzer::else_part(){
    int errors = 0;
    int rule = GetRule(9, token);
    string nonTerminal = "else_part";
    print(nonTerminal, token, rule);

    if (rule == -1) {
	int array[5] = {NUMLIT_T, IDENT_T, QUOTE_T, LPAREN_T, RPAREN_T};
	vector<int>expected_vector(array, array+5);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(9,token);
    }
    rules.startNonterminal(rule);
    if (rule == 17) {
	errors += runNonterminal("stmt");

    } else if (rule == 18) {
        //Do nothing for lambda.
    }
    ending("else_part", token, errors);
    return errors;
}

/**
 * called when non-terminating action() is reached
 **/
int SyntacticalAnalyzer::action(){
    int errors = 0;
    int rule = GetRule(10, token);
    string nonTerminal = "action";
    print(nonTerminal, token, rule);

    if (rule == -1) {
	int array[26] = {IDENT_T, CONS_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP_T, AND_T, OR_T, NOT_T,
			 NUMBERP_T, SYMBOLP_T, LISTP_T, ZEROP_T, NULLP_T, CHARP_T, STRINGP_T, PLUS_T, MINUS_T,
			 DIV_T, MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, RPAREN_T};
	vector<int>expected_vector(array, array+26);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(10,token);
    }
    rules.startNonterminal(rule);
    switch (rule) {
    case 19:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt");
	errors += runNonterminal("else_part");
	break;
    case 20:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt");
	break;
    case 21:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt");
	break;
    case 22 ... 23:
        rules.addToken(token);
        token = NextToken();
        errors += runNonterminal("stmt_list");
        break;
    case 24 ... 31:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt");
	break;
    case 32:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt_list");
	break;
    case 33 ... 34:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt_list");
	break;
    case 35 ... 41:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt_list");
	break;
    case 42:
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("stmt");
	break;
    case 43:
	rules.addToken(token);
	token = lex ->GetToken();
	break;
    }

    ending(nonTerminal, token, errors);
    return errors;
}

/**
 * called when non-terminating any_other_token() is reached
 **/
int SyntacticalAnalyzer::any_other_token(){
    int errors = 0;
	
    int rule = GetRule(11, token);
    string nonTerminal = "any_other_token";
    print(nonTerminal, token, rule);

    if (rule == -1) {
	int array[30] = {IDENT_T, NUMLIT_T, CONS_T, IF_T, DISPLAY_T, NEWLINE_T, LISTOP_T, AND_T, OR_T, NOT_T, DEFINE_T, NUMBERP_T, SYMBOLP_T, LISTP_T, ZEROP_T, NULLP_T, CHARP_T,
			 STRINGP_T, PLUS_T, MINUS_T, DIV_T ,MULT_T, EQUALTO_T, GT_T, LT_T, GTE_T, LTE_T, LPAREN_T, RPAREN_T, QUOTE_T};
	vector<int>expected_vector(array, array+30);
	errors += enforce(token,expected_vector);
        if(token == EOF_T) {
	    ending(nonTerminal, token, errors);
	    return errors;
	}

	rule = GetRule(11, token);
    }
    rules.startNonterminal(rule);
    if (rule == 44) {
	rules.addToken(token);
	token = NextToken();
	errors += runNonterminal("more_tokens");
	rules.addToken(token);
	token = NextToken();	//Get one additional lexeme
    } else if (rule >= 45 && rule <= 72) {
	rules.addToken(token);
	token = NextToken();	//Get one additional lexeme
    }
    ending(nonTerminal, token, errors);
    return errors;

}

/**
 * helper, which is used to index the rules table
 **/
int SyntacticalAnalyzer::GetRule(int row, token_type col){
    return firstsTable[row][col];
}

/**
 * helper, prints starting statements for non-terminators
 **/
void SyntacticalAnalyzer::print(string nonTerm, token_type token, int rule){
    p2file << "Starting <" << nonTerm << ">. Current token = " << lex->GetTokenName(token) << endl;
    p2file << "Using rule " << rule << endl;
    lex->debug << "\t<" << nonTerm << "> started, using rule " << rule << "\n";
  
}


/** 
 * helper, prints ending statement
 **/
void SyntacticalAnalyzer::ending(string nonTerm, token_type token, int errors){
    p2file << "Ending <" << nonTerm << ">. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors;
    lex->debug << "\t<" << nonTerm << "> ending\n";
    if (nonTerm != "program") {
	p2file << "\n";
    }
    rules.endNonterminal();
}


/** 
 * Description:	Forces the code to continue to read in tokens until you find one that is valid, based on
 * the current rule that is being worked on. If the end of the file is reached, the function
 * will also end and token will be equal to EOF_T.
 **/
int SyntacticalAnalyzer::enforce(token_type &token, vector<int>expected_vector) {
    int errors = 0;
    bool flag = true;
    for(int i=0; i<expected_vector.size(); i++){
	if(expected_vector[i] == token){
	    rules.addToken(token);
	    return errors;
	}
    }
    while(flag){
	token = NextToken();
	for(int i=0; i<expected_vector.size(); i++){
	    if(expected_vector[i] == token || token == EOF_T){
		flag = false;
	    }
	}
	lstfile << "Error, this token was not expected: " << token << endl;
	rules.addToken(token);
	errors ++;
    }

    return errors;
}


/**
 * Description:	Use this function in place of lex->GetTokenName(). It makes it easier if you want to
 * 		add cout statements to print out the tokens as you get them.
 **/
token_type SyntacticalAnalyzer::NextToken(){
    token_type t = lex->GetToken();
    return t;
}


/**
 * Description:	This function will run the appropriate nonTerminal function, based on the given
 * 		string. It will also add the nonterminal to the current rule in the RuleMonitor
 * 		object.
 * 
 * Pre:		n is the name of a nonterminal (without the < and > around it).
 *
 * Post:	one of the nonterminal functions will be run.
 */
int SyntacticalAnalyzer::runNonterminal(string n){
    rules.addNonterminal(n);
    if(n == "program")
	return program();
    if(n == "define")
	return define();
    if(n == "more_defines")
	return more_defines();
    if(n == "stmt_list")
	return stmt_list();
    if(n == "stmt")
	return stmt();
    if(n == "literal")
	return literal();
    if(n == "quoted_lit")
	return quoted_lit();
    if(n == "more_tokens")
	return more_tokens();
    if(n == "param_list")
	return param_list();
    if(n == "else_part")
	return else_part();
    if(n == "action")
	return action();
    if(n == "any_other_token")
	return any_other_token();
    return 1000000; //This will never be called.
} 
