#include "precompiled.h"

ConVar *RegisterVariable(const char *name, const char *value, const char *desc, FnChangeCallback_t pfnOnChange)
{
	static auto hEngine = GetModuleHandleA("engine.dll");
	TRegisterVariable p = nullptr; *(void **)&p = Transpose(hEngine, 0x00249F90);

	auto cvar = new ConVar();
	p(cvar, 0, name, value, 0, desc, pfnOnChange);

	return cvar;
}

void SidValue_OnChange(IConVar *var, const char *pOldValue, float flOldValue)
{
	auto pvar = ((ConVar *)var);
	gnSteamIDValue = pvar->m_nValue;
}

void SidShift_OnChange(IConVar *var, const char *pOldValue, float flOldValue)
{
	auto pvar = ((ConVar *)var);
	gbSteamIDShift = pvar->m_nValue != 0;
}

void RegisterSpooferCVars()
{
	RegisterVariable(
		"sid_value",
		"0",
		"Current Steam ID value. Use 0 to generate random value every time client connects to server.",
		SidValue_OnChange
	);

	RegisterVariable(
		"sid_shift",
		"1",
		"Sometimes server can interpret your ticket differently and SteamID will be also different. If that happened, you can try to set this variable to 0 to change ticket generation algorithm.",
		SidShift_OnChange
	);
}