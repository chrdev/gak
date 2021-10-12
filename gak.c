// BSD Zero Clause License
//
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


typedef void* GPULightControl;

DWORD WINAPI EnumerateGPU(GPULightControl handles[], DWORD size);
DWORD WINAPI GetGPULedCount(GPULightControl handle);
DWORD WINAPI SetGPUColor(GPULightControl handle, BYTE* color, DWORD size);


static const wchar_t kMsgReadme[] = L"GPU AURA Killer 1.0.1\n";
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

	DWORD count = EnumerateGPU(NULL, 0);
	if (!count) {
		show(L"No compatible GPU.\n");
		ExitProcess(3);
	}

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
