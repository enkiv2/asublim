#!/usr/bin/env python
import pyosd
import sys, time
import getopt
from random import Random
random=Random()

font="fixed"
color="LawnGreen"
inputStream=sys.stdin
delayShowMax=100
delayShowMin=1
delayWordMax=50
delayWordMin=10
width=1024
height=768
timeout=1

helpstr="[options]\n\
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
	--screen-height	Specify screen height\n\n"

opts="ht:c:f:F:S:s:D:d:w:y:"
longopts=["help", "timeout=", "color=", "colour=", "font=", "file=", "delayShowMax=", "delayShowMin=", "delayWordMax=", "delayWordMin=", "screen-width=", "screen-height="]
if(len(sys.argv)>1):
	try:
		optlist, args=getopt.getopt(sys.argv[1:], opts, longopts)
	except getopt.GetoptError as err:
		sys.stderr.write(str(err)+"\n")
		sys.stderr.write("Usage:\t"+sys.argv[0]+" "+helpstr)
		sys.exit(1)
	for opt in optlist:
		optname=opt[0]
		if optname in ["-h", "--help"]:
			sys.stderr.write("Usage:\t"+sys.argv[0]+" "+helpstr)
			sys.exit()
		elif optname in ["-c", "--color", "--colour"]:
			color=opt[1]
		elif optname in ["-f", "--font"]:
			font=opt[1]
		elif optname in ["-F", "--file"]:
			inputStream=open(opt[1], 'r')
		elif optname=="--delayShowMax":
			delayShowMax=int(opt[1])
		elif optname=="--delayShowMin":
			delayShowMin=int(opt[1])
		elif optname=="--delayWordMax":
			delayWordMax=int(opt[1])
		elif optname=="--delayWordMin":
			delayWordMin=int(opt[1])
		elif optname in ["-x", "--screen-width"]:
			width=int(opt[1])
		elif optname in ["-y", "--screen-height"]:
			height=int(opt[1])
		elif optname=="--timeout":
			timeout=int(opt[1])
		

def usleep(ms):
	time.sleep(ms*0.0001)
def set_pos(x, y):
	osd.set_horizontal_offset(x)
	osd.set_vertical_offset(y)
def displayWord(word):
	set_pos(random.randint(0, width), random.randint(0, height))
	osd.display(word)
	osd.show()
	usleep(random.randint(delayShowMin, delayShowMax))
	osd.hide()
	usleep(random.randint(delayWordMin, delayWordMax))

osd=pyosd.osd(font, color)
osd.set_outline_colour("black")
osd.set_timeout(timeout)

def main():
	for line in inputStream.xreadlines():
		for word in line.split():
			displayWord(word)
main()
osd.wait_until_no_display()
