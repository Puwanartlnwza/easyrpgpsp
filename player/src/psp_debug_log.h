// Temporary diagnostic helper: appends a line to psp_debug_log.txt next to
// the EBOOT so we can see exactly how far startup got before a crash,
// without touching the display (pspDebugScreenPrintf conflicts with SDL's
// own video init). Safe no-op on non-PSP builds. Remove once the startup
// crash is tracked down.
#ifndef PSP_DEBUG_LOG_H
#define PSP_DEBUG_LOG_H

#ifdef PSP
#include <cstdio>

inline void PspDebugLog(const char* msg) {
	FILE* f = fopen("psp_debug_log.txt", "a");
	if (f) {
		fprintf(f, "%s\n", msg);
		fclose(f); // close immediately so the line is flushed to disk
		           // even if the very next instruction crashes
	}
}
#else
inline void PspDebugLog(const char* /*msg*/) {}
#endif

#endif
