#pragma once
#include <string>
#include <functional>

#include "libs/stb_sprintf.h"

#define CALL_EAX(addr) __asm mov eax, addr __asm call eax
#define JMP_EAX(addr)  __asm mov eax, addr __asm jmp eax
#define JMP_EDX(addr)  __asm mov edx, addr __asm jmp edx
#define __HOOK __declspec(naked) void

class Script;

void DumpClass(void * theClassPtr, UInt32 nIntsToDump = 512);
const char * GetObjectClassName(void * obj);
const std::string & GetFalloutDirectory(void);
std::string GetNVSEConfigOption(const char * section, const char * key);
bool GetNVSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut);

// this has been tested to work for non-varargs functions
// varargs functions end up with 'this' passed as the last parameter (ie. probably broken)
// do NOT use with classes that have multiple inheritance

// if many member functions are to be declared, use MEMBER_FN_PREFIX to create a type with a known name
// so it doesn't need to be restated throughout the member list

// all of the weirdness with the _GetType function is because you can't declare a static const pointer
// inside the class definition. inlining automatically makes the function call go away since it's a const

#define MEMBER_FN_PREFIX(className)	\
	typedef className _MEMBER_FN_BASE_TYPE

#define DEFINE_MEMBER_FN_LONG(className, functionName, retnType, address, ...)		\
	typedef retnType (className::* _##functionName##_type)(__VA_ARGS__);			\
																					\
	inline _##functionName##_type * _##functionName##_GetPtr(void)					\
	{																				\
		static const UInt32 _address = address;										\
		return (_##functionName##_type *)&_address;									\
	}

#define DEFINE_MEMBER_FN(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)

#define CALL_MEMBER_FN(obj, fn)	\
	((*(obj)).*(*((obj)->_##fn##_GetPtr())))


// ConsolePrint() limited to 512 chars; use this to print longer strings to console
void Console_Print_Long(const std::string& str);

// Macro for debug output to console at runtime
#if RUNTIME
#ifdef _DEBUG
#define DEBUG_PRINT(x, ...) { Console_Print((x), __VA_ARGS__); }
#define DEBUG_MESSAGE(x, ...) { _MESSAGE((x), __VA_ARGS__); }
#else
#define DEBUG_PRINT(x, ...) { }
#define DEBUG_MESSAGE(x, ...) { }
#endif	//_DEBUG
#else
#define DEBUG_PRINT(x, ...) { }
#define DEBUG_MESSAGE(x, ...) { }
// This is so we don't have to handle size change with EditorData :)
#undef STATIC_ASSERT
#define STATIC_ASSERT(a)
#endif	// RUNTIME

#define ASSERT_SIZE(aClass, size) STATIC_ASSERT((sizeof(aClass) == size))
#define SIZEOF_ARRAY(arrayName, elementType) (sizeof(arrayName) / sizeof(elementType))

class TESForm;

class FormMatcher
{
public:
	virtual bool Matches(TESForm* pForm) const = 0;
};

namespace MersenneTwister
{

	/* initializes mt[N] with a seed */
	void init_genrand(unsigned long s);

	/* initialize by an array with array-length */
	void init_by_array(unsigned long init_key[], int key_length);

	/* generates a random number on [0,0xffffffff]-interval */
	unsigned long genrand_int32(void);

	/* generates a random number on [0,0x7fffffff]-interval */
	long genrand_int31(void);

	/* generates a random number on [0,1]-real-interval */
	double genrand_real1(void);

	/* generates a random number on [0,1)-real-interval */
	double genrand_real2(void);

	/* generates a random number on (0,1)-real-interval */
	double genrand_real3(void);

	/* generates a random number on [0,1) with 53-bit resolution*/
	double genrand_res53(void);

};

// alternative to strtok; doesn't modify src string, supports forward/backward iteration
class Tokenizer
{
public:
	Tokenizer(const char* src, const char* delims);
	~Tokenizer();

	// these return the offset of token in src, or -1 if no token
	UInt32 NextToken(std::string& outStr);
	UInt32 PrevToken(std::string& outStr);

private:
	std::string m_delims;
	size_t		m_offset;
	std::string m_data;
};

#if RUNTIME

const char GetSeparatorChar(Script * script);
const char * GetSeparatorChars(Script * script);

#endif

const char * GetDXDescription(UInt32 keycode);

bool ci_equal(char ch1, char ch2);
bool ci_less(const char* lh, const char* rh);
void MakeUpper(std::string& str);
void MakeUpper(char* str);
void MakeLower(std::string& str);

// this copies the string onto the FormHeap - used to work around alloc/dealloc mismatch when passing
// data between nvse and plugins
char* CopyCString(const char* src);

// Generic error/warning output
// provides a common way to output errors and warnings
class ErrOutput
{
	typedef void (* _ShowError)(const char* msg);
	typedef bool (* _ShowWarning)(const char* msg);		// returns true if user requests to disable warning

	_ShowError		ShowError;
	_ShowWarning	ShowWarning;
public:
	ErrOutput(_ShowError errorFunc, _ShowWarning warningFunc);

	struct Message
	{
		const char*		fmt;
		bool			bCanDisable;
		bool			bDisabled;
	};

	void Show(Message* msg, ...);
	void Show(const char* msg, ...);
	void vShow(Message* msg, va_list args);
	void vShow(const char* msg, va_list args);
};

size_t GetCurrentMemoryUsage();

class DirectoryIterator
{
	HANDLE				handle;
	WIN32_FIND_DATA		fndData;

public:
	DirectoryIterator(const char* path) : handle(FindFirstFile(path, &fndData)) {}
	~DirectoryIterator() { Close(); }

	bool IsFile() const { return !(fndData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY); }
	bool IsFolder() const
	{
		if (IsFile())
			return false;
		if (fndData.cFileName[0] != '.')
			return true;
		if (fndData.cFileName[1] != '.')
			return fndData.cFileName[1] != 0;
		return fndData.cFileName[2] != 0;
	}
	const char* Get() const { return fndData.cFileName; }
	void Close()
	{
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose(handle);
			handle = INVALID_HANDLE_VALUE;
		}
	}

	explicit operator bool() const { return handle != INVALID_HANDLE_VALUE; }
	void operator++() { if (!FindNextFile(handle, &fndData)) Close(); }
};

// thread-safe template versions of ThisCall()

template <typename T_Ret = UInt32, typename ...Args>
__forceinline T_Ret ThisCall(UInt32 _addr, const void *_this, Args ...args)
{
	return ((T_Ret (__thiscall *)(const void*, Args...))_addr)(_this, std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret StdCall(UInt32 _addr, Args ...args)
{
	return ((T_Ret(__stdcall*)(Args...))_addr)(std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret CdeclCall(UInt32 _addr, Args ...args)
{
	return ((T_Ret(__cdecl*)(Args...))_addr)(std::forward<Args>(args)...);
}

char* GameHeapStrdup(const char* src);

BOOL CopyTextToClipboard(const char* text);

bool IsOnlyAlphaNumeric(const char* str);
char* trim(char* str);

void SelectAllItemsInListView(HWND listView);
void* GetNthListItem(HWND hWnd, int n);
TESForm* GetNthListForm(HWND hWnd, int n);
void SetDeferListUpdate(HWND hWnd, bool bDefer = true);
bool IsDialog(HWND hWnd);

void ForEachFileInPath(const char* path, const char* typeMatch, std::function<void(const char*)> callback);