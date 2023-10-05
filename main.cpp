
#include <QStringList>
#include "mainwindow.h"
#include <QMenuBar>
#include <QDBusMetaType>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusArgument>
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <QEvent>
#include <stdio.h>
#include <X11/XKBlib.h>
#include<QObject>
#include <X11/extensions/record.h>
#include<pthread.h>
class HsQApplication: public QApplication {
    Q_OBJECT
public:
    //bool eventFilter(QObject *obj,QEvent *e) override;
    bool notify(QObject *, QEvent *) override;
    HsQApplication(int &argc, char **argv, int = ApplicationFlags):QApplication(argc,argv,ApplicationFlags) {

    }



};
using namespace std::chrono_literals;
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

            if(key==65 && ctrldown==1) {
                foreach (QWidget *w,qApp->topLevelWidgets())
                    if (MainWindow *mainWin =qobject_cast<MainWindow *>(w)) {
                  emit mainWin->setsearchfocus();
		  ctrldown=0;
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

void * getChangeWindow(void *ptr) {
    Display *dsp = XOpenDisplay(NULL);

    Window root = DefaultRootWindow(dsp);
    XSelectInput(dsp,root, PropertyChangeMask);
    XEvent e;
    while(1) {

        XNextEvent(dsp,&e);

        switch(e.type)
        {
        case PropertyNotify: {


            if(e.xproperty.atom==XInternAtom(dsp, "_NET_ACTIVE_WINDOW", True)) {
                foreach (QWidget *w,qApp->topLevelWidgets())
                if (MainWindow *mainWin =qobject_cast<MainWindow *>(w)) {
                        emit mainWin->windowChanged();
                        break;

                    }
            break;
        }
        break;
        default:
            break;
        }
    }

    }

    XCloseDisplay(dsp);
    return 0;
}
#include "main.moc"
int main(int argc, char *argv[]) {

    HsQApplication a(argc, argv);

    MainWindow w;
    w.show();
    pthread_t thread0,thread1;
    pthread_create(&thread0, NULL, *getChangeWindow,0);
    pthread_create(&thread1, NULL, *scan,0);

     return a.exec();



}
bool HsQApplication::notify(QObject *object, QEvent * event) {
    //Workaround for focusloss on dynamic menus.. it ends up deactivting,
    // which closes all popups.
    if(event->type()==QEvent::ApplicationDeactivate) {
    QWidget  *ap;
    if((ap = activePopupWidget())) {
        QTimer::singleShot(10,this,[ap]{
                if(ap->parentWidget()!=nullptr && qobject_cast<QMenuBar*>(ap->parentWidget()->parentWidget())!=nullptr) {
                    ap->blockSignals(true);
                    qobject_cast<QMenuBar*>(ap->parentWidget()->parentWidget())->setActiveAction(qobject_cast<QMenu*>(ap)->menuAction());
                    ap->blockSignals(false);
                }
            });
    }

    }

    return QApplication::notify(object,event);

}
