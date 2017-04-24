/********************************************************************************/
/* Project: 2, Syntactical Analyzer 	      					*/
/* Author: Jeff Bahns, Erich Bucher, Jordan Bergero		    		*/
/* Date: 4/16/2017							    	*/
/* Description:	A syntactical analyzer built on top of Project 1. When a new    */
/* 		SyntacticalAnalyzer object is created, it will open up the      */
/*		file given as the constructors argument, parse through it, and  */
/*		write out information to the following files:			*/
/*		.p2 file:  A list of when each nonterminal begins and ends,	*/
/*			   which rule starts the nonterminals, the first	*/
/*			   token of each nonterminal, and the number of errors  */
/*		.dbg file: Each line of the input file, followed by the tokens  */
/*			   in that line and the nonterminals beginning or	*/
/*			   beginning.						*/
/*		.lst file: All of the lines from the input file, followed by	*/
/*			   all of the lexical and syntactical errors.		*/
/********************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include "SyntacticalAnalyzer.h"

using namespace std;

int firstsTable[][33] =
    {{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1},
     {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, -1, -1},
     {0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, 4},
     {0, 5, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6, 5, -1, -1},
     {0, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, -1, 7, -1, -1},
     {0, -1, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, -1, -1},
     {0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, -1, 12, -1, -1},
     {0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, -1, -1},
     {0, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1},
     {0, 17, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17, 18, 17, -1, -1},
     {0, 41, -1, 21, 19, 42, 43, 20, 22, 23, 24, -1, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, -1, -1, -1, -1, -1},
     {0, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 44, -1, 72, -1, -1}};

/**
 * constructor, takes filename as arg
 **/
SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
    /********************************************************************************/
       /* This function is the default constructor of the Syntactical Analyzer
       /* It will create the output files <testing>.lst, <testing>.p2 
       /* Then it sets our global private member token to the token that starts the
       /* program. This should be a LPAREN_T. It then calls program. 
    /********************************************************************************/
    lex = new LexicalAnalyzer (filename);
    int fnlength = strlen (filename);
    filename[fnlength-2] = 'p';
    filename[fnlength-1] = '2';
    p2file.open (filename);

    filename[fnlength-2] = 'l';
    filename[fnlength-1] = 's';
    std::string lstFilename(filename);
    lstFilename += 't';
    lstfile.open (lstFilename.c_str(), std::ofstream::out | std::ofstream::app);

    token = NextToken();
    int errors = program ();
    lstfile << lstOutput<< endl;
    lstfile << "Errors found during syntactical analysis: " << errors << endl;
    lstOutput = "";
}

/******
 * destructor
 **/
SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
  /************************************************* 
  /* close the files that we opened 
  /* Delete our instance of the lexical analyzer
  ***************************************************/
    p2file.close();
    lstfile.close();
    cout << "Lexical errors: ";
    delete lex;
}

/**
 * called when non-terminating program() is reached
 **/
int SyntacticalAnalyzer::program (){
    /********************************************************************************/
    /* This function will take in a token and find a rule. Ultimately, the token is 
    /* a LPAREN_T and we get rule 1, and we define, upon when define returns we will
    /* call more_defines, until we hit a lambda in there and  get to the EOF_T
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

    if (rule == 1){
	errors += runNonterminal("define");
	errors += runNonterminal("more_defines");
    }

    if (token != EOF_T){
	lex->ReportError ("Expected end of file; " + lex->GetLexeme ());
	errors++;
    }

    ending(nonTerminal, token, errors);
    cout << "Syntactical errors: " << errors << " errors found in input file" << endl;
    return errors;
}      

/** 
 * called when non-terminating define() is reached
 **/
int SyntacticalAnalyzer::define(){
  /********************************************************************
  /* This funciton will take in a token, and it should be LPAREN_T, 
  /* this will then start the chain reaction of function calls to build
  /* out the rest of the syntactical analyzer.
  *********************************************************************/
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

    if (rule == 2){
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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/
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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/

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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/
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
    if (rule == 7){
	errors += runNonterminal("literal");	
    } else if (rule == 8){
	token = NextToken();	//Get one additional token
    } else if (rule == 9){
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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/

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
    if (rule == 10) {
	token = NextToken();	//Get one additional token
    } else if (rule == 11) {
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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/

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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/
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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/
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
    if (rule == 15) {
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
    /********************************************************************             
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/

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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/
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
   
    switch (rule) {
    case 19:
	token = NextToken();
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt");
	errors += runNonterminal("else_part");
	break;
    case 20:
      token = NextToken();
      errors += runNonterminal("stmt");
      break;
    case 21:
	token = NextToken();
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt");
	break;
    case 22 ... 23:
        token = NextToken();
        errors += runNonterminal("stmt_list");
        break;
    case 24 ... 31:
	token = NextToken();
	errors += runNonterminal("stmt");
	break;
    case 32:
	token = NextToken();
	errors += runNonterminal("stmt_list");
	break;
    case 33 ... 34:
	token = NextToken();
	errors += runNonterminal("stmt");
	errors += runNonterminal("stmt_list");
	break;
    case 35 ... 41:
	token = NextToken();
	errors += runNonterminal("stmt_list");
	break;
    case 42:
	token = NextToken();
	errors += runNonterminal("stmt");
	break;
    case 43:
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
  /********************************************************************                                                                                               
  /* This funciton will take in a token,
  /* find a rule number for it, and proceed with the rule it recieves
  /* If the rule is -1, we will cycle through the tokens until we get a
  /* token we want, incrementing the errors until then
  *********************************************************************/

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
    if (rule == 44) {
	token = NextToken();
	errors += runNonterminal("more_tokens");
	token = NextToken();	//Get one additional lexeme
    } else if (rule >= 45 && rule <= 72) {
	token = NextToken();	//Get one additional lexeme
    }
    ending(nonTerminal, token, errors);
    return errors;

}

/**
 * Description:	Helper, which is used to index the rules table. 
 **/
int SyntacticalAnalyzer::GetRule(int row, token_type col){
    return firstsTable[row][col];
}

/**
 * Description:	Prints out the following information to the following files:
 * 		.p2 file:	The nonterminal that is beginning, the first token of it,
 * 				and the rule that is being used for it.
 * 		.dbg file:	The nonterminal that is beginning and what rule is being used
 * 				for it.
 *
 * Pre:		A new nonterminal starting to be read in and the first token of it is stored
 * 		in token.
 **/
void SyntacticalAnalyzer::print(string nonTerm, token_type token, int rule){
    p2file << "Starting <" << nonTerm << ">. Current token = " << lex->GetTokenName(token) << endl;
    p2file << "Using rule " << rule << endl;
    lex->debug << "\t<" << nonTerm << "> started, using rule " << rule << "\n";
  
}


/** 
 * Description:	Prints out the following information to the following files:
 * 		.p2 file: 	The nonterminal that is ending, the next proceding token from
 * 			  	the input file, and the number of errors that were encountered
 * 			  	in that nonterminal.
 * 		.dbg file: 	The nonterminal that is ending.
 *
 * Pre:		A nonterminal has finished being read in and the token being stored in token
 * 		is the next token from the file after the nonterminal.
 **/
void SyntacticalAnalyzer::ending(string nonTerm, token_type token, int errors){
    p2file << "Ending <" << nonTerm << ">. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors;
    lex->debug << "\t<" << nonTerm << "> ending\n";
    if (nonTerm != "program") {
	p2file << "\n";
    }
}


/** 
 * Description:	Forces the lexical analyzer to continue to read in tokens until you find
 * 		one that is valid, based on the current rule that is being worked on.
 * 		If the end of the file is reached, the function will also end and token
 * 		will be equal to EOF_T. Every time that a new token is read in because
 * 		there are no matches, the error counter will increase.
 *
 * Pre:		token has been initialized.
 * 		expected_vector should contain ints that correspond with token values
 * 		that are valid for the given token to be equal to so that it satisfies
 * 		a specific rule.
 *
 * Post:	The number of new tokens read in will be returned.
 * 		token will be changed to a new token from the input file if it was
 * 		not originally in the expected_vector.
 **/
int SyntacticalAnalyzer::enforce(token_type &token, vector<int>expected_vector) {
    int errors = 0;
    bool flag = true;
    string expected_tokens = "<";
    for(int i=0; i<expected_vector.size(); i++){
	expected_tokens += lex->GetTokenName((token_type)expected_vector.at(i));
	if (i+1 != expected_vector.size()) {
	    expected_tokens += ", ";
	}
    }
    expected_tokens += ">";

    for(int i=0; i<expected_vector.size(); i++){
	if(expected_vector[i] == token || token == EOF_T){
	    flag = false;
	}
    }
    while(flag){
	token = NextToken();
	for(int i=0; i<expected_vector.size(); i++){
	    if(expected_vector[i] == token || token == EOF_T){
		flag = false;
	    }
	}
	cout << "Error, token expected: " << expected_tokens << " , received: " << lex->GetTokenName(token) << endl;
	lstOutput += "Error, token expected: " + expected_tokens + " , received: " + lex->GetTokenName(token) + "\n";
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
