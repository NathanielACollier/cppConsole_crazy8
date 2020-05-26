/*
  Nathaniel Collier
  2/10/2007
*/



struct undo_record
{
  card_type  data;
  int   method; // 0 add card back to hand   
                // 1 remove card from hand and add back to deck
};


#define NUM_PLAYERS  4
/*
Need to move away from the 4 define statements below
A possibility could include passing an individual player
to a function instead of this direct refference
*/
#define PC player_list[0]
