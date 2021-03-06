/*
  Stephen Alexander Friedman
  saf217
  CSE 109
  Prog4
  This program involves the Lex and Token classes as well as a test
  class to take in text and determine what Tokens that text contains.
  This acts as a text reader for a compiler, determining what type of
  input or code is being put in a file. This class specifically does
  the work of determining what the strings of text mean. There are
  many different ways this program classifies these strings, and
  ultimately each Token can be one of 33 types that this program can
  recognize.The token class is the type object that Lex creates based
  on what the input was. A token has a type, a lexeme, line #, and
  line position. There are 33 different types for a token.
  While it does compile, I have some sort of run time error
  that I know involves handilng end of files or end of lines. I
  suspect that when the program reaches the end of line or end of
  file, I am trying to use the EOF or \n character in a token causing
  some sort of memory issue.
 */
#include <iostream>
#include<cstdlib>
#include<string>
#include "Lex.h"
#include "Token.h"
using namespace std;



Lex::Lex(istream &text):input(text)
{
  lineNum=0;
  linePosition=0;
  endOfFile=false;
}


char Lex::nextChar()
{
  linePosition++;
  char letter;
  if(input.good())
    {
      if(input.eof())
	{
	  //endfile
	  return '^';
	  endOfFile=true;
	}
      else
	{
	 letter=input.get();
	 if(letter=='\n')
	   {
	     lineNum++;
	     //endline
	     linePosition=0;
	     return '$';
	   }
	 //found a comment
	 else if(letter=='#')
	   {
	     letter=input.get();
	     if(input.eof())
		 return '^';
	     
	     while(letter!='#')
	       {
		 letter=input.get();
		 if(input.eof())
		   return '^';
	       }  
	   }
	 return letter;
        }
 }
  else
    return '^';
}

bool Lex::isSpecialChar(char c)
{
  //note that this does not account for == <= >= !=
  if(c=='+' ||c=='-' ||c=='*' ||c=='/' ||c=='%' ||c=='=' ||c=='(' ||c==')' ||c==',' ||c=='<' ||c=='>' ||c=='!')
    return true;
  else
    return false;

}

Token Lex::nextToken()
{
 
  bool isStringLit=false;
  int type=0;
  string lexeme="";
  int line=1;
  int pos=0;
  char leftChar;
  char rightChar=nextChar();
  // cout<<rightChar<<endl;
  while(rightChar=='^'||rightChar=='$')
    {
      cout<<rightChar<<endl;
      if(rightChar=='^')
	{
	  type=33;
	  line=lineNum;
	  pos=linePosition;
	  Token t= Token(type,"EOF",line,pos);
	  return t;
	}
      else
	rightChar=nextChar();
    }
  //now right char should be a valid char
  //so set leftChar to rightChar
  //get a new rightChar
  leftChar=rightChar;
  rightChar=nextChar();
 
  while(leftChar==' ')
    {
      //cout<<"fhregedgerwefew"<<endl;
      leftChar=rightChar;
      rightChar=nextChar();
    }
	  //SPECIAL CHARACTERS
	  if(isSpecialChar(leftChar)==true && isStringLit==false && leftChar!='"')
	    {
	      isStringLit=false;
		
	      if(rightChar=='=')
		{

		  if(leftChar=='<')
		    {
		      //cout<<"<<<<<<<<"<<endl;
		      leftChar=rightChar;
		      //cout<<leftChar<<endl;
		      rightChar=nextChar();
		      //cout<<rightChar<<endl;
		      if(rightChar=='^' ||rightChar=='$'||rightChar==' ')	
			{
			  type=17;
			  lexeme="<=";
			  line=lineNum;
			  pos=linePosition;
			  Token t= Token(type,"<=",line,pos);
			  return t;
			}
		      else
			{
			  type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t= Token(type,"ERROR",line,pos);
			  return t;
			}
		    }
		  else if(leftChar=='>')
		    {
		      leftChar=rightChar;
		      rightChar=nextChar();
		      if(rightChar=='^' ||rightChar=='$')
			{
			  type=15;
			  lexeme=">=";
			}
		      else
			{
			  type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"ERROR",line,pos);
			  return t;
			}
		      
		    }
		  else if(leftChar=='=')
		    {
		      leftChar=rightChar;
		      rightChar=nextChar();
		      if(rightChar=='^' ||rightChar=='$')
			{
			  type=19;
			  lexeme="==";
			}
		      else
			{
			  type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"ERROR",line,pos);
			  return t;
			}
		      
		    }
		  else if(leftChar=='!')
		    {
		      leftChar=rightChar;
		      rightChar=nextChar();
		      if(rightChar=='^' ||rightChar=='$')
			{
			  type=14;
			  lexeme="!=";
			}
		      else
			{
			  type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"Error",line,pos);
			  return t;
			}  
		    }
		  else
		    {
		      type=32;
		      line=lineNum;
		      pos=linePosition;
		      Token t = Token(type,"ERROR",line,pos);
		      return t;
		    }
		}
		  else if(rightChar!='=')
		    {
		      if(leftChar=='=' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=13;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"=",line,pos);
			  return t;
			}
		      else  if(leftChar=='(' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=12;
			  lexeme.append(&rightChar);
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"(",line,pos);
			  return t;
			}
		      else  if(leftChar==')' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=11;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,")",line,pos);
			  return t;
			}
		      else  if(leftChar=='+' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=9;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"+",line,pos);
			  return t;
			}
		      else  if(leftChar=='-' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=8;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"-",line,pos);
			  return t;
			}
		      else  if(leftChar=='/' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=6;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"/",line,pos);
			  return t;
			}
		      else  if(leftChar=='*' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=7;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"*",line,pos);
			  return t;
			}
		      else  if(leftChar=='%' && (rightChar==' ' || rightChar=='\n' || rightChar=='$'))
			{
			  type=5;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"%",line,pos);
			  return t;
			}
		      else
			{
			  type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"ERROR",line,pos);
			  return t;
			}
		    }
	    }
	  
	  else if(isdigit(leftChar) && isStringLit==false && leftChar!='"')
	    { 
	      while(isdigit(leftChar))
		{
		  lexeme+=leftChar;
		 
		  leftChar=rightChar;
		  if(isdigit(leftChar))
		     rightChar=nextChar();
		}
	     
	      //INTLIT
	      if(rightChar=='x'||rightChar==' '||rightChar=='^'||rightChar=='$')
		{
		 
		  if(rightChar==' ' || rightChar=='^' || rightChar=='$')
		    {
		      type=1;
		      line=lineNum;
		      pos=linePosition;
		      Token t= Token(type,lexeme,line,pos);
		      return t;
		    }
		  bool isIntLit=false;
		  leftChar=rightChar;
		  rightChar=nextChar();
		  lexeme+=leftChar;
		  while(isdigit(rightChar) || rightChar=='a' || rightChar=='b' || rightChar=='c' || rightChar=='d'||rightChar=='e' || rightChar=='f')
		    {
		      lexeme+=rightChar;
		      leftChar=rightChar;
		      rightChar=nextChar();
		      isIntLit=true;
		    }
		  if((rightChar=='\n' || rightChar=='$')&&isIntLit==true)
		    {
		       type=4;
		       line=lineNum;
		       pos=linePosition;
		       Token t = Token(type,lexeme,line,pos);
		       return t;
		    }
		  else
		    {
		       type=32;
		       line=lineNum;
		       pos=linePosition;
		       Token t = Token(type,"Error",line,pos);
		       return t;
		    }
		}
	      //FLOATLIT
	      else if(rightChar=='.')
		{
		  bool isFloatLit=false;
		  leftChar=rightChar;
		  rightChar=nextChar();
		  
		   while(isdigit(rightChar))
		    {
		      lexeme+=rightChar;
		      leftChar=rightChar;
		      rightChar=nextChar();
		      isFloatLit=true;
		    }
		   if((rightChar=='\n' || rightChar=='$')&&isFloatLit==true)
		    {
		       type=3;
		       line=lineNum;
		       pos=linePosition;
		       Token t = Token(type,lexeme,line,pos);
		       return t;
		    }
		   else
		    {
		          type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"Error",line,pos);
			  return t;
		    }
		}
	    }



	  //STRINGLIT
	  else if(leftChar=='"')
	    {
	      //not true yet
	      type=2;
	      
	      while(rightChar!='"')
		{
		  if(rightChar=='\n' || rightChar=='$')
		    {
		          type=32;
			  line=lineNum;
			  pos=linePosition;
			  Token t = Token(type,"Error",line,pos);
			  return t;
		    }
		  lexeme+=rightChar;
		  leftChar=rightChar;
		  rightChar=nextChar();
		}
	      if(rightChar=='"')
		{
		       type=2;
		       line=lineNum;
		       pos=linePosition;
		       Token t = Token(type,lexeme,line,pos);
		       return t;
		}
	    }
     
	  
	  //THE REST
	  else
	    {
    	      while(leftChar!= ' ')
		{
     		  if(rightChar=='^')
		    {
       		      type=33;
		      line=lineNum;
		      pos=linePosition;
		      Token t = Token(type,"EOF",line,pos);
		      return t;
		    }
		  if(rightChar=='$')
		    {
      		      type=32;
		      line=lineNum;
		      pos=linePosition;
		      Token t = Token(type,"ENDL",line,pos);
		      return t;
		    }

		  //IDENT
		  if(isalpha(leftChar))
		    {
		      bool isIdent=false;
		      lexeme+=leftChar;
		      if(rightChar==' ' || rightChar=='$' || rightChar=='^')
			{			
      			  type=1;
			  //lexeme+=leftChar;
			  line=lineNum;
			  pos=linePosition;
			  Token t= Token(type,lexeme,line,pos);
			  return t;
			}
		      while(isalpha(rightChar)||rightChar=='_'||isdigit(rightChar))
			{
			  lexeme+=rightChar;
			  leftChar=rightChar;
			  rightChar=nextChar();
			  isIdent=true;

			  if(lexeme=="SET" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=21;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			  else  if(lexeme=="PRINT" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=20;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else  if(lexeme=="WHILE" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=24;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else  if(lexeme=="DO" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=23;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="END" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=22;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="AND" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=26;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="OR" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=25;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="IF" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=30;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="THEN" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=29;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="ELSE" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=28;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="ENDIF" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=27;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if(lexeme=="PROGRAM" && (rightChar=='$'|| rightChar==' '||rightChar=='^'))
			    {
			      type=31;
			      line=lineNum;
			      pos=linePosition;
			      Token t = Token(type,lexeme,line,pos);
			      return t;
			    }
			    else if((rightChar=='$'||rightChar==' ')&&isIdent==true)
			      {
				type=1;
				line=lineNum;
				pos=linePosition;
				Token t = Token(type,lexeme,line,pos);
				return t;
			      }
			  /*else if(isdigit(rightChar)||rightChar=='$' || rightChar=='^')
			      {
				cout<<"FUNNY ERROR"<<endl;
				type=32;
				line=lineNum;
				pos=linePosition;
				Token t = Token(type,"ERROR",line,pos);
				return t;
			      }
			  */
			      }
		    }
		}
	    }	 
  //this instance happens if the 
  Token t = Token(32,"ERROR",-100,-100);
  return t;
  
}
