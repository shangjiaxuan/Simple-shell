#include "Names.h"

#ifdef _WIN32

#ifdef _UNICODE
const std::wstring Exit{ L"exit" };
const std::wstring Cd{ L"cd" };
const std::wstring Calculator{ L"calculator" };
const std::wstring Man{ L"man" };
const std::wstring Exec{ L".exe" };
const std::wstring SwapEnc{ L"SwapEnc" };

const wchar_t* calcDLL{ L"Calculator.dll" };
const wchar_t* encDLL{ L"SwapEnc.dll" };

#endif

#ifdef _MBCS
const std::string Exit{ "exit" };
const std::string Cd{ "cd" };
const std::string Calculator{ "calculator" };
const std::string Man{ "man" };
const std::string Exec{ ".exe" };
const std::string SwapEnc{ "SwapEnc" };

const char* calcDLL{ "Calculator.dll" };
const char* encDLL{ "SwapEnc.dll" };

#endif

#endif