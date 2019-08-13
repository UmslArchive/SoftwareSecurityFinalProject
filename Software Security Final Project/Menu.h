#pragma once

#include <iostream>
#include <cstdlib>
#include "AccountManager.h"
#include <Windows.h>

class Menu
{

public:
	enum class MenuState
	{
		START, CREATE_LOGIN, ACCOUNT_CREATE, ACCOUNT_LOGIN, LOGIN_SUCCESS, LOGIN_FAIL, CREATE_SUCCESS
	};

private:
	
	AccountManager am;

	//Controls which menu gets displayed.
	MenuState currentMenu;


	//Menus.
	void fileSelectMenu();
	void loginOrCreateMenu();
	void createAccountMenu();
	void loginMenu();
	void loginSuccessMenu();
	void loginFailMenu();
	void createAccountSuccessMenu();


	//Read user input function w/ input validation.
	int getMenuSelectionInput(int numOptions);
	

public:

	Menu();
	
	//Displays current menu.
	void displayMenu();

	//Changes MenuState based on selection.
	int optionSelect();
	
};

