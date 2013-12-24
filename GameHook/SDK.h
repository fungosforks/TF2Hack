#ifndef SDK_H
#define SDK_H

// include windows header
#include <Windows.h>
#include "VMT.h"
#include "Entity.h"
#include "SDK\public\materialsystem\imaterialsystem.h"
#include "SDK\public\Color.h"
#include "SDK\public\mathlib\vector.h"
#include "SDK\public\client_class.h"
#include "SDK\public\cdll_int.h"
#include "SDK\public\icliententity.h"
#include "SDK\public\icliententitylist.h"
#include "SDK\public\tier1\convar.h"
#include "SDK\public\icvar.h"
#include "SDK\public\ienginevgui.h"
#include "SDK\public\vgui\ISurface.h"

extern IBaseClientDLL		*gClient;
extern IVEngineClient		*gEngine;
extern vgui::ISurface		*gSurface;
extern IEngineVGui			*gVgui;
extern ICvar				*gCvar;
extern IClientEntityList	*gEntityList;

#endif // SDK_H
