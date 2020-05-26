#include <string>
#include <fstream>

using namespace std;

#include "hash_table.h"
#include "card_type.h"

/* nstring.cpp */
string cap_args( string text );
bool check_comment( string text );
string get_first_arg( string& text );
string get_first_quote( string & text );

/* nfunc.cpp */
void open_in_file( ifstream & fin,string filename );


#ifndef CARD_LIST_H
#define CARD_LIST_H




class card_list
{
  private:
     int card_count;
	 vector<card_type> card_container;
	 HashTable< string, int > card_table;
  public:
     card_list();
	 ~card_list(); 
     void load_card_list( string filename );	 
     card_type get_card_with_abv( string abv_name );
	 card_type & operator[]( int index );
	 int size();
	 friend ostream & operator << ( ostream & out, card_list & card_list_obj ); 
};


/////////////////////////////////////////////////// PRIVATE FUNCTIONS ///////////////////////////////////////////////////////




///////////////////////////////////////////////////  PUBLIC FUNCTIONS ////////////////////////////////////////////////////////


card_list::card_list() : card_table( -1, 101 )
{
  card_count = 0;
}

card_list::~card_list()
{


}


/**
 
 */ 
 void card_list::load_card_list( string filename )
 {
    ifstream fin;
    string line;
    string arg;
	int num_cards = 0;
    HashTable<string,int> dict( -1, 101 );
    card_type temp_card;

	open_in_file( fin, filename );

	//cout << "\nValue\tType\tName\tabv_name\tindex";

    while( getline( fin, line ) )
	{

      if( check_comment( line ) == true )
	  {
        continue;
		   // read the next line
	  }

    // get the first space seperated word so that we can determine what this line does
    arg = get_first_arg( line );
    
     if( arg == "NUM_CARDS" )
	 {
	    // get the next argument in the line
	    arg = get_first_arg( line );
		// convert it to a number
        num_cards = atoi( arg.c_str() );
		continue;
	 }

	 if( arg == "DEF" )
	 {
       // temporary values
	   string def_name,
		      def_value;
	   // first argument should be the definition name
	   def_name = get_first_arg( line );
	   // second argument should be the value of the definition
	   def_value = get_first_arg( line );
	   // insert the string int pair into the hash table
	   dict.insert( def_name, atoi( def_value.c_str() ) );
	   continue;
	 }

	 if( arg == "CARD" )
	 {
       // temporary values
		 string value,
			    face,
				suit,
				name,
				abv_name,
				index;

		 ++card_count;

		 face = get_first_arg( line );
		 value = get_first_arg( line );
		 if( dict[value] == -1 || dict[face] == -1)
		 {
             temp_card.value = atoi( value.c_str() );
			 temp_card.face = atoi( face.c_str() );
         }
		 else
		 {
           temp_card.value = dict[value];
		   temp_card.face = dict[face];
		 }

		 suit = get_first_arg( line );
		 temp_card.suit = atoi( suit.c_str() );

		 name = get_first_quote( line );
		 temp_card.name = name;

		 abv_name = get_first_quote( line );
		 temp_card.abv_name = abv_name;

		 index = line;
		 temp_card.index = atoi( index.c_str() );

		 // create a mapping between abv_name and index
		 card_table.insert( cap_args( temp_card.abv_name ), temp_card.index );

		 card_container.push_back( temp_card );

		 cout << temp_card;
	 }

    }// end of while


	cout << "\nCards Found:          " << card_count
		 << "\nReported Card Count:  " << num_cards;
 }





 /*
  Returns card that matches the abv_name supplied
   The card is copied from the Card list 
   If the card is not found A card with
     -1 as the type is returned
 */
 card_type card_list::get_card_with_abv( string abv_name )
 {
  card_type temp;
  int pos = 0;

  pos = card_table[ cap_args( abv_name ) ];

  if( pos == -1 )
  {
    temp.face = -1; // used if card is not found
  }
  else
  {
    temp = card_container[pos];
  }

  return temp;
 }



 card_type & card_list::operator[]( int index )
 {
   return card_container[index];
 }

 int card_list::size()
 {
   return card_count;
 }


 ostream & operator << ( ostream & out, card_list & card_list_obj )
 {
   for( int i=0; i < card_list_obj.card_count; ++i )
   {
     out << card_list_obj[i];
   }
   return out;

 }


#endif
