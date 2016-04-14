#include "stdafx.h"

#include "cfiledef.h"

CFileDef::CFileDef(const string file)
{
	/* m_error is true at the beginning */
	m_opened = false;

	/* Initialize num of keys */
	m_numkeys = 0;

	/* try to open the file */
	m_filedef.open(file);

	/* if there is an error opening the file */
	if (m_filedef.good() && !m_filedef.eof())
	{
		m_opened = true;
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
void CFileDef::getObjectKeysValues()
{
	GLuint i = 0;
	std::string line;
			
	if (m_opened)
	{
		while (m_filedef.good() && !m_filedef.eof())
		{
			/* get line from file and stores into buffer */
			std::getline(m_filedef, line);
			
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
				std::getline(m_filedef, line);

				// Inits
				if (line.at(0) == '{')
				{
					//Next line
					std::getline(m_filedef, line);

					//Read and format data
					while (line.at(0) != '}')
					{
						//Delete key/subkeys
						m_keys[m_numkeys].key.erase();
						for (i = 0; i < MAX_SUBKEYS; i++)
							m_keys[m_numkeys].values[i].erase();

						//Get the main key
						std::stringstream(line) >> m_keys[m_numkeys].key;
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

						//Next line
						std::getline(m_filedef, line);
					}
				}
			}
		}

		/* Close the file stream */
		m_filedef.close();
		m_opened = false;
	}
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
string* CFileDef::getObjectValues(const string _key)
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
