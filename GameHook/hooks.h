#ifndef HOOKS_H
#define HOOKS_H

#include "SDK.h"
#include "VMT.h"

void __stdcall PaintTraverse(vgui::VPANEL panel);

extern CVMTHook paintTraverse;

DWORD *hookVFunc(DWORD *vtable, int index, DWORD *newFunction);

#endif