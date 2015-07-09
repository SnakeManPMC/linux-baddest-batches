/*

        Baddest Batches datafile maker for DayDream
        (c) PMC 1998 - 2015.

 */

#include <dd.h>
#include <ddlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/pmc.h"
#include "baddest.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

FILE *datafile;
FILE *logfile;

Baddest BB;
struct dif *d;
struct tm *tm;

char buf[300];
char temp_username[25];
char temp[80];
char temp_date[10];
char bbsname[80] = "MyBBS";
int newuser = 0, record = 0;

/* crap */
unsigned long temp_megs;
unsigned long temp_cps;
unsigned long temp_files;
unsigned long temp_time;
/* crap */

int grab_date() /* date grabbing routine */
{
	char tmp_time[80];

	time_t t;
	t = time(0);
	tm = localtime(&t);
	/* grabbing current date */

	strcpy(tmp_time, ctime(&t));
	bzero(temp_date, sizeof(temp_date));
	sprintf(temp_date, "%2.2d/%2.2d/%2.2d", tm->tm_mon + 1, tm->tm_mday, tm->tm_year % 100);
	/* above makes xx/xx/xx date fixation */

	fputs("******* log entry at: ", logfile);
	fputs(tmp_time, logfile);
	fputs("-- user: ", logfile);
	fputs(temp_username, logfile);
	sprintf(tmp_time, " -- %lu megs -- %lu files -- %lu time\n", temp_megs, temp_files, temp_time);
	fputs(tmp_time, logfile);

	return 0;
}

int Search_User()
{
	fread(&BB, sizeof(Baddest), 1, datafile);

	while (strstr(temp_username, BB.User_Name) == NULL)
	{
		fread(&BB, sizeof(Baddest), 1, datafile);

		if (feof(datafile))
		{
			newuser = 1;
			return 0;
		}
	}

	fseek(datafile, -sizeof(Baddest), SEEK_CUR);
	newuser = 0;

	return 0;
}

int Add_User()
{
	strcpy(BB.User_Name, "");
	BB.Megs = 0;
	BB.Cps = 0;
	BB.Files = 0;
	BB.Time = 0;
	strcpy(BB.Date, "");
	/* ^ clean the bitch up */

	strcpy(BB.User_Name, temp_username);
	BB.Megs = temp_megs;
	BB.Cps = temp_cps;
	BB.Files = temp_files;
	BB.Time = temp_time;
	strcpy(BB.Date, temp_date);

	fwrite(&BB, sizeof(Baddest), 1, datafile);

	return 0;
}

int My_Bytes()
{
	if (BB.Megs < temp_megs)
	{
		strcpy(BB.User_Name, "");
		BB.Megs = 0;
		BB.Cps = 0;
		BB.Files = 0;
		BB.Time = 0;
		strcpy(BB.Date, "");

		strcpy(BB.User_Name, temp_username);
		BB.Megs = temp_megs;
		BB.Cps = temp_cps;
		BB.Files = temp_files;
		BB.Time = temp_time;
		strcpy(BB.Date, temp_date);
		fwrite(&BB, sizeof(Baddest), 1, datafile);

		record = 1;
	}

	return 0;
}

int main(int argv, char *argc[])
{
	if (argv == 1)
	{
		printf("need to run from DayDream\n");
	}

	d = dd_initdoor(argc[1]);

/*   sprintf(buf,"%s/pmc/baddest.dat",getenv("DAYDREAM"));*/
	strcpy(buf, "/home/bbs/pmc/baddest.dat");
	datafile = fopen(buf, "r+b");
	if (!datafile)
	{
		printf("error at %s!\n", buf);
		return 1;
	}

	logfile = fopen("/home/bbs/pmc/baddest.log", "at");
	if (!logfile)
	{
		printf("error at /home/bbs/pmc/baddest.log!\n");
		return 1;
	}

	dd_sendstring(d, "\n[ Baddest Batches (c) PMC 1996-2015 ]\n");

	dd_getstrval(d, buf, USER_HANDLE);
	strcpy(temp_username, buf);

	dd_getstrval(d, buf, DOOR_PARAMS);

	sscanf(buf, "%lu %lu %lu", &temp_files, &temp_megs, &temp_time);

	grab_date();
	Search_User();

	if (newuser == 1)
		Add_User();
	if (newuser == 0)
		My_Bytes();

	if (temp_files > 1)
	{
		Create_Announce();
		/*  RehashBot();*/
	}

	fclose(datafile);
	fclose(logfile);
	dd_close(d);

	return 0;
}

int Create_Announce()
{
	FILE *announce;
	char ann_string[200];

	announce = fopen("/eggdrop/pmc-announce.txt", "wt");
	if (!announce)
	{
		dd_sendstring(d, "error at /eggdrop/pmc-announce.txt! inform sysop.\n");
		return 0;
	}

	if (record == 0)
	{
		sprintf(ann_string, "%s  daydream bbs ul batch complete!\ntrader [ %s ] uploaded - %lu files %lu megs in %lu mins  (ps fuck ftp)\n",
		        bbsname, temp_username, temp_files, (temp_megs / (1024 * 1024)), (temp_time / 60));
	}
	else
	{
		sprintf(ann_string, ".%s  daydream bbs ul .batch. complete!\ntrader [ .%s. ] uploaded - .%lu. files .%lu. megs in .%lu. mins  (ps fuck ftp)\n --- this is %s's new baddest batches record\n",
		        bbsname, temp_username, temp_files, (temp_megs / (1024 * 1024)), (temp_time / 60), temp_username);
	}

	fputs(ann_string, announce);

	fclose(announce);
	return 0;
}

/* rehash the eggdrop bot */
int RehashBot()
{
	FILE *botpidfile;

	char iptemp[80];
	int olduid = getuid();
	int botpid;

	botpidfile = fopen("/eggdrop/pid.PMCbitch", "rt");
	if (!botpidfile)
	{
		printf("error at eggdrop pid file...\n");
		return 0;
	}
	/* if cannot find bot pid, PMC bitches ass, just return and sux */

	fgets(iptemp, 80, botpidfile);
	botpid = atoi(iptemp);

	if (!setuid(0))
	{
		kill(botpid, SIGHUP);
		setuid(olduid);
	}
	else
	{
		printf("setuid failed\n");
	}

	fclose(botpidfile);
	return 0;
}