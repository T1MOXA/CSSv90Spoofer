#include "precompiled.h"

FARPROC(WINAPI *pfnGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);

int gGacSpecialCallTime = 0;

int __cdecl gac_special(...)
{
	//
	// Stub for RevEmu2013 ticket generator function.
	//

	if (++gGacSpecialCallTime == 7)
	{
		//
		// A little hack here. I hate these kind of solutions, but I don't have
		// any other simple way to print my magnificent copyright in game console.
		//

		Msg("SteamID Changer for CSSv90 initialized.\n");
		Msg("Alexander B. aka 2010kohtep (c) 2019\n");
	}

	return 1;
}

FARPROC WINAPI hkGetProcAddressA(HMODULE hModule, LPCSTR lpProcName)
{
	char modName[512];
	GetModuleFileNameA(hModule, modName, sizeof(modName) - 1);
	auto p = strrchr(modName, '.');
	if (!p)
		return pfnGetProcAddress(hModule, lpProcName);

	if (_stricmp(p, ".dat"))
		return pfnGetProcAddress(hModule, lpProcName);

	if (lpProcName && !_stricmp(lpProcName, "gac_special"))
		return (FARPROC)gac_special;

	return pfnGetProcAddress(hModule, lpProcName);
}

void Hook_GetProcAddress()
{
	//
	// In this function we are going to disable default RevEmu steamclient.dll
	// ticket generator, which in 'gac_special' function. We can skip this, but
	// I prefer to leave it here, since RevEmu2013 crashes the game on some OSes
	// because of it caused exceptions.
	//

	if (pfnGetProcAddress)
	{
		//
		// Already hooked, exit.
		//

		return;
	}

	auto h = GetModuleHandleA("kernel32.dll");
	pfnGetProcAddress = HookExport(h, "GetProcAddress", hkGetProcAddressA);
}

void __stdcall hkCSteam3Client_InitiateConnection(void *data, int nMaxData, int &nOutSize, long nIP, short nPort, unsigned long long nGSSteamID, bool bSecure)
{
	long nSteamID = (gnSteamIDValue == 0) ? (__rdtsc() % INT_MAX) : (gnSteamIDValue);

	int revHash;
	nOutSize = GenerateRevEmu2013(Transpose(data, 8), (gbSteamIDShift != 0) ? (nSteamID >> 1) : (nSteamID), &revHash);
	revHash = revHash << 1;
	if (nOutSize == 0)
	{
		Msg("Could not change SteamID value.\n");
		return;
	}

	Msg("SteamID successfully changed to %d.\n", nSteamID);

	//
	// Modern Source Engine builds require that SteamID value was in
	// the beginning of ticket. 
	//

	auto pdata = (long *)data;
	pdata[0] = (nSteamID & 1) ? (revHash | 1) : (revHash); // SteamID, low part
	pdata[1] = 0x01100001;                                 // SteamID, high part
}

void Hook_CSteam3Client_InitiateConnection()
{
	HMODULE hEngine = NULL;

	while (!hEngine)
	{
		hEngine = GetModuleHandleA("engine.dll");
		Sleep(50);
	}

	auto jnz1 = Transpose(hEngine, 0x9EAD4);
	auto jnz2 = Transpose(hEngine, 0x9EB81);
	auto call = Transpose(hEngine, 0x9EB77);

	if (!Check<unsigned char>(jnz1, 0x75))
	{
		Error("Could not find first jump opcode.");
		return;
	}

	if (!Check<unsigned char>(jnz2, 0x75))
	{
		Error("Could not find second jump opcode.");
		return;
	}

	if (!Check<unsigned char>(call, 0xE8))
	{
		Error("Could not find call opcode.");
		return;
	}

	{
		//
		// Ignore SteamClient() null value, needs for emulator crash fix.
		//

		Write<unsigned char>(jnz1, 0xEB);
		Write<unsigned char>(jnz2, 0xEB);
	}

	{
		//
		// Insert SteamID changer.
		//

		InsertCall(call, hkCSteam3Client_InitiateConnection);
	}

	RegisterSpooferCVars();
}

DWORD CALLBACK SpooferEntry(LPVOID)
{
	Hook_GetProcAddress();
	Hook_CSteam3Client_InitiateConnection();

	return TRUE;
}