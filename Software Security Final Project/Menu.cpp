#include "Menu.h"

Menu::Menu()
	:
	currentMenu(MenuState::START)
{
}

void Menu::fileSelectMenu()
{
	std::cout <<
		"========== WELCOME! ==========\n\n"
		"Choose Account Storage File:\n\n"
		"1. Plaintext\n"
		"2. Hashed\n"
		"3. Hashed + Salted\n";
}

void Menu::loginOrCreateMenu()
{
	std::cout <<
		"Create Account OR Login?\n\n"
		"1. Create Account\n"
		"2. Login\n";
}

void Menu::createAccountMenu()
{
	am.createAccount();
	currentMenu = MenuState::CREATE_SUCCESS;
}

void Menu::loginMenu()
{
	if (am.login())
	{
		currentMenu = MenuState::LOGIN_SUCCESS;
	}
	else
	{
		currentMenu = MenuState::LOGIN_FAIL;
	}
}

void Menu::loginSuccessMenu()
{
	std::cout << "========== LOGGED IN SUCCESFULLY ==========";
}

void Menu::loginFailMenu()
{
	std::cout << "========== COULD NOT LOG IN ==========";
}

void Menu::createAccountSuccessMenu()
{
	std::cout << "========== ACCOUNT CREATED ==========";
}

void Menu::displayMenu()
{
	//Display different menus depending on currentMenu.
	switch (currentMenu)
	{
	case MenuState::START:
		system("CLS");
		fileSelectMenu();
		optionSelect();
		break;

	case MenuState::CREATE_LOGIN:
		system("CLS");
		loginOrCreateMenu();
		optionSelect();
		break;

	case MenuState::ACCOUNT_CREATE:
		system("CLS");
		createAccountMenu();
		break;

	case MenuState::ACCOUNT_LOGIN:
		system("CLS");
		loginMenu();
		break;

	case MenuState::CREATE_SUCCESS:
		system("CLS");
		createAccountSuccessMenu();
		Sleep(100);
		break;

	case MenuState::LOGIN_SUCCESS:
		system("CLS");
		loginSuccessMenu();
		Sleep(100);
		break;

	case MenuState::LOGIN_FAIL:
		system("CLS");
		loginFailMenu();
		Sleep(100);
		break;
	}
}

int Menu::optionSelect()
{
	int selection = -1;

	std::cout << "\nInput Selection: ";

	switch (currentMenu)
	{
	case MenuState::START:
		selection = getMenuSelectionInput(3);
		currentMenu = MenuState::CREATE_LOGIN;

		//Set currentFile in AccountManager object.
		switch (selection)
		{
		case 1:
			am.setCurrentFile(AccountManager::FileType::PLAINTEXT);
			break;

		case 2:
			am.setCurrentFile(AccountManager::FileType::HASHED);
			break;

		case 3:
			am.setCurrentFile(AccountManager::FileType::SALTED);
			break;
		}
		break;
		
	case MenuState::CREATE_LOGIN:
		selection = getMenuSelectionInput(2);
		
		switch (selection)
		{
		case 1:
			currentMenu = MenuState::ACCOUNT_CREATE;
			break;

		case 2:
			currentMenu = MenuState::ACCOUNT_LOGIN;
			break;
			
		default: std::cout << "ERROR: Invalid selection on Menu -> CREATE_LOGIN." << std::endl;
		}
		break;
	}

	return selection;
}

int Menu::getMenuSelectionInput(int numOptions)
{
	int choice = -1;

	//While std::cin does not read an int and input is not a valid choice.
	while (!(std::cin >> choice) || choice < 1 || choice > numOptions)
	{
		//Clear istream buffer's error flags.
		std::cin.clear(); 

		//Flush the input buffer.
		std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

		std::cout << "Invalid input. Re-enter: ";
	}

	//Flush the input buffer.
	std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

	return choice;
}


