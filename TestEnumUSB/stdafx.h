// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <assert.h>

#define	CHK_STS	if(wSts<0) {assert(0);goto Exit;}
#define	CHK_ASSERT(x)	if(!(x)) {wSts=-1; assert(0);goto Exit;}

// TODO: reference additional headers your program requires here
