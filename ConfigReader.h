#ifndef __ConfigReader_h__
#define __ConfigReader_h__

// ConfigReader is an interface for reading configuration files
// It leaves the implementations of several virtual methods for its sub classes
// the format of the configuration file is key-value pair

class ConfigReader{
public:
	ConfigReader();
	virtual ~ConfigReader();

	virtual bool readConfigFile(const char* filename) = 0;
	virtual const char* getProperty(const char* key) const = 0;

public:
	static const int SPACE; 
	static const int TAB;

	static const int EQUAL; // "="
	static const int COLON; // ":"

	static const int LINE_BEGIN;
	static const int LINE_END;

	static const int CARRIAGE; // carriage return

	static const int   KEY_MAX_LENGTH;
	static const int VALUE_MAX_LENGTH;
};

#endif