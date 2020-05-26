/*
7/11/2006
Crazy 8 Card Game
Nathaniel Collier
*/


// make sure this string is allways as long as the asteriks
                 //********
#define C8_VERSION "  0.50"

#include <iostream>
using namespace std;


#define INPUT_DIR       "config/"
#define OUTPUT_DIR      "output/"
#define HIGH_SCORE_FILE  OUTPUT_DIR "high_score.txt"
#define CARD_LIST        INPUT_DIR  "cards.txt"

#define HTML_HIGH_SCORE  OUTPUT_DIR "high_score.html"

#include "options.h"
#include "player.h"
#include "card_list.h"
#include "high_score.h"
#include "stack.h"
#include "hash_table.h"
#include "deck.h"

#include "card_type.h"

/* ncolor.cpp */
void cprintf( const char* text, ... );

/* nstring.cpp */
string get_line_kyb();
char* string_date();
string format_time( double seconds );
string cap_args( string text );
string get_first_arg( string& text );

/* nfunc.cpp */
void open_in_file( ifstream & fin,string filename );
void open_out_file( ofstream & fout,string filename );

/* cmd.cpp */
bool interpret( string command );
void setup_cmd_hashing();

/* local functions */
void new_game();
void title_screen();
int main_menu();
void player_display();
bool interpret( string command );
void computer_turn( player& name );
void finish_game( player& winner );
void display_score();
void quit_game_early();
void record_score();
void no_winner();
bool check_no_winner();
void setup_cpu_names();
void game_loop();
string get_user_input();
void setup_names();



#include "crazy8.h"

deck hand;  // crazy8.h
player* player_list;  // player.h
HSL high_score_table; // high_score.h


card_list bank;  // another dangerous global variable


string playtime;

stack< undo_record > undo_stack;

double start_time = 0;

card_type top_card;


/* ncolor.cpp */
extern bool color_on;

int main()
{
  int choice = 0;

  	 bank.load_card_list( CARD_LIST );


  color_on = true; // turn off color since the eclipse console has problems with color

  // setup all the games
  srand( (unsigned int) time(0) ); // seed the random number generator
  setup_cmd_hashing();

  start_time = time(0);
  player_list = new player [NUM_PLAYERS];
  title_screen();
  setup_names();
  high_score_table.load_from_file( HIGH_SCORE_FILE );


  // program loop
  while( true )
  {
    choice = main_menu();

    if( choice == 2 )
    {
      display_score();

    }
    else
    if( choice == 3 )
    {
       cprintf((high_score_table.get_listing()).c_str());
    }
    else
    if( choice == 1 )
    {
       game_loop();
    }
    else
     break;
  }


  cprintf( "\nPlay Time: \n%s\n", format_time( time(0) - start_time ).c_str() );
  record_score();
  high_score_table.write_html( HTML_HIGH_SCORE );
  high_score_table.save(HIGH_SCORE_FILE);

 delete [] player_list;

 cout << "\n";
 return 0;
}




/*
Moved out of main to make things easier to manage

*/
void game_loop()
{
  bool game_over = true;
  string user_input;
  bool end_turn = false;


      new_game();
      game_over = false;
      // when the game is over break out of this loop
      player_display();

      // inner loop that is used to play a single game
      while( true )
      {
        user_input = get_line_kyb();

         if( user_input == "quit" || user_input == "q" || user_input == "exit" )
         {
           game_over = true;
           quit_game_early();
         }

         end_turn = interpret( user_input );

          // this loop is for commands that do not end the players turn
         while( game_over == false && end_turn != true ) // continue player turn
         {
           player_display();

            user_input = get_line_kyb();

             // we need to reuse these if statements so that we can quit here
              // or display help if we want to
              if( user_input == "quit" || user_input == "q" )
              {
                game_over = true;
                quit_game_early();
                break;
              }

           end_turn = interpret( user_input );
         }



        if( check_no_winner() == true && game_over == false )
        {
           cprintf( "\n&RNO WINNER !!!!" );
           no_winner();
           game_over = true;
        }

        // this loop is where game play for the computer takes place
        if( game_over == false )
        for( int i=0; i < NUM_PLAYERS; ++i )
        {
          if( i > 0 ) computer_turn( player_list[i] );
          if( player_list[i].hand_length() <= 0 )
          {
            // victor message
            cprintf( "\n&B*** &W%s &R!!!  &YWINS &B *** ", player_list[i].get_playername().c_str() );
            // finish the game with the object that wins
            finish_game( player_list[i] );
            game_over = true;
            break; // new game
          }
        }

         // reduced breaking out of game loop to 1 place
        if( game_over == true )
        {
          break;
        }

        player_display();
      }

}

void setup_cpu_names()
{
 char buf[7];
  for( int i=1; i < NUM_PLAYERS; ++i )
  {
    sprintf( buf, "CPU %d", i );
    player_list[i].set_playername( buf );
  }
}



void setup_names()
{
  string player_name;

  // prompt user to input their name
  cprintf( "\nPlease enter name&W(&YNo space&W)&p: " );
  cin>> player_name;

  if( player_name == "quit" )
  {
	  exit(0);
	}

  // we can get rid of this by reading it all in from a settings file
  PC.set_playername( player_name );
  setup_cpu_names();

}


bool check_no_winner()
{
  for( int i=0; i < NUM_PLAYERS; ++i )
  {
    if( player_list[i].get_pass() == false )
    {
      // we reached a player that had not passed
      return false;
    }
  }
 return true;
}





void record_score()
{
  HSE temp;
  string newline;

  temp.name = PC.get_playername();
  temp.date = string_date();
  temp.games_won = PC.get_games_won();
  temp.games_played = PC.get_games_played();
  temp.total_score = PC.get_total_score();
  temp.time = playtime;

  high_score_table.add_high_score( temp );

}


void display_score()
{
  cprintf( "\n&RPlayer Name    Total Score    Games Won    Games Lost  " );

  for( int i=0; i < NUM_PLAYERS; ++i )
  cprintf( "\n&G%s\t\t&R%d\t\t&W%d\t\t&B%d", player_list[i].get_playername().c_str(), player_list[i].get_total_score(),
                                             player_list[i].get_games_won(), player_list[i].get_games_lost() );

  cprintf( "\n&RPlay Time&W:  &p%s   &RGames Played&W:  &p%d&W\n", format_time( time(0) - start_time ).c_str(), PC.get_games_played() );
}

/*
END_GAME Macro
Is used 3 times to call the 4 functions listed in the macro definition
*/
#define END_GAME( winner, points )                 \
    for( int i=0; i < NUM_PLAYERS; ++i )           \
    {                                              \
      player_list[i].end_game( winner, points );   \
    }


void quit_game_early()
{
  cprintf( "\n&REnding Game Early" );
  END_GAME( "NONE", 0 )
}



void no_winner()
{
  int points = 0;

  cprintf( "\n&RFinal Point Count" );
  for( int i=0; i < NUM_PLAYERS; ++i )
  {
    points += player_list[i].get_points();
    cprintf( "\n&W%s  &p%d", player_list[i].get_playername().c_str(), player_list[i].get_points() );
  }
    cprintf( "\n%s &Ris awarded &W%d &Rtotal points.",  "&RN&rO &RO&rN&RE&Y!!!", points  );
  END_GAME( "NONE", 0 )
}


void finish_game( player& winner )
{
  int points = 0;

  cprintf( "\n&RFinal Point Count" );
  for( int i=0; i < NUM_PLAYERS; ++i )
  {
    points += player_list[i].get_points();
    cprintf( "\n&W%s  &p%d", player_list[i].get_playername().c_str(), player_list[i].get_points() );
  }
    cprintf( "\n%s &Ris awarded &W%d &Rtotal points.",  winner.get_playername().c_str() , points  );
  END_GAME( winner.get_playername() , points )
}





int main_menu()
{
  int choice = 0;
  cprintf( "\n&W(&R0&W) &wQuit"
           "\n&W(&R1&W) &wNew Game"
           "\n&W(&R2&W) &wScore"
           "\n&W(&R3&W) &wHigh Scores"
           "\n"
         );
  cin >> skipws >> choice;
  return choice;
}



void title_screen()
{
  cprintf( "&R+&w--------------------------------------------&R+\n"
           "&w|                &BC&Yr&pa&Wz&Py &G8&g'&Os                   &w|\n"
           "&w|                    &Wversion: &R%s           &w|\n"
           "&R+&w--------------------------------------------&R+\n",
		 C8_VERSION );

}



void new_game()
{

  // shuffle the deck
  hand.shuffle();
  // need to call a new game function for each player
  for( int j=0; j < NUM_PLAYERS; ++j ) player_list[j].new_game();

  undo_stack.clear();

  // now we need to deal each player 8 cards
  for( int i=0; i < 8; ++i )
  {
    for( int j=0; j < NUM_PLAYERS; ++j ) player_list[j].insert_card( hand.draw() );
  }

  top_card = hand.draw();
}


/*
 The Display for the player

*/
void player_display()
{
  cprintf(
       "\n&WTop Card&R: &G%s"
       "\n&WHand&R: &W%s",
          top_card.abv_name.c_str(), PC.get_hand_text().c_str()
         );

   // lists the number of cards in all the other players hands
  for( int i=0; i < NUM_PLAYERS; ++i )
      {
        if( PC.get_playername() != player_list[i].get_playername() )
        {
          cprintf( "\n&W%s&R: &w%d &wcards.", player_list[i].get_playername().c_str(), player_list[i].hand_length() );
        }
      }

  cprintf( "\n&wEnter CMD or type help: " );


}






void computer_turn( player& cpu )
{
  card_type played;
  int draw_count = 0;
  card_type drawn;
  card_type temp;

  played = cpu.AI_can_play( top_card ); // lets find out if a card can be played

  while( played.face == -1 )
  {
    // we are going to draw unless we cannot draw then we are just going to return
    if( hand.is_empty() == true )
    {
      if( draw_count > 0 )
      cout << "\n" << cpu.get_playername() << " drew " << draw_count << " cards.";
      cout << "\n" << cpu.get_playername() << " will pass this round.";
      cpu.set_pass( true );
      return;
    }
    // draw card and see if we can play it
    drawn = hand.draw();
    cpu.insert_card( drawn );
    ++draw_count;
    played = cpu.AI_can_play( top_card );
  }

     cpu.set_pass( false );
    if( draw_count > 0 )
    cout << "\n" << cpu.get_playername() << " drew " << draw_count << " cards.";
    cprintf( "&W\n%s plays &R%s&W.", cpu.get_playername().c_str(), played.abv_name.c_str() );
    top_card = played;
  return;
}




















