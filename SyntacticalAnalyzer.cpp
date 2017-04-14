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
	{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 2, -1, -1, -1, -2},
	{0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, 4},
	{0, 5, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6, 5, -1, -1},
	{0, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, -2, 7, -1, -1},
	{0, -2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, 11, -1, -1},
	{0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, -2, 12, -1, -1},
	{0, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 13, -1, -1},
	{0, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 16, -1, -1, -1},
	{0, 17, 17, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 17, 18, 17, -1, -1},
	{0, 41, -1, 21, 19, 42, 43, 20, 22, 23, 24, -1, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, -1, -2, -1, -1, -1},
	{0, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 44, -2, 72, -1, -1}};

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
	  // throw an error
	  // Write to error message file???
	  errors += 1;
	  
	  cout << "There was an Error" << endl;
	}else if (rule == 1){
	  cout << "This works, will call define()" << endl;
	  errors += define();
	  errors += more_defines();
	}
	
	if (token != EOF_T){
	  lex->ReportError ("Expected end of file; " + lex->GetLexeme ());
	  errors++;
	}

	lex->debug << "program function returning " << errors << " errors\n";
	cout << "Errors: " << errors << endl;
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

	if(rule == -1){
	  // throw an error
	  // Write to error message file???
	  errors += 1;
	  
	  cout << "There was an Error" << endl;
	}else if (rule == 2){
	  
	  token = lex->GetToken();
	  errors += enforce(token, DEFINE_T);

	  token = lex->GetToken();
	  errors += enforce(token, LPAREN_T);

	  token = lex->GetToken();
	  errors += enforce(token, IDENT_T);

	  errors += param_list();

	  //token = lex->GetToken(); Unneeded since param_list() should get one extra token
	  errors += enforce(token, RPAREN_T);

	  errors += stmt();

	  errors += stmt_list();

	  //token = lex->GetToken(); Unneeded since stmt_list() should get one extra token
	  errors += enforce(token, RPAREN_T);

	  token = lex->GetToken();	//Get one additional token
	  rule = GetRule(1, token);
	  ending("define", token, errors);
	}
	
	
	lex->debug << "program function returning " << errors << " errors\n";
	cout << "Errors in define: " << errors << endl;
	return errors;
  
}


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
		define();
		more_defines();
	  	rule = GetRule(1, token);
	  	ending("more_define", token, errors);
	} else if (rule == 4){
	  	rule = GetRule(2, token);
	  	ending("more_define", token, errors);

	}
	return errors;
}


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
		errors += stmt();
		errors += stmt_list();
	  	rule = GetRule(3, token);
	  	ending("stmt_list", token, errors);
	} else if (rule == 6){
	  	rule = GetRule(3, token);
	  	ending("stmt_list", token, errors);

	}
	return errors;
}


int SyntacticalAnalyzer::stmt(){
	int rule = GetRule(4,token);
	int errors = 0;
	string nonTerminal = "stmt";
	print(nonTerminal, token, rule);

	if(rule == -1){
	  //throw an error
	  //Write to error message file???
	  errors++;
	} else if (rule == 7){
		literal();	
	  	rule = GetRule(4, token);
	  	ending("stmt", token, errors);
	} else if (rule == 8){
		token = lex->GetToken();	//Get one additional token
	  	rule = GetRule(4, token);
	  	ending("stmt", token, errors);
	} else if (rule == 9){
		action();
		token = lex->GetToken();	//Get the RPAREN_T
		errors += enforce(token, RPAREN_T);
		token = lex->GetToken();	//Get one additional token
	  	rule = GetRule(4, token);
	  	ending("stmt", token, errors);
	}
	return errors;
}

int SyntacticalAnalyzer::literal(){
	lex->debug << "literal function called\n";
	p2file << "literal\n";
    	int errors = 0;
	
	int rule = GetRule(5,token);
	string nonTerminal = "literal";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

    	if(rule == -1){
		//throw an error
		//Write to error message file???
		errors += 1;
	  
		cout << "There was an Error" << endl;
    	} else if (rule == 10) { // NUMLIT_T
		token = lex->GetToken();	//Get one additional token
	  	rule = GetRule(5, token);
	  	ending("literal", token, errors);
    	} else if (rule == 11) { // QUOTE_T <quoted_lit>
		errors += quoted_lit();
		rule = GetRule(5, token);
	  	ending("literal", token, errors);

    	}

	lex->debug << "literal function returning " << errors << " errors\n";
	return errors;
}


int SyntacticalAnalyzer::quoted_lit() {

    lex->debug << "quoted_lit function called\n";
    p2file << "quoted_lit\n";
    int errors = 0;
	
    int rule = GetRule(6,token);
    string nonTerminal = "quoted_lit";
    print(nonTerminal, token, rule);
    cout << "Token_name: " << lex->GetTokenName(token) << endl;
    cout << "Rule: " << rule << endl;
    cout << "Token: " << token << endl;

    if (rule == -1) {
	//throw an error
	//Write to error message file???
	errors += 1;
    } else if (rule == 12) {
	errors += any_other_token();
	rule = GetRule(6, token);
	ending("quoted_lit", token, errors);

    }

    lex->debug << "quoted_lit function returning " << errors << " errors\n";
    return errors;

}


int SyntacticalAnalyzer::more_tokens(){
	lex->debug << "more_tokens function called\n";
	p2file << "more_tokens\n";
	int errors = 0;
	
	int rule = GetRule(7,token);
	string nonTerminal = "more_tokens";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

	if (rule == -1) {
		//throw an error
		//Write to error message file???
		errors += 1;
	} else if (rule == 13) {
		errors += any_other_token();
		errors += more_tokens();
		rule = GetRule(7, token);
		ending("more_tokens", token, errors);
	} else if (rule == 14) {
		rule = GetRule(7, token);
		ending("more_tokens", token, errors);
	}

	lex->debug << "more_tokens function returning " << errors << " errors\n";
	return errors;


}

int SyntacticalAnalyzer::param_list(){
	lex->debug << "param_list function called\n";
	p2file << "param_list\n";
	int errors = 0;
	
	int rule = GetRule(8,token);
	string nonTerminal = "param_list";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

	if (rule == -1) {
		//throw an error
		//Write to error message file???
		errors += 1;
	} else if (rule == 15) {
		errors += param_list();
		rule = GetRule(8, token);
		ending("param_list", token, errors);

	} else if (rule == 16) {
		rule = GetRule(8, token);
		ending("param_list", token, errors);

	}

	lex->debug << "param_list function returning " << errors << " errors\n";
	return errors;



}

int SyntacticalAnalyzer::else_part(){
	return 0;

}


int SyntacticalAnalyzer::action(){
	return 0;

}

int SyntacticalAnalyzer::any_other_token(){
	return 0;

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
  
  if(token == expected){
    return errors;
  }
  else{
    while(token != expected){
      token = lex->GetToken();
      errors += 1;
    }
    return errors;
  }

}

  
