#include <iostream>
#include <string>

#include <Windows.h>
#include <shlobj.h>
#include <tchar.h>


#define nstring std::wstring

using namespace std;

nstring get_LnkInfo(const std::wstring& LnkFile_Path) {
	if(FAILED(CoInitialize(NULL))) {
		cout << "get_Lnkpath(const nstring&): CoInitialize(NULL) failed!\n" << endl;
		return L"";
	}
	IShellLink* shell_link_instance = nullptr;
	if(FAILED(CoCreateInstance(CLSID_ShellLink,
							//create a single object
							NULL,
							//default
							CLSCTX_INPROC_SERVER,
							IID_IShellLink,
							(LPVOID*)&shell_link_instance))) {
		cout << "get_Lnkpath(const nstring&): CoCreateInstance failed!\n" << endl;
		return L"";
	}
	IPersistFile* file_interface;
	if(FAILED(shell_link_instance->QueryInterface(IID_IPersistFile, (void**)&file_interface))) {
		cout << "get_Lnkpath(const nstring&): shell_link_instance->QueryInterface failed!\n" << endl;
		return L"";
	}
	if(FAILED(file_interface->Load(LnkFile_Path.c_str(), STGM_READ))) {
		cout << "get_Lnkpath(const nstring&): file_interface->Load failed!\n" << endl;
		return L"";
	}
	if(FAILED(shell_link_instance->Resolve(NULL, SLR_UPDATE))) {
		cout << "get_Lnkpath(const nstring&): shell_link_instance->Resolve failed!\n" << endl;
		return L"";
	}
	TCHAR temp[MAX_PATH];
	if(FAILED(shell_link_instance->GetPath(temp, MAX_PATH, NULL, NULL))) {
		cout << "get_Lnkpath(const nstring&): shell_link_instance->GetPath failed!\n" << endl;
		return L"";
	}
	nstring path=temp;
	file_interface->Release();
	shell_link_instance->Release();
	CoUninitialize();
	return path;
}


int main() {
	std::wcout << get_LnkInfo(L"");
	system("pause");
}

