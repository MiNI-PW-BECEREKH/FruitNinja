#ifndef BEJEWELED_H
#define BEJEWELED_H
#include <windows.h>
#include <string>
#include <map>

#include "GemWindow.h"

#define MAX_GEM_COUNT_Y 12
#define MAX_GEM_COUNT_X 16

//a singleton static class holds my game objects
class App
{
private:
	App()
	{

	}




public:
	//any game data
	//is stored below
	BOOL Initialized;
	HCURSOR hCursorSword;


	GemWindow Gems[MAX_GEM_COUNT_X][MAX_GEM_COUNT_Y];

	/**
	 * Singletons should not be clone able.
	 */
	App(App& other) = delete;
	/**
	 * Singletons should not be assignable.
	 */
	void operator=(const App&) = delete;

	static App& GetInstance()
	{
		static App app;

		return app;
	}


};
#endif
