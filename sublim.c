/*
 * asublim - an application to display visual subliminal messages from a text stream
 *
 * (c) 2011 John Ohno
 * Licensed under the GNU GPL v.3
 *
 * Requires xosd, which can be downloaded from:
 *  https://sourceforge.net/projects/libxosd/
 *
 * 
 * TODO: add options to configure screen size.
 *
 */
#include <xosd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

#define ASublimOptString "ht:c:f:F:S:s:D:d:"

struct option lopts[]={ 
	{ "help", 		0, 0, 'h'},
	{ "timeout",		1, 0, 't'},
	{ "color", 		1, 0, 'c'},
	{ "colour", 		1, 0, 'c'},
	{ "font",		1, 0, 'f'},
	{ "file",	 	1, 0, 'F'},
	{ "delayShowMax",  	1, 0, 'S'},
	{ "delayShowMin",	1, 0, 's'},
	{ "delayWordMax", 	1, 0, 'D'},
	{ "delayWordMin", 	1, 0, 'd'}
};

int main(int argc, char** argv) {
	xosd *osd;

	int timeout;
	char* color="LawnGreen";
	char* font="-*-fixed-*-*-*-*-15-140-*-*-*-*-*-*";
	FILE* input=stdin;
	int delayShowMax=50;
	int delayShowMin=10;
	int delayWordMax=50;
	int delayWordMin=10;

	int ret;
	while((ret=getopt_long(argc, argv, ASublimOptString, lopts, NULL)) != -1) {
		switch(ret) {
			case 'S':
				delayShowMax=atoi(optarg);
				break;
			case 's':
				delayShowMin=atoi(optarg);
				break;
			case 'D':
				delayWordMax=atoi(optarg);
				break;
			case 'd':
				delayWordMin=atoi(optarg);
				break;
			case 't':
				timeout=atoi(optarg);
				break;
			case 'f':
				font=strcpy(malloc(strlen(optarg)+1), optarg);
				break;
			case 'c':
				color=strcpy(malloc(strlen(optarg)+1), optarg);
				break;
			case 'F':
				input=fopen(optarg, "r");
				if(errno) {
					fprintf(stderr, "File error! Defaulting to stdin.\n");
					input=stdin;
				}
				break;
			case 'h':
			case '?':
				fprintf(stderr, "Usage: %s [options]\n\nOptions:\n\t-h\n\t--help\t\tPrint this help\n\t-t\n\t--timeout\tTimeout in seconds for automatic refresh\n\t-c\n\t--color\n\t--colour\tSpecify text colour\n\t-f\n\t--font\t\tSpecify X font string\n\t-F\n\t--file\t\tSpecify input file (default stdin)\n\t--delayShowMax\tMaximum display time in usecs\n\t--delayShowMin\tMinimum display time in usecs\n\t--delayWordMax\tMaximum delay between words in usecs\n\t--delayWordMin\tMinimum delay between words in usecs\n\n", argv[0]);
				exit(0);
				break;
		}
				
	}

	osd = xosd_create(1);
	if(xosd_set_font(osd, font)==-1) {
		fprintf(stderr, "Font not found\n");
		exit(1);
	}
	if(xosd_set_colour(osd, color)==-1) {
		fprintf(stderr, "Color set error\n");
		exit(1);
	}
	if(xosd_set_timeout(osd, 1)==-1) {
		fprintf(stderr, "Timeout set error\n");
		exit(1);
	}

	xosd_set_shadow_offset(osd, 3);
	srand48(time(NULL));
	char* x=calloc(1024, sizeof(char));

	scanf(" %1023[^ ]s", x);
	while(x[0]!=EOF) {
		xosd_set_vertical_offset(osd, lrand48()%768);
		xosd_set_horizontal_offset(osd, lrand48()%1024);
		xosd_display(osd, 0, XOSD_string, x);
		xosd_show(osd);
		usleep((unsigned int)((lrand48()%(delayShowMax-delayShowMin)))+delayShowMin);
		xosd_hide(osd);
		scanf(" %1023[^ ]s", x);
		usleep((unsigned int)((lrand48()%(delayWordMax-delayWordMin)))+delayWordMin);
	}
	
	xosd_wait_until_no_display(osd);
	xosd_destroy(osd);

	return 0;
}

