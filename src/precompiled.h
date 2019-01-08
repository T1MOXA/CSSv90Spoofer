#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <time.h>
#include <stdlib.h>
#include <heapapi.h>

#include <stdio.h> // vsprintf
#include <stdarg.h> // va_start, va_end
#include <WinUser.h> // MessageBoxA

#include <exception>
#include <stdexcept>
#include <cstring>
#include <string>
#include <cassert>
#include <fstream>
#include <strstream>

#include "public/opcode_len_calc.h"

#include "common/MsgAPI.h"
#include "common/XMemory.h"

#include "public/Cryptography/CRijndael.h"
#include "public/Cryptography/DoubleBuffering.h"
#include "public/Cryptography/MessageDigest.h"
#include "public/Cryptography/SHA.h"

#include "public/Emulators/RevSpoofer.h"
#include "public/Emulators/RevEmu2013.h"

#include "public/SourceSDK/iconvar.h"
#include "public/SourceSDK/convar.h"

#include "Global.h"
#include "ConPrint.h"
#include "CVars.h"
#include "MemPatch.h"