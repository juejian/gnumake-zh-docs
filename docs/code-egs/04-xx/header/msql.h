#ifndef __INCLUDE_MYSQL_h__
#define __INCLUDE_MYSQL_h__

#include "base.h"

struct SQLBASE
{
	char usr[32];
	char pwd[32];
	char url[256];
};

int get_data_from_db(const struct SQLBASE *pdb, struct DAO *pdao);

#endif // __INCLUDE_MYSQL_h__
