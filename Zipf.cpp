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

	//store indices of each word
	indices[0] = table->getIndex("hello");
	indices[1] = table->getIndex("unique");
	indices[2] = table->getIndex("clever");
	indices[3] = table->getIndex("happy");
	indices[4] = table->getIndex("exact");
	indices[5] = table->getIndex("cleopatra");	//not in table

	//print out table data
	cout << "The table size is: " << table->getSize() << endl;
	cout << "The table is " << (table->getVolume())*100 << "% full." << endl;

	//print out index and count of each word.
	for (int i = 0; i < 5; i++)
		cout << "Index: " << indices[i] << "\tString: " << table->getKey(indices[i]) << "\tCount: " << table->getCount(indices[i]) << endl;

	//print index of string not in table
	cout << "Looking for cleopatra..." << endl;
	cout << "Index: " << indices[5] << "\tString: " << table->getKey(indices[5]) << "\tCount: " << table->getCount(indices[5]) << endl;


	int index = 0;
	bool unused = false;
	
	//Look for an index that isn't in indices
	while (unused && (index < 97))
	{
		//Sorry, I wasn't sure how else to check each of the used indices and increment index...
		if ((index != indices[0]) && (index != indices[1]) && (index != indices[2])&& (index != indices[3])&& (index != indices[4]) && (index != indices[5]))
				unused = true;
		else
			index++;
	}

	//print string & count of unused index
	cout << "Index: " << index << "\tString: " << table->getKey(index) << "\t\tCount: " << table->getCount(index) << endl;


	return 0;
}