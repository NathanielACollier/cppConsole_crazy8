/*
Nathaniel Collier
8/3/2006
Player High Score Class
Written for the crazy8 game
*/
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

/* ncolor.cpp */
void wprintf(string html_file, const char* text, ... );


#ifndef HIGH_SCORE
#define HIGH_SCORE





#define HSE  high_score_entry
#define HSL  high_score_list

struct high_score_entry
{
  string name;
  string time;
  int games_won; // this is actualy also the number of games wich it took to earn the score
  int games_played;
  int total_score;
  string date; // not sure yet how the date will be stored
  high_score_entry* next; // setup for double liked list
  high_score_entry* prev;
};





class high_score_list
{
  private:
    high_score_entry* head;
    high_score_entry* end;
    high_score_entry* current;

  public:
     high_score_list();
     ~high_score_list();
     void write_html(string filename);
     void save( string filename );
     void load_from_file( string filename );
     void add_high_score( high_score_entry new_entry );
     void sort_by_date();
     void sort_by_high_score();
     string get_listing();
};


// Private Functions






// Public Functions
HSL::high_score_list()
{
  current = 0;
  head = 0;
  end = 0;

}


HSL::~high_score_list()
{
  for( HSE* cur = head; cur != 0; cur = cur->next )
  {
	 if( cur->prev != 0 ) delete cur->prev;
     if( cur->next == 0 ) delete cur; // this is for the last link in the chain
  }

}

void HSL::sort_by_date()
{



}


void HSL::sort_by_high_score()
{


}


void HSL::write_html(string filename)
{
  
  wprintf( filename.c_str(), get_listing().c_str() );

}


/*
Save
 write_ascii is an alternative name
 Saves the high score list to a file
*/
void HSL::save( string filename )
{
  ofstream fout;

  fout.open( filename.c_str() );

  if( !fout )
  {
     throw "Unable to open file for writting severe Error 0";
  }
  
  for( HSE* cur = head; cur != 0; cur = cur->next )
  {
    fout << cur->name << "\n"
         << cur->date << "\n"
         << cur->games_won << "\n"
         << cur->games_played << "\n"
         << cur->total_score << "\n"
         << cur->time << "\n";
  }
  
  fout.close();
}


void HSL::load_from_file( string filename )
{
  current = 0;
  head = 0;
  end = 0;
  
  HSE  cur;

  // figure out how to read it in from file

  ifstream fin;
  fin.open( filename.c_str() );

  if( !fin )
  {
    // this just means we don't have a high score list yet so lets just return without doing anything
    return;
  }

  while( fin >> cur.name  )
  {
     fin>> cur.date;
     fin>> cur.games_won;
     fin>> cur.games_played;
     fin>> cur.total_score;
     fin>> cur.time;

     if( (cur.date).size() > 0 )
     add_high_score( cur );
     
  }
  

  fin.close();
}


void HSL::add_high_score( high_score_entry new_entry )
{
  

  current = new HSE;


  if( head == 0 )
  {
    head = current;
    current->prev = 0;
    end = current;
  }
  else
  {
    end->next = current;
    current->prev = end;
  }


  current->next = 0;

  current->name = new_entry.name;
  current->games_won = new_entry.games_won;
  current->games_played = new_entry.games_played;
  current->total_score = new_entry.total_score;
  current->date = new_entry.date;
  current->time = new_entry.time;
  

  end = current;
}




string HSL::get_listing()
{
  ostringstream buffer;

  if( head == 0 )
  {
    return "\nNo High Score Entries\n";
  }
  buffer << "\n&RHigh Scores&R.                          &g-------&WGames&g------" 
         << "\n&gDate        &wName       &RTotalScore&p:    &YWon   &BPlayed   &GLost  &PPlayTime\n";
  for( HSE* cur = head; cur != 0; cur = cur->next )
  {
     buffer << "&W" <<  left << setw(10) << cur->date << "  &p" << setw( 15 ) << cur->name << " &R" << setw(6) << cur->total_score <<  "\t&w " << cur->games_won << "\t" << cur->games_played << "\t" << ( cur->games_played - cur->games_won )
            << "\t&g" << cur->time << "\n";
  }
  return buffer.str();
}







#endif






