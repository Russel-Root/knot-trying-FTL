#include "Entry.h"
#include <string>

#ifndef NULL
#define NULL 0
#endif// NULL

Entry::Entry(){
	this->key	= NULL;
	this->value	= NULL;
}

Entry::~Entry(){
	if (this->key)
		delete this->key;
	if (this->value)
		delete this->value;
}

Entry* Entry::createNewEntry(const char* _key, const char* _value){
	if ( !_key || !_value )
		return NULL;

	size_t keyLength	= strlen(_key);
	size_t valueLength	= strlen(_value);

	if ( keyLength == 0 || valueLength == 0 )
		return NULL;

	Entry* entry = new Entry();
	entry->key = new char[keyLength + 1];
	entry->value = new char[valueLength + 1];

	strcpy(entry->key, _key);
	strcpy(entry->value, _value);

	return entry;
}