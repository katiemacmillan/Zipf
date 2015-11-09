/* Hashtable.cpp */

#include "Hashtable.h"

/**********************************************************************
                            Constructor
**********************************************************************/
Hashtable :: Hashtable( int n )
{
	//create new table of size n, set size to n
	table = new Data[n];
	size = n;
}

/**********************************************************************
                            Copy Constructor
**********************************************************************/
Hashtable :: Hashtable( const Hashtable& hash )
{
	//create new table same size as hash
	size = hash.size;
	table = new Data[size];

	//copy values from hash into new table
	for (int i = 0; i < size; i++)
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
//Returns the count of a given index into the table
int Hashtable :: getCount(int i)
{ 
	if ((i < 0)||(i >= size))
		return -1;				//invalid index

	return table[i].count; 
}
//Returns the count of a given string
int Hashtable :: getCount(string k)
{ 
	int i = 0;
	for (i; i < size; i++)
		//find matching string in table, return index
		if (table[i].key == k)
			return table[i].count;

	return 0;			//string not found in table
}

//Returns the Key string for a given index. Returns "" if empty.
string Hashtable :: getKey(int i)
{ 
	if ((i < 0)||(i >= size))
		return "BAD INDEX";

	return table[i].key; 
}

//Returns the stored size of the hashtable
int Hashtable :: getSize () {return size;}

//Returns the index containing a given string, -1 if not found
int Hashtable :: getIndex(const string& k)
{
	int i = 0;
	for (i; i < size; i++)
		//find matching string in table, return index
		if (table[i].key == k)
			return i;

	return -1;			//string not found in table
}

//Returns how full the hashtable is.
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
The only setting of indices a user can do is remove items from the
table. Users may not manually set the count or key of a specific 
index. Nor may they manually set the count of a specific key.
**********************************************************************/

//Remove entry by index
void Hashtable :: Remove(int i)
{ 
	if ((i < 0)||(i >= size))
		return;				//invalid index
	table[i].count = 0;
	table[i].key = "";
}

//Remove entry by string
void Hashtable :: Remove(string k)
{ 
	int num = 0;
	int i;
	//convert characters of k into a number to be hashed
	for (int i = 0; i < k.size(); i++)
		num += (int)k[i];

	int index = hashFunction(num);
	i = 0;
	while ((table[index].count != 0) && (table[index].key != k))
	{
		//i^2, after evaluation increment i, don't go larger than size of table
		index = hashFunction ((num + (i * i)) % size);
		i++;
	}

	if (table[index].key == k)		//if key is there, remove it & count 
	{
		table[index].key = "";
		table[index].count = 0;
	}
}


/**********************************************************************
                            Hash
**********************************************************************
Hash takes in a string, k, and uses the characters to get a number
which is then used in a hash function to determine the index into the
table.
**********************************************************************/
void Hashtable :: Hash (const string& k)
{
	int index = 0;				//hashed index number
	int num = 0;				//key translated into an integer
	int i;

	//convert characters of k into a number to be hashed
	for (i = 0; i < k.size(); i++)
		num += (int)k[i];

	//Get hashed index
	index = hashFunction(num);

	i = 1;										//probe sequence modifier

	//probe table until either empty spot or matching key is found
	while ((table[index].count != 0) && (table[index].key != k))
	{
		//i^2, after evaluation increment i, don't go larger than size of table
		index = hashFunction ((num + (i * i)) % size);
		i++;
	}

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
	/*A suggested formula for hashfunctions involved multiplying
	n by a natual number, then multiplying the fractionl part of that
	number by a multiple of 2. Mod the number by the size of table to
	achieve index.*/

	
    float r = n*(0.5*(sqrt(5) - 1));	//n* real number
    float f = r - (int) r;				//fractional part of s
    int index = floor(342*f);			//342*f rounded down
    index = index % size;				//Mod index by table size

    return index;
}

