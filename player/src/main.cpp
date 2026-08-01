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

#ifdef PSP
#include <pspkernel.h>
#include <pspdebug.h>
#include <psppower.h>

PSP_MODULE_INFO("EasyRPG", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
// The default heap (build.mak default) is small and was never raised, so
// bigger maps/chipsets/games could run out of memory and abort on real
// hardware even though they run fine in an emulator with more lenient
// memory handling. Ask for as much of the available user RAM as possible.
PSP_HEAP_SIZE_MAX();

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
#ifdef PSP
	setup_callbacks();
	// Overclock CPU/bus (safe, widely used values for PSP homebrew) to
	// reduce slowdown on bigger maps / lots of events / parallax scrolling.
	scePowerSetClockFrequency(333, 333, 166);
#endif

	Player::Init(argc, argv);
	Graphics::Init();
	Input::Init();
	Audio::Init();

	Player::Run();
	
	return EXIT_SUCCESS;
}
