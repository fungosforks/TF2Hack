#include "SDK.h"
#include "hooks.h"
#include <iostream>
#include <fstream>

IBaseClientDLL		*gClient	= nullptr;
IVEngineClient		*gEngine	= nullptr;
vgui::ISurface		*gSurface	= nullptr;
IEngineVGui			*gVgui		= nullptr;
ICvar				*gCvar		= nullptr;
IClientEntityList	*gEntityList = nullptr;

void DumpClass(ClientClass *pClass, RecvTable *pTable, int tab)
{
	std::ofstream dumpFile;
	dumpFile.open("dump.txt", std::ios::app);

	RecvProp *pProp = NULL;

	if (pTable->GetNumProps() < 0)
		return;

	char pad[1024] = "";

	for (int j = 0; j < tab; j++)
		strcat(pad, "\t");

	dumpFile << pad << " - " << pTable->GetName() << std::endl;

	for (int i = 0; i < pTable->GetNumProps(); i++)
	{
		pProp = pTable->GetPropA(i);

		if (!pProp)
			continue;

		dumpFile << pad << pTable->GetName() << " - " << pProp->GetName() << " : " << pProp->GetOffset() << std::endl;

		if (pProp->GetDataTable())
			DumpClass(pClass, pProp->GetDataTable(), tab + 1);
		else
			continue;
	}

	dumpFile.close();
}

DWORD WINAPI MainThread(LPVOID lpParam)
{
	HMODULE hClient = NULL;
	// wait for the process to load its client.dll, which we'll use for fun.
	while (hClient == NULL)
	{
		hClient = GetModuleHandle("client.dll");
		Sleep(100);
	}

	CreateInterfaceFn clientCreateInterface = (CreateInterfaceFn)GetProcAddress(hClient, "CreateInterface");
	gClient = (IBaseClientDLL*)(clientCreateInterface(CLIENT_DLL_INTERFACE_VERSION, 0));
	printf("gClient: 0x%x \n", gClient);

	CreateInterfaceFn vstdlibCreateInterface = (CreateInterfaceFn)(GetProcAddress(LoadLibraryA("vstdlib.dll"), "CreateInterface"));
	gCvar = (ICvar*)(vstdlibCreateInterface(CVAR_INTERFACE_VERSION, 0));
	printf("gCvar: 0x%x \n", gCvar);

	CreateInterfaceFn engineCreateInterface = (CreateInterfaceFn)(GetProcAddress(GetModuleHandle("engine.dll"), "CreateInterface"));
	gEngine = (IVEngineClient*)(engineCreateInterface(VENGINE_CLIENT_INTERFACE_VERSION, 0));
	gEntityList = (IClientEntityList*)(clientCreateInterface(VCLIENTENTITYLIST_INTERFACE_VERSION, 0));
	printf("gEngine: 0x%x \n", gEngine);
	printf("gEntityList: 0x%x \n", gEntityList);

	CreateInterfaceFn vguiCreateInterface = (CreateInterfaceFn)(GetProcAddress(GetModuleHandle("vgui2.dll"), "CreateInterface"));
	gSurface = (vgui::ISurface*)(vguiCreateInterface(VGUI_SURFACE_INTERFACE_VERSION, 0));
	gVgui = (IEngineVGui*)(clientCreateInterface(VENGINE_VGUI_VERSION, 0));
	printf("gSurface: 0x%x \n", gSurface);
	printf("gVGUI: 0x%x \n", gVgui);

	// set cheats without approval from server!
	ConVar *cheats = gCvar->FindVar("sv_cheats");
	ConVar *speed = gCvar->FindVar("host_timescale");

	while (true) {
		if (cheats->GetInt() == 0) {
			cheats->SetValue(1);
		}

		if (GetAsyncKeyState(VK_MBUTTON)) {
			printf("Speed hack!");
			speed->SetValue(10);
		}
		else {
			speed->SetValue(1);
		}

		Sleep(500);
	}

	/*
	ClientClass* classes = gClient->GetAllClasses();
	while (classes) {
		DumpClass(classes, classes->m_pRecvTable, 0);
		classes = classes->m_pNext;
	}
	*/

	gSurface->DrawFilledRect(0, 0, 100, 100);
	
	paintTraverse.Hook((DWORD)PaintTraverse, (PDWORD)gSurface, 84);

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD dwReason, LPVOID lpReserved)
{
	// make debug console
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MainThread, NULL, NULL, NULL);
	}
	return TRUE;
}