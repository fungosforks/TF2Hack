#include "SDK.h"
#include "hooks.h"

CVMTHook paintTraverse;

DWORD *hookVFunc(DWORD *vtable, int index, DWORD *newFunction)
{
	DWORD dwOldProt, *oldFunc;
	VirtualProtect(&vtable[index], 4, PAGE_EXECUTE_READWRITE, &dwOldProt);
	oldFunc = (DWORD*)vtable[index];
	vtable[index] = (DWORD)newFunction;
	VirtualProtect(&vtable[index], 4, dwOldProt, &dwOldProt);
	return oldFunc;
}

void RunFrame()
{
	// some example ESP code
	if (true)
	{
		int iEntCount = gEntityList->GetHighestEntityIndex();

		void *localPlayer = gEntityList->GetClientEntity(gEngine->GetLocalPlayer());

		for (int i = 0; i < iEntCount; ++i)
		{
			void *pEnt = (void*)gEntityList->GetClientEntity(i);

			if (!pEnt)
				continue;

			if (i == gEngine->GetLocalPlayer())
				continue;

			if (GetNetVar<int>(pEnt, 161) != LIFE_ALIVE)
				continue;

			player_info_t pi;
			if (!gEngine->GetPlayerInfo(i, &pi))
				continue;

			Vector vScreen = gEngine->WorldToViewMatrix() * GetNetVar<Vector>(pEnt, 864);

			int y = (int)vScreen.y;

			if (GetNetVar<int>(pEnt, 172) != GetNetVar<int>(localPlayer, 172)) {
				gSurface->DrawSetColor(Color(255, 0, 0));
				gSurface->DrawFilledRect(vScreen.x - 10, vScreen.y - 10, vScreen.x + 10, vScreen.y + 10);
			}
			else {
				gSurface->DrawSetColor(Color(0, 255, 0));
				gSurface->DrawFilledRect(vScreen.x - 10, vScreen.y - 10, vScreen.x + 10, vScreen.y + 10);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Hook_PaintTraverse
//! Hooked PaintTraverse method. All drawing is done from here
//! 
//! \param vguiPanel - 
//! \param forceRepaint - 
//! \param allowForce - 
//! \return void __stdcall - 
////////////////////////////////////////////////////////////////////////////////
void __stdcall PaintTraverse(vgui::VPANEL panel)
{

	// only render on the in-game panel
	/*
	if (panel == gVgui->GetPanel(PANEL_INGAMESCREENS))
	{
		//RunFrame();
	}
	*/

	printf("Hooked! \n");

	if (gEngine->IsInGame()) {
		RunFrame();
	}

	/*
	else {
		gSurface->DrawFilledRect(0, 0, 100, 100);
	}
	*/

	paintTraverse.UnHook();
	gSurface->PaintTraverse(panel);
	paintTraverse.ReHook();
}