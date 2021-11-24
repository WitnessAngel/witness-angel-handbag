#include <stdio.h>

#include "app.h"
#include "aviFile.h"

void micro_record(app_t *app, const char *fname)
{
	FILE *avifile = fopen(fname, "wb");
	FILE *idxfile = fopen(fname, "wb");
	if (idxfile == NULL || avifile == NULL)
		return ;
}
