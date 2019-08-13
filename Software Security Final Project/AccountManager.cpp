#include "AccountManager.h"

void AccountManager::setCurrentFile(FileType select)
{
	currentFile = select;
}

bool AccountManager::login()
{
	switch (currentFile)
	{
	case FileType::PLAINTEXT:
		//Get user input and validate.
		accountFieldsEntry(false);

		//Compare user inputted data to plaintext file.
		return compareToPlaintext();
		break;

	case FileType::HASHED:
		//Get validated user input.
		accountFieldsEntry(false);

		//Compare hashed user input to each line in hashed.txt.
		return compareToHashed();
		break;

	case FileType::SALTED:
		accountFieldsEntry(false);
		return compareToSalted();
		break;

	default:
		return false;
	}
}

void AccountManager::createAccount()
{
	switch (currentFile)
	{
	case FileType::PLAINTEXT:
		//Get user input and validate.
		accountFieldsEntry(true);

		//Store validated data in plaintext CSV file.
		writeToPlainText();
		break;

	case FileType::HASHED:
		//Get validated user input.
		accountFieldsEntry(true);

		//Store data after crypto-hashing in hashed.txt
		writeToHashed();
		break;

	case FileType::SALTED:
		//Get validated user input.
		accountFieldsEntry(true);

		//Store data in file.
		writeToSalted();
		break;
	}
}

void AccountManager::accountFieldsEntry(bool isAccountCreate)
{
	bool isValid = false;
	while (!isValid)
	{
		system("CLS");
		
		if (isAccountCreate)
			std::cout << "========== CREATE ACCOUNT ==========\n\n";
		else
			std::cout << "========== LOGIN ==========\n\n";

		//Get user input.
		std::cout << "Username: ";
		std::getline(std::cin, usernameEntry, '\n');
		std::cout << "Password: ";
		std::getline(std::cin, passwordEntry, '\n');

		//Input validation:

		//Validate field lengths.
		if (usernameEntry.length() < minEntryLength || usernameEntry.length() > maxEntryLength || 
			passwordEntry.length() < minEntryLength || passwordEntry.length() > maxEntryLength)
		{
			isValid = false;
			continue;
		}

		//Validate username.
		for (char &c : usernameEntry)
		{
			if (!isalnum(c) || !islower(c))
			{
				isValid = false;
				break;
			}
			else
			{
				isValid = true;
			}
		}
		if (!isValid)
			continue;

		//Validate password.
		for (char &c : passwordEntry)
		{
			if (!isalpha(c) || !islower(c))
			{
				isValid = false;
				break;
			}
			else
			{
				isValid = true;
			}
		}
		if (!isValid)
			continue;

		isValid = true;

	}
}

bool AccountManager::compareToPlaintext()
{
	//Open plaintext file (read-only).
	plainTextFileInput.open("plaintext.txt", std::fstream::in);

	//Create comparison string.
	std::string compare = usernameEntry + "," + passwordEntry;

	//Compare line by line.
	std::string currentLine;
	while (std::getline(plainTextFileInput, currentLine, '\n'))
	{
		if (compare == currentLine)
			return true;
	}

	plainTextFileInput.close();

	return false;


}

void AccountManager::writeToPlainText()
{
	//Open plaintext file (write, append).
	plainTextFileInput.open("plaintext.txt", std::fstream::out | std::fstream::app);

	//TODO: Check for duplicate usernames.

	//Append username and password to plaintext file.
	plainTextFileInput << usernameEntry << "," << passwordEntry << "\n";

	plainTextFileInput.close();
}

bool AccountManager::compareToHashed()
{
	//create an unsigned char array from usernameEntry std::string.
	unsigned char uName[maxEntryLength + 1];
	for (int i = 0; i < usernameEntry.size(); ++i)
	{
		uName[i] = usernameEntry[i];

	}
	uName[usernameEntry.size()] = '\0';

	//create an unsigned char array from passwordEntry std::string.
	unsigned char pWord[maxEntryLength + 1];
	for (int i = 0; i < passwordEntry.size(); ++i)
	{
		pWord[i] = passwordEntry[i];
	}
	pWord[passwordEntry.size()] = '\0';

	//Hash buffers.
	unsigned char userHashBuffer[32];
	unsigned char passwordHashBuffer[32];

	SHA256(uName, usernameEntry.length(), userHashBuffer); 
	SHA256(pWord, passwordEntry.length(), passwordHashBuffer);

	//Create comparison string.
	char compare[130];
	compare[0] = '\0';
	for (int i = 0; i < 32; ++i)
	{
		char hex[3];
		sprintf(hex, "%02x", userHashBuffer[i]);
		strcat(compare, hex);

	}
	compare[64] = ',';
	compare[65] = '\0';
	
	for (int i = 0; i < 32; ++i)
	{
		char hex[3];
		sprintf(hex, "%02x", passwordHashBuffer[i]);
		strcat(compare, hex);
	}

	//Open hashed.txt (read-only).
	hashedFileInput.open("hashed.txt", std::fstream::in);

	//Compare c string "compare" to each line in hashed.txt.
	std::string currentLine;
	while (std::getline(hashedFileInput, currentLine, '\n'))
	{
		bool match = false;
		for (int i = 0; i < 130 && i < currentLine.size(); ++i)
		{
			if (currentLine[i] == compare[i])
				match = true;
			else
			{
				match = false;
				break;
			}
		}
		if (match)
		{
			hashedFileInput.close();
			return true;
		}
			
	}

	hashedFileInput.close();
	return false;
}

void AccountManager::writeToHashed()
{
	//create an unsigned char array from usernameEntry std::string.
	unsigned char uName[maxEntryLength + 1];
	for (int i = 0; i < usernameEntry.size(); ++i)
	{
		uName[i] = usernameEntry[i];
		
	}
	uName[usernameEntry.size()] = '\0';

	//create an unsigned char array from passwordEntry std::string.
	unsigned char pWord[maxEntryLength + 1];
	for (int i = 0; i < passwordEntry.size(); ++i)
	{
		pWord[i] = passwordEntry[i];
	}
	pWord[passwordEntry.size()] = '\0';
	
	//Hash buffers
	unsigned char userHashBuffer[32];
	unsigned char passwordHashBuffer[32];

	//Exectute hashes.
	SHA256(uName, usernameEntry.size(), userHashBuffer);
	SHA256(pWord, passwordEntry.size(), passwordHashBuffer);

	//Open hashed.txt (write, append)
	FILE* hashedFile = fopen("hashed.txt", "a");
	

	//Write hashed username to file.
	for (int i = 0; i < 32; ++i)
	{
		fprintf(hashedFile, "%02x", userHashBuffer[i]);
	}

	fprintf(hashedFile, ","); //separator

	//Write hashed password to file.
	for (int i = 0; i < 32; ++i)
	{
		fprintf(hashedFile, "%02x", passwordHashBuffer[i]);
	}

	fprintf(hashedFile, "\n");

	//Close file.
	fclose(hashedFile);
}

bool AccountManager::compareToSalted()
{
	//create an unsigned char array from usernameEntry std::string.
	unsigned char uName[maxEntryLength + 1];
	for (int i = 0; i < usernameEntry.size(); ++i)
	{
		uName[i] = usernameEntry[i];

	}
	uName[usernameEntry.size()] = '\0';

	//create an unsigned char array from passwordEntry std::string.
	unsigned char pWord[maxEntryLength + 2];
	for (int i = 0; i < passwordEntry.size(); ++i)
	{
		pWord[i] = passwordEntry[i];
	}

	//Retrieve salt.
	unsigned char salt;
	saltedFileInput.open("salted.txt", std::fstream::in); //read-only

	//Find matching username.
	bool usernameMatch = false;
	while (saltedFileInput.good())
	{
		std::string currentUsername;

		//read username field up to first comma.
		std::getline(saltedFileInput, currentUsername, ',');
		
		for (int i = 0; i < usernameEntry.size() && i < currentUsername.size(); ++i)
		{
			if (usernameEntry[i] == currentUsername[i])
				usernameMatch = true;
			else
			{
				usernameMatch = false;
				break;
			}
		}

		if (!usernameMatch)
		{
			//Ignore rest of currentLine.
			saltedFileInput.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		else
		{
			char tempSalt;
			saltedFileInput.get(tempSalt);
			salt = (unsigned char)tempSalt;
			break;
		}
	}
	saltedFileInput.close();
	
	//If no username matches were found, return false.
	if (!usernameMatch)
		return false;

	//Append salt to pWord
	pWord[passwordEntry.size()] = salt;
	pWord[passwordEntry.size() + 1] = '\0';

	//Hash user-inputted password with retrieved salt:
	unsigned char passwordHashBuffer[32];
	SHA256(pWord, passwordEntry.size() + 1, passwordHashBuffer);


	//Create comparison string.
	char compare[150];

	//Append username to compare string.
	for (int i = 0; i < usernameEntry.size(); ++i)
		compare[i] = usernameEntry[i];

	//Append salt to compare string.
	compare[usernameEntry.size()] = ',';
	compare[usernameEntry.size() + 1] = salt;
	compare[usernameEntry.size() + 2] = ',';
	compare[usernameEntry.size() + 3] = '\0';

	//Append hashed password hex values to compare string.
	for (int i = 0; i < 32; ++i)
	{
		char hex[3];
		sprintf(hex, "%02x", passwordHashBuffer[i]);
		strcat(compare, hex);
	}

	//Reopen file.
	saltedFileInput.open("salted.txt", std::fstream::in);

	//Search for match.
	std::string currentLine;
	while (std::getline(saltedFileInput, currentLine, '\n'))
	{
		bool match = false;
		for (int i = 0; i < 150 && i < currentLine.size(); ++i)
		{
			if (currentLine[i] == compare[i])
				match = true;
			else
			{
				match = false;
				break;
			}
		}
		if (match)
		{
			saltedFileInput.close();
			return true;
		}
	}

	saltedFileInput.close();
	return false;
}

void AccountManager::writeToSalted()
{
	//create an unsigned char array from usernameEntry std::string.
	unsigned char uName[maxEntryLength + 1];
	for (int i = 0; i < usernameEntry.size(); ++i)
	{
		uName[i] = usernameEntry[i];
	}
	uName[usernameEntry.size()] = '\0';

	//create an unsigned char array from passwordEntry std::string.
	unsigned char pWord[maxEntryLength + 2];
	for (int i = 0; i < passwordEntry.size(); ++i)
	{
		pWord[i] = passwordEntry[i];
	}

	//Generate random 1 byte salt.
	char salt = -1;
	while (salt == -1 || salt == 44) //44 = comma
		salt = rand() % (std::numeric_limits<char>::max() - 32) + 32;

	//Append salt to pWord
	pWord[passwordEntry.size()] = salt;
	pWord[passwordEntry.size() + 1] = '\0';

	//Hash salted password.
	unsigned char passwordHashBuffer[32];
	SHA256(pWord, passwordEntry.size() + 1, passwordHashBuffer);

	//Open salted.txt (write, append).
	FILE* saltedFile = fopen("salted.txt", "a");

	//Write username.
	fprintf(saltedFile, "%s,", uName);

	//Write salt.
	fprintf(saltedFile, "%c,", salt);

	//Write salted+hashed password.
	for (int i = 0; i < 32; ++i)
	{
		fprintf(saltedFile, "%02x", passwordHashBuffer[i]);
	}

	fprintf(saltedFile, "\n");

	//Close salted.txt
	fclose(saltedFile);
}



