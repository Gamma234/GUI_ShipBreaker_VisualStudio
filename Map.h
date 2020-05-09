#ifndef Map_H_INCLUDED
#define Map_H_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <ctime>

#include "Gamma.h"

using namespace std ;

namespace SBM {
	
	class Map {

    	private :
    		
			int xLength ; // a, b, c, ...
    		int yLength ; // 0, 1, 2, ...
   			vector<int> blockCondition ; // -1 : no ship // 0,1,2... : ship number
   			vector<string> blockLocation ;
   			vector<int> randomBox ;
   			int rbIndex ; // randomBox's index
   			
   			vector<int> GetRandom( int size ) ;
   			vector<int> GetDirectionPlace( int cur, int shipSize ) ; // size == 0 : can't put here
   			int LevelOf( int num ) ;
   			
   		public :
   			
   			Map( int x, int y ) ; // constructor
   			
   			vector<int> GetAvailablePosition( int shipSize ) ; // size == 0 : input fail
   			bool MoveAvailable( int cur, int step, int condition ) ; // condition -> 0 : up, 1 : down, 2 : left, 3 : right   			
   			void InsertShipLocation( int number, vector<int> location ) ;
			int GetHitCondition( int wp ) ;  // -1 : miss // 0, 1, 2, ... : ship number
			int GetHitPlace( string userInput ) ;
			int GetXLength() ;
			int GetYLength() ;
			void ResetLocation( vector<int> location ) ;   			
   			void OutputMap( bool displayBlockCondition ) ;
   			
	} ; // class Map
	
} // namespace SBM

SBM::Map::Map( int x, int y ) { // constructor

    xLength = x ;
    yLength = y ;

    for ( int i = 0 ; i < xLength * yLength ; i++ ) {
        blockCondition.push_back( -1 ) ;
    } // for

    for ( int i = 0 ; i < yLength ; i++ ) {
        for ( int j = 0, ch = 97 ; j < xLength ; j++, ch++ ) {
        	string str = "" ;
        	str = str + (char)ch + GA::IntToString( i ) ;
            blockLocation.push_back( str ) ;
        } // for
     } // for

    randomBox = GetRandom( xLength * yLength ) ;
    rbIndex = 0 ;

} // SB::Map::Map

vector<int> SBM::Map::GetAvailablePosition( int shipSize ) { // size == 0 : input fail

    vector<int> position ;
    bool findPlace = false ;

    int rbIndexTemp = rbIndex ;

    for ( int place = 0 ; rbIndex < (int)randomBox.size() ; rbIndex++ ) {

        place = randomBox[rbIndex] ; // get a place
        position = GetDirectionPlace( place, shipSize ) ; // size == 0 : can't put here

        if ( position.size() != 0 ) {
            findPlace = true ;
            rbIndex++ ;
            break ;
        } // if

    } // for

    if ( ! findPlace ) {
        rbIndex = rbIndexTemp ;
    } // if

    return position ;

} // SBM::Map::GetAvailablePosition()

vector<int> SBM::Map::GetDirectionPlace( int cur, int shipSize ) { // size == 0 : can't put here

        vector<int> position ;
        bool findPlace = false ;
        vector<int> directionBox = GetRandom( 4 ) ; // 0 1 2 3 random
        int curLevel = -1 ;

        for ( int i = 0 ; blockCondition[cur] == -1 && i < (int)directionBox.size() ; i++ ) {

            if ( directionBox[i] == 0 || directionBox[i] == 1 ) { // 0 : up // 1 : down
                findPlace = true ;
                int changeNum = ( directionBox[i] == 0 ? xLength * (-1) : xLength ) ;
                for ( int j = 1, temp = cur + changeNum ; j < shipSize ; j++, temp = temp + changeNum ) {
                    if ( ( directionBox[i] == 0 ? temp < 0 : temp >= xLength * yLength ) ||
                         ( blockCondition[temp] != -1 ) ) {
                        findPlace = false ;
                        break ;
                    } // if
                } // for
                if ( findPlace ) {
                    for ( int j = 0, temp = cur ; j < shipSize ; j++, temp = temp + changeNum ) {
                        //System.out.print( temp + "  " ); // test
                        position.push_back( temp ) ;
                    } // for
                    //System.out.println() ; // test
                    break ;
                } // if
            } // if
            else { // 2 : left // 3 : right
                curLevel = LevelOf( cur ) ;
                findPlace = true ;
                int changeNum = ( directionBox[i] == 2 ? -1 : 1 ) ;
                for ( int j = 1, temp = cur + changeNum ; j < shipSize ; j++, temp = temp + changeNum ) {
                    if ( ( directionBox[i] == 2 ? temp < 0 : temp >= xLength * yLength ) ||
                         ( curLevel != LevelOf( temp ) ) || ( blockCondition[temp] != -1 ) ) {
                        findPlace = false ;
                        break ;
                    } // if
                } // for
                if ( findPlace ) {
                    for ( int j = 0, temp = cur ; j < shipSize ; j++, temp = temp + changeNum ) {
                        //System.out.print( temp + "  " ); // test
                        position.push_back( temp ) ;
                    } // for
                    //System.out.println() ; // test
                    break ;
                } // if
            } // else

        } // for

        return position ;

} // SBM::Map::GetDirectionPlace()

int SBM::Map::LevelOf( int num ) {

    for ( int last = xLength - 1, level = 1 ; last < xLength * yLength ; last += xLength, level++ ) {
        if ( num <= last ) {
            return level ;
        } // if
    } // for

    return -1 ;

} // SBM::Map::LevelOf()

bool SBM::Map::MoveAvailable( int cur, int step, int condition ) { // condition -> 0 : up, 1 : down, 2 : left, 3 : right

    bool available = true ;

    if ( condition == 0 ) { // up
         for ( int i = 0, temp = cur - xLength ; i < step ; i++, temp = temp - xLength ) {
            if ( temp < 0 || blockCondition[temp] != -1 ) {
                available = false ;
                break ;
            } // if
        } // for
    } // if
    else if ( condition == 1 ) { // down
        for ( int i = 0, temp = cur + xLength ; i < step ; i++, temp = temp + xLength ) {
            if ( temp >= xLength * yLength || blockCondition[temp] != -1 ) {
                available = false ;
                break ;
            } // if
        } // for
    } // else if
    else if ( condition == 2 ) { // left
        int curLevel = LevelOf( cur ) ;
        for ( int i = 0, temp = cur - 1 ; i < step ; i++, temp = temp - 1 ) {
            if ( temp < 0 || curLevel != LevelOf( temp ) || blockCondition[temp] != -1 ) {
                available = false ;
                break ;
            } // if
        } // for
    } // else if
    else { // condition == 3 // right
        int curLevel = LevelOf( cur ) ;
        for ( int i = 0, temp = cur + 1 ; i < step ; i++, temp = temp + 1 ) {
            if ( temp >= xLength * yLength || curLevel != LevelOf( temp ) || blockCondition[temp] != -1 ) {
                available = false ;
                break ;
            } // if
        } // for
    } // else

    return available ;

} // SBM::Map::MoveAvailable()

void SBM::Map::InsertShipLocation( int number, vector<int> location ) {

    for ( int i = 0 ; i < (int)location.size() ; i++ ) {
        blockCondition[location[i]] = number ;
    } // for

} // SBM::Map::InsertShipLocation()

int SBM::Map::GetHitCondition( int wp ) {  // -1 : miss // 0, 1, 2, ... : ship number
	
	return blockCondition[wp] ;

} // SBM::Map::GetHitCondition()

int SBM::Map::GetHitPlace( string userInput ) {
	
	for ( int i = 0 ; i < (int)blockLocation.size() ; i++ ) {
		if ( userInput == blockLocation[i] ) {
			return i ;
		} // if
	} // for

    return -1 ;

} // SBM::Map::GetHitPlace()

int SBM::Map::GetXLength() {

    return xLength ;

} // GetXLength()

int SBM::Map::GetYLength() {

    return yLength ;

} // SBM::Map::GetXLength()

void SBM::Map::ResetLocation( vector<int> location ) {

    for ( int i = 0 ; i < (int)location.size() ; i++ ) {
        blockCondition[location[i]] = -1 ;
    } // for

} // SBM::Map::ResetLocation()

vector<int> SBM::Map::GetRandom( int size ) { // [ 0, size-1 ]

    vector<int> temp ;

    for ( int i = 0 ; i < size ; i++ ) {
        temp.push_back( i ) ;
    } // for
    
    srand( (unsigned)time(0) ) ;

    int g = temp.size() + 1 ;

	for ( int i = 1 ; i < g ; i++ ) {
		int j = rand() % ( g - i ) + i ;
		int k = temp[j-1] ;
		temp[j-1] = temp[i-1] ;
		temp[i-1] = k ;
	} // for

    return temp ;

} // SBM::Map::GetRandom()

void SBM::Map::OutputMap( bool displayBlockCondition ) {

    cout << "--------Map--------" << endl ; 

    for ( int i = 0 ; i < xLength * yLength ; i++ ) {
        cout << blockLocation[i] ;
        if ( displayBlockCondition ) {
        	cout << "(" << blockCondition[i] << ")" ; 
		} // if
		cout << "   " ;
        if ( ( i + 1 ) % xLength == 0 ) {
            cout << endl ;
        } // if
    } // for

    cout << "-------------------" << endl ;

} // SBM::Map::OutputMap()    
    
#endif








