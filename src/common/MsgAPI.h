#pragma once

static void RaiseError(const char *fmt, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buf, fmt, args);
	va_end(args);

	MessageBoxA(HWND_DESKTOP, buf, "Fatal Error", MB_SYSTEMMODAL | MB_ICONWARNING);
	ExitProcess(EXIT_SUCCESS);
}

#ifdef DEBUG
#define Error(fmt, ...) \
	RaiseError(__FUNCTION__ ": " fmt, __VA_ARGS__)
#else
#define Error(...) \
	ExitProcess(EXIT_FAILURE)
#endif