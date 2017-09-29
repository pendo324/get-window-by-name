#include "functions.h"

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

struct handle_data {
	unsigned long process_id;
	HWND best_handle;
};

typedef struct {
	std::string processName;
	int processId;
	std::string processTitle;
} processInfo;

BOOL is_main_window(HWND handle) {
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam) {
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle)) {
		return TRUE;
	}
	data.best_handle = handle;
	return FALSE;
}

HWND find_main_window(unsigned long process_id) {
	handle_data data;
	data.process_id = process_id;
	data.best_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.best_handle;
}

processInfo getProcessInfo(DWORD processID, std::string processName) {
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

	// Get a handle to the process.

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	// Get the process name.

	if (NULL != hProcess) {
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded)) {
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}
	}

	// Print the process name and identifier.
	std::string nameString(szProcessName);

	if (nameString.find(processName) == 0) {
		int maxTitleLength = 255;
		LPSTR windowTitle = new char[maxTitleLength];
		GetWindowText(find_main_window(processID), windowTitle, maxTitleLength);

		std::string titleString(windowTitle);

		/* DEBUG
		_tprintf(TEXT("%s  (PID: %u) (Title: %s)\n"), szProcessName, processID, windowTitle); */

		processInfo pi;
		pi.processId = (int)processID;
		pi.processTitle = titleString;
		pi.processName = nameString;

		CloseHandle(hProcess);
		return pi;
	}

	// Release the handle to the process.
	CloseHandle(hProcess);

	// Set pid to -1 if the process doewns
	processInfo pi;
	pi.processId = -1;
	return pi;
}

NAN_METHOD(getWindowText) {
	v8::Local<v8::Value> input = v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), "");
	if (info[0]->IsString()) {
		input = info[0];
	}

	//auto input = info[0];

	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
		return;
	}

	// Calculate how many process identifiers were returned.

	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.

	v8::Local<v8::Array> arr = Nan::New<v8::Array>();

	v8::String::Utf8Value inputAsUnicode(input->ToString());

	auto inputAsString = std::string(*inputAsUnicode);

	auto nextElem = 0;
	for (i = 0; i < cProcesses; i++) {
		if (aProcesses[i] != 0) {
			auto pi = getProcessInfo(aProcesses[i], inputAsString);
			if (pi.processId != -1) {
				v8::Local<v8::Object> obj = Nan::New<v8::Object>();
				v8::Isolate* isolate = v8::Isolate::GetCurrent();

				auto processNameProp = v8::String::NewFromUtf8(isolate, "processName");
				auto processName = v8::String::NewFromUtf8(isolate, pi.processName.c_str());

				auto processTitleProp = v8::String::NewFromUtf8(isolate, "processTitle");
				auto processTitle = v8::String::NewFromUtf8(isolate, pi.processTitle.c_str());

				auto processIdProp = v8::String::NewFromUtf8(isolate, "processId");
				auto processId = v8::Integer::New(isolate, pi.processId);

				obj->Set(processNameProp, processName);
				obj->Set(processTitleProp, processTitle);
				obj->Set(processIdProp, processId);

				Nan::Set(arr, nextElem, obj);
				nextElem = nextElem + 1;
			}
		}
	}

	info.GetReturnValue().Set(arr);
}
