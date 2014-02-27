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
#include <X11/Xlib.h>

#include <X11/extensions/Xrandr.h>

#define ASublimOptString "ht:c:f:F:S:s:D:d:w:y:"

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
	{ "delayWordMin", 	1, 0, 'd'},
	{ "screen-width",	1, 0, 'w'},
	{ "screen-height",	1, 0, 'y'}
};
const char* helpstr="[options]\n\
\n\
Options:\n\
	-h\n\
	--help		Print this help\n\
\n\
	--timeout	Timeout in seconds for automatic refresh\n\
	-c\n\
	--color\n\
	--colour	Specify text colour\n\
	-f\n\
	--font		Specify X font string\n\
	-F\n\
	--file		Specify input file (default stdin)\n\
	--delayShowMax	Maximum display time in usecs\n\
	--delayShowMin	Minimum display time in usecs\n\
	--delayWordMax	Maximum delay between words in usecs\n\
	--delayWordMin	Minimum delay between words in usecs\n\
	--screen-width	Specify screen width\n\
	--screen-height	Specify screen height\n\n";
int main(int argc, char** argv) {
	xosd *osd;

	Display* d=XOpenDisplay(":0");
	
	int screenCount = ScreenCount(d);
	int *supportedSizes = (int*)malloc(255);
	fprintf(stderr, "\nNumber of screens: %d\n", screenCount);
	int i;
	for (i=0; i<screenCount ; i++) {
		fprintf(stderr, " %d: %dx%x\n", i, DisplayWidth(d, i), DisplayHeight(d, i));
	}	

	int timeout;
	char* color="LawnGreen";
	char* font="-*-fixed-*-*-*-*-15-140-*-*-*-*-*-*";
	FILE* input=stdin;
	int delayShowMax=50;
	int delayShowMin=10;
	int delayWordMax=50;
	int delayWordMin=10;

	int screenWidthOverride=0;
	int screenHeightOverride=0;

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
				font=strncpy(malloc(strlen(optarg)+1), optarg, strlen(optarg)+1);
				break;
			case 'c':
				color=malloc(strlen(optarg));
				color=strncpy(malloc(strlen(optarg)+1), optarg, strlen(optarg)+1);
				break;
			case 'F':
				input=fopen(optarg, "r");
				if(errno) {
					fprintf(stderr, "File error! Defaulting to stdin.\n");
					input=stdin;
				}
				break;
			case 'w':
				screenWidthOverride=atoi(optarg);
				break;
			case 'y':
				screenHeightOverride=atoi(optarg);
				break;
			case 'h':
			case '?':
				fprintf(stderr, "Usage: %s %s", argv[0], helpstr);
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
	if(xosd_set_timeout(osd, timeout)==-1) {
		fprintf(stderr, "Timeout set error\n");
		exit(1);
	}

	xosd_set_shadow_offset(osd, 3);
	srand48(time(NULL));
	char* x=calloc(1024, sizeof(char));

	int height=screenHeightOverride;
	int width=screenWidthOverride;
	scanf(" %1023[^ \t\n]s", x);
	while(x[0]!=EOF) {
		int screen = lrand48() % screenCount;
		//printf("Screen #d\n", screen);
		if (!(screenWidthOverride && screenHeightOverride)) {
			height=	DisplayHeight(d, screen);
			width=	DisplayWidth(d, screen);
		}
		xosd_set_vertical_offset(osd, lrand48()%height);
		xosd_set_horizontal_offset(osd, lrand48()%width);
		xosd_display(osd, screen, XOSD_string, x);
		xosd_show(osd);
		usleep((unsigned int)((lrand48()%(delayShowMax-delayShowMin)))+delayShowMin);
		xosd_hide(osd);
		fscanf(input, " %1023[^ \t\n]s", x);
		usleep((unsigned int)((lrand48()%(delayWordMax-delayWordMin)))+delayWordMin);
	}
	
	xosd_wait_until_no_display(osd);
	xosd_destroy(osd);

	return 0;
}

