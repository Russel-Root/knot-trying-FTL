#ifndef __PropertyReader_h__
#define __PropertyReader_h__

#include <stdio.h>
#include "ConfigReader.h"
#include "Entry.h"
#include "LinkedList.h"

class PropertyReader : public ConfigReader{
public:
	PropertyReader();
	~PropertyReader();

	bool readConfigFile(const char* filepath);
	const char* getProperty(const char* key) const;

private:
	MyLinkedList<Entry>*			properties;
	MyLinkedListIterator<Entry>*	iterator;
};
#endif