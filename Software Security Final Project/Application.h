#pragma once

#include "Menu.h"

//Forward declarations
void generateRandomAccounts(int numGenerate, int minFieldLength, int maxFieldLength, AccountManager::FileType fType); //task 2
void cracknhack(AccountManager::FileType fType); //task 3

class Application
{
public:
	enum class TaskState
	{
		TASK1, TASK2, TASK3, TESTING
	};

private:
	TaskState currentTask;
	Menu menuControl;

public:
	Application();

	void task1();
	void task2();
	void task3();
	void testingAndAnalysis();
	void run(const TaskState taskSelection);
};

