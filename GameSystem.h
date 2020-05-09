#ifndef GameSystem_H_INCLUDED
#define GameSystem_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>

#include "Gamma.h"
#include "Map.h"
#include "Ship.h"

using namespace std ;

namespace SBG {
	
	class GameSystem {

	    private :
	    	
			SBM::Map * map ;
	    	int mapX, mapY, shipNum ;
	    	vector<SBS::Ship> shipBox ; // index is the ship number 
	    	int breakShip ;
	    	string * warningStr ;
	
	    	vector<string> shipNameList, shipNameListTemp ;
	    	vector<int> shipSizeBox, shipSizeBoxTemp ;
	    	vector<int> shipMoveAbilityBox, shipMoveAbilityBoxTemp ;
	    	vector<string> shipBreakSoundBox, shipBreakSoundBoxTemp ;
	    	
	    	string OutputShip() ; // small(2), medium(3), big(4)
	    	void BubbleSort( vector<int> &data ) ; // small to big  
	    
	    public :
	    	
	    	GameSystem( int mapX, int mapY, int shipNum, string * warningStr ) ; // constructor
	    	~GameSystem() { delete map ; } // destructor
	    	
	    	int AvailableShipNum() ;
	    	
	    	string Introduction() ;
	    	string Play( int wp ) ;
	    	int GetMapX() ;
	    	int GetMapY() ;
	    	void SetGame() ;
	    	string GetLocationInfo( int loc ) ;
	    	void PushInTemp( int existShip, string nameStr, int sizeNum, int mvNum, string bsStr ) ;
	    	void SetShip() ;
	    	void SetMap( int xNum, int yNum ) ;
	    
	} ; // class GameSystem
	
} // namespace SBG

SBG::GameSystem::GameSystem( int mapX, int mapY, int shipNum, string * warningStr ) : map( NULL ) { // constructor

    this->mapX = mapX ;
    this->mapY = mapY ;
    this->shipNum = shipNum ;
    this->warningStr = warningStr ;
    breakShip = 0 ;

    shipNameList.push_back( "Small Ship" ) ;
    shipNameList.push_back( "Medium Ship" ) ;
    shipNameList.push_back( "Big Ship" ) ;
    shipSizeBox.push_back( 2 ) ;
    shipSizeBox.push_back( 3 ) ;
    shipSizeBox.push_back( 4 ) ;
    shipMoveAbilityBox.push_back( 2 ) ;
    shipMoveAbilityBox.push_back( 1 ) ;
    shipMoveAbilityBox.push_back( 1 ) ;
    shipBreakSoundBox.push_back( "HIT !" ) ;
    shipBreakSoundBox.push_back( "WHACK !" ) ;
    shipBreakSoundBox.push_back( "SMASH !" ) ;

    SetGame() ;

} // SBG::GameSystem::GameSystem()

string SBG::GameSystem::Introduction() {
	
	string str = "" ;

    str = str + "-----Ship Breaker-----\r\n" ;
    str = str + "The map is " + GA::IntToString( mapX ) + "x" + GA::IntToString( mapY ) 
	      + ". \r\nThere are " + GA::IntToString( AvailableShipNum() ) + " ships :" + OutputShip() + ". \r\n" ;
    str = str + "0. quit \r\n" ;
    str = str + "1. Start \r\n" ;
    str = str + "2. Setting \r\n" ;
    str = str + "-------------------\r\n" ;
    
    //map->OutputMap( true ) ; // test
    
    return str ;

} // SBG::GameSystem::Introduction()

int SBG::GameSystem::AvailableShipNum() {

    int num = 0 ;

    for ( int i = 0 ; i < shipNum ; i++ ) {
        if ( shipBox[i].GetLocation().size() != 0 ) {
            num++ ;
        } // if
    } // for

    return num ;

} // SBG::GameSystem::AvailableShipNum()

string SBG::GameSystem::OutputShip() { // small(2), medium(3), big(4)

    string str = "" ;
    int availableShip = AvailableShipNum() ;

    for ( int i = 0, num = 0 ; i < shipNum ; i++ ) {
        if ( shipBox[i].GetLocation().size() != 0 ) {
            str = str + " " + shipNameList[i] + "(" + GA::IntToString( shipSizeBox[i] ) + ")" + ( num + 1 != availableShip ? "," : "" ) ;
            num++ ;
        } // if
    } // for

    return str ;

} // SBG::GameSystem::OutputShip()

string SBG::GameSystem::Play( int wp ) {

    // cout << "--------Play--------" << endl ;

    string conditionStr = "", message = "" ;

    int hitCondition = map->GetHitCondition( wp ) ; // -1 : miss // 0, 1, 2, ... : ship number

    if ( hitCondition >= 0 ) {
        message = shipBox[hitCondition].CheckHitCondition( wp, conditionStr ) ; // "hit", "already hit", "kill"
        if ( message == "hit" ) {
            shipBox[hitCondition].Move( *map ) ;
        } // if
        else if ( message == "kill" ) {
            breakShip++ ;
            if ( breakShip == AvailableShipNum() ) {
                // cout << "\nSuccessful !" << endl ;
                conditionStr = "Successful !" ;
            } // if
        } // else if
    } // if
    else {
        //cout << "miss" << endl ;
        conditionStr = "miss" ;
    } // else

    // cout << "---------------------" << endl ;
    
    //map->OutputMap( true ) ; // test
    
    return conditionStr ;

} // SBG::GameSystem::Play()

int SBG::GameSystem::GetMapX() {
	
	return mapX ;
	
} // SBG::GameSystem::GetMapX()

int SBG::GameSystem::GetMapY() {
	
	return mapY ;
	
} // SBG::GameSystem::GetMapX()

void SBG::GameSystem::SetGame() {
	
	*warningStr = "Warning :\r\n" ;
	
	if ( map != NULL ) delete map ;

    map = new SBM::Map( mapX, mapY ) ;
    shipBox.clear() ;
    breakShip = 0 ;
    vector<int> position ;

    for ( int i = 0 ; i < shipNum ; i++ ) {
        position = map->GetAvailablePosition( shipSizeBox[i] ) ; // size == 0 : input fail
        if ( position.size() == 0 ) {
            // cout << "[Input Fail] " << shipNameList[i] << "(" << shipSizeBox[i] << ")" << endl ;
            *warningStr = *warningStr + "[Input Fail] " + shipNameList[i] + "(" + GA::IntToString( shipSizeBox[i] ) + ")\r\n" ;
        } // if
        BubbleSort( position ) ;
        SBS::Ship ship( i, shipNameList[i], shipSizeBox[i], shipMoveAbilityBox[i], shipBreakSoundBox[i], position,
                      ( map->GetXLength() != 1 && position.size() != 0 && position.size() != 1 && position[1] - position[0] == 1 ? false : true ) ) ;
        map->InsertShipLocation( i, position ) ;
        shipBox.push_back( ship ) ;
    } // for

} // SBG::GameSystem::SetGame()

void SBG::GameSystem::BubbleSort( vector<int> &data ) { // small to big

    for ( int i = 0 ; i < (int)data.size() ; i++ ) {
        for ( int j = 1 ; j < (int)data.size() ; j++ ) {
            if ( data[j-1] > data[j] ) {
                int temp = data[j-1] ;
                data[j-1] = data[j] ;
                data[j] = temp ;
            } // if
        } // for
    } // for

} // SBG::GameSystem::BubbleSort()

string SBG::GameSystem::GetLocationInfo( int loc ) {
	
	return GA::IntToString( map->GetHitCondition( loc ) ) ;
	
} // SBG::GameSystem::GetLocationInfo()  

void SBG::GameSystem::PushInTemp( int existShip, string nameStr, int sizeNum, int mvNum, string bsStr ) {
	
	if ( existShip == 0 ) {
		shipNameListTemp.clear() ;
		shipSizeBoxTemp.clear() ;
		shipMoveAbilityBoxTemp.clear() ;
		shipBreakSoundBoxTemp.clear() ;
	} // if
	
	shipNameListTemp.push_back( nameStr ) ;
	shipSizeBoxTemp.push_back( sizeNum ) ;
	shipMoveAbilityBoxTemp.push_back( mvNum ) ;
	shipBreakSoundBoxTemp.push_back( bsStr ) ;
	
} // SBG::GameSystem::PushInTemp()

void SBG::GameSystem::SetShip() {

    shipNameList = shipNameListTemp ;
	shipSizeBox = shipSizeBoxTemp ;
	shipMoveAbilityBox = shipMoveAbilityBoxTemp ;
	shipBreakSoundBox = shipBreakSoundBoxTemp ;
	
	shipNum = shipNameListTemp.size() ;
	
	SetGame() ;
    // cout << "\nCompleted !\n" << endl ;

} // SBG::GameSystem::SetShip()

void SBG::GameSystem::SetMap( int xNum, int yNum ) {

    mapX = xNum ;
    mapY = yNum ;

    SetGame() ;
    // cout << "\nCompleted !\n" << endl ;

} // SBG::GameSystem::SetMap()

#endif












