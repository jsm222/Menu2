#include "menux11.h"
#include <QDebug>
#include <X11/Xlib.h>
#include <X11/Xatom.h>


MenuX11::MenuX11()
{

}

unsigned long MenuX11::getActiveWindow() {
    Display *dsp = XOpenDisplay(NULL);
    unsigned long root = XDefaultRootWindow(dsp);
    int format;
    Atom return_type;
    unsigned long after, nitems, window;
    unsigned char *data;
    qDebug() << XGetWindowProperty(dsp,root,XInternAtom(dsp, "_NET_ACTIVE_WINDOW", False),0,1,False,XA_WINDOW,&return_type,&format,&after,&nitems,&data);
    qDebug() << ((unsigned long*)data)[0] <<__LINE__;
    XCloseDisplay(dsp);
    return ((unsigned long*)data)[0];
}
void MenuX11::setStrut(unsigned long winId,int height,int width) {
    Display *dsp = XOpenDisplay(NULL);
    unsigned long data[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    data[2]=30;
    data[8]=0;
    data[9]=1920;
    qDebug() << winId <<  data[10] << dsp <<__LINE__;
    XChangeProperty(dsp,winId,XInternAtom(dsp, "_NET_WM_STRUT", False),XInternAtom(dsp, "CARDINAL", False) ,32,PropModeReplace,(unsigned char*)&data,4);
    XChangeProperty(dsp,winId,XInternAtom(dsp, "_NET_WM_STRUT_PARTIAL", False),XInternAtom(dsp, "CARDINAL", False) ,32,PropModeReplace,(unsigned char*)&data,12);
    XFlush(dsp);
}
