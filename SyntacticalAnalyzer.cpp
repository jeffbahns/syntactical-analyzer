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

int SyntacticalAnalyzer::program ()
{
/********************************************************************************/
/* This function will								*/
/********************************************************************************/
	lex->debug << "program function called\n";
	p2file << "program\n";
	int errors = 0;
	// token should be in the firsts of program
	// if true... keep going
	// if false...
	// Error message -
	// 	then keep going or keep getting token until token is
	// 	in the firsts of program

	// errors += define ();

	// errors += more_defines ();

	if (token != EOF_T)
	{
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

