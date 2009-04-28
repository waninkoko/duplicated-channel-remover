#include <stdio.h>
#include <ogcsys.h>

#include "gui.h"
#include "menu.h"
#include "restart.h"
#include "sys.h"
#include "video.h"
#include "wpad.h"


void Disclaimer(void)
{
	/* Print disclaimer */
	printf("[+] [DISCLAIMER]:\n\n");

	printf("    THIS APPLICATION COMES WITH NO WARRANTY AT ALL,\n");
	printf("    NEITHER EXPRESS NOR IMPLIED.\n");
	printf("    I DO NOT TAKE ANY RESPONSIBILITY FOR ANY DAMAGE IN YOUR\n");
	printf("    WII CONSOLE BECAUSE OF A IMPROPER USAGE OF THIS SOFTWARE.\n\n");

	printf(">> If you agree, press A button to continue.\n");
	printf(">> Otherwise, press B button to restart the Wii.\n");

	/* Wait for user answer */
	for (;;) {
		u32 buttons = Wpad_WaitButtons();

		if (buttons & WPAD_BUTTON_A)
			break;
		if (buttons & WPAD_BUTTON_B)
			Restart();
	}
}

int main(int argc, char **argv)
{
	/* Initialize subsystems */
	Sys_Init();

	/* Set video mode */
	Video_SetMode();

	/* Initialize console */
	Gui_InitConsole();

	/* Draw background */
	Gui_DrawBackground();

	/* Initialize Wiimote */
	Wpad_Init();

	/* Show disclaimer */
	Disclaimer();



	/* IMPORTANT NOTE : Do not forget to remove this in final version because it will not have any sense, lol. */
	Con_Clear();

	printf("THIS IS A __BETA__ VERSION OF WII DUPLICATED CHANNEL REMOVER!!\n");
	printf("DO NOT USE THIS VERSION EXCEPT IF YOU KNOW WHAT YOU ARE DOING!!\n");
	printf("I CANNOT GUARANTEE IT WILL NOT REMOVE THE SYSTEM MENU OR IOS!!\n");

	sleep(30);



	/* Main Menu */
	Menu_Main();

	/* Restart Wii */
	Restart_Wait();

	return 0;
}
