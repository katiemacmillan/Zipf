/* shape.h */

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
        //Array of structs
        struct Data
        {
            int count;
            string key;
        };
        Data* table;
        int size;                       //How big the table is
        int entries = 0;                //How many items are in the table
        int HashFunction( int );        //Base hash function
        int HashFunction2( int );       //Double hashing collision resolution
        int Rehash ();                  //Rehash the table
        int ConvertKey (const string&); //Convert string key to int
        int FindHash(const string&);    //Hash with probing to find a key
        int InsertHash(const string&);  //Hash with probing to insert a key
        bool IsPrime(int);              //Find prime table sizes

    public:
        /******Constructor******/
        Hashtable( int = 1033 );

        /******Copy Constructor******/
        Hashtable( const Hashtable& );

        /******Destructor******/
        ~Hashtable();

        /******Getters******/
        int getCount(int);
        int getCount(const string&);
        string getKey(int);
        int getIndex(const string&);
        int getSize ();
        int getItemCount();
        float getLoadFactor ();

        /******Setters******/
        void Remove(int);
        void Remove(const string&);
        void Decrease(int);
        void Decrease(const string&);
        void Insert (const string& );

};
#endif
