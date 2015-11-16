/******Hashtable.h******/

#ifndef _Hashtable_H_
#define _Hashtable_H_

#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

class Hashtable
{
    /******Class Data******/
    private:
        struct Data
        {
            int count;
            string key;
        };
        Data* table;                    //Array of Data structs to hold data
        int size;                       //How big the table is
        int entries = 0;                //How many items are in the table

        /******Private Functions******/
        unsigned long HashFunction ( const string& );   //Base hash function
        int InsertHash ( const string& );               //Hash with probing to insert a key
        int FindHash ( const string& );                 //Hash with probing to find a key
        void Rehash ();                                 //Rehash the table
        bool IsPrime ( int );                           //Find prime table sizes
    public:
        /******Constructor******/
        Hashtable( int = 1033 );

        /******Copy Constructor******/
        Hashtable( const Hashtable& );

        /******Destructor******/
        ~Hashtable();

        /******Getters******/
        int GetCount ( int );
        int GetCount ( const string& );
        string GetKey ( int );
        int GetIndex ( const string& );
        int GetSize ();
        int GetEntryCount ();
        float GetLoadFactor ();

        /******Mutators******/
        void Insert ( const string& );
        void Remove ( int );
        void Remove ( const string& );
        void Decrease ( int );
        void Decrease ( const string& );

};
#endif
