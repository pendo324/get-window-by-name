#ifndef NATIVE_EXTENSION_GRAB_H
#define NATIVE_EXTENSION_GRAB_H

#pragma comment(lib, "psapi.lib")

#include <nan.h>
#include <node.h>

#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <string>

// Example top-level functions. These functions demonstrate how to return various js types.
// Implementations are in functions.cc

NAN_METHOD(getWindowText);

#endif
