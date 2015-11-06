/* Hashtable.cpp */

#include "Hashtable.h"

/**********************************************************************
                            Constructor
**********************************************************************/
Hashtable :: Hashtable( int n )
{
	table = new Data[n];
	size = n;
}

/**********************************************************************
                            Copy Constructor
**********************************************************************/
Hashtable :: Hashtable( const Hashtable& hash )
{
	table = new Data[hash.size];
	for (int i = 0; i < hash.size; i++)
	{
		table[i].count = hash.table[i].count;
		table[i].key = hash.table[i].key;
	}
}

/**********************************************************************
                            Destructor
**********************************************************************/
Hashtable :: ~Hashtable()
{}

/**********************************************************************
                            Getters
**********************************************************************
These get functions retrieve the individual components of the Hashtable
object
**********************************************************************/
int Hashtable :: getCount(int i){ return table[i].count; }
string Hashtable :: getKey(int i){ return table[i].key; }
int Hashtable :: getSize () {return size;}
int Hashtable :: getIndex(const string& k)
{
	int i = 0;
	for (i; i < size; i++)
	{
		if (table[i].key == k)
			return i;
	}
	return -1;
}
float Hashtable :: getVolume ()
{
	int count = 0;
	//count how many positions are filled
	for (int i = 0; i < size; i++)
		if (table[i].count != 0)
			count++;

	//volume = count / table size
	return (float)count/size;
}
/**********************************************************************
                            Setters
**********************************************************************
These set functions set the individual components of the Hashtable object
to new values
**********************************************************************/
void Hashtable :: setCount( int c, int i ) { table[i].count = c; }

void Hashtable :: setKey( const string& k,int i ) { table[i].key = k; }

/**********************************************************************
                            Hash
**********************************************************************
Hash takes in a string, k, and uses the characters to get a number
which is then used in a hash function to determine the index into the
table.
**********************************************************************/
void Hashtable :: Hash (const string& k)
{
	int index = 0;
	int num = 0;
	int i;

	//convert characters of k into a number to be hashed
	for (i = 0; i < k.size(); i++)
		num += (int)k[i];

	//Get hashed index
	index = hashFunction(num);

	i = 1;										//probe sequence modifier
	//probe table until either empty spot or matching key is found
	while ((table[index].count != 0) && (table[index].key != k))
		//i^2, after evaluation increment i, don't go larger than size of table
		index = hashFunction ((num + (i * i++)) % size);
	if (table[index].key == k)		//increase frequency count if key is already there 
		table[index].count++;
	else
	{											//Insert key and increment count
		table[index].key = k;
		table[index].count = 1;
	}
}

/**********************************************************************
                            hashFunction
**********************************************************************
hashFunction uses a key number and returns an integer which utilizes
the key number in a function. This will be the index of the key into
the hashtable
**********************************************************************/
int Hashtable :: hashFunction (int n)
{
	//integer key (k) to a small integer bucket 
	//Return: value h(k). 
	//m is the size of the hash table (number of buckets). 
	//Choose m to be a power of 2. --> 342 
	//Let A be some random-looking real number. Knuth suggests M = 0.5*(sqrt(5) - 1). Then do the following:
    float s = n*(0.5*(sqrt(5) - 1));
    float x = s - (int) s;			//fractional part of s
    int index = floor(342*x);
    index = index % size;
    return index;
}
