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
		if (table[i].key != "")
		{
			table[i].count = hash.table[i].count;
			table[i].key = hash.table[i].key;
		}
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
int Hashtable :: getCount(const string& k)
{ 
	int index = FindHash(k);
	if (index > -1)
		return table[index].count;

	return 0;			//string not found in table
}

//Returns the number of items in the table
int Hashtable :: getItemCount () { return entries;}

//Returns the Key string for a given index. Returns "" if empty.
string Hashtable :: getKey(int i)
{ 
	if ((i < 0)||(i >= size))
		return "*BAD INDEX*";

	return table[i].key; 
}

//Returns the stored size of the hashtable
int Hashtable :: getSize () {return size;}

//Returns the index containing a given string, -1 if not found
int Hashtable :: getIndex(const string& k)
{
	int index = FindHash(k);
	return index;			//string not found in table
}

//Returns how full the hashtable is.
float Hashtable :: getLoadFactor () { return (float)entries/size; }


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
	table[i].key = "*DELETED*";
	entries--;
}

//Remove entry by string
void Hashtable :: Remove(const string& k)
{ 
	int i = 0;
	int index = FindHash(k);

	if (table[index].key == k)		//if key is there, remove it & count 
	{
		table[index].key = "*DELETED*";
		table[index].count = 0;
		entries--;					//decrement entries count
	}
}

//Decrease the count of an index
void Hashtable :: Decrease(int i)
{ 
	if ((i < 0)||(i >= size))
		return;				//invalid index
	table[i].count--;
}

//Decrease the count of a string
void Hashtable :: Decrease(const string& k)
{ 
	int i = 0;
	int index = FindHash(k);
	if (table[index].key == k)		//if key is there, remove it & count 
		table[index].count--;
}


/**********************************************************************
                            Hash
**********************************************************************
Hash takes in a string, k, and uses the characters to get a number
which is then used in a hash function to determine the index into the
table.
**********************************************************************/
int Hashtable :: InsertHash ( const string& k)
{
	int num = ConvertKey(k);

	//Get hashed index
	int index = HashFunction(num);

	int i = 1;
	//probe table until either empty/deleted spot or matching key is found
	while ((table[index].count != 0) && (table[index].key != k) && (i < size))
	{
		//i^2, after evaluation increment i, don't go larger than size of table
		index = HashFunction (i * HashFunction2(num) % size);
		i++;
	}
	
	return index;
}

int Hashtable :: FindHash (const string& k)
{
	int num = ConvertKey(k);

	//Get hashed index
	int index = HashFunction(num);

	int i = 1;
	//probe table until either empty spot or matching key is found
	while ( (table[index].key != "") && (table[index].key != k) )
	{
		//i^2, after evaluation increment i, don't go larger than size of table
		index = HashFunction (i * HashFunction2(num) % size);
		i++;
	}
	if (table[index].key == k)
		return index;
	else 
		return -1;					//not in the table
}

void Hashtable :: Insert (const string& k)
{
	int index = InsertHash(k);

	if (table[index].key == k)		//increase frequency count if key is already there 
		table[index].count++;

	else
	{											//Insert key and increment count
		table[index].key = k;
		table[index].count = 1;
		entries++;						//Increment number of items in table
		if (((float)entries/size) > 0.75 )
			Rehash();
	}	
}

/**********************************************************************
                            HashFunction
**********************************************************************
HashFunction uses a key number and returns an integer which utilizes
the key number in a function. This will be the index of the key into
the hashtable
**********************************************************************/
int Hashtable :: HashFunction (int n)
{
	/*A suggested formula for HashFunctions involved multiplying
	n by a natual number, then multiplying the fractionl part of that
	number by a multiple of 2. Mod the number by the size of table to
	achieve index.*/

	
    float r = n*(0.5*(sqrt(1321) - 1));	//n* real number
    float f = r - (int) r;				//fractional part of s
    int index = floor(342*f);			//342*f rounded down
    index = index % size;				//Mod index by table size

    return index;
}

int Hashtable :: HashFunction2 (int n)
{
	/*A suggested formula for HashFunctions involved multiplying
	n by a natual number, then multiplying the fractionl part of that
	number by a multiple of 2. Mod the number by the size of table to
	achieve index.*/

    return ((n*197)%101)+((n*1709)%857);
}


/**********************************************************************
                            PrivateFunctions
**********************************************************************/
int Hashtable :: Rehash ()
{
	//store data values
	Data* temp = new Data[entries];
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		if ((table[i].key != "") && (table[i].key != "*DELETED*"))
		{
			temp[j].key = table[i].key;
			temp[j].count = table[i].count;
			j++;
		}
	}

	//get new table size
	size = size*2;
	while (!IsPrime(size))
		size++;

	//resize table
	table = new Data [size];
	int index;

	//Hash all entries from orginal table into new table
	for (int i = 0; i < entries; i++)
	{
		index = InsertHash (temp[i].key);
		table[index].key = temp[i].key;
		table[index].count = temp[i].count;
	}
}

int Hashtable :: ConvertKey (const string& k)
{
	int num = 0;
	//convert characters of k into a number to be hashed
	for (int i = 0; i < k.size(); i++)
		num += (int)k[i];
	return num;
}

bool Hashtable :: IsPrime (int n)
{
    if(n < 2) return false;
    if(n == 2) return true;
    if(n % 2 == 0) return false;
    for(int i = 3; (i*i) <= n; i += 2){
        if(n % i == 0 ) 
        	return false;
    }
    return true;

}