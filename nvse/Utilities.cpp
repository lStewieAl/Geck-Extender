#include <string>
#include "Utilities.h"
#include "SafeWrite.h"
#include <algorithm>
#include <Psapi.h>
#include "GameTypes.h"
#include <commctrl.h>
#include <functional>

void DumpClass(void * theClassPtr, UInt32 nIntsToDump)
{
	_MESSAGE("DumpClass:");
	UInt32* basePtr = (UInt32*)theClassPtr;

	gLog.Indent();

	if (!theClassPtr) return;
	for (UInt32 ix = 0; ix < nIntsToDump; ix++ ) {
		UInt32* curPtr = basePtr+ix;
		const char* curPtrName = NULL;
		UInt32 otherPtr = 0;
		float otherFloat = 0.0;
		const char* otherPtrName = NULL;
		if (curPtr) {
			curPtrName = GetObjectClassName((void*)curPtr);

			__try
			{
				otherPtr = *curPtr;
				otherFloat = *(float*)(curPtr);
			}
			__except(EXCEPTION_EXECUTE_HANDLER)
			{
				//
			}

			if (otherPtr) {
				otherPtrName = GetObjectClassName((void*)otherPtr);
			}
		}

		_MESSAGE("%3d +%03X ptr: 0x%08X: %32s *ptr: 0x%08x | %f: %32s", ix, ix*4, curPtr, curPtrName, otherPtr, otherFloat, otherPtrName);
	}

	gLog.Outdent();
}

#pragma warning (push)
#pragma warning (disable : 4200)
struct RTTIType
{
	void	* typeInfo;
	UInt32	pad;
	char	name[0];
};

struct RTTILocator
{
	UInt32		sig, offset, cdOffset;
	RTTIType	* type;
};
#pragma warning (pop)

// use the RTTI information to return an object's class name
const char * GetObjectClassName(void * objBase)
{
	const char	* result = "<no rtti>";

	__try
	{
		void		** obj = (void **)objBase;
		RTTILocator	** vtbl = (RTTILocator **)obj[0];
		RTTILocator	* rtti = vtbl[-1];
		RTTIType	* type = rtti->type;

		// starts with ,?
		if((type->name[0] == '.') && (type->name[1] == '?'))
		{
			// is at most 100 chars long
			for(UInt32 i = 0; i < 100; i++)
			{
				if(type->name[i] == 0)
				{
					// remove the .?AV
					result = type->name + 4;
					break;
				}
			}
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		// return the default
	}

	return result;
}

const std::string & GetFalloutDirectory(void)
{
	static std::string s_falloutDirectory;

	if(s_falloutDirectory.empty())
	{
		// can't determine how many bytes we'll need, hope it's not more than MAX_PATH
		char	falloutPathBuf[MAX_PATH];
		UInt32	falloutPathLength = GetModuleFileName(GetModuleHandle(NULL), falloutPathBuf, sizeof(falloutPathBuf));

		if(falloutPathLength && (falloutPathLength < sizeof(falloutPathBuf)))
		{
			std::string	falloutPath(falloutPathBuf, falloutPathLength);

			// truncate at last slash
			std::string::size_type	lastSlash = falloutPath.rfind('\\');
			if(lastSlash != std::string::npos)	// if we don't find a slash something is VERY WRONG
			{
				s_falloutDirectory = falloutPath.substr(0, lastSlash + 1);

				_DMESSAGE("fallout root = %s", s_falloutDirectory.c_str());
			}
			else
			{
				_WARNING("no slash in fallout path? (%s)", falloutPath.c_str());
			}
		}
		else
		{
			_WARNING("couldn't find fallout path (len = %d, err = %08X)", falloutPathLength, GetLastError());
		}
	}

	return s_falloutDirectory;
}

static const std::string & GetNVSEConfigPath(void)
{
	static std::string s_configPath;

	if(s_configPath.empty())
	{
		std::string	falloutPath = GetFalloutDirectory();
		if(!falloutPath.empty())
		{
			s_configPath = falloutPath + "Data\\NVSE\\nvse_config.ini";

			_MESSAGE("config path = %s", s_configPath.c_str());
		}
	}

	return s_configPath;
}

std::string GetNVSEConfigOption(const char * section, const char * key)
{
	std::string	result;

	const std::string & configPath = GetNVSEConfigPath();
	if(!configPath.empty())
	{
		char	resultBuf[256];
		resultBuf[0] = 0;

		UInt32	resultLen = GetPrivateProfileString(section, key, NULL, resultBuf, 255, configPath.c_str());

		result = resultBuf;
	}

	return result;
}

bool GetNVSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut)
{
	std::string	data = GetNVSEConfigOption(section, key);
	if(data.empty())
		return false;

	return (sscanf(data.c_str(), "%lu", dataOut) == 1);
}

namespace MersenneTwister
{

/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.

   Before using, initialize the state by using init_genrand(seed)  
   or init_by_array(init_key, key_length).

   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.                          

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     3. The names of its contributors may not be used to endorse or promote 
        products derived from this software without specific prior written 
        permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)
*/

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}

/* generates a random number on [0,0x7fffffff]-interval */
long genrand_int31(void)
{
    return (long)(genrand_int32()>>1);
}

/* generates a random number on [0,1]-real-interval */
double genrand_real1(void)
{
    return genrand_int32()*(1.0/4294967295.0); 
    /* divided by 2^32-1 */ 
}

/* generates a random number on [0,1)-real-interval */
double genrand_real2(void)
{
    return genrand_int32()*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on (0,1)-real-interval */
double genrand_real3(void)
{
    return (((double)genrand_int32()) + 0.5)*(1.0/4294967296.0); 
    /* divided by 2^32 */
}

/* generates a random number on [0,1) with 53-bit resolution*/
double genrand_res53(void) 
{ 
    unsigned long a=genrand_int32()>>5, b=genrand_int32()>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
/* These real versions are due to Isaku Wada, 2002/01/09 added */

#undef N
#undef M
#undef MATRIX_A
#undef UPPER_MASK
#undef LOWER_MASK

};

Tokenizer::Tokenizer(const char* src, const char* delims)
: m_offset(0), m_delims(delims), m_data(src)
{
	//
}

Tokenizer::~Tokenizer()
{
	//
}

UInt32 Tokenizer::NextToken(std::string& outStr)
{
	if (m_offset == m_data.length())
		return -1;

	size_t start = m_data.find_first_not_of(m_delims, m_offset);
	if (start != -1)
	{
		size_t end = m_data.find_first_of(m_delims, start);
		if (end == -1)
			end = m_data.length();

		m_offset = end;
		outStr = m_data.substr(start, end - start);
		return start;
	}

	return -1;
}

UInt32 Tokenizer::PrevToken(std::string& outStr)
{
	if (m_offset == 0)
		return -1;

	size_t searchStart = m_data.find_last_of(m_delims, m_offset - 1);
	if (searchStart == -1)
		return -1;

	size_t end = m_data.find_last_not_of(m_delims, searchStart);
	if (end == -1)
		return -1;

	size_t start = m_data.find_last_of(m_delims, end);	// okay if start == -1 here

	m_offset = end + 1;
	outStr = m_data.substr(start + 1, end - start);
	return start + 1;
}

struct ControlName
{
	UInt32		unk0;
	const char	* name;
	UInt32		unkC;
};

ControlName ** g_keyNames = (ControlName **)0x011D52F0;
ControlName ** g_mouseButtonNames = (ControlName **)0x011D5240;
ControlName ** g_joystickNames = (ControlName **)0x011D51B0;

const char * GetDXDescription(UInt32 keycode)
{
	const char * keyName = "<no key>";

	if(keycode <= 220)
	{
		if(g_keyNames[keycode])
			keyName = g_keyNames[keycode]->name;
	}
	else if(255 <= keycode && keycode <= 263)
	{
		if(keycode == 255)
			keycode = 256;
		if(g_mouseButtonNames[keycode - 256])
			keyName = g_mouseButtonNames[keycode - 256]->name;
	}
	else if (keycode == 264)		//OB doesn't provide names for wheel up/down
		keyName = "WheelUp";
	else if (keycode == 265)
		keyName = "WheelDown";

	return keyName;
}

bool ci_equal(char ch1, char ch2)
{
	return tolower((unsigned char)ch1) == tolower((unsigned char)ch2);
}

bool ci_less(const char* lh, const char* rh)
{
	ASSERT(lh && rh);
	while (*lh && *rh) {
		char l = toupper(*lh);
		char r = toupper(*rh);
		if (l < r) {
			return true;
		}
		else if (l > r) {
			return false;
		}
		lh++;
		rh++;
	}

	return toupper(*lh) < toupper(*rh);
}

void MakeUpper(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), toupper);
}

void MakeLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}

#pragma warning(push)
#pragma warning(disable: 4996)	// warning about std::transform()

void MakeUpper(char* str)
{
	if (str) {
		UInt32 len = strlen(str);
		std::transform(str, str + len, str, toupper);
	}
}

void MakeLower(char* str)
{
	if (str) {
		UInt32 len = strlen(str);
		std::transform(str, str + len, str, tolower);
	}
}

#pragma warning(pop)

// ErrOutput
ErrOutput::ErrOutput(_ShowError errorFunc, _ShowWarning warningFunc)
{
	ShowWarning = warningFunc;
	ShowError = errorFunc;
}

void ErrOutput::vShow(ErrOutput::Message* msg, va_list args)
{
	char msgText[0x400];
	vsprintf_s(msgText, sizeof(msgText), msg->fmt, args);
	if (msg->bCanDisable)
	{
		if (!msg->bDisabled)
			if (ShowWarning(msgText))
				msg->bDisabled = true;
	}
	else
		ShowError(msgText);
}

void ErrOutput::Show(ErrOutput::Message* msg, ...)
{
	va_list args;
	va_start(args, msg);

	vShow(msg, args);
}

void ErrOutput::Show(const char* msg, ...)
{
	va_list args;
	va_start(args, msg);

	vShow(msg, args);
}

void ErrOutput::vShow(const char* msg, va_list args)
{
	Message tempMsg;
	tempMsg.fmt = msg;
	tempMsg.bCanDisable = false;
	tempMsg.bDisabled = false;

	vShow(&tempMsg, args);
}

size_t GetCurrentMemoryUsage()
{
	PROCESS_MEMORY_COUNTERS procMem;
	GetProcessMemoryInfo(GetCurrentProcess(), &procMem, sizeof(procMem));
	return procMem.WorkingSetSize;
}


char* GameHeapStrdup(const char* src)
{
	if (!src) return nullptr;
	auto len = strlen(src);
	char* result = (char*)FormHeap_Allocate(len + 1);
	memcpy(result, src, len + 1);
	return result;
}

BOOL CopyTextToClipboard(const char* text) {
	// Open the clipboard
	if (!OpenClipboard(NULL)) {
		return FALSE;
	}

	// Empty the clipboard
	if (!EmptyClipboard()) {
		CloseClipboard();
		return FALSE;
	}

	// Allocate a global memory object for the text
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (strlen(text) + 1) * sizeof(char));
	if (hglbCopy == NULL) {
		CloseClipboard();
		return FALSE;
	}

	// Lock the handle and copy the text to the buffer
	char* lptstrCopy = (char*)GlobalLock(hglbCopy);
	memcpy(lptstrCopy, text, strlen(text) + 1);
	GlobalUnlock(hglbCopy);

	// Place the handle on the clipboard
	if (SetClipboardData(CF_TEXT, hglbCopy) == NULL) {
		// If SetClipboardData failed, we need to free the memory ourselves.
		GlobalFree(hglbCopy);
		CloseClipboard();
		return FALSE;
	}

	// Close the clipboard
	CloseClipboard();

	return TRUE;
}

void SelectAllItemsInListView(HWND listView)
{
	int itemCount = ListView_GetItemCount(listView);
	for (int i = 0; i < itemCount; i++)
	{
		ListView_SetItemState(listView, i, LVIS_SELECTED, LVIS_SELECTED);
	}
}

TESForm* GetNthListForm(HWND hWnd, int n)
{
	if (n == -1)
	{
		return 0;
	}

	LVITEMA itemA;
	memset(&itemA, 0, sizeof(itemA));
	itemA.iItem = n;
	itemA.mask = LVIS_CUT;
	SendMessageA(hWnd, LVM_GETITEMA, 0, (LPARAM)&itemA);
	return (TESForm*)itemA.lParam;
}

void SetDeferListUpdate(HWND hWnd, bool bDefer)
{
	if (bDefer)
	{
		SendMessage(hWnd, WM_SETREDRAW, FALSE, 0);
	}
	else
	{
		SendMessage(hWnd, WM_SETREDRAW, TRUE, 0);
		RedrawWindow(hWnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
}

bool IsDialog(HWND hWnd)
{
	return WC_DIALOG == MAKEINTATOM(GetClassLong(hWnd, GCW_ATOM));
}

void ForEachFileInPath(const char* path, const char* typeMatch, std::function<void(const char*)> callback)
{
	char jsonPath[MAX_PATH];
	GetModuleFileNameA(NULL, jsonPath, MAX_PATH);
	int pathLen = strlen(path);
	char* namePtr = strncpy((char*)(strrchr(jsonPath, '\\') + 1), path, pathLen) + pathLen; // keep pointer "namePtr" to the end of the string
	strcpy(namePtr, typeMatch);

	for (DirectoryIterator dirIter(jsonPath); dirIter; ++dirIter)
	{
		if (!dirIter.IsFile()) continue;
		strcpy(namePtr, dirIter.Get());
		callback(jsonPath);
	}
}