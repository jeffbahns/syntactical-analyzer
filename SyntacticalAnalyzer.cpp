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
	token = AND_T;
	int fnlength = strlen (filename);
	filename[fnlength-2] = 'p';
	filename[fnlength-1] = '2';
	p2file.open (filename);
	token = NextToken();
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
	  token = NextToken();
	  errors += enforce(token, DEFINE_T);

	  token = NextToken();
	  errors += enforce(token, LPAREN_T);

	  token = NextToken();
	  errors += enforce(token, IDENT_T);

	  token = NextToken();
	  errors += param_list();

	  //token = NextToken(); Unneeded since param_list() should get one extra token
	  errors += enforce(token, RPAREN_T);
	  token = NextToken();

	  errors += stmt();

	  errors += stmt_list();

	  //token = NextToken(); Unneeded since stmt_list() should get one extra token
	  errors += enforce(token, RPAREN_T);

	  token = NextToken();	//Get one additional token
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
		token = NextToken();	//Get one additional token
	  	rule = GetRule(4, token);
	  	ending("stmt", token, errors);
	} else if (rule == 9){
		token = NextToken();
		action();
		errors += enforce(token, RPAREN_T);
		token = NextToken();	//Get one additional token
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
		token = NextToken();	//Get one additional token
	  	rule = GetRule(5, token);
	  	ending("literal", token, errors);
    	} else if (rule == 11) { // QUOTE_T <quoted_lit>
		token = NextToken();
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
		token = NextToken();
		errors += param_list();
		rule = GetRule(8, token);
		ending("param_list", token, errors);

	} else if (rule == 16) {
		//token = NextToken(); //Didn't think this was necessary for lambas but maybe it is?
		rule = GetRule(8, token);
		ending("param_list", token, errors);

	}

	lex->debug << "param_list function returning " << errors << " errors\n";
	return errors;
}

int SyntacticalAnalyzer::else_part(){
	lex->debug << "else_part function called\n";
	p2file << "else_part\n";
	int errors = 0;
	
	int rule = GetRule(9, token);
	string nonTerminal = "else_part";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

	if (rule == -1) {
		//throw an error
		//Write to error message file???
		errors += 1;
	} else if (rule == 17) {
		errors += stmt();
		rule = GetRule(9, token);
		ending("else_part", token, errors);

	} else if (rule == 18) {
		rule = GetRule(9, token);
		ending("else_part", token, errors);

	}

	lex->debug << "else_part function returning " << errors << " errors\n";
	return errors;


}


int SyntacticalAnalyzer::action(){
	lex->debug << "action function called\n";
	p2file << "action\n";
	int errors = 0;
	
	int rule = GetRule(10, token);
	string nonTerminal = "action";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

	if (rule == -1) {
		//throw an error
		//Write to error message file???
		errors += 1;
	} else if (rule == 19) {
		token = NextToken();
		errors += stmt();
		errors += stmt();
		errors += else_part();
	} else if (rule == 20) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 21) {
		token = NextToken();
		errors += stmt();
		errors += stmt();
	} else if (rule == 22) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 24) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 25) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 26) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 27) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 28) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 29) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 30) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 31) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 32) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 33) {
		token = NextToken();
		errors += stmt();
		errors += stmt_list();
	} else if (rule == 34) {
		token = NextToken();
		errors += stmt();
		errors += stmt_list();
	} else if (rule == 35) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 36) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 37) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 38) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 39) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 40) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 41) {
		token = NextToken();
		errors += stmt_list();
	} else if (rule == 42) {
		token = NextToken();
		errors += stmt();
	} else if (rule == 43) {
		token = lex ->GetToken();
	}

	rule = GetRule(10, token);
	ending("action", token, errors);
	lex->debug << "action function returning " << errors << " errors\n";
	return errors;
}

int SyntacticalAnalyzer::any_other_token(){
	lex->debug << "any_other_token function called\n";
	p2file << "any_other_token\n";
	int errors = 0;
	
	int rule = GetRule(11, token);
	string nonTerminal = "any_other_token";
	print(nonTerminal, token, rule);
	cout << "Token_name: " << lex->GetTokenName(token) << endl;
	cout << "Rule: " << rule << endl;
	cout << "Token: " << token << endl;

	if (rule == -1) {
		//throw an error
		//Write to error message file???
		errors += 1;
	} else if (rule == 44){
		token = NextToken();
		more_tokens();
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 45){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 46){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 47){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 48){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 49){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 50){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 51){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 52){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 53){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 54){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 55){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 56){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 57){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 58){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 59){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 60){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 61){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 62){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 63){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 64){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 65){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 66){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 67){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 68){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 69){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 70){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 71){
		token = NextToken();	//Get one additional lexeme
	} else if (rule == 72){
		token = NextToken();	//Get one additional lexeme
	}

	rule = GetRule(11, token);
	ending("any_other_token", token, errors);
	lex->debug << "any_other_token function returning " << errors << " errors\n";
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
  
  if(token == expected){
    return errors;
  }
  else{
    while(token != expected){
     cout << "WE'RE ENFORCING A " << lex->GetTokenName(expected) << "!" << endl;
      token = NextToken();
      errors += 1;
    }
    return errors;
  }

}


token_type SyntacticalAnalyzer::NextToken(){
	token_type t = lex->GetToken();
	char c;
	cout << "Picked up a " << lex->GetTokenName(t) << endl;
	cin >> c;
	return t;
} 
