// BSD Zero Clause License
//
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Begin copied from AURA SDK ver 02.03
// So that we don't need the full SDK to compile
typedef void* GPULightControl;

typedef DWORD(WINAPI* EnumerateGPUFunc)(GPULightControl handles[], DWORD size);
typedef DWORD(WINAPI* GetGPULedCountFunc)(GPULightControl handle);
typedef DWORD(WINAPI* SetGPUColorFunc) (GPULightControl handle, BYTE* color, DWORD size);
// End copied from AURA SDK ver 02.03

static EnumerateGPUFunc EnumerateGPU;
static GetGPULedCountFunc GetGPULedCount;
static SetGPUColorFunc SetGPUColor;

static const wchar_t kMsgReadme[] = L"GPU AURA Killer 1.00\n";
static const wchar_t kMsgAllDone[] = L"GPU AURA all turned off.\n";
static const wchar_t kMsgPartiallyDone[] = L"GPU AURA partially turned off.\n";

enum {
	kCbColor = 3,
};

static inline void
show(const wchar_t* text) {
	WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), text, lstrlenW(text), (DWORD*){0}, NULL);
}

int
wmain(void) {
	show(kMsgReadme);
	
	HMODULE lib = LoadLibraryW(L"AURA_SDK.dll");
	if (!lib) ExitProcess(1);
	
	EnumerateGPU = (EnumerateGPUFunc)GetProcAddress(lib, "EnumerateGPU");
	GetGPULedCount = (GetGPULedCountFunc)GetProcAddress(lib, "GetGPULedCount");
	SetGPUColor = (SetGPUColorFunc)GetProcAddress(lib, "SetGPUColor");
	if (!EnumerateGPU || !GetGPULedCount || !SetGPUColor) ExitProcess(2);

	DWORD count = EnumerateGPU(NULL, 0);
	if (!count) ExitProcess(3);

	GPULightControl* handles = HeapAlloc(GetProcessHeap(), 0, sizeof(GPULightControl) * count);
	if (!handles) ExitProcess(4);
	EnumerateGPU(handles, count);
	
	int ret = 0;
	for (DWORD i = 0; i < count; ++i) {
		DWORD ledCount = GetGPULedCount(handles[i]);
		DWORD size = ledCount * kCbColor;
		BYTE* color = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
		if (!color) {
			ret = 5;
			continue;
		}

		SetGPUColor(handles[i], color, size);
		HeapFree(GetProcessHeap(), 0, color);
	}
	
	if (ret) show(kMsgPartiallyDone);
	else show(kMsgAllDone);

	ExitProcess(ret);
}
