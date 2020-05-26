/*
Nathaniel Collier
8/19/2006
*/
#include <string>
#include <fstream>


using namespace std;





#ifndef OPTIONS_H
#define OPTIONS_H

const int default_num_players = 4;

const char * default_names [ default_num_players ] = {
                "pc", "cpu1", "cpu2", "cpu3" };

// all of the information that will be stored in the
// options.txt is stored here
struct option_data_type
{
  int num_players; // number of players
  string * player_names; // player names there will be one for every through num_players

};


class option_type
{
  private:
    option_data_type temp_data;
  public:
    option_type();
    ~option_type();

    void load_options(string filename);
    void save_options(string filename);
    void restore_default_options();
};



////////////// Private Functions //////////////////



/////////////  Public Functions ///////////////////

option_type::option_type()
{

  restore_default_options();

}

option_type::~option_type()
{


}



void option_type::restore_default_options()
{
  
}


void option_type::load_options(string filename)
{
  ifstream op_file;
  string line;
  op_file.open( filename.c_str() );

  if( !op_file )
  {
    // there is a problem
     throw "Error loading the options file in class option_type";
  }
  
  // read file line by line
  while( getline( op_file, line ) )
  {


  }

  
  op_file.close();
}

void option_type::save_options(string filename)
{
  ofstream op_file;
  op_file.open( filename.c_str() );
  
  if( !op_file )
  {
    // problem
    throw "Error saving the options to the options file in class option_type";
  }


  op_file.close();
}






#endif

