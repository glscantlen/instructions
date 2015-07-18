#include<stdio.h>
#include<stdlib.h>

#include<X11/Xlib.h>

int main(int argc, char **argv)
{
	Display *dpy=XOpenDisplay(NULL);

	if(!dpy)
	{	fprintf(stderr, "Failed to open display\n");
		exit(1);
	}

	int scr = DefaultScreen(dpy);

	Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, scr),
	             1, 1, 250, 250, 0,
	             BlackPixel(dpy, scr), BlackPixel(dpy, scr));

	GC gc = XCreateGC(dpy, win, 0, NULL);

	XStoreName(dpy, win, "helloX");
	XSetForeground(dpy, gc, WhitePixel(dpy, scr));
	XSelectInput(dpy, win, ExposureMask|ButtonPressMask);
	XMapWindow(dpy, win);

	Atom wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(dpy, win, &wmDeleteMessage, 1);

	XEvent e;
	while(1)
	{	XNextEvent(dpy, &e);
		if(e.type==Expose && e.xexpose.count<1)
			XDrawString(dpy, win, gc, 100, 120, "Howdy!", 6);
		else if( e.type==ClientMessage
		      && e.xclient.data.l[0] == (int)wmDeleteMessage)
			break;
		else if(e.type==ButtonPress) break;
	}

	XCloseDisplay(dpy);
	return 0;
}

