#include <iostream>
#include <fstream>

#include "../AES/AES.h"
#include "../MD5/MD5.h"
#include "Crypt5.h"

const unsigned char baseKey[] = { 141, 75, 21, 92, 201, 255, 129, 229, 203, 246, 250, 120, 25, 54, 106, 62, 198, 33, 166, 86, 65, 108, 215, 147 };
const unsigned char initVector[] = { 0x1E,0x39,0xF3,0x69,0xE9,0xD,0xB3,0x3A,0xA7,0x3B,0x44,0x2B,0xBB,0xB6,0xB0,0xB9 };

long loadFile(const std::string &filename, char **output)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	file.seekg(0, std::ios::end);
	int filesize = file.tellg();
	file.seekg(0, std::ios::beg);

	*output = new char[filesize];
	file.read(*output, filesize);
	file.close();

	return filesize;
}

long loadFileUnsigned(const std::string &filename, unsigned char **output)
{
	std::ifstream file(filename.c_str(), std::ios::binary);
	file.seekg(0, std::ios::end);
	int filesize = file.tellg();
	file.seekg(0, std::ios::beg);

	*output = new unsigned char[filesize];
	file.read(reinterpret_cast<char *>(*output), filesize);
	file.close();

	return filesize;
}

void buildKey(unsigned char *key, const char *accountName)
{
	MD5 md5;
	unsigned char *accountNameMd5 = md5.digestStringRaw(accountName);

	for (int i = 0; i < 24; i++)
	{
		key[i] = baseKey[i] ^ accountNameMd5[i & 0xF];
	}
}

void decryptWhatsappDatabase(const std::string &filename, unsigned char *key)
{
	unsigned char *databaseBytes;
	int filesize = loadFileUnsigned("../data/msgstore.db.crypt5", &databaseBytes);

	decrypt_aes_cbc_192(databaseBytes, databaseBytes, filesize, key, const_cast<unsigned char *>(initVector));

	// TODO: use databaseBytes

	delete[] databaseBytes;
}
