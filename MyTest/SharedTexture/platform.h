// Copyright (c) 2018 Daktronics. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef WINVER
#define WINVER 0x0601 // win 7 or later
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0601
#endif


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif



#include <windows.h>
#include <objbase.h>
#include <Shellapi.h>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <assert.h>

