#ifndef LEX_H
#define LEX_H

#include <iostream>
#include <fstream>

using namespace std;

enum token_type {NONE = -1,
		 LAMBDA ,  //0
 		 IDENT_T,  //1
		 NUMLIT_T, //2
		 CONS_T,   //3
		 IF_T,     //4
		 DISPLAY_T, //5 
		 NEWLINE_T, //6
		 LISTOP_T,  //7
		 AND_T,     //8
		 OR_T,      //9
		 NOT_T,     //10
		 DEFINE_T,  //11
		 NUMBERP_T, //12
		 SYMBOLP_T, //13
		 LISTP_T,   //14
		 ZEROP_T,   //15
		 NULLP_T,   //16
		 CHARP_T,   //17
		 STRINGP_T, //18
		 PLUS_T,    //19
		 MINUS_T,   //20
		 DIV_T,     //21
		 MULT_T,    //22
		 EQUALTO_T, //23
		 GT_T,      //24
		 LT_T,      //25
		 GTE_T,     //26
		 LTE_T,     //27
		 LPAREN_T,  //28
		 RPAREN_T,  //29
		 QUOTE_T,   //30
		 ERROR_T,   //31
		 EOF_T,     //32
		 MAX_TOKENS}; //33

extern string token_names[];

class LexicalAnalyzer 
{
    public:
	LexicalAnalyzer (char * filename);
	~LexicalAnalyzer ();
	token_type GetToken ();
	string GetTokenName (token_type t) const;
	string GetLexeme () const;
	void ReportError (const string & msg);
	ofstream debug;
    private:
	ifstream input;
	ofstream listing;
	ofstream p1file;
	string line;
	int linenum;
	int pos;
	string lexeme;
	int errors;
};
	
#endif
