#include "Game Components\TEAL\Game.h"

#ifdef _WIN32 // Fix ATI bug which crashes on exit in debug mode: http://en.sfml-dev.org/forums/index.php?topic=3976.0
	#ifdef _DEBUG
		#include <Windows.h>
	#endif
#endif

int main()
{
	Game::Run();

	#ifdef _WIN32	// Fix ATI bug which crashes on exit in debug mode: http://en.sfml-dev.org/forums/index.php?topic=3976.0
		#ifdef _DEBUG
			TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
		#endif
	#endif

    return 0;
}
