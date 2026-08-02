/////////////////////////////////////////////////////////////////////////////
// This file is part of EasyRPG Player.
//
// EasyRPG Player is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// EasyRPG Player is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "player.h"
#include "graphics.h"
#include "input.h"
#include "audio.h"
#include <cstdlib>
#include "psp_debug_log.h"

#ifdef PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>

// NOTE: no PSP_MODULE_INFO(), no exit-callback thread, no
// PSP_MAIN_THREAD_ATTR() here. This build links against SDL's own PSP main
// stub (libSDLmain.a's SDL_psp_main.c, pulled in via the `-Dmain=SDL_main`
// trick in the Makefile), which ALREADY sets all of that up on its own
// (sdl_psp_setup_callbacks() / sdl_psp_exit_callback()) before calling our
// SDL_main(). Declaring our own copies on top of that caused a
// "multiple definition of module_info" link error, and very likely also
// caused a startup crash from creating duplicate/conflicting kernel
// objects (thread + exit callback) on top of SDL's own.

extern "C"
#endif
int main(int argc, char* argv[]) {
	PspDebugLog("[1] entering main()");

#ifdef PSP
	// Overclock CPU/bus (safe, widely used values for PSP homebrew) to
	// reduce slowdown on bigger maps / lots of events / parallax scrolling.
	// SDL's stub does NOT do this for us, so it's the one thing left here.
	scePowerSetClockFrequency(333, 333, 166);
	PspDebugLog("[3] clock set");
#endif

	Player::Init(argc, argv);
	PspDebugLog("[4] Player::Init done");

	Graphics::Init();
	PspDebugLog("[5] Graphics::Init done");

	Input::Init();
	PspDebugLog("[6] Input::Init done");

	Audio::Init();
	PspDebugLog("[7] Audio::Init done -- entering Player::Run()");

	Player::Run();
	
	return EXIT_SUCCESS;
}
