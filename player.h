/*
Nathaniel Collier
7/17/2006
Player Class for Crazy8 Card Game
*/

#ifndef PLAYER_CLASS
#define PLAYER_CLASS

#include "deck.h"

#include "card_type.h"

class player
{
 private:
  int num_heart;
  int num_club;
  int num_spade;
  int num_diamond;
  int num_eight;
  card_type* pdeck;
  int deck_length;
  int total_score;
  int games_won;
  int games_lost;
  int games_played;
  int card_points;
  bool pass;
  string player_name;
  // functions
  int count_card_points();
  int high_suit();
  void reset_game_stats();
 public:
  // functions
  player();
  ~player();
  void new_game();
  void insert_card( card_type temp );
  string get_hand_text();
  card_type play_card( string abv_name );
  int get_points(  );
  int get_total_score();
  int get_games_won();
  int get_games_lost();
  int get_games_played();
  int get_num_card_suite( int suite );
  int hand_length();
  void set_playername( string name );
  string get_playername();
  void end_game( string winner, int points );
  void set_pass(bool value);
  bool get_pass();
  card_type find_move( card_type top_card );
  bool can_play( card_type top_card );
  void remove_card( card_type x );
  void get_suite_stat( int array[] );
  void set_iscomputer( bool x );
  bool get_iscomputer();
  // AI Functions
  card_type AI_can_play( card_type top_card );
  
};


// private functions

 /*
  This function will determine wich suit has the most
   cards in the hand
     1 hearts
     2 diamonds
     3 spades
     4 clubs
 */
 int player::high_suit()
 {
   int high = -1; // there is no suite -1 this is for errors

   // simple if checks for this
    if( num_heart >= num_club && num_heart >= num_spade && num_heart >= num_diamond )
     high = 1;
     else
    if( num_club >= num_heart && num_club >= num_spade && num_club >= num_diamond )
     high = 4;
     else
    if( num_spade >= num_heart && num_spade >= num_diamond && num_spade >= num_club )
     high = 3;
     else
    if( num_diamond >= num_heart && num_diamond >= num_spade && num_diamond >= num_club )
     high = 2;

   return high;
 }
 
 /*
 Goes through the deck adding up the points
 */
 int player::count_card_points()
 {
  int temp = 0;
   for( int i=0; i < deck_length; ++i )
   {
    temp += pdeck[i].value;
   }
  return temp;
 }

 void player::reset_game_stats()
 {
  deck_length = 0;
  num_heart = 0;
  num_spade = 0;
  num_diamond = 0;
  num_club = 0;
  num_eight = 0;
  card_points = 0;
  pass = false;
 }

// public functions

player::player()
{
  pdeck = new card_type[64]; // the deck could not be any larger than this
  total_score = 0;
  games_won = 0;
  games_lost = 0;
  games_played = 0;
  reset_game_stats(); // sets current game stats to beginning values
}

player::~player()
{
  delete [] pdeck;
}

/*
    1 hearts
     2 diamonds
     3 spades
     4 clubs
*/
int player::get_num_card_suite( int suite )
{
  int count = 0;

  switch( suite )
  {
    case 1: count = num_heart; break;
    case 2: count = num_diamond; break;
    case 3: count = num_spade; break;
    case 4: count = num_club; break;
    default: count = 0; break;
  }

  return count;
}


void player::end_game( string winner, int points )
{
  if( winner == player_name )
  {
    total_score += points;
    ++games_played;
    ++games_won;
  }
  else
  {
    ++games_played;
    ++games_lost;
  }
 return;
}


void player::new_game()
{
  reset_game_stats(); // sets current game stats to beginning values
}

void player::insert_card( card_type temp )
{
  // update our card count
  
  if( temp.value == 8 )
  {
    ++num_eight;
  }
  else
  switch( temp.suit )
  {
    case 1:
         ++num_heart;
         break; 
      case 2:
         ++num_diamond;
         break;
      case 3:
         ++num_spade;
         break;
      case 4:
         ++num_club;
         break;
  }
  
  pdeck[deck_length] = temp;
  ++deck_length;


}


string player::get_hand_text()
{
  string temp;

  
  if( deck_length <= 0 )
  {
    temp += "&REmpty!!!";
  }
  else
  for( int i=0; i < deck_length; ++i )
  {
    temp += bank[i].color_abv_name( pdeck[i].abv_name );
    temp += " ";
  }

 return temp;
}

/*
Finds the card with abv_name equal to that received by the function
If the card is not found the null card is returned by the function
This function could also be considered a 
Remove card with return of removed card
*/
card_type player::play_card( string abv_name )
{
  card_type temp;
  temp.face = -1; // null card

  // try and find the card
  for( int i=0; i < deck_length; ++i )
  {
    if( abv_name == pdeck[i].abv_name )
    {
      --deck_length;
      temp = pdeck[i];
       // remove the card from the deck by shifting
      for( int j=i; j < deck_length; ++j )
      {
        pdeck[j] = pdeck[j+1];
      }
      // update other counters and the deck_length
        if( temp.value == 50 )
  	{
  	  --num_eight;
  	}
  	else
  	switch( temp.suit )
  	{
    	  case 1:
           --num_heart;
           break; 
          case 2:
           --num_diamond;
           break;
          case 3:
           --num_spade;
           break;
          case 4:
           --num_club;
           break;
        }
    }
  }
  return temp; // either returns the found card or the null card
}

/*
Returns the number of points a player has in their hand
*/
int player::get_points(  )
{
  card_points = count_card_points();
  return card_points;
}

int player::get_total_score() 
{
  return total_score;
}

 int player::get_games_won() 
 {
  return games_won;
 }

 int player::get_games_lost() 
 {
  return games_lost;
 }

 int player::get_games_played() 
 {
  return games_played;
 }

 int player::hand_length() 
 {
   return deck_length;
 }


 void player::set_playername( string name )
 {
   player_name = name;
 }
 
 string player::get_playername() 
 {
   return player_name;
 }

 bool player::get_pass()
 {
   return pass;
 }

 void player::set_pass(bool value)
 {
   pass = value;
 }


  bool player::can_play( card_type top_card )
  {
    for( int i = 0; i < deck_length; ++i )
    {
       if( pdeck[i].face == 8 ) return true;
	   else
	   if( pdeck[i].suit == top_card.suit ) return true;
 	   else
  	   if( pdeck[i].face == top_card.face ) return true;
    }
     return false; // we found no card that could be played
   }

 /*
  This function is almost identical to the AI_can_move function
  except this function does not remove the found card from the player
  hand.
  When an 8 is found it returns the card found not a chosen 8 card from the static card list
 */
 card_type player::find_move( card_type top_card )
 {
   card_type temp;
   int high = 0;
   
   temp.face = -1; // just incase we don't find anything
   for( int i=0; i < deck_length; ++i )
   {
     if( pdeck[i].value == 50 )
     {
       temp = pdeck[i];
       return temp;
     }
     else
     if( pdeck[i].suit == top_card.suit )
     {
       temp = pdeck[i];
       // we found a card lets see if we can find another one
       // of the same type with more value else we will return this one
       for( int j=0; j < deck_length; ++j )
       {
         if( pdeck[j].value > temp.value && pdeck[j].suit == temp.suit && pdeck[j].face != 8 )
         {
           temp = pdeck[j];
         }
       }
       return temp;
     }
     else
     if( pdeck[i].face == temp.face )
     {
       // if for example you have a 5h and a 5s you can play the 5s if the topcard is 5h
        // because the values of the cards = even though the suites do not =
       temp = pdeck[i];
       return temp;
     }

   }
  return temp;
 }

 void player::remove_card( card_type x )
 {
  // find the card
  for( int i=0; i < deck_length; ++i )
  {
    if( x.abv_name == pdeck[i].abv_name )
    {
      --deck_length;
       // remove the card from the deck by shifting
      for( int j=i; j < deck_length; ++j )
      {
        pdeck[j] = pdeck[j+1];
      }
      // update other counters and the deck_length
        if( x.value == 50 )
  	{
  	  --num_eight;
  	}
  	else
  	switch( x.suit )
  	{
    	  case 1:
           --num_heart;
           break; 
          case 2:
           --num_diamond;
           break;
          case 3:
           --num_spade;
           break;
          case 4:
           --num_club;
           break;
        }
    }
  }
  return;
}


void player::get_suite_stat( int array[] )
{
  array[0] = 0; array[1] = 0; array[2] = 0; array[3] = 0; array[4] = 0;
  for( int i=0; i < deck_length; ++i )
  {
    if( pdeck[i].value == 50 ) ++array[0];

    switch( pdeck[i].suit )
    {
       case 1:  ++array[1]; break;
       case 2:  ++array[2]; break;
       case 3:  ++array[3]; break;
       case 4:  ++array[4]; break;
    }
  }

}

  // AI Functions

 /*
  Loop through the pdeck to determine if a card can be played
  if one is found return it
 */
 card_type player::AI_can_play( card_type top_card )
 {
   card_type temp;
   int high = 0;
   
   temp.face = -1; // just incase we don't find anything
   for( int i=0; i < deck_length; ++i )
   {
     if( pdeck[i].value == 50 )
     {
       temp = pdeck[i];
       play_card( temp.abv_name ); // lets get rid of that card from the hand

       // alright we have an 8 card so we need to choose wich one we want
       // lets compare the number of each suite and change the card to that suite
       high = high_suit();

        switch( high )
        {
         case 1: temp = bank[28]; break;
         case 2: temp = bank[29]; break;
         case 3: temp = bank[30]; break;
         case 4: temp = bank[31]; break;
         default:  temp = bank[28]; break;
        }
      
      return temp;
     }
     else
     if( pdeck[i].suit == top_card.suit )
     {
       temp = pdeck[i];
       // we found a card lets see if we can find another one
       // of the same type with more value else we will return this one
       for( int j=0; j < deck_length; ++j )
       {
         if( pdeck[j].value > temp.value && pdeck[j].suit == temp.suit && pdeck[j].face != 8 )
         {
           temp = pdeck[j];
         }
       }
       play_card( temp.abv_name );
       return temp;
     }
     else
     if( pdeck[i].face == top_card.face )
     {
       // if for example you have a 5h and a 5s you can play the 5s if the topcard is 5h
        // because the values of the cards = even though the suites do not =
       temp = pdeck[i];
       play_card( temp.abv_name );
       return temp;
     }

   }
  return temp;
 }


#endif






