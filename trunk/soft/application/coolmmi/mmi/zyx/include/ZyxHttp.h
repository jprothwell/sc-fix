#ifndef __ZYX_HTTP_H__
#define __ZYX_HTTP_H__

#include "zyxtypes.h"

typedef int (*ZyxOnDownCb)(int result,char *outFileName);

void ZyxHttpDownload(char *infileUrl,char*outFileName,ZyxOnDownCb cb);
void ZyxCancelDownload();
void ZyxExitDownload();

#endif
