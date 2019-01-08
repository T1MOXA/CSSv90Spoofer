#pragma once

typedef void(*FnChangeCallback_t)(IConVar *var, const char *pOldValue, float flOldValue);

using TRegisterVariable = void(__fastcall *)(
	ConVar *that,
	int /*dummy*/,
	const char *name,
	const char *value,
	int flags,
	const char *desc,
	FnChangeCallback_t pfnOnChange);

ConVar *RegisterVariable(const char *name, const char *value, const char *desc = nullptr, FnChangeCallback_t pfnOnChange = nullptr);

void RegisterSpooferCVars();