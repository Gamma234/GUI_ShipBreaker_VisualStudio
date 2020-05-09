#ifndef Ship_H_INCLUDED
#define Ship_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Map.h"

using namespace std ;

namespace SBS {
	
	class Ship {

    	private :
    		
			int number ;
   			string name ;
    		int size ;
    		int moveAbility ;
    		bool isVertical, alive ;
    		int brokenNum ;
    		vector<int> shipCondition ; // 0 : not broken // 1 : broken
    		vector<int> mapLocation ; // sort : small to big
    		string breakSound ;
    		
    		vector<int> GetRandom( int size ) ; // [ 0, size-1 ]
    		void SetNewLocation( SBM::Map map, int step, int condition ) ;
    		
    	public :
    		
    		Ship( int number, string name, int size, int moveAbility, string breakSound, vector<int> mapLocation, bool isVertical ) ; // constructor
    		
    		void SetPosition( vector<int> mapLocation ) ;
    		vector<int> GetLocation() ;
    		string CheckHitCondition( int hitPlace, string &conditionStr ) ; // "hit", "already hit", "kill"
    		void Move( SBM::Map &map ) ;
    		
    } ; // class Ship	
	
} // namespace SBS

SBS::Ship::Ship( int number, string name, int size, int moveAbility, string breakSound, vector<int> mapLocation, bool isVertical ) { // constructor

    this->number = number ;
    this->name = name ;
    this->size = size ;
    this->moveAbility = moveAbility ;
    this->breakSound = breakSound ;
    brokenNum = 0 ;

    for ( int i = 0 ; i < size ; i++ ) {
        shipCondition.push_back( 0 ) ;
    } // for

    SetPosition( mapLocation ) ;
    this->isVertical = isVertical ;
    alive = true ;

} // SBS::Ship::Ship()

void SBS::Ship::SetPosition( vector<int> mapLocation ) {

        this->mapLocation = mapLocation ;

} // SBS::Ship::SetPosition()

vector<int> SBS::Ship::GetLocation() {

    return mapLocation ;

} // SBS::Ship::GetLocation()

string SBS::Ship::CheckHitCondition( int hitPlace, string &conditionStr ) { // "hit", "already hit", "kill"

    string tellStr = "" ;

	if ( ! alive ) {
		conditionStr = conditionStr + name + " is already dead !" ;
		return "already hit" ;
	} // if 

    int index = 0 ;

    for ( ; index < (int)mapLocation.size() ; index++ ) {
        if ( mapLocation[index] == hitPlace ) {
             break ;
        } // if
    } // for

    if ( shipCondition[index] == 1 ) {
        // cout << "Already hit the place on " << name << " !" << endl ;
        conditionStr = conditionStr + "Already hit the place on " + name + " !" ;
        tellStr = "already hit" ;
    } // if
    else { // shipCondition[index] == 0
        shipCondition[index] = 1 ;
        brokenNum++ ;
        if ( brokenNum == size ) {
            // cout << name << " is killed !" << endl ;
            conditionStr = conditionStr + name + " is killed !" ;
            tellStr = "kill" ;
            alive = false ;
        } // if
        else {
            // cout << breakSound << endl ;
            conditionStr = conditionStr + breakSound ;
            tellStr = "hit" ;
        } // else
    } // else

    return tellStr ;

} // SBS::Ship::CheckHitCondition()

void SBS::Ship::Move( SBM::Map &map ) {

    vector<int> directionBox = GetRandom( 2 ) ; // 0, 1 random
    vector<int> step = GetRandom( moveAbility + 1 ) ;

    if ( isVertical ) { // 0 : up // 1 : down
        for ( int i = 0 ; i < (int)directionBox.size() ; i++ ) {
            for ( int j = 0 ; j < (int)step.size() ; j++ ) {
                if ( step[j] == 0 ) continue ;
                if ( directionBox[i] == 0 ) { // up
                    if ( map.MoveAvailable( mapLocation[0], step[j], 0 ) ) {
                        map.ResetLocation( mapLocation ) ;
                        SetNewLocation( map, step[j], 0 ) ;
                        map.InsertShipLocation( number, mapLocation );
                        return ;
                    } // if
                } // if
                else { // down
                    if ( map.MoveAvailable( mapLocation[mapLocation.size()-1], step[j], 1 ) ) {
                        map.ResetLocation( mapLocation ) ;
                        SetNewLocation( map, step[j], 1 ) ;
                        map.InsertShipLocation( number, mapLocation );
                        return ;
                    } // if
                } // else
            } // for
        } // for
    } // if
    else { // 0 : left // 1 : right
        for ( int i = 0 ; i < (int)directionBox.size() ; i++ ) {
            for ( int j = 0 ; j < (int)step.size() ; j++ ) {
                if ( step[j] == 0 ) continue ;
                if ( directionBox[i] == 0 ) { // left
                    if ( map.MoveAvailable( mapLocation[0], step[j], 2 ) ) {
                        map.ResetLocation( mapLocation ) ;
                        SetNewLocation( map, step[j], 2 ) ;
                        map.InsertShipLocation( number, mapLocation );
                        return ;
                    } // if
                } // if
                else { // right
                    if ( map.MoveAvailable( mapLocation[mapLocation.size()-1], step[j], 3 ) ) {
                        map.ResetLocation( mapLocation ) ;
                        SetNewLocation( map, step[j], 3 ) ;
                        map.InsertShipLocation( number, mapLocation );
                        return ;
                    } // if
                } // else
            } // for
        } // for
    } // else

} // SBS::Ship::Move()

void SBS::Ship::SetNewLocation( SBM::Map map, int step, int condition ) {

    vector<int> temp = mapLocation ;
    mapLocation.clear() ;

    if ( condition == 0 ) { // up
        int xLength = map.GetXLength() ;
        for ( int i = 0 ; i < (int)temp.size() ; i++ ) {
            mapLocation.push_back( temp[i] - xLength * step ) ;
        } // for
    } // if
    else if ( condition == 1 ) { // down
        int xLength = map.GetXLength() ;
        for ( int i = 0 ; i < (int)temp.size() ; i++ ) {
            mapLocation.push_back( temp[i] + xLength * step ) ;
        } // for
    } // else if
    else if ( condition == 2 ) { // left
        for ( int i = 0 ; i < (int)temp.size() ; i++ ) {
            mapLocation.push_back( temp[i] - step ) ;
        } // for
    } // else if
    else { // condition == 3 // right
        for ( int i = 0 ; i < (int)temp.size() ; i++ ) {
            mapLocation.push_back( temp[i] + step ) ;
        } // for
    } // else

} // SBS::Ship::SetNewLocation()

vector<int> SBS::Ship::GetRandom( int size ) { // [ 0, size-1 ]

    vector<int> temp ;

    for ( int i = 0 ; i < size ; i++ ) {
        temp.push_back( i ) ;
    } // for
    
    srand((unsigned)time(0) ) ;

    int g = temp.size() + 1 ;

	for ( int i = 1 ; i < g ; i++ ) {
		int j = rand() % ( g - i ) + i ;
		int k = temp[j-1] ;
		temp[j-1] = temp[i-1] ;
		temp[i-1] = k ;
	} // for

    return temp ;

} // SBS::Ship::GetRandom()

#endif









