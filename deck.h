
#include <string>

using namespace std;

#include "card_type.h"

#include "card_list.h"

/* nfunc.cpp */
int RNUM( int min, int max );



#ifndef CLASS_DECK
#define CLASS_DECK


/* crazy8.cpp */
extern card_list bank;


 class deck
 {
   private:
     vector<card_type> data;
	 int max_length;
	 int length;
   public:
     deck();
     ~deck();
     string get_hand_text();
     void shuffle();
     card_type draw();
     bool is_empty();
     int get_length();
     void add_card_front( card_type x );
     void add_card_back( card_type x );
     void get_suite_stat( int array[] );
 };

// private functions

 // public functions
 deck::deck(  )
 {
	 max_length = 52; // we do not want the special cards to appear in the deck
	                  // they are not used for crazy 8's
	 length = 0;

 }

 deck::~deck()
 {
   
 }

 string deck::get_hand_text()
 {
  string temp;

  
  if( length <= 0 )
  {
    temp += "&REmpty!!!";
  }
  else
  for( int i=0; i < length; ++i )
  {
    temp += bank[i].color_abv_name( data[i].abv_name );
    temp += " ";
  }

  return temp;
 }

 void deck::shuffle(   )
 {
  int pos = 0;

  int * key = new int[ max_length ];

  memset( key, 0, max_length  );

  // this function also resets our deck
  length = 0;

  data.clear(); // erase all of the cards we currently have

  // we sequentialy go through the card array finding random cards in the master 
  // deck marking the card and inserting it into our deck

  for( int i= 0;  i <  max_length ;   ++i  ) // random cards chosen for sequential positions
  {
    pos = RNUM(0,max_length  - 1); // we find a random position

    if( key[pos] == 0 ) // we found a position not already taken
    {
      data.push_back( bank[pos] ); // put a random card into the next position
	  ++length;
      key[pos] = 1; // mark our key that we have used that card
    }
    else
    {
      while( key[pos] == 1 )
      {
		  
        pos = RNUM(0, max_length  - 1); // find a new position in the master deck
	
      }
      // now we have the position of our new card so lets insert it
      data.push_back( bank[pos] );
	  ++length;
      key[pos] = 1;
    }
  }// end of for loop


  delete [] key;
 }// end of function shuffle


 /*
 Draw
 returns a random card from the deck sent to the function
 decklength should be checked before being sent to this function
 */
 card_type deck::draw()
 {
  card_type temp = data[ 0 ]; // take the first card of an shuffled deck
 
  // now that we have our card we are going to remove it
  for( int i=0; i < length - 1; ++i )
  {
     // this shifts data backward 1 element position
    data[i] = data[i+1];  
  }
  
   --length;
   
  return temp;
 } 


 bool deck::is_empty()
 {
   if( length <= 0 ) return true;
   else return false;
 }

 int deck::get_length()
 {
   return length;
 }

 void deck::add_card_front( card_type x )
 {
   // our pos is going to be 0
   for( int i=0; i < length; ++i )
   {
     // this shifts data forward 1 element position
     data[i+1] = data[i];
   }
   data[0] = x; // now that all our data has been shifted forward 1 we can set pos 0 to be x
   // after adjusting we incriment length
   ++length;
 }

 void deck::add_card_back( card_type x )
 {
   // check size of deck
   if( length >= max_length  - 1 )
   {
      throw "Extream Error !!! Duplicate Cards exsist in Main Deck";
   }
   // our pos is going to be length 
   data[ length ] = x;
    
   // after adjusting we incriment length
   ++length;
 }

 void deck::get_suite_stat( int array[] )
 {
   array[0] = 0; array[1] = 0; array[2] = 0; array[3] = 0; array[4] = 0;
   for( int i=0; i < length; ++i )
   { 
     if( data[i].value == 50 ) ++array[0];

     switch( data[i].suit )
     {
        case 1:  ++array[1]; break;
        case 2:  ++array[2]; break;
        case 3:  ++array[3]; break;
        case 4:  ++array[4]; break;
     }
   }
 }



#endif














