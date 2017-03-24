#include "stdafx.h"

#include "cfiledef.h"

CFileDef::CFileDef(const string &file, bool binary)
{
	/* Initialize m_opened */
	m_opened = false;

	/* Initialize num of keys */
	m_numkeys = 0;

	/* try to open the file */
	if (binary)
		m_Filedef.open(file.c_str(), std::ifstream::in | std::ifstream::binary);
	else
		m_Filedef.open(file.c_str());

	/* if there is an error opening the file */
	if (m_Filedef.good() && !m_Filedef.eof())
	{
		m_opened = true;
	}
}

CFileDef::CFileDef(const string &file)
{
	/* Initialize m_opened */
	m_opened = false;

	/* Initialize num of keys */
	m_numkeys = 0;

	/* try to open the file */
	m_Filedef.open(file.c_str());

	/* if there is an error opening the file */
	if (m_Filedef.good() && !m_Filedef.eof())
	{
		m_opened = true;
	}
}

/*
	Destructor: Close and delete object
 */
CFileDef::~CFileDef()
{
	if ( m_Filedef && m_opened )
	{
		/* Close the file stream */
		m_Filedef.close();
		m_opened = false;
	}
}

/*
	Return if the file was opened
*/
bool CFileDef::getIsOpen()
{
	return m_opened;
}

/* This method get the value from 'k' key in the file in this way:

P.e.
# comment
key key_value
{
	subkey1 subkey1_value subkey2_value .. subkey[MAX]_value
	...
}
*/
void CFileDef::readObjectKeysValues()
{
	GLuint i = 0;
	std::string line;
			
	if (m_opened)
	{
		while (m_Filedef.good() && !m_Filedef.eof())
		{
			/* get line from file and stores into buffer */
			std::getline(m_Filedef, line);
			
			/* ignores '#' '\n' '{' or '\0' characters */
			if (line.size() != 0 && line.at(0) != '#' && line.at(0) != '\n' && line.at(0) != '\0')
			{
				//Get the line into string
				std::stringstream(line) >> line;

				//Get the name (the entire line without minus sign)
				if (line.at(0) == '-')
				{
					m_name = line.substr(1, line.length());
				}
				
				//Get next line
				std::getline(m_Filedef, line);

				// Inits
				if (line.at(0) == '{')
				{
					//Next line discarding empty or comments
					line = GetValidLine();

					//Read and format data
					while (line.at(0) != '}')
					{
						//Delete key/subkeys
						m_keys[m_numkeys].key.erase();
						for (i = 0; i < MAX_SUBKEYS; i++)
							m_keys[m_numkeys].values[i].erase();

						//Get the main key
						std::stringstream (line) >> m_keys[m_numkeys].key;
						
						if (m_keys[m_numkeys].key.length() + 2 < line.length()) //Avoid error
							line = line.substr(m_keys[m_numkeys].key.length() + 2, line.length());

						//Get the subkeys if exist
						i = 0;
						while (std::stringstream(line) >> m_keys[m_numkeys].values[i])
						{
							if (m_keys[m_numkeys].values[i].length() >= line.length())
								break;
							else
							{
								line = line.substr(m_keys[m_numkeys].values[i].length() + 1, line.length());
								i++;
							}
						}
						
						//Next key/values
						m_numkeys++;

						//Next line discarding empty or comments
						line = GetValidLine();
					}
				}
			}
		}

		m_Filedef.close();
	}
}

string CFileDef::GetValidLine()
{
	string line = "";

	while (m_Filedef.good() && !m_Filedef.eof())
	{
		std::getline(m_Filedef, line);

		//Test if line is empty
		if (line.size() >= 1)
		{
			if (line.at(0) == '}')
				return line;
		}

		if (line.size() >= 2)
		{
			//Discard comments
			if (line.at(0) != '#' && line.at(1) != '#')
			{
				return line;
			}
		}
	}

	return line;
}


/*
	Get the name into object file
*/
string CFileDef::getObjectName()
{
	return m_name;
}

/* 
	Get the value associated to key 
*/
string* CFileDef::getObjectValues(const string &_key)
{
	for (int i = 0; i < m_numkeys; i++)
	{
		if (!m_keys[i].key.compare(_key))
		{
			return m_keys[i].values;
		}
	}

	return NULL;
}

/*
	Get the keys i
 */
string CFileDef::getObjectKey(int ind)
{
	if (m_keys[ind].key.length() > 0)
	{
		return m_keys[ind].key;
	}

	return NULL;
}

/*
 * Get the number of total objects that share the same key
 */
int CFileDef::getCount(const string &_key)
{
	int count = 0;

	for (int i = 0; i < m_numkeys; i++)
	{
		if (!m_keys[i].key.compare(_key))
		{
			count++;
		}
	}

	return count;
}

string* CFileDef::getObjectValuesIndex(const string &_key, int index)
{
	string cad[100];
	int ind = 0;

	for (int i = 0; i < m_numkeys; i++)
	{
		if (!m_keys[i].key.compare(_key))
		{
			if (ind == index)
				return m_keys[i].values;
			ind++;
		}
	}

	return NULL;
}

