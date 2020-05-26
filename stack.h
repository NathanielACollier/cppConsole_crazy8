/*
Stack Implemented Using Double Linked List
Nathaniel Collier
3/30/2006 12:54:09 PM
*/

#ifndef STACK
#define STACK

// container for data in stack
template< class object_type >
struct stack_object
{
 stack_object* prev;
 stack_object* next;
 object_type data;
};


template< class object_type >
class stack
{
 private:
  stack_object<object_type>* head;
  stack_object<object_type>* current;
  stack_object<object_type>* prev;
  int stack_size;
 public:
  stack();
  ~stack();
  void push( object_type & temp );
  object_type & pop();
  object_type & top();
  int size();
  void clear();
};

template<class object_type>
stack<object_type>::stack()
{
 head = 0;
 current = 0;
 prev = 0;
 stack_size = 0;
}

template<class object_type>
stack<object_type>::~stack()
{
  clear();
  delete head;
  delete current;
  delete prev;
}

template<class object_type >
void stack<object_type>::push( object_type & temp )
{
  current = new stack_object<object_type>;
  if( head == 0 || stack_size <= 0) // if for some reason the stack is empty but head isn't 0 we need a new stack
  {
   head = current;
   current->prev = 0; // no prev node to head
  }
  else
  {
   current->prev = prev;
   prev->next = current;
  }
  
  current->data = temp;
  prev = current;
  ++stack_size;

}

template<class object_type >
object_type & stack<object_type>::pop()
{
 static object_type temp;
 if( head == 0 || current == 0 || prev == 0 ) throw "Error end of stack";

   current = prev;
   prev = current->prev;
   temp = current->data;
   delete current;
  --stack_size;
 return temp;
}

template<class object_type >
object_type & stack<object_type>::top()
{
  // check for empty
 if( stack_size <= 0 ) throw "Error:  empty stack";
 static  object_type temp;
  temp = prev->data;
  return temp;
}

template< class object_type >
int stack<object_type>::size()
{
 return stack_size;
}

template< class object_type >
void stack<object_type>::clear()
{
  while( size() > 0 )
  {
     pop();
  }
}


#endif










