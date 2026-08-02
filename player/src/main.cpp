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

// NOTE: no PSP_MODULE_INFO() here -- this build links against SDL's own
// PSP main stub (libSDLmain.a, via the `-Dmain=SDL_main` trick in the
// Makefile), which already provides one. Declaring a second one here
// caused a "multiple definition of `module_info`" link error.
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
// NOTE: older pspsdk had a PSP_HEAP_SIZE_MAX() macro here, but current
// pspdev/pspsdk removed it as part of a heap-allocation rework -- the
// runtime now uses all available RAM for the heap by default (minus a
// small reserve), so no explicit declaration is needed anymore.

// Without an exit callback, pressing HOME on real hardware doesn't return
// to the XMB -- the game just hangs and the console needs a battery pull.
// This is the standard PSP homebrew boilerplate to handle that cleanly.
int exit_callback(int /*arg1*/, int /*arg2*/, void* /*common*/) {
	sceKernelExitGame();
	return 0;
}

int callback_thread(SceSize /*args*/, void* /*argp*/) {
	int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int setup_callbacks() {
	int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, NULL);
	if (thid >= 0) {
		sceKernelStartThread(thid, 0, NULL);
	}
	return thid;
}

extern "C"
#endif
int main(int argc, char* argv[]) {
	PspDebugLog("[1] entering main()");

#ifdef PSP
	setup_callbacks();
	PspDebugLog("[2] callbacks set up");

	// Overclock CPU/bus (safe, widely used values for PSP homebrew) to
	// reduce slowdown on bigger maps / lots of events / parallax scrolling.
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
