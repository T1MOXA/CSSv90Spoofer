#include "precompiled.h"

void Msg(const char *fmt, ...)
{
	char buf[2048];
	va_list args;
	va_start(args, fmt);
	vsprintf_s(buf, fmt, args);
	va_end(args);

	static void(__cdecl *pfnMsg)(const char *fmt, ...) =
		decltype(pfnMsg)(GetProcAddress(GetModuleHandleA("tier0.dll"), "Msg"));

	if (pfnMsg)
		pfnMsg(buf);
}