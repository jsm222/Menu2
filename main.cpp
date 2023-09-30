
#include <QStringList>
#include "mainwindow.h"
#include <QMenuBar>
#include <QDBusMetaType>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusArgument>
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <stdio.h>
#include <X11/XKBlib.h>
#include <X11/extensions/record.h>
#include<pthread.h>
int ctrldown=0;
void key_pressed_cb(XPointer arg, XRecordInterceptData *d);
void *scan(void *ptr) {
    XRecordRange* rr;
    XRecordClientSpec rcs;
    XRecordContext rc;
    Display *dpy = XOpenDisplay(NULL);
    rr = XRecordAllocRange();
    rr->device_events.first = KeyPress;
    rr->device_events.last = KeyReleaseMask;
    rcs = XRecordAllClients;
    rc = XRecordCreateContext (dpy, 0, &rcs, 1, &rr, 1);
    XFree (rr);
    XRecordEnableContext(dpy, rc, key_pressed_cb, NULL);
    return 0;
}
void key_pressed_cb(XPointer arg, XRecordInterceptData *d) {
    if (d->category != XRecordFromServer)
        return;

    int key = ((unsigned char*) d->data)[1];
    int type = ((unsigned char*) d->data)[0] & 0x7F;
    int repeat = d->data[2] & 1;


    if(!repeat) {
        switch (type) {
        case KeyPress: {
            if(key == 37) {
                ctrldown =1;
            }

            if(key==65 && ctrldown) {
                foreach (QWidget *w,qApp->topLevelWidgets())
                    if (MainWindow *mainWin =qobject_cast<MainWindow *>(w)) {
                  emit mainWin->setsearchfocus();

                }

            }
            break;
        }
        case KeyRelease:
            if(key==37) {
                ctrldown=0;
            }
        default:
            break;
        }
    }
    XRecordFreeData (d);
}


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.onWindowShow();

    //a.setStyleSheet("QMenuBar::item {         color: red;         padding: 0px 0px 4px 4px;   }");


   // a.setStyleSheet("QMenuBar {height:30px;color:red;} QMenuBar:item {color:red;height:28px;}");
    qDebug() << a.styleSheet();
    pthread_t thread1;
    pthread_create(&thread1, NULL, *scan,0);
     return a.exec();



}
