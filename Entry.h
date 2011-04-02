#ifndef __Entry_h__
#define __Entry_h__

// An Entry is a key-value pair
class Entry{
private:
	Entry();
public:
	~Entry();

	static Entry* createNewEntry(const char* _key, const char* _value);

public:
	char* key;
	char* value;
};
#endif