#ifndef GAMMA_H_INCLUDED
#define GAMMA_H_INCLUDED

#include <iostream>
#include <string>
#include <math.h>

using namespace std ;

namespace GA {

    int StringToInt( string temp ) ; // string -> int 
    string IntToString( int num ) ; // int -> string 
    string CharStringToString( char * str ) ; // char string -> string 
    
    bool IsInteger( string temp ) ;

} ; // namespace GA 

int GA::StringToInt( string temp ) { // string -> int 

	string str = "" ;
	
	for ( int i = 0 ; i < (int)temp.length() ; i++ ) {
		if ( temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' ) { // not white 
			str = str + temp[i] ;
		} // if
	} // for

	int i = ( str[0] == '-' ? 1 : 0 ), value = 0 ;

	for (int mul = str.length() - i - 1; i < (int)str.length(); i++, mul--) {
		int mulResult = 1;
		for (int j = 0; j < mul ; j++) {
			mulResult = mulResult * 10;
		} // for
		value = value + (str[i] - '0') * mulResult;
	} // for 

    return ( str[0] == '-' ? value * (-1) : value ) ;

} // GA::StringToInt()

string GA::IntToString( int num ) { // int -> string 
	
	string temp = "", temp2 = "" ;
	
	if ( num < 0 ) {
		num *= (-1) ;
		temp2 += '-' ;
	} // if	
	
	do {
		temp += (char)( num % 10 + '0' ) ;
		num /= 10 ;
	} while ( num != 0 ) ;
	
	for ( int i = temp.length() - 1 ; i >= 0 ; i-- )
		temp2 += temp[i] ;
	
	return temp2 ;
	
} // GA::IntToString()

string GA::CharStringToString( char * str ) { // char string -> string 
	
	string temp = "" ;
	
	for ( int i = 0 ; str[i] != '\0' ; i++ ) {
		temp = temp + str[i] ;
	} // for	
  
    return temp ;	
	
} // GA::CharStringToString() 

bool GA::IsInteger( string temp ) {
	
	string str = "" ;
	
	for ( int i = 0 ; i < (int)temp.length() ; i++ ) {
		if ( temp[i] != ' ' && temp[i] != '\t' && temp[i] != '\n' ) { // not white 
			str = str + temp[i] ;
		} // if
	} // for
	
	if ( str == "" ) return false ;
	
	for ( int i = ( str[0] == '-' ? 1 : 0 ) ; i < (int)str.length() ; i++ ) {
		if ( str[i] < '0' || str[i] > '9' ) {
			return false ;
		} // if
	} // for
	
	return true ;
	
} // GA::IsInteger() 


#endif // GAMMA_H_INCLUDED
