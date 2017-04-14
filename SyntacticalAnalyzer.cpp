/********************************************************************************/
/* Project: 									*/
/* Author: 									*/
/* Date: 									*/
/* Description:									*/
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

SyntacticalAnalyzer::SyntacticalAnalyzer (char * filename)
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	lex = new LexicalAnalyzer (filename);
	int fnlength = strlen (filename);
	filename[fnlength-2] = 'p';
	filename[fnlength-1] = '2';
	p2file.open (filename);
	token = lex->GetToken();
	int errors = program ();

}

SyntacticalAnalyzer::~SyntacticalAnalyzer ()
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	delete lex;
}

int SyntacticalAnalyzer::program (){
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	lex->debug << "program function called\n";
	p2file << "program\n";
	int errors = 0;
	
	
	int rule = GetRule(0,token);
	string nonTerminal = "program";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

	if(rule == -1){
	  errors += enforce(token,LPAREN_T);
	  cout << "There was an Error" << endl;
	  // throw an error
	  // Write to error message file???

	}else if (rule == 1){
	  cout << "This works, will call define()" << endl;
	  errors += define();
	  //more_defines();
	}

	// token should be in the firsts of program
	// if true... keep going
	// if false...
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the firsts of program

	//Haven't defined these functions yet, uncoment later
	//errors += define ();
	//errors += more_defines ();

	if (token != EOF_T){
	  lex->ReportError ("Expected end of file; " + lex->GetLexeme ());
	  errors++;
	}
	      
	// token should be in the follows of program
	// if true... keep going
	// if false...
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the follows of program

	lex->debug << "program function returning " << errors << " errors\n";
	return errors;

}      

int SyntacticalAnalyzer::define(){
	lex->debug << "define function called\n";
	p2file << "define\n";
	int errors = 0;
	
	int rule = GetRule(1,token);
	string nonTerminal = "define";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;
	
	//In this case, the Rule will be 2. Always.
	  
	token = lex->GetToken();
	errors += enforce(token, DEFINE_T);
	
	token = lex->GetToken();
	errors += enforce(token, LPAREN_T);
	
	token = lex->GetToken();
	errors += enforce(token, IDENT_T);
	
	//errors += param_list();
	
	token = lex->GetToken();
	errors += enforce(token, RPAREN_T);

	token = lex->GetToken();
	errors += stmt();

	errors += stmt_list();
	
	token = lex->GetToken();
	errors += enforce(token, RPAREN_T);
	
	//print function for end
	
	  
	cout << "Errrors: " << errors << endl;
	lex->debug << "program function returning " << errors << " errors\n";
	return errors;
  
}


int SyntacticalAnalyzer::literal(){
  
}

int SyntacticalAnalyzer::quoted_lit(){

}

int SyntacticalAnalyzer::more_tokens(){

}

int SyntacticalAnalyzer::param_list(){

}

int SyntacticalAnalyzer::else_part(){

}


int SyntacticalAnalyzer::action(){


}

int SyntacticalAnalyzer::any_other_token(){

}



/**
 * Pre: the first token has already been read in
 * Post: The first token of the next nonterminal will be read in at the end
 * 	 of the function.
 **/
int SyntacticalAnalyzer::stmt(){
    cout << "stmt called\n";
    int rule = GetRule(4,token);
    cout << "token cock " << token << endl;
    int errors = 0;
    string nonTerminal = "stmt";
    print(nonTerminal, token, rule);
    
    if(rule == -1){
	// throw an error
	// Write to error message file???
	errors++;
    } else if (rule == 7){
	//literal();	
    } else if (rule == 8){
	//cout << "saw a bitch ass ident_t in stmt\n";
	token = lex->GetToken();	//Get the token in the next nonterminal
    } else if (rule == 9){
	//action();
	token = lex->GetToken();	//Get the RPAREN_T
	//errors += enforce(token, RPAREN_T);
	token = lex->GetToken();	//Get the token in the next nonterminal
    }
    return errors;
}
      


/**
 * Pre	The first token of the stmt_list has already been read in.
 * Post	The first token of the next nonterminal will be read in by this function.
 */
int SyntacticalAnalyzer::stmt_list(){
	int rule = GetRule(3, token);
	int errors = 0;
	string nonTerminal = "stmt_list";
	print(nonTerminal, token, rule);

	if(rule == -1){
		//throw an error
		//Write to error message file???
		errors++;
	} else if(rule == 5){
		//stmt();
		stmt_list();
		token = lex->GetToken();
	} else if (rule == 6){
		//Do nothing since it's Lambda
	}
	return errors;
}

/**
 * Pre:		The first token has already been read in
 * Post: 	The first token of the proceduring nonterminal will be read in.
 **/
int SyntacticalAnalyzer::more_defines(){
	int rule = GetRule(2, token);
	int errors = 0;
	string nonTerminal = "more_defines";
	print(nonTerminal, token, rule);
	if(rule == -1){
		//Throw an error
		//Write an error message file?
		errors += 1;
		cout << "There was an error from more_defines()" << endl;
	} else if (rule == 3){
		//define();
		more_defines();
		token = lex->GetToken();
	} else if (rule == 4){
		return errors;
	}
	return errors;
}
	

int SyntacticalAnalyzer::GetRule(int row, token_type col){

  return firstsTable[row][col];
}

void SyntacticalAnalyzer::print(string nonTerm, token_type token, int rule){

  //change from cout when we know what the hell we are doing.
  p2file << "Starting <" << nonTerm << ">. Current token = " << lex->GetTokenName(token) << endl;
  p2file << "Using rule " << rule << endl;
  
}

void SyntacticalAnalyzer::ending(string nonTerm, token_type token, int errors){
  p2file << "Ending <" << nonTerm << ">. Current token = " << lex->GetTokenName(token) << ". Errors = " << errors << endl;
  
}


int SyntacticalAnalyzer::enforce(token_type token, token_type expected) {
  int errors = 0;
  //cout << "Bam" << endl;
  cout << "Token: " << lex->GetTokenName(token) << endl;
  if(token == expected){
    return errors;
  }
  else{
    while(token != expected && token != EOF_T){
      token = lex->GetToken();
      errors += 1;
      //cout << "more errors: " << errors << endl;
    }
    return errors;
  }

}

  
