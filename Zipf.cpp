#include <iostream>
#include "Hashtable.h"

using namespace std;

int main (int argc, char** argv)
{

	//series of tests to hash items into a hashtable object
	Hashtable* table = new Hashtable(97);
	int indices[6] = {0};


	//Hash strings into table
	table->Hash("hello");

	for (int i=0; i< 5; i++)
		table->Hash("unique");

	for (int i=0; i< 10; i++)
		table->Hash("clever");

	for (int i=0; i< 17; i++)
		table->Hash("happy");

	for (int i=0; i< 12; i++)
		table->Hash("exact");

	/*store indices of each word, plus index of a "not found" word.
	This is so I have a short list to iterate through of indices,
	meaning I don't have to pring out a 92 blank lines, and can make sure
	I print out data for an out of bounds index.*/
	indices[0] = table->getIndex("hello");
	indices[1] = table->getIndex("unique");
	indices[2] = table->getIndex("clever");
	indices[3] = table->getIndex("happy");
	indices[4] = table->getIndex("exact");
	indices[5] = table->getIndex("earth");	//not in table


	//print out table data
	cout << "The table size is: " << table->getSize() << endl;
	cout << "The table is " << (table->getVolume())*100 << "% full." << endl;

	//Display lookup of key and count by index number
	for (int i = 0; i < 5; i++)
		if (table->getCount(i) != 0)
			cout << "Index: " << indices[i] << "\tString: " << table->getKey(indices[i]) << "\tCount: " << table->getCount(indices[i]) << endl;

	//print data for an outof bounds index
	cout << "Index: -1" << "\tString: " << table->getKey(-1) << "\tCount: " << table->getCount(-1) << endl;		

	/*Look for an index that isn't in indices, 
	My goals is to print data for an index that is in the table range,
	but does not actually contain data*/
	int index = 0;
	bool unused = false;
	
	while (!unused && (index < 97))
	{
		//Sorry, I wasn't sure how else to check each of the used indices and increment index...
		if ((index != indices[0]) && (index != indices[1]) && (index != indices[2])&& (index != indices[3])&& (index != indices[4]) && (index != indices[5]))
				unused = true;
		else
			index++;
	}

	//print string & count of unused index
	cout << "Index: " << index << "\tString: " << table->getKey(index) << "\t\tCount: " << table->getCount(index) << endl;


	//Display look up of index number and count by key
	cout << "String: hello\tIndex: " << table->getIndex("hello") << " \tCount: " << table->getCount("hello") << endl;
	cout << "String: unique\tIndex: " << table->getIndex("unique") << " \tCount: " << table->getCount("unique") << endl;
	cout << "String: clever\tIndex: " << table->getIndex("clever") << " \tCount: " << table->getCount("clever") << endl;
	cout << "String: happy\tIndex: " << table->getIndex("happy") << " \tCount: " << table->getCount("happy") << endl;
	cout << "String: exact\tIndex: " << table->getIndex("exact") << " \tCount: " << table->getCount("exact") << endl;
	//display index and count of a key not in the table
	cout << "String: earth\tIndex: " << table->getIndex("earth") << " \tCount: " << table->getCount("earth") << endl;

	/*I know the index of "hello" is in indices[0], so I'm going to delete that index from the table*/
	table->Remove(indices[0]);
	cout << "String: hello\tIndex: " << table->getIndex("hello") << " \tCount: " << table->getCount("hello") << endl;
	//I know the index of "happy" is in indices[3]
	cout << "Index: " << indices[0] << "\tString: " << table->getKey(indices[0]) << "\tCount: " << table->getCount(indices[0]) << endl;
	
	/*Remove the string "happy" from the table, then try to print data by key*/
	table->Remove("happy");
	cout << "String: happy\tIndex: " << table->getIndex("happy") << " \tCount: " << table->getCount("happy") << endl;
	//I know the index of "happy" is in indices[3]
	cout << "Index: " << indices[3] << "\tString: " << table->getKey(indices[3]) << "\tCount: " << table->getCount(indices[3]) << endl;


	return 0;
}