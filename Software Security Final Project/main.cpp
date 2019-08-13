#include "Application.h"
#include <vector>
#include <cmath>
#include "Timer.h"

Timer STOPWATCH;

int main()
{
	//Seed rand.
	srand(time(NULL));

	Application Project2;

	Project2.run(Application::TaskState::TASK1);

	return 0;
}

//===============FUNCTION DEFINITIONS========================

void generateRandomAccounts(int numGenerate, int minFieldLength, int maxFieldLength, AccountManager::FileType fType)
{
	//Strings to contain randomly generated strings.
	std::string user;
	std::string pass;

	//Create one account per loop iteration.
	for (int i = 0; i < numGenerate; ++i)
	{
		//Character-set
		static const char alphanum[] = "0123456789abcdefghijklmnopqrstuvwxyz";

		//Randomly determine username length and password length.
		unsigned int userLength = rand() % (maxFieldLength - minFieldLength + 1) + minFieldLength;
		unsigned int passLength = rand() % (maxFieldLength - minFieldLength + 1) + minFieldLength;

		//Generate username.
		for (int i = 0; i < userLength; ++i)
		{
			user.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
		}

		//Generate password.
		for (int i = 0; i < passLength; ++i)
		{
			pass.push_back(alphanum[rand() % (sizeof(alphanum) - 11) + 10]);
		}

		//Add to file.
		unsigned char* uName;
		unsigned char* pWord;
		unsigned char userHashBuffer[32];
		unsigned char passwordHashBuffer[32];
		char salt = -1;
		FILE* hashedFile;
		FILE* saltedFile;
		std::fstream fout;

		switch (fType)
		{
		//--------------PLAINTEXT-------------------------------------------------------

		case AccountManager::FileType::PLAINTEXT:
			fout.open("plaintext.txt", std::fstream::out | std::fstream::app);
			
			fout << user << "," << pass << "\n";

			fout.close();
			break;

		//---------------HASHED-------------------------------------------------------

		case AccountManager::FileType::HASHED:

			//create an unsigned char array from user std::string.
			uName = new unsigned char[maxFieldLength + 1];
			for (int i = 0; i < user.size(); ++i)
			{
				uName[i] = user[i];
			}
			uName[user.size()] = '\0';

			//create an unsigned char array from pass std::string.
			pWord = new unsigned char[maxFieldLength + 1];
			for (int i = 0; i < pass.size(); ++i)
			{
				pWord[i] = pass[i];
			}
			pWord[pass.size()] = '\0';

			//Exectute hashes.
			SHA256(uName, user.size(), userHashBuffer);
			SHA256(pWord, pass.size(), passwordHashBuffer);

			//Open file.
			hashedFile = fopen("hashed.txt", "a");

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

			fclose(hashedFile);

			//Free unsigned char arrays from heap.
			delete[] uName;
			delete[] pWord;
			break;

		//---------------SALTED--------------------------------------------------------

		case AccountManager::FileType::SALTED:
			//create an unsigned char array from user std::string.
			uName = new unsigned char[maxFieldLength + 1];
			for (int i = 0; i < user.size(); ++i)
			{
				uName[i] = user[i];
			}
			uName[user.size()] = '\0';

			//create an unsigned char array from pass std::string.
			pWord = new unsigned char[maxFieldLength + 2];
			for (int i = 0; i < pass.size(); ++i)
			{
				pWord[i] = pass[i];
			}

			//Generate random 1 byte salt.
			while (salt == -1 || salt == 44) //44 = comma
				salt = rand() % ((std::numeric_limits<char>::max)() - 32) + 32;

			//Append salt to pWord
			pWord[pass.size()] = salt;
			pWord[pass.size() + 1] = '\0';

			//Hash salted password.
			SHA256(pWord, pass.size() + 1, passwordHashBuffer);

			//Open salted.txt (write, append).
			saltedFile = fopen("salted.txt", "a");

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

			//Free uName and pWord.
			delete[] uName;
			delete[] pWord;

			break;

		default:
			std::cout << "ERROR: Invalid Filetype (Task 2)" << std::endl;
			break; 
		}

		//Clear user and pass.
		user.clear();
		pass.clear();
	}

}

void cracknhack(AccountManager::FileType fType)
{
	//vars.
	std::vector<std::string> filePasswords;
	std::string compare;
	std::fstream fin;
	std::vector<int> arrayAnswer;
	std::vector<int> arrayAddOne;
	unsigned char hashBuffer[32];
	char hex[3];
	unsigned char* tempPass;
	std::fstream hashLogger;
	std::fstream saltedLogger;

	
	switch (fType)
	{
	case AccountManager::FileType::HASHED:
		hashLogger.open("hashlog.txt", std::fstream::out | std::fstream::trunc);
		hashLogger << "========HASH LOG================\n";

		STOPWATCH.start();
		
		//Read all passwords into an arrray:
		//Open file.
		fin.open("hashed.txt", std::fstream::in);

		//Iterate through entire file.
		while (fin.good())
		{
			//Skip username field.
			fin.ignore((std::numeric_limits<std::streamsize>::max)(), ',');

			//Read password field into array.
			std::string currentPassword;
			std::getline(fin, currentPassword, '\n');
			filePasswords.push_back(currentPassword);
		}
		//Remove last entry which is just a '\n'.
		filePasswords.pop_back();

		//Generate every possible compination of characters in c strings size 3 to 8:
		for (int i = 3; i < 9; ++i)
		{
			arrayAnswer.resize(i);
			arrayAddOne.resize(i);

			for (int a = 0; a < i; a++)
			{
				arrayAnswer[a] = 0;
				arrayAddOne[a] = 0;
			}

			arrayAddOne[i - 1] = 1;

			for (int j = 0; j < pow(26,i); ++j)
			{
				//Print progress.
				if(j % 100000 == 0)
					std::cout << j << "i = " << i << '\n';

				for (int k = i-1; k >= 0; --k)
				{
					arrayAnswer[k] += arrayAddOne[k];
					if (arrayAnswer[k] > 25 && k != 0)
					{
						arrayAnswer[k] = arrayAnswer[k] - 26;
						arrayAnswer[k - 1] = arrayAnswer[k - 1] + 1;
					}
				}

				//Copy arrayAnswer into cString.
				tempPass = new unsigned char[i + 1];
				for (int c = 0; c < i; ++c)
				{
					tempPass[c] = (char)(arrayAnswer[c] + 97);
				}
				tempPass[i] = '\0';

				//Hash tempPass.
				SHA256(tempPass, arrayAnswer.size(), hashBuffer);

				//Create comparison string.
				for (int i = 0; i < 32; ++i)
				{
					sprintf(hex, "%02x", hashBuffer[i]);
					compare.push_back(hex[0]);
					compare.push_back(hex[1]);
				}

				//Compare comparsison string to vector of passwords read from file.
				for (auto &password : filePasswords)
				{
					if (password == compare)
					{
						STOPWATCH.stop();
						std::cout << "\nCRACKED A HASHED PASSWORD.\n";

						//Write to log.
						hashLogger << "Found matching password: ";
						for (int pSize = 0; pSize < i; pSize)
						{
							hashLogger << tempPass[pSize];
						}
						hashLogger << "\nHash: " << password << std::endl;
						STOPWATCH.getElapsed(hashLogger);
						

						
					}
						
				}

				compare.clear();
				delete[] tempPass;

			}
		}
		hashLogger.close();
		break;

	case AccountManager::FileType::SALTED:
		saltedLogger.open("saltedlog.txt", std::fstream::out | std::fstream::trunc);
		saltedLogger << "========SALTED LOG================\n";
		STOPWATCH.start();
		//Read all passwords into an arrray:
		//Open file.
		fin.open("salted.txt", std::fstream::in);

		//Iterate through entire file.
		while (fin.good())
		{
			//Skip username field.
			fin.ignore((std::numeric_limits<std::streamsize>::max)(), ',');

			//Skip salt field.
			fin.ignore((std::numeric_limits<std::streamsize>::max)(), ',');

			//Read password field into array.
			std::string currentPassword;
			std::getline(fin, currentPassword, '\n');
			filePasswords.push_back(currentPassword);
		}
		//Remove last entry which is just a '\n'.
		filePasswords.pop_back();

		//Generate every possible compination of characters in c strings size 3 to 8:
		for (int i = 3; i < 9; ++i)
		{
			arrayAnswer.resize(i);
			arrayAddOne.resize(i);

			for (int a = 0; a < i; a++)
			{
				arrayAnswer[a] = 0;
				arrayAddOne[a] = 0;
			}

			arrayAddOne[i - 1] = 1;

			for (int j = 0; j < pow(26, i); ++j)
			{
				//Print progress.
				if (j % 100000 == 0)
					std::cout << j << "i = " << i << '\n';

				for (int k = i - 1; k >= 0; --k)
				{
					arrayAnswer[k] += arrayAddOne[k];
					if (arrayAnswer[k] > 25 && k != 0)
					{
						arrayAnswer[k] = arrayAnswer[k] - 26;
						arrayAnswer[k - 1] = arrayAnswer[k - 1] + 1;
					}
				}

				//Copy arrayAnswer into cString.
				tempPass = new unsigned char[i + 2];
				for (int c = 0; c < i; ++c)
				{
					tempPass[c] = (char)(arrayAnswer[c] + 97);
				}

				//For every possible salt.
				for (char salt = 32; salt < 127; ++salt)
				{
					if (salt == 44)//no comma.
						continue;

					//Append to tempPass.
					tempPass[i] = salt;
					tempPass[i + 1] = '\0';

					//Hash tempPass.
					SHA256(tempPass, arrayAnswer.size() + 1, hashBuffer);

					//Create comparison string.
					for (int i = 0; i < 32; ++i)
					{
						sprintf(hex, "%02x", hashBuffer[i]);
						compare.push_back(hex[0]);
						compare.push_back(hex[1]);
					}

					//Compare comparsison string to vector of passwords read from file.
					for (auto &password : filePasswords)
					{
						if (password == compare)
						{
							STOPWATCH.stop();
							std::cout << "\nCRACKED A SALTED PASSWORD.\n";

							//Write to log.
							saltedLogger << "Found matching password: ";
							for (int pSize = 0; pSize < i; pSize)
							{
								saltedLogger << tempPass[pSize];
							}
							saltedLogger << "\nHash: " << password << std::endl;
							STOPWATCH.getElapsed(saltedLogger);

						}
							
					}
					compare.clear();
				}
				delete[] tempPass;
			}
		}
		saltedLogger.close();
		break;
	}
}

