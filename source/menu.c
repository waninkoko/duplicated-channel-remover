#include <stdio.h>
#include <stdlib.h>
#include <ogcsys.h>

#include "title.h"
#include "title_install.h"
#include "video.h"


u32 __Menu_CountTitle(u64 tid, u64 *list, u32 nbList)
{
	u32 cnt, idx;

	for (cnt = idx = 0; idx < nbList; idx++) {
		u64 res = (list[idx] >> 8) & (~tid >> 8);

		/* Title found in the list */
		if (!res)
			cnt++;
	}

	return cnt;
}

s32 __Menu_CheckRegion(u64 tid)
{
	s32 ret;

	/* Get console region */
	ret = CONF_GetRegion();

	/* Check title region */
	switch (tid & 0xFF) {
	case 'J':
		return (ret == CONF_REGION_JP) ? 1 : 0;
	case 'E':
		return (ret == CONF_REGION_US) ? 1 : 0;
	case 'P':
		return (ret == CONF_REGION_EU) ? 1 : 0;
	case 'K':
		return (ret == CONF_REGION_KR) ? 1 : 0;
	case 'A':
		return 1;
	};

	return -1;
}

void __Menu_UninstallTitle(u64 tid, u64 *list, u32 nbList)
{
	u32 cnt;
	s32 ret;

	/* Find wrong region installs */
	for (cnt = 0; cnt < nbList; cnt++) {
		u64 title = list[cnt];
		u64 res   = (title >> 8) & (~tid >> 8);

		/* Check title region */
		ret = __Menu_CheckRegion(title);

		/* Title found and check for region */
		if (!res && !ret) {
			char *regName = "Unknown";

			u32 prefix = (u32)(title >> 32);
			u32 suffix = (u32)(title & 0xFFFFFFFF);

			/* Determine region name */
			switch (suffix & 0xFF) {
			case 'J':
				regName = "JAP";
				break;

			case 'E':
				regName = "USA";
				break;

			case 'P':
				regName = "EUR";
				break;

			case 'K':
				regName = "KOR";
				break;
			}

			printf("\t>> Deleting %08x-%08x (%s), please wait...\n", prefix, suffix, regName);

			/* Uninstall title */
			ret = Title_Uninstall(title);
			if (ret < 0)
				printf("\t   ERROR! (ret = %d)\n\n", ret);
			else
				printf("\t   SUCCESS!\n\n");
		}
	}
}


void Menu_Main(void)
{
	u64 *list = NULL;

	u32 cnt, nbList;
	s32 ret;

	/* Clear console */
	Con_Clear();

	/* Get title list */
	ret = Title_GetList(&list, &nbList);
	if (ret < 0) {
		printf("[+] ERROR: Could not retrieve title list! (ret = %d)\n", ret);
		return;
	}

	printf("[+] Searching for duplicated titles, please wait...\n\n");

	for (cnt = 0; cnt < nbList; cnt++) {
		u64 tid = list[cnt];

		/* Check for known titles */
		switch (tid >> 32) {
		case 0x00010001: /* Downloadable channels */
		case 0x00010002: /* System channels */
		case 0x00010008: /* Hidden channels */
			/* Count title installs */
			ret = __Menu_CountTitle(tid, list, nbList);

			/* Uninstall non-valid installs */
			if (ret >= 2)
				__Menu_UninstallTitle(tid, list, nbList);
		}
	}

	printf("[+] Search finished!\n");

	/* Free memory */
	free(list);
}
