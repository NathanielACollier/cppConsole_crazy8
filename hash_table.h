#ifndef QUADRATIC_PROBING_H_
#define QUADRATIC_PROBING_H_
/*
  Data Structures and Algorithm Analysis in c++
    Mark Allen Weiss
  Additions and Changes by Nathaniel Collier
  -------------------------------------------
  2/3/2006  Made isPrime and nextPrime private member functions to try and resolve some bad linking problems
  9/22/2006  Changed the form of the hashtable from  HashTable<HashedObj> to
             HashTable<Key_Type,HashedObj> 
  9/22/2006  Changed name of file from "QuadraticProbing.h" to "hash_table.h"
  9/22/2006  overloaded [] for returning a find on a key
             *at a furture date perhaps change [] to return a reference so that we no longer need the insert
              method and can insert and extract on the [] operator alone.
*/
      
      
      /*
        special note:
        this file needs a linker flag of --allow-multiple-definition
        because of the hash function not being able to be a member of this class
        because of the way Mr. Weiss wrote this class and I can't figure out a better way so
        for now just pass that command to the linker */
       

        #include <vector>
        #include <string>

        using namespace std;
        
        


        
#ifndef HASH_FUNCTIONS_NAMESPACE
#define HASH_FUNCTIONS_NAMESPACE

namespace hash_functions
{
/**
* Data Structures and Algorithm Analysis in c++
    Mark Allen Weiss
* A hash routine for string objects.
*/
int hash( const string & key, int tableSize )
{
   int hashVal = 0;

   for( int i = 0; i < key.length( ); i++ )
   {
     hashVal = 37 * hashVal + key[ i ];
   }
   
   hashVal %= tableSize;
   
   if( hashVal < 0 )
   {
      hashVal += tableSize;
   }
   
   return hashVal;
}


/**
 * Data Structures and Algorithm Analysis in c++
    Mark Allen Weiss
  * A hash routine for ints.
*/
int hash( int key, int tableSize )
{
  if( key < 0 ) key = -key;
  return key % tableSize;
}

};       
        
#endif  
        

 
        template <class Key_Type,class HashedObj>
        class HashTable
        {
          public:
            explicit HashTable( const HashedObj & notFound, int size = 101 );
            HashTable( const HashTable & rhs )
              : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                array( rhs.array ), currentSize( rhs.currentSize ) { }

            const HashedObj & find( const Key_Type & key ) const;

            void makeEmpty( );
            void insert( const Key_Type & key, const HashedObj & x );
            void remove( const Key_Type & key );

            const HashedObj & operator[]( const Key_Type & key ) const;
            
            const HashTable & operator=( const HashTable & rhs );

            enum EntryType { ACTIVE, EMPTY, DELETED };
          private:
            struct HashEntry
            {
                HashedObj element;
                EntryType info;
                Key_Type key; // 9/22/2006 - Nathaniel Collier
                int pos; // 9/23/2006 - Nathaniel Collier

                HashEntry( int position = 0, const Key_Type & k = Key_Type(), const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
                  : pos(position), key(k),element( e ), info( i ) { }
            };
            


            vector<HashEntry> array;
            int currentSize;
            const HashedObj ITEM_NOT_FOUND;

            bool isActive( int currentPos ) const;
            int findPos( const Key_Type & key ) const;
            void rehash( );
            int nextPrime( int n );
            bool isPrime( int n );
        };

        



        

        /**
         * Construct the hash table.
         */
        template <class Key_Type,class HashedObj>
        HashTable<Key_Type,HashedObj>::HashTable( const HashedObj & notFound, int size )
          : ITEM_NOT_FOUND( notFound ), array(  nextPrime( size ) )
        {
            makeEmpty( );
        }

        /**
         * Insert item x into the hash table. If the item is
         * already present, then do nothing.
         */
        template <class Key_Type,class HashedObj>
        void HashTable<Key_Type,HashedObj>::insert( const Key_Type & key,const HashedObj & x )
        {
                // Insert x as active
            int currentPos = findPos( key );
            if( isActive( currentPos ) )
                return;
            array[ currentPos ] = HashEntry( currentPos, key, x, ACTIVE );
                // Rehash; see Section 5.5
            if( ++currentSize > array.size( ) / 2 )
                rehash( );
        }

        /**
         * Expand the hash table.
         */
        template <class Key_Type,class HashedObj>
        void HashTable<Key_Type,HashedObj>::rehash( )
        {
            vector<HashEntry> oldArray = array;

                // Create new double-sized, empty table
            array.resize( nextPrime( 2 * oldArray.size( ) ) );
            for( int j = 0; j < array.size( ); j++ )
                array[ j ].info = EMPTY;

                // Copy table over
            currentSize = 0;
            for( int i = 0; i < oldArray.size( ); i++ )
                if( oldArray[ i ].info == ACTIVE )
                    insert( oldArray[ i ].key, oldArray[ i ].element );
        }

        /**
         * Method that performs quadratic probing resolution.
         * Return the position where the search for x terminates.
         */
        template <class Key_Type,class HashedObj>
        int HashTable<Key_Type,HashedObj>::findPos( const Key_Type & key ) const
        {
/* 1*/      int collisionNum = 0;
/* 2*/      int currentPos =  hash_functions::hash( key, array.size( ) );

/* 3*/      while( array[ currentPos ].info != EMPTY &&
                   array[ currentPos ].key != key )
            {
/* 4*/          currentPos += 2 * ++collisionNum - 1;  // Compute ith probe
/* 5*/          if( currentPos >= array.size( ) )
/* 6*/              currentPos -= array.size( );
            }

/* 7*/      return currentPos;
        }
        
        
        /*
          Nathaniel Collier
           9/22/2006
        */
        template <class Key_Type,class HashedObj>
        const HashedObj & HashTable<Key_Type,HashedObj>::operator[]( const Key_Type & key ) const
        {
          return find( key );
        }


        /**
         * Remove item x from the hash table.
         */
        template <class Key_Type,class HashedObj>
        void HashTable<Key_Type,HashedObj>::remove( const Key_Type & key )
        {
            int currentPos = findPos( key );
            if( isActive( currentPos ) )
                array[ currentPos ].info = DELETED;
        }

        /**
         * Find item x in the hash table.
         * Return the matching item or ITEM_NOT_FOUND if not found
         */
        template <class Key_Type,class HashedObj>
        const HashedObj & HashTable<Key_Type,HashedObj>::find( const Key_Type & key ) const
        {
            int currentPos = findPos( key );
            if( isActive( currentPos ) )
                return array[ currentPos ].element;
            else
                return ITEM_NOT_FOUND;
        }

        /**
         * Make the hash table logically empty.
         */
        template <class Key_Type,class HashedObj>
        void HashTable<Key_Type,HashedObj>::makeEmpty( )
        {
            currentSize = 0;
            for( int i = 0; i < array.size( ); i++ )
                array[ i ].info = EMPTY;
        }

        /**
         * Deep copy.
         */
        template <class Key_Type,class HashedObj>
        const HashTable<Key_Type,HashedObj> & HashTable<Key_Type,HashedObj>::
        operator=( const HashTable<Key_Type,HashedObj> & rhs )
        {
            if( this != &rhs )
            {
                array = rhs.array;
                currentSize = rhs.currentSize;
            }
            return *this;
        }


        /**
         * Return true if currentPos exists and is active.
         */
        template <class Key_Type,class HashedObj>
        bool HashTable<Key_Type,HashedObj>::isActive( int currentPos ) const
        {
            return array[ currentPos ].info == ACTIVE;
        }

                	     /**
         * Internal method to test if a positive number is prime.
         * Not an efficient algorithm.
         */
        template <class Key_Type,class HashedObj>
        bool HashTable<Key_Type,HashedObj>::isPrime( int n )
        {
            if( n == 2 || n == 3 )
                return true;

            if( n == 1 || n % 2 == 0 )
                return false;

            for( int i = 3; i * i <= n; i += 2 )
                if( n % i == 0 )
                    return false;

            return true;
        }

        /**
         * Internal method to return a prime number at least as large as n.
         * Assumes n > 0.
         */
        template <class Key_Type,class HashedObj>
        int HashTable<Key_Type,HashedObj>::nextPrime( int n )
        {
            if( n % 2 == 0 )
                n++;

            for( ; !isPrime( n ); n += 2 )
                ;

            return n;
        }

      

        
   
        
        
        
// hash table
#endif
        
        
        

