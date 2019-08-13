#include "Application.h"

Application::Application()
{
}

void Application::task1()
{
	while (currentTask == TaskState::TASK1)
	{
		menuControl.displayMenu();
	}
	
}

void Application::task2()
{
	//CONTROL VARIABLES.
	int numAccounts = 1000;
	int min = 3;
	int max = 8;

	std::cout << "Generating " << numAccounts << " random accounts in each file.\n" << "Min: " << min << "\nMax: " << max << std::endl;
	//Account generators.
	generateRandomAccounts(numAccounts, min, max, AccountManager::FileType::PLAINTEXT);
	generateRandomAccounts(numAccounts, min, max, AccountManager::FileType::HASHED);
	generateRandomAccounts(numAccounts, min, max, AccountManager::FileType::SALTED);

	//Finished.
	std::cout << "\nCOMPLETE";
}

void Application::task3()
{
	cracknhack(AccountManager::FileType::SALTED);
}

void Application::testingAndAnalysis()
{
}

void Application::run(const TaskState taskSelection)
{
	switch (taskSelection)
	{
	case TaskState::TASK1:
		currentTask = taskSelection;
		task1();
		break;

	case TaskState::TASK2:
		currentTask = taskSelection;
		task2();
		break;

	case TaskState::TASK3:
		currentTask = taskSelection;
		task3();
		break;

	case TaskState::TESTING:
		currentTask = taskSelection;
		testingAndAnalysis();
		break;
	}
}
