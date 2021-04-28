#include <stdio.h>
#include <string.h>
#include "header/mnet.h"
#include "header/msql.h"

int main(int argc, char const *argv[])
{
	int flag = 1;
	int ret;

	struct DAO data;
	struct SQLBASE db;

	// ...

	if (flag == 1)
	{
		ret = get_data_from_internet("", &data);
	}
	else
	{
		ret = get_data_from_db(&db, &data);
	}

	//...
	return 0;
}