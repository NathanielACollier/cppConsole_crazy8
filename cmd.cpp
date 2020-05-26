/*
  Nathaniel Collier
  2/10/2007
  Crazy 8 Command Interpriter
  * uses technology from nsh 
*/
#include <iostream>

using namespace std;

#include "hash_table.h"
#include "deck.h"
#include "player.h"
#include "stack.h"

#include "crazy8.h"
#include "card_list.h"
#include "card_type.h"

/* external global variables */


extern card_type top_card;
extern stack< undo_record > undo_stack;

extern deck hand;  // crazy8.h
extern player* player_list;  // player.h
extern card_list bank;  // deck.h 


/* ncolor.cpp */
void cprintf( const char* text, ... );

/* local functions */
int cmd_lookup( string cmd );
void setup_cmd_hashing();
bool cmd_handler( int inum, string cmd );
bool interpret( string command );

/* commands */
bool cmd_undo( string argument );
bool cmd_help( string argument );
bool cmd_hint( string argument );
bool cmd_pass( string argument );
bool cmd_play( string argument );
bool cmd_draw( string argument );
bool cmd_cardstat( string argument );
bool cmd_cardlist( string argument );
bool cmd_addcard( string argument );
bool cmd_dump_bank( string argument );

/* global variables */
#define NUM_INT_CMD 10

struct cmd_type
{
  bool (*function) ( string cmd );
  
  int id;
  string name;
};

const struct cmd_type cmd_list[NUM_INT_CMD] = {
             { cmd_undo, 0, "undo" },
             { cmd_help, 1, "help" },
             { cmd_hint, 2, "hint" },
             { cmd_pass, 3, "pass" },
             { cmd_play, 4, "play" },
             { cmd_draw, 5, "draw" },
             { cmd_cardstat, 6, "cardstat" },
             { cmd_cardlist, 7, "cardlist" },
             { cmd_addcard, 8, "addcard" },
             { cmd_dump_bank, 9, "dump_bank" }
             
             
             
             
};  

HashTable<string,int> i_cmd_table(-1,0);



/*

*/
bool interpret( string command )
{
  bool end_turn = false;
  
  int inum = -1;
  
  inum = cmd_lookup( command );
  
  if( inum != -1 )
  {
    end_turn = cmd_handler( inum, command );
  }
  else
  {
    cprintf( "\n&R!!! &BCommand &W%s &Bnot found &R!!!", command.c_str() );
  }
  return end_turn;
}


/*
  Determines the number of the internal command 
    if it is not an internal command -1 is returned
*/
int cmd_lookup( string cmd )
{
  int pos = -1;
  string temp = cmd;
  string x = get_first_arg( temp );
  pos = i_cmd_table[x];
  
  return pos;
}



/*
  builds the hash table for number lookups on internal commands
*/
void setup_cmd_hashing()
{
  for( int i = 0; i < NUM_INT_CMD; ++i )
  {
    i_cmd_table.insert( cmd_list[i].name, i );
  }
}



/*
 calls the appropriate function for the local command
*/
bool cmd_handler( int inum, string cmd )
{
  return cmd_list[inum].function( cmd );
}



/*

*/
bool cmd_undo( string argument )
{
  undo_record change;
  
  if( undo_stack.size() <= 0 )
    {
      cprintf( "\n&RThere are no changes that can be undone&W." );
      return false;
    }
    change = undo_stack.pop();

    switch( change.method )
    {
      case 0:
           // add the card back to the player's hand
           PC.insert_card( change.data );
          break;
      case 1:
           // remove the card from the player's hand and add it back to the deck
           PC.remove_card( change.data );
           hand.add_card_back( change.data );
          break;
      default:
           // error
           cprintf( "\n&RERROR&r!!! &WIn reinstating change from undo_stack&p.&W" );
          break;
    }
    return false;
}


/*

*/
bool cmd_help( string argument )
{
  // verry simple function
   cprintf( 
     "\n&B----------------------------------------------------------------------"
     "\n&Cdebug &g- &wusefull tools that show information for the developer"
     "\n      &Ccardlist &g- "
     "\n      &Ccardstat &g- "
     "\n      &Caddcard  &g- "
	 "\n      &Cdump_bank&g- &wshows the output of the loaded cards"
     "\n&Cundo  &g- "
     "\n&Cplay  &g- "
     "\n&Cdraw  &g- "
     "\n&Cpass  &g- "
     "\n&Chint  &g- "
     "\n&B----------------------------------------------------------------------"
     );
   return false;
}


/* 
  tells all the possible cards that a player 
   can play based on the top card
*/
bool cmd_hint( string argument )
{
  card_type playable = PC.find_move( top_card );

  if( playable.face == -1 )
  {
    cprintf( "\n&RYou cannot play a card&W." );
  }
  else
  {
    cprintf( "\n&RYou can play the &W%s&R.&W", playable.color_abv_name( playable.abv_name ).c_str() );
  }
  return false;
}

/*

*/
bool cmd_pass( string argument )
{
   // before allowing the player to pass we need to check two things

   // the first thing we should check is: is the player able to play a card
   if( PC.can_play(top_card) == true )
   {
     cprintf( "\n&YThere is a card you can play." );
	 // now run the hint command and show the player the card they can play
	 cmd_hint( argument );
   }
   else
   if( hand.is_empty() == false )
   {
     cprintf( "\n&YThere are &W%d &Ycards left in the deck draw one.", hand.get_length() );
	 // now run the draw command and get a card from the deck
	 cmd_draw( argument );
   }
   else
   {
    cprintf( "\n&RPlayer will pass this round.&W" );
    PC.set_pass( true );
    return true;
   }

   return false;
}


/*

*/
bool cmd_play( string argument )
{
  string cmd_name = get_first_arg( argument );
  string arg2;
  card_type played;
  undo_record change;
  
    PC.set_pass( false );
    if( argument.size() != 0 )
    arg2 = get_first_arg( argument );

    // arg2 should hold the card the user wants to play
    played = PC.play_card( cap_args( arg2 ) );

     if( played.face == -1 )
     {
       cprintf( "\n&RThat is not a valid card.&W");
       return false;
     }

     // put the change we just made on to the undo_stack
     change.data = played;
     change.method = 0;
     undo_stack.push( change );

    if( played.value == 50 )
    {
      // now we must pick a suite for the card that was just played
      cprintf( "\n&gNum&RHeart  &p= &B%d"
               "\n&gNum&RDiamond&p= &B%d"
               "\n&gNum&WSpade  &p= &B%d"
               "\n&gNum&WClub   &p= &B%d"
               "\n&WPlease Pick a Suite for the 8 card you played"
               "\n&W[&R1&W] &wHearts "
               "\n&W[&R2&W] &wDiamonds "
               "\n&W[&R3&W] &wSpades "
               "\n&W[&R4&W] &wClubs "
               "\n&B: &W",
               PC.get_num_card_suite(1), PC.get_num_card_suite(2), PC.get_num_card_suite(3), PC.get_num_card_suite(4) 
             );
      int suit = 0;
      cin >>  suit;
      while( suit < 1 || suit > 4 )
      {
       cprintf( "\n&RThat is not a valid suite please choose again: " );
       cin>>  suit;
      }
      
      switch( suit )
      {
        case 1: top_card = bank[28]; break;
        case 2: top_card = bank[29]; break;
        case 3: top_card = bank[30]; break;
        case 4: top_card = bank[31]; break;
      }

      cprintf( "&W\nPlayer plays &R%s&W.", top_card.abv_name.c_str() );
    

      return true;
    }
    else
    if( played.abv_name.at(0) != top_card.abv_name.at(0) )
    {
      // the card can be any suite if its only difference is the suite
      // since I am doing a bad job of explaining that here is an example
       // if top card is    9S   a 9H can be played even though S!=H
      if( played.suit != top_card.suit )
      {
       cprintf( "\n&RSuits do not match please play another card\n" );
       PC.insert_card( played ); // put the card we tried to play back in our deck
       return false;
      }
    }
    cprintf( "&W\n%s plays &R%s&W.", (PC.get_playername()).c_str(), played.abv_name.c_str() );
    top_card = played;
    
    return true;
}

/*

*/
bool cmd_draw( string argument )
{
  card_type temp;
  undo_record change;
  
    if( hand.is_empty() == false )
    {
     temp = hand.draw();
     PC.insert_card( temp );
     /* put the change onto the undo_stack
        we took a card from the deck so when we undo we will need to add the card back to the
        deck and remove it from the player's hand  */
     change.data = temp;
     change.method = 1;
     undo_stack.push( change );
     
    } 
    
    return false;
}



bool cmd_addcard( string argument )
{
  card_type temp;
  string abreviated_name;
  string cmd_name;
  
  
  cmd_name = get_first_arg( argument );
  abreviated_name = get_first_arg( argument );
  
  temp = bank.get_card_with_abv( abreviated_name );

  if( temp.face != -1 )
  {
    PC.insert_card( temp );
  }
  else
  {
   cprintf( "\n&R%s &Gwas not found in the master deck", argument.c_str() );
  }
  
  return false;
}

bool cmd_cardlist( string argument )
{
  cprintf( "\n&RDebug Card List" 
           "\n&B-----------------------------------------------------------&W" );
   // run through the player list and print out the list of cards they currently have in their hand
  for( int i=0; i < NUM_PLAYERS; ++i )
  {
    cprintf( "\n&W%s&R: &W%s", player_list[i].get_playername().c_str(), player_list[i].get_hand_text().c_str() );
  }

  cprintf( "\n&W%s: %s"
           "\n&B-----------------------------------------------------------&W"
            ,"Deck", hand.get_hand_text().c_str()
        );
  return false;
}


bool cmd_cardstat( string argument )
{
   int suite_stat[5];

   // run through player list printing out statistics on the # of each face of card suite in the hand
   for( int i=0; i < NUM_PLAYERS; ++i )
   {
     player_list[i].get_suite_stat( suite_stat );
     cprintf( "\n&W%s:   &RHeart&B(&W%d&B)  &RDiamond&B(&W%d&B)  &wSpade&B(&W%d&B)  &wClub&B(&W%d&B)  &GEight&B(&W%d&B)",
              player_list[i].get_playername().c_str(), suite_stat[1],suite_stat[2],suite_stat[3] , suite_stat[4], suite_stat[0] );
   }
   return false;
}


bool cmd_dump_bank( string argument )
{
  cout << bank;
  return false;
}








