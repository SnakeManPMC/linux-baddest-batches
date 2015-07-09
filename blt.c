/*

        Baddest Batches bulletin making binary

        version unknown.

 */

#include <stdio.h>
#include <string.h>
#include "../include/pmc.h"
#include "baddest.h"

Baddest PMC;

FILE *datafile;
FILE *bulletinASC;
FILE *bulletinANS;

char buf[300];
char temp_username[25];
int newuser = 0;

/* crap to the BULLETIN making routine */
char bigstring[250];
char temp[80];
char clr_frame[10];  /* frame color */
char clr_header[10]; /* header color */
char clr_footer[10]; /* footer color */
char clr_pos[10];    /* pos color */
char clr_uname[10];  /* username color */
char clr_batch[10];  /* batch color */
char clr_cps[10];    /* avg.cps color */
char clr_min[10];    /* min color */
char clr_files[10];  /* files color */
char clr_date[10];   /* date color */
char clr_status[10]; /* status color */

char blt_username[200][25];
unsigned long blt_megs[200];
unsigned long blt_cps[200];
unsigned long blt_files[200];
unsigned long blt_time[200];
char blt_date[200][10];

unsigned long temp_megs;
unsigned long temp_cps;
unsigned long temp_files;
unsigned long temp_time;
unsigned long max_cps;
int i = 0, userbyte_pos[200], x = 0, maxuser = 0, Pos = 0, blt_total = 10;

void Header()
{
	strcpy(bigstring, " .------------------------------------------------------------------------.\n");
	fprintf(bulletinASC, bigstring);

	strcpy(bigstring, "[2J[H");
	strcat(bigstring, clr_frame);
	strcat(bigstring, " .------------------------------------------------------------------------.\n");
	fprintf(bulletinANS, bigstring);
/* most upper line */

	strcpy(bigstring, " |               Baddest Batches ");
	strcat(bigstring, Version);
	strcat(bigstring, " (c) PMC 1996-2015.               |\n");
	fprintf(bulletinASC, bigstring);

	strcpy(bigstring, " |");
	strcat(bigstring, clr_header);
	strcat(bigstring, "               Baddest Batches ");
	strcat(bigstring, Version);
	strcat(bigstring, " (c) PMC 1996-2015.");
	strcat(bigstring, clr_frame);
	strcat(bigstring, "               |\n");
	fprintf(bulletinANS, bigstring);
/* middle name part */

	strcpy(bigstring, " |------------------------------------------------------------------------|\n");
	fprintf(bulletinASC, bigstring);

	strcpy(bigstring, " |------------------------------------------------------------------------|\n");
	fprintf(bulletinANS, bigstring);
/* line under the name shit */

	strcpy(bigstring, " | pos  username          |  batch  | avg.cps |  mins | files |   date    |");
	strcat(bigstring, "\n |------------------------+---------+---------+-------+-------+-----------|");
	fprintf(bulletinASC, bigstring);
/*  ascii status_line  */

	sprintf(bigstring, " | %spos  username          %s|%s  batch  %s|%s avg.cps %s|%s  mins %s|%s files %s|%s   date    %s",
	        clr_status, clr_frame, clr_status, clr_frame, clr_status, clr_frame, clr_status, clr_frame, clr_status, clr_frame, clr_status, clr_frame);

	strcat(bigstring, "|\n |------------------------+---------+---------+-------+-------+-----------|");
	strcat(bigstring, clr_uname);
	fprintf(bulletinANS, bigstring);
/*  ansi status_line  */
}

void Footer()
{
	strcpy(bigstring, "\n |------------------------------------------------------------------------|\n");
	fprintf(bulletinASC, bigstring);

	strcpy(bigstring, clr_frame);
	strcat(bigstring, "\n |------------------------------------------------------------------------|\n");
	fprintf(bulletinANS, bigstring);

	strcpy(bigstring, " |           idea by painkiller.  coding by snake man. (c) PMC            |\n");
	fprintf(bulletinASC, bigstring);

	strcpy(bigstring, " |");
	strcat(bigstring, clr_footer);
	strcat(bigstring, "           idea by painkiller.  coding by snake man. (c) PMC           ");
	strcat(bigstring, clr_frame);
	strcat(bigstring, " |\n");
	fprintf(bulletinANS, bigstring);

	strcpy(bigstring, " `------------------------------------------------------------------------'\n");
	fprintf(bulletinASC, bigstring);

	strcpy(bigstring, " `------------------------------------------------------------------------'[0m\n");
	fprintf(bulletinANS, bigstring);
}

int Bulletin()
{
	bulletinASC = fopen("baddest.txt", "wt");
	if (!bulletinASC)
	{
		printf("error at baddest.txt!\n");
		exit(1);
	}
	bulletinANS = fopen("baddest.gfx", "wt");
	if (!bulletinANS)
	{
		printf("error at baddest.ans!\n");
		exit(1);
	}

	datafile = fopen("baddest.dat", "rb");
	if (!datafile)
	{
		printf("error at baddest.dat!\n");
		fclose(bulletinASC);
		fclose(bulletinANS);
		exit(1);
	}

	Header(); /* write header */

	i = 0;

	while (!feof(datafile))
	{
		i++;
		fread(&PMC, sizeof(Baddest), 1, datafile);

		if (feof(datafile))
			break;
		/* if we have end of file detected, so last entry wont be dupe */

		strcat(blt_username[i], PMC.User_Name);
		blt_megs[i] = 0;
		blt_megs[i] = PMC.Megs;
		blt_cps[i] = 0;
		if (PMC.Time != 0)
		{
			blt_cps[i] = PMC.Megs / PMC.Time; /* count CPS */
			if (blt_cps[i] > max_cps)
				blt_cps[i] = 0;
		}
		blt_files[i] = 0;
		blt_files[i] = PMC.Files;
		blt_time[i] = 0;
		blt_time[i] = PMC.Time;
		strcpy(blt_date[i], PMC.Date);
	}

	maxuser = i; /* how many users */

/* sorting routine by Miz (c) PMC 1997.	*/
	for (i = 1; i <= maxuser; i++)
	{
		userbyte_pos[i] = 1;

		for (x = 1; x <= maxuser; x++)
		{
			if (blt_megs[i] < blt_megs[x])
				userbyte_pos[i]++;
		}
	}

/* displaying the sorted by Miz (c) PMC 1997. */
	while (Pos <= blt_total)
	{
		for (x = 1; x <= maxuser; x++)
			if (userbyte_pos[x] == Pos)
			{
				if (blt_megs[x] == 0)
					break;  /* no more guys, break the loop */

				sprintf(bigstring, "\n | %2d)  %-18s|%5dmb  | %7d |%5d  |%5d  | %s  |",
				        Pos,
				        blt_username[x],
				        blt_megs[x] / 1024 / 1024,
				        blt_cps[x],
				        blt_time[x] / 60,
				        blt_files[x],
				        blt_date[x]);

				fprintf(bulletinASC, bigstring);
/* write datas. format will happen automatically. this is the ASC version */

				sprintf(bigstring, "\n %s|%s %2d)%s  %-18s%s|%s%5dmb  %s|%s %7d %s|%s%5d  %s|%s%5d  %s|%s %s  %s|",
				        clr_frame, clr_pos,
				        Pos, clr_uname,
				        blt_username[x], clr_frame, clr_batch,
				        blt_megs[x] / 1024 / 1024, clr_frame, clr_cps,
				        blt_cps[x], clr_frame, clr_min,
				        blt_time[x] / 60, clr_frame, clr_files,
				        blt_files[x], clr_frame, clr_date,
				        blt_date[x], clr_frame);

				fprintf(bulletinANS, bigstring);
/* this is the ANSI version */
			}
		Pos++;
	} /* bulletin write finished! */

	Footer();

	fclose(bulletinASC);
	fclose(bulletinANS);
	fclose(datafile);

	return 0;
}

CFGread()
{
	char temp1[80];
	char temp2[80];

	FILE *cfg_file;

	printf("\n   Baddest Batches (c) PMC. bulletin making in progress...");

	/* lets read the how_many_users_to_list_in_the_bulletin & colors */
	cfg_file = fopen("blt.cfg", "rt");
	if (!cfg_file)
	{
		printf("error at blt.cfg!\n");
		exit(1);
	}

	fgets(temp, 80, cfg_file);
	fgets(temp1, 80, cfg_file); /* frame color */
	fgets(temp2, 80, cfg_file); /* header color */
	strncpy(clr_frame, temp1, strlen(temp1) - 1);
	strncpy(clr_header, temp2, strlen(temp2) - 1);

	fgets(temp1, 80, cfg_file); /* footer color */
	fgets(temp2, 80, cfg_file); /* uname color */
	strncpy(clr_footer, temp1, strlen(temp1) - 1);
	strncpy(clr_pos, temp2, strlen(temp2) - 1);

	fgets(temp1, 80, cfg_file); /* uname color */
	fgets(temp2, 80, cfg_file); /* batch color */
	strncpy(clr_uname, temp1, strlen(temp1) - 1);
	strncpy(clr_batch, temp2, strlen(temp2) - 1);

	fgets(temp1, 80, cfg_file); /* cps color */
	fgets(temp2, 80, cfg_file); /* min color */
	strncpy(clr_cps, temp1, strlen(temp1) - 1);
	strncpy(clr_min, temp2, strlen(temp2) - 1);

	fgets(temp1, 80, cfg_file); /* files color */
	fgets(temp2, 80, cfg_file); /* date color */
	strncpy(clr_files, temp1, strlen(temp1) - 1);
	strncpy(clr_date, temp2, strlen(temp2) - 1);

	fgets(temp1, 80, cfg_file); /* date color */
	strncpy(clr_status, temp1, strlen(temp1) - 1);

	blt_total = atoi(temp); /* total number of users */

	fgets(temp, 80, cfg_file);
	max_cps = atoi(temp);

	if (blt_total < 1)
		blt_total = 10;
	if (blt_total > 50)
		blt_total = 10;
	/* if under 1 or over 50 default it to 10 */

	fclose(cfg_file);
}

/*  main fucking bitch program starts here... */
int main()
{
	CFGread();  /* read cfg file */
	Bulletin(); /* do the wild thing */

	printf("Done!\n   created files: baddest.txt and baddest.gfx\n\n");

	return 0;
}