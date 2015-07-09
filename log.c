/*

    this bitch displays the logfile into the screen.

 */
#include <stdio.h>
#include "../include/pmc.h"

FILE *datafile;

Baddest PMC;

int ViewLog()
{
	int i = 0;

	datafile = fopen("baddest.dat", "rb");
	if (!datafile)
	{
		printf("error at baddest.dat!\n");
		exit (1);
		return 1;
	}

	while (!feof(datafile))
	{
		i++;
		fread(&PMC, sizeof(Baddest), 1, datafile);
		if (feof(datafile))
			break;

		printf("************************************\n");
		printf("user-record number: %i of database\n", i);
		printf("Username:         %s\n", PMC.User_Name);
		printf("megs (/1024*1024) %lumb\n", PMC.Megs / (1024 * 1024));
		printf("bytes             %lu\n", PMC.Megs);
		printf("time (seconds)    %lu\n", PMC.Time);
		printf("files             %lu\n", PMC.Files);
		printf("time/60 (minutes) %lu\n", PMC.Time / 60);
		printf("date              %s\n", PMC.Date);
		if (PMC.Time != 0)
			printf("cps megs/time     %lu\n", PMC.Megs / PMC.Time);
	}
	printf("************************************\n");

	fclose(datafile);

	return 0;
}

int main()
{
	ViewLog();

	return 0;
}