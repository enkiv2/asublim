#!/usr/bin/env python
import cairo
from gi.repository import Gtk, Gdk, Pango, GObject
from random import Random
random=Random()
import getopt
import sys, time

font="fixed"
color="LawnGreen"
inputStream=sys.stdin
delayShowMax=100
delayShowMin=10
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
		
def WordGenerator(path):
	for line in path.xreadlines():
	    for word in line.split():
		    yield word
	

def usleep(ms):
	time.sleep(ms*0.0001)

class SublimPane (Gtk.Window):
	def __init__(self, body, fnt, max_x, max_y, delayShowMin, delayShowMax, delayWordMin, delayWordMax, color):
		super(SublimPane, self).__init__()
		self.set_decorated(False)
		self.set_accept_focus(False)
                self.set_modal(False)
                self.set_keep_above(True)
                self.set_skip_taskbar_hint(True)
                self.set_skip_pager_hint(True)
		self.body=body
		self.max_x=max_x
		self.max_y=max_y
		self.delayShowMax=delayShowMax
		self.delayShowMin=delayShowMin
		self.delayWordMax=delayWordMax
		self.delayWordMin=delayWordMin
		self.color=color
		self.set_position(Gtk.WindowPosition.CENTER)
		self.screen = self.get_screen()
		self.visual = self.screen.get_rgba_visual()
		if self.visual != None and self.screen.is_composited():
			self.set_visual(self.visual)

		box = Gtk.Box()
		self.label=Gtk.Label()
		self.label.modify_font(fnt)
		box.add(self.label)
		self.add(box)

		self.set_app_paintable(True)
		self.connect("draw", self.area_draw)
		GObject.timeout_add(0, self.showStep)
		self.show_all()
	def hideStep(self):
		self.hide()
		GObject.timeout_add(random.randint(self.delayWordMin, self.delayWordMax), self.showStep)
	def showStep(self):
		self.move(random.randint(0, self.max_x+1), random.randint(0, self.max_y+1))
		try:
			self.label.set_markup("<span foreground=\""+self.color+"\">"+self.body.next().replace("<", "&lt;").replace(">", "&lt;")+"</span>")
		except:
			sys.exit()
		self.show()
		GObject.timeout_add(random.randint(self.delayShowMin, self.delayShowMax), self.hideStep)

	def area_draw(self, widget, cr):
		cr.set_source_rgba(0, 0, 0, 0.0)
		cr.set_operator(cairo.OPERATOR_SOURCE)
		cr.paint()
		cr.set_operator(cairo.OPERATOR_OVER)

win=SublimPane(WordGenerator(inputStream), Pango.FontDescription(font), width, height, delayShowMin, delayShowMax, delayWordMin, delayWordMax, color)
win.connect("delete-event", Gtk.main_quit)
Gtk.main()
