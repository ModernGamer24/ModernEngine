
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include "ModernApp.h"

int main() 
{
    //Create new instance of app
    MEngine::GModernApp App{};

	try
	{
		App.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';

		return EXIT_FAILURE;
	}
	
	return EXIT_SUCCESS;
}