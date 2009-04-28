#include <stdio.h>
#include <ogcsys.h>

#include "title.h"
#include "video.h"


s32 Title_Uninstall(u64 tid)
{
	tikview *viewData = NULL;
	u32      viewCnt;

	s32 res, ret = 0;

	/* Get ticket views */
	res = Title_GetTicketViews(tid, &viewData, &viewCnt);
	if (res < 0)
		ret = res;

	/* Delete tickets */
	if (ret >= 0) {
		u32 cnt;

		/* Delete all tickets */
		for (cnt = 0; cnt < viewCnt; cnt++) {
			res = ES_DeleteTicket(&viewData[cnt]);
			if (res < 0)
				break;
		}

		if (res < 0)
			ret = res;
	}

	/* Delete title contents */
	res = ES_DeleteTitleContent(tid);
	if (res < 0)
		ret = res;

	/* Delete title */
	res = ES_DeleteTitle(tid);
	if (res < 0)
		ret = res;

	return ret;
}

