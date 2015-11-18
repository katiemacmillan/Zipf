/**********************************************************************
                            Hashtable.cpp
***********************************************************************
					Author:	Katherine MacMillan
***********************************************************************
The hashtable class consists of two ineger variables called 'size' and 
'entries', and an array called 'table'.

'size' holds the total size of the 'table', while 'entries' holds
the number of actual entries that 'table' contains. 

'table' is an array of integer and string pairs, refered to as a
'tableEntry'. The for each index 'i' in 'table' the the first 
field holds an integer for the number of times a key appears in
the loaded file, and the second field holds the string representing
the key. 

When created, the user may use the default tablesize, or specify
their own. 'table' is then dynamically allocated to hold an 'n' 
tableEntries. Each table[i].first is initialized to 0 and each
table[i].second is set to an empty string.

Whenever a string is inserted into the hashtable object, the second 
field in the index is set to the string value, the first field is set 
to 1, and the 'entries' variable is incremented by 1.

Entries are instered into 'table' based on hashing, using linear
probing and open addressing to resolve collisions. 

Entries can be removed by the user via the Remove function, and
entry counts can be decremented using the Decrease function.

**********************************************************************/

#include "Hashtable.h"

/**********************************************************************
                            Constructor
**********************************************************************/
Hashtable::Hashtable ( int n )
{
	// create new table of size n, set size to n
	table = new tableEntry[n];
	size = n;
}


/**********************************************************************
                            Copy Constructor
**********************************************************************/
Hashtable::Hashtable ( const Hashtable& hash )
{
	// create new table same size as hash
	size = hash.size;
	table = new tableEntry[size];

	// copy values from hash into new table
	for ( int i = 0; i < size; i++ )
	{
		if ( table[i].second != "" )
		{
			table[i].first = hash.table[i].first;
			table[i].second = hash.table[i].second;
		}
	}
}

/**********************************************************************
                            Destructor
**********************************************************************/
Hashtable::~Hashtable ()
{}

/***************************Getters***********************************/

/**********************************************************************
                            GetCount
**********************************************************************
GetCount retrieves the value of the first field for a given index or
target string.

When an integer is passed to the function, it is used as an index into
the hashtable. If it is a valid index for the table, the value of that
index's first field is returned to the user. If 'i' is not a valid 
index into the table, -1 is returned instead.

When a string is passed to the function, the FindHash function is used
to determine the string's index in the table. If the string is found
in the returned index's second field then the index's first field is 
returned to the user. Otherwise, 0 is returned.
**********************************************************************/
int Hashtable::GetCount ( int i )
{ 
	// invalid index
	if ( ( i < 0 ) || ( i >= size ) )
		return -1;

	return table[i].first; 
}

int Hashtable::GetCount ( const string& k )
{ 
	int i = FindHash(k);
	if ( i > -1 )
		return table[i].first;

	// string not found in table
	return 0;
}

/**********************************************************************
                            GetKey
**********************************************************************
This function takes in an index into the hashtable as the variable 'i'
and then returns to the user the string that is held in that index's
second field. If the value in 'i' is not a valid index into the table,
the string "*BAD INDEX*" is returned to the user instead. 
**********************************************************************/
string Hashtable::GetKey ( int i )
{ 
	if ( ( i < 0 ) || ( i >= size ) )
		return "*BAD INDEX*";

	return table[i].second; 
}

/**********************************************************************
                            GetIndex
**********************************************************************
This function takes in a character string as the variable 'k' and
utilizes the FindHash function to determine the key's position index
in the table. This index is returned to the user. It will return as
a -1 from FindHash if the key is not found in the table.
**********************************************************************/
int Hashtable::GetIndex ( const string& k ){ return FindHash(k); }


/**********************************************************************
                            GetSize
**********************************************************************
This function returns to the user the total size of the hashtable,
meaning the total number of available positions in the table, not
just the filled ones. 
The table size is stored in the 'size' variable within the hashtable
class.
**********************************************************************/
int Hashtable::GetSize () { return size; }

/**********************************************************************
                            GetEntryCount
**********************************************************************
This function returns the total number of filled positions that exist
in the hashtable. This data is stored in the 'entries' variable in 
the hashtable class.
**********************************************************************/
int Hashtable::GetEntryCount () { return entries; }

/**********************************************************************
                            GetLoadFactor
**********************************************************************
This function calculates and returns the percentage of the hashtable
which is filled. This number is found by dividing the stored number
of filled positions in the 'entries' variable by the stored size of
the table in the 'size' variable.
**********************************************************************/
float Hashtable::GetLoadFactor () { return (float) entries / size; }

/***************************Mutators**********************************/

/**********************************************************************
                            Insert
**********************************************************************
Insert takes an insertion string the the variable 'k' and uses the
InsertHash fuction to retrieve an index. If the insertion string is 
already held in the retrieved index's second field, then the index's 
first field is incremented by one.

Otherwise, the index's second field is set to the insertion string,
its first field is set to 1 and the hashtable's 'entries' variable
is incremented by one, indicating that another entry has been added
to the table.

After a new entry has been added to the table, the Insert function
checks the load factor of the table. If the table is more than 75%
full, the Rehash function is called
**********************************************************************/
void Hashtable::Insert ( const string& k )
{
	int i = InsertHash(k);

	// increase frequency count if key exists
	if ( table[i].second == k )
		table[i].first++;

	else
	{	
		// insert and increment table entries
		table[i].first++;
		table[i].second = k;
		entries++;
		
		// if table is more than 75% full after insert, rehash it
		if ( ( (float) entries / size ) > 0.75 )
		{
			cout << "Table is at least 75% full!\nRehashing now..."<< endl;
			Rehash();
		}
	}	
}

/**********************************************************************
                            Remove
**********************************************************************
The Remove function allows the user to remove an entry from the
hashtable, either by specifying an index in the table, or by 
specifying the string to be deleted. When an entry is deleted the
position's first field is set to 0, its second field is set to 
"*DELETED*" and the hashtable's 'entries' variable is decremented by one.

When an integer is passed to the function, it is used as an index into
the hashtable. If it is a valid index for the table the entry is removed.

When a string is passed to the function, the FindHash function is used
to determine the string's index in the table. If the string is found
then the entry at its index is removed
**********************************************************************/
void Hashtable::Remove ( int i )
{ 
	// invalid index
	if ( ( i < 0 ) || ( i >= size ) )
		return;	

	// reset count, mark as deleted and decrement entries
	table[i].first = 0;
	table[i].second = "*DELETED*";
	entries--;
}

void Hashtable::Remove ( const string& k )
{ 
	int i = FindHash(k);

	// if key is there reset count, mark as deleted and decrement entries
	if ( table[i].second == k )
	{
		table[i].first = 0;
		table[i].second = "*DELETED*";
		entries--;
	}
}

/**********************************************************************
                            Decrease
**********************************************************************
The Decrease function allows the user to decrement the count of a 
specific word or index in the hashtable. When the count gets to 0
the string in the second field is then marked as deleted by setting it
to "*DELETED*"

When an integer is passed to the function, it is used as an index into
the hashtable. If it is a valid index for the table, the count is 
decremented. 

When a string is passed to the function, the FindHash function is used
to determine the string's index in the table. If the string is found
then the corresponding count field for its index is decremented.
**********************************************************************/
void Hashtable::Decrease ( int i )
{ 
	// invalid index
	if ( ( i < 0 ) || ( i >= size ) )
		return;

	table[i].first--;

	// if count hits 0, mark key as deleted
	if ( table[i].first == 0 )
		table[i].second = "*DELETED*";
}


void Hashtable::Decrease ( const string& k )
{ 
	int i = FindHash(k);

	// if key is there, decrement count
	if ( table[i].second == k )
		table[i].first--;

	// if count hits 0, mark key as deleted
	if ( table[i].first == 0 )
		table[i].second = "*DELETED*";
}

/***************************PrivateFunctions**************************/

/**********************************************************************
                            HashFunction
**********************************************************************
HashFunction utilizes the numeric values of each character in the
given string, held in variable 'k'. A variable 'hash' is set to an
initial number and then for each letter in 'k', 'hash' is multiplied
by 33, and the numeric value of the character in 'k[i]' is added.

This method for a hashfunction was derived from formula "djb2" at
http://www.cse.yorku.ca/~oz/hash.html
**********************************************************************/
unsigned long Hashtable::HashFunction ( const string& k )
{
    unsigned long hash = 5381;

    for ( unsigned int i  = 0; i < k.length(); i++ )
        hash = ( hash * 33 ) + k[i];

    return hash;
}

/**********************************************************************
                            InsertHash
**********************************************************************
The InsertHash function performs its probe sequence by looking for 
entries with a count of 0, or entries with a second field matching the
insertion string held in the variable 'k'. 

By looking for a first field value of 0, insertion strings that are 
not already in the hashtable can be inserted into positions that had
held entries, but which were deleted, thus wasting as little space
within the table as possible.

**********************************************************************/
int Hashtable::InsertHash ( const string& k )
{
	int i = ( HashFunction(k) )%size;

	// probe table until either empty/deleted spot or matching key is found
	while ( (table[i].first != 0 ) && ( table[i].second != k ) )
		i = ( i + 1 ) % size;
	
	return i;
}

/**********************************************************************
                            FindHash
**********************************************************************
The FindHash function performs its probe sequence by looking for
either the target string or an empty string in the second field of each
examined index.

The function will stop probing only when the target string is found,
or when an empty string is encountered. This ensures that if an entry
which preceeds the target gets deleted, the probe sequence will 
continue to search past it for the target string.
**********************************************************************/
int Hashtable::FindHash ( const string& k )
{
	int i = ( HashFunction(k) ) % size;

	// probe table until either empty string or matching key is found
	while ( ( table[i].second != "" ) && ( table[i].second != k ) )
		i = ( i + 1 ) % size;
	
	if ( table[i].second == k )
		return i;

	// not in table
	else 
		return -1;
}

/**********************************************************************
                            Rehash
**********************************************************************
Rehash creates a new table which is roughly twice the size of the 
original. After doubling the size, by using the 'size' variable,
the new size is incremented until it is a prime number. This is the 
size of the new table, and is stored in the hashtable's 'size'
variable.

After the new table is created, all of the entries in the original
table which were not empty or deleted are inserted into the new
table using the Insert function, so they are hashed to new locations
within the new table.
**********************************************************************/
void Hashtable::Rehash ()
{
	// store data values
	tableEntry* temp = table;
	int oldSize = size;

	// get new table size
	size = size*2;
	while ( !IsPrime(size) )
		size++;

	// resize table
	table = new tableEntry [size];
	int index;

	// hash entries from orginal table into new table
	for ( int i = 0; i < oldSize; i++ )
	{
		// skip empty and deleted entries
		if ( ( temp[i].second != "" ) && ( temp[i].second != "*DELETED*" ) )
		{
			index = InsertHash ( temp[i].second );
			table[index].first = temp[i].first;
			table[index].second = temp[i].second;
		}
	}
}

/**********************************************************************
                            IsPrime
**********************************************************************
IsPrime evaluates an integer passed in through the variable 'n' to 
determine if it is a prime number. 

First IsPrime checks to see if 'n' is divisible by 2. If not, then an
integer 'i' starts at 3 and is incremented by 2, until it is greater
than the square root of 'n'. With each incrementation, IsPrime checks 
to see if 'n' is divisible by 'i'. If at any point it is, then 'n' is 
not a prime number, and false is returned.
**********************************************************************/
bool Hashtable::IsPrime ( int n )
{
	// if n is divisible by 2 it's not prime
    if ( n % 2 == 0 ) 
    	return false;

    // if n is divisible by i, it's not prime
    for ( int i = 3; ( i * i ) <= n; i += 2 )
        if ( n % i == 0 )
        	return false;
    
    return true;
}