#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "sifs-internal.h"

#define SIFS_PATH_THISONE 0
#define SIFS_PATH_PARENT 1
#define SIFS_PATH_PARENT_FOR_NEW 2

extern int SIFS_pathmatch(const char *volumename, const char *pathname, int mode);
extern int SIFS_defrag(const char *volumename);
extern int SIFS_lastname(const char *pathname, char* lastname);

