/******Hashtable.h******/

#ifndef _Hashtable_H_
#define _Hashtable_H_

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

// pair ints default to 0
typedef pair<int, std::string> tableEntry;

class Hashtable
{
    /******Class Data******/
    private:
        //table counts and keys
        tableEntry* table;
        
        // how big the table is
        int size;
        
        // how many entries are in the table
        int entries = 0;

        /******Private Functions******/
        unsigned long HashFunction( const string& );   
        int InsertHash( const string& );               
        int FindHash( const string& );                 
        void Rehash();                                 
        bool IsPrime( int );                           
    public:
        /******Constructor******/
        Hashtable( int n = 1033);

        /******Copy Constructor******/
        Hashtable( const Hashtable& );

        /******Destructor******/
        ~Hashtable();

        /******Getters******/
        int GetCount( int );
        int GetCount( const string& );
        string GetKey( int );
        int GetIndex( const string& );
        int GetSize();
        int GetEntryCount();
        float GetLoadFactor();

        /******Mutators******/
        void Insert( const string& );
        void Remove( int );
        void Remove( const string& );
        void Decrease( int );
        void Decrease( const string& );

};
#endif
