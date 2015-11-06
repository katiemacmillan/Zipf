#include <iostream>
#include "Hashtable.h"

using namespace std;

int main (int argc, char** argv)
{

	//series of tests to hash items into a hashtable object
	Hashtable* table = new Hashtable(31);

	table->Hash("hello");
	table->Hash("unique");
	table->Hash("clever");
	table->Hash("unique");
	table->Hash("unique");

	int index = table->getIndex("hello");
	cout << "\"Hello\" is indexed to position: " << index << endl;
	cout << "It has a count of: " << table->getCount(index) << endl;

	index = table->getIndex("unique");
	cout << "\"Unique\" is indexed to position: " << index << endl;
	cout << "It has a count of: " << table->getCount(index) << endl;
	
	index = table->getIndex("clever");
	cout << "\"Clever\" is indexed to position: " << index << endl;
	cout << "It has a count of: " << table->getCount(index) << endl;

	cout << "The string in table[" <<index << "] is: " << table->getKey(index) <<endl;
	return 0;
}