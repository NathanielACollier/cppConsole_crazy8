/*
  Nathaniel Collier
  3/25/2007
  Card Datatype
*/


#ifndef CARD_TYPE_H
#define CARD_TYPE_H

class card_type
{
  private:

  public:
	 int suit;
	 int face;
	 int value;
	 string name;
	 string abv_name;
	 int index;

	 card_type();
	 ~card_type();
	 string color_abv_name( string abv_name );
     char conv_suite_char_to_symbol( char x );
	 card_type & operator=( const card_type  & x );
	 char face_to_char( int face_i );
	 string face_to_string( int face_i );
	 string suit_to_string( int suit );
	 char suit_to_char( int suit_i );
	 card_type create_card( int suit, int face );
	 friend ostream& operator << (ostream& , card_type& );

};


 card_type::card_type()
 {

 }

 card_type::~card_type()
 {

 }


  /*
    char_change
	receives a char and changes it to
	a symbol using a code
	Only used in the console based version of this program
	This function is mostly hear for historical reasons
   */
 char card_type::conv_suite_char_to_symbol( char x )
 { 
   char temp;
   switch( x ) 
   {
     case 'H': temp = (char)259; break;
     case 'D': temp = (char)260; break;
     case 'C': temp = (char)261; break;
     case 'S': temp = (char)262; break;
     case 'O': temp = (char)257; break;
     case 'T': temp = (char)258; break;
     default: temp = x; break;
   }
   return temp;
 }


 /*
  Colorizes the abint determine_suite_with_highest_count();reviated name of the cards
 */
 string card_type::color_abv_name( string abv_name )
 {
   string new_abv;
   char type = abv_name.at(0); // 3
   char suite = abv_name.at(1); // S
   // from these pieces of information we can now colorize our abreviation
   
   // if we have unicode we can also do this
   char symbol_suite = conv_suite_char_to_symbol( suite );

   switch( type )
   {
     case 'A': new_abv += "&r"; break;
     case 'K': 
     case 'Q':
     case 'J': new_abv += "&C"; break;
     case '8': new_abv += "&G"; break;
     default: new_abv += "&c"; break;
   }
   new_abv += type;
   
   switch( suite )
   {
     case 'H': 
     case 'D': new_abv += "&R"; break; // red
     case 'S':
     case 'C': new_abv += "&w"; break; // black
     default: break; // don't want any coloring right now on special cards
   } 
   new_abv += suite;
     

   return new_abv;
 }


 card_type & card_type::operator=( const card_type  & x )
 {
   if( this != &x )
   {
     face = x.face;
	 suit = x.suit;
     name = x.name;
     abv_name = x.abv_name;
     index = x.index;
     value = x.value;
   }
   return *this;
 }


    /* 
       converts an integer to the face character
     */
	char card_type::face_to_char( int face_i )
    {
      char face_ch = 'X';
      
       switch( face_i )
       {
           case 1: face_ch = 'J'; break;
           case 2: face_ch = '2'; break;
           case 3: face_ch = '3'; break;
           case 4: face_ch = '4'; break;
           case 5: face_ch = '5'; break;
           case 6: face_ch = '6'; break;
           case 7: face_ch = '7'; break;
           case 8: face_ch = '8'; break;
           case 9: face_ch = '9'; break;
           case 10: face_ch = 'T'; break;
           case 11: face_ch = 'J'; break;
           case 12: face_ch = 'Q'; break;
           case 13: face_ch = 'K'; break;
           case 14: face_ch = 'A'; break;
       }
       
       return face_ch;
    }

    /*
     * Takes a face 1-4 and converts it to a string
     */
    string card_type::face_to_string( int face_i )
    {
      string face_str = "X";
      switch( face_i )
       {
           case 1: face_str = "Joker"; break;
           case 2: face_str = "Two"; break;
           case 3: face_str = "Three"; break;
           case 4: face_str = "Four"; break;
           case 5: face_str = "Five"; break;
           case 6: face_str = "Six"; break;
           case 7: face_str = "Seven"; break;
           case 8: face_str = "Eight"; break;
           case 9: face_str = "Nine"; break;
           case 10: face_str = "Ten"; break;
           case 11: face_str = "Jack"; break;
           case 12: face_str = "Queen"; break;
           case 13: face_str = "King"; break;
           case 14: face_str = "Ace"; break;
       }
      return face_str;
    }

    /*
       Takes a suite 1-4 and turns it into a string
     */
    string card_type::suit_to_string( int suit )
    {
       string text = "X";
       
       switch( suit )
       {
           case 1: text = "Heart"; break;
           case 2: text = "Diamond"; break;
           case 3: text = "Spade"; break;
           case 4: text = "Club"; break;
       }
      return text;
    }

    /*
     *  Takes a suite 1-4 and turns it into a character
     */
    char card_type::suit_to_char( int suit_i )
    {
      char suit = 'X';
      
      switch( suit_i )
      {
          case 1: suit = 'H'; break;
          case 2: suit = 'D'; break;
          case 3: suit = 'S'; break;
          case 4: suit = 'C'; break;
      }
      return suit;
    }

    /*
     *  Generates a card based on the type which is the 'face' of the card
     *       2 - 10 Jack Queen King Ace Joker etc...
     *  The suit which can be a 
     *       1 - Heart
     *       2 - Diamond
     *       3 - Spade
     *       4 - Club
     */
    card_type card_type::create_card( int suit, int face )
    {
       card_type card;
       string suit_str = suit_to_string( suit );
       string face_str = face_to_string( face);
       string abv_name =  "" + face_to_char( face ) + suit_to_char( suit ) ;
       string name =  face_to_string( face ) + " of " + suit_to_string( suit ) ;      
       
       card.suit = suit;
       card.face = face;
       
       if( face == 8 )
       {
          card.value = 50;
       }
       else
       {
           card.value = face;
       }
       
       card.abv_name = abv_name;
       
       
       
       card.name = name;
       
       card.index = 0;
       
       cout << card; 
       
       return card;
    }


 /*
  *
  */
 ostream& operator<<(ostream& scr,  card_type& x)
 {
    scr << "\n------------------------------------"
		<< "\nSuit= " << x.suit_to_string( x.suit )
		<< "\nFace= " << x.face_to_string( x.face )
		<< "\nAbvName= " << x.abv_name 
		<< "\nName= " << x.name 
		<< "\nIndex= " << x.index
		<< "\nFace#= " << x.face
		<< "\nSuit#= " << x.suit
	    << "\n------------------------------------";
  
 	return scr;
 }


#endif
