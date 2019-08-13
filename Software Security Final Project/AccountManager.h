#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sha.h>
#include <stdio.h>
#include <ctime>
#include <limits>

class AccountManager
{
public:
	enum class FileType
	{
		PLAINTEXT, HASHED, SALTED
	};


	void setCurrentFile(FileType select);
	FileType getCurrentFile() { return currentFile; }

	bool login();
	void createAccount();
	void accountFieldsEntry(bool isAccountCreate);
	bool compareToPlaintext();
	void writeToPlainText();
	bool compareToHashed();
	void writeToHashed();
	bool compareToSalted();
	void writeToSalted();

private:

	FileType currentFile;

	//Temporary account field entry storage containers.
	std::string usernameEntry;
	std::string passwordEntry;

	//Max and minimum field entry input size.
	static const int minEntryLength{ 3 };
	static const int maxEntryLength{ 8 };

	//File stream objects for each file.
	std::fstream plainTextFileInput;
	std::fstream hashedFileInput;
	std::fstream saltedFileInput;


};

