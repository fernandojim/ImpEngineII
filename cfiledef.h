#ifndef _CFILEDEF_H_
#define _CFILEDEF_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using std::ifstream;
using std::string;

#define MAX_KEYS 50
#define MAX_SUBKEYS 5

/********************
 * DEFINITION OF KEY:
 * {
 *      Key value1 value2 ... value[MAX_SUBKEYS]
 * }
 *
 ********************/
typedef struct _key
{
	string key;
	int n_values;
	string values[MAX_SUBKEYS];
} key;

class CFileDef
{
public:
	/* File of definition */
	ifstream m_Filedef;

	/* Num of keys loaded */
	int m_numkeys;

private:
	/* name of object */
	string m_name;

	/* array of loaded values */
	key m_keys[MAX_KEYS];

	/* If there is an error */
	bool m_opened;

	string GetValidLine();

public:
	/* Constructor */
	CFileDef(const string &file);
	CFileDef(const string &file, bool binary);
	~CFileDef();

	bool getIsOpen();

	/* Gets the name of object */
	string getObjectName();

	/* Get the number of same objects */
	int getCount(const string &_key);

	/* Gets the values from key */
	string* getStringObjectValues(const string &_key);
	float* getFloatObjectValues(const string &_key);
	int* getIntObjectValues(const string &_key);

	/* Gets the values from key at the 'index' position */
	string* getObjectValuesIndex(const string &_key, int index);

	/* Get the keys from file */
	string getObjectKey(int ind);

	/* Gets the keys/values from file */
	void readObjectKeysValues();
};

#endif
