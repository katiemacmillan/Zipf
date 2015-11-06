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
    protected:
        //Array of structs
        struct Data
        {
            int count;
            string key;
        };
        Data* table;
        int size;
        int hashFunction( int );

    public:
        /******Constructor******/
        Hashtable( int );

        /******Copy Constructor******/
        Hashtable( const Hashtable& );

        /******Destructor******/
        ~Hashtable();

        /******Getters******/
        int getCount(int);
        string getKey(int);
        int getIndex(const string&);
        int getSize ();
        float getVolume ();

        /******Setters******/
        void setCount(int, int);
        void setKey(const string&, int);

        /******Functions******/
        void Hash (const string& );

};
#endif