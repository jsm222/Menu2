#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include "menuimporteradaptor.h"
#include <QKeyEvent>
#include <QMap>
#include "menux11.h"
HsSearchMenu::HsSearchMenu(QWidget *parent) :QMenu(parent)
     {
}

     void HsSearchMenu::keyPressEvent(QKeyEvent *event) {
         if(event->type() == QKeyEvent::KeyPress && event->key()==Qt::Key_Up)   {
             QAction *firstEn = nullptr;
             for(QAction *en:actions()) {
                 if(en->isEnabled()) {
                     firstEn =en;
                     break;
                 }
             }
             if(this->activeAction() == firstEn || firstEn==nullptr) {
                 if(qobject_cast<HsLineEdit*>(parent())) {


                     //qobject_cast<MainWindow*>(qobject_cast<HsLineEdit*>(parent())->parent())->activateWindow();
                    qobject_cast<HsLineEdit*>(parent())->grabKeyboard();
                    QTimer::singleShot(0,qobject_cast<HsLineEdit*>(parent()), SLOT(setFocus()));
                 }
             }



         }
         if(event->type() == QKeyEvent::KeyPress && event->key()==Qt::Key_Escape)  {
             hide();
         }
        QMenu::keyPressEvent(event);
     }



HsMenu::HsMenu(QWidget *parent) :QMenu(parent)
       {

     }

void HsMenu::actionEvent(QActionEvent* event)  {



    if(event->action() && event->type() == QEvent::ActionAdded) {
        if(qobject_cast<QMenuBar*>(parent()) != nullptr) {
                 if(!qobject_cast<QMenuBar*>(parent())->actions().isEmpty()
                     && qobject_cast<QMenuBar*>(parent())->actions().contains(event->action())) {
                     qDebug()<< __LINE__ << __FILE__;
                        return;
                 }
            qobject_cast<QMenuBar*>(parent())->addAction(event->action());


        }

    }
    if(event->action() && event->type() == QEvent::ActionRemoved) {

        if(qobject_cast<QMenuBar*>(parent()) != nullptr) {
            qobject_cast<QMenuBar*>(parent())->removeAction(event->action());
        }

    }


    return QMenu::actionEvent(event);



}
HsLineEdit::HsLineEdit(QWidget *parent) : QLineEdit(parent)
{

}
void HsLineEdit::keyPressEvent(QKeyEvent *event)
{
        if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
        emit returnPressed();
        }
        else if(event->key() == Qt::Key_Escape) {
           QLineEdit::keyPressEvent(event);
           emit escapeFromSearch();
        }
        else {

        QLineEdit::keyPressEvent(event);
        }
}



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{


        const QRect screenGeometry = qApp->screens()[0]->availableGeometry();


        setFixedWidth(screenGeometry.width());
        move(0,0);
        setFixedHeight(30);

        m_searchLineEdit = new HsLineEdit(this);
        m_searchMenu = new HsSearchMenu(m_searchLineEdit);

        m_searchMenu->setTitle("SearchResults");
        connect(m_searchMenu,&QMenu::aboutToHide,this,[this]{

            setWindowFlag(Qt::WindowDoesNotAcceptFocus,true);
            setAttribute(Qt::WA_X11DoNotAcceptFocus,true);
            setAttribute(Qt::WA_X11NetWmWindowTypeDock,true);
            m_searchLineEdit->clear();
            m_searchLineEdit->releaseKeyboard();

            show();

        });

      connect(this,&MainWindow::setsearchfocus,this,[this] {
            beforeSearch(m_menu);
            setWindowFlag(Qt::WindowDoesNotAcceptFocus,false);
            setAttribute(Qt::WA_X11DoNotAcceptFocus,false);
            setAttribute(Qt::WA_X11NetWmWindowTypeDock,false);
            show();
            QTimer::singleShot(0,m_searchLineEdit,SLOT(setFocus()));




        });
setFocusProxy(m_searchLineEdit);
connect(m_searchLineEdit,&QLineEdit::returnPressed,this,[this] {
        m_searchLineEdit->releaseKeyboard();



      });
     connect(m_searchLineEdit,&HsLineEdit::escapeFromSearch,this,[this] {
    m_searchMenu->aboutToHide();
        });

        connect(m_searchLineEdit,&QLineEdit::textChanged,this,[this] {

          if(m_searchLineEdit->text()!="") {
                m_searchMenu->clear();
                QPoint pos = this->mapToGlobal(QPoint(0,0));
                m_searchMenu->move(pos.rx(),pos.ry()+m_searchLineEdit->height());
                if(!m_searchMenu->isVisible())
                    m_searchMenu->show();



                    QStringList names;


                    searchMenu(m_menu,names,m_searchLineEdit->text());
                    m_searchMenu->adjustSize();
                    activateWindow();
                    m_searchLineEdit->setFocus();
                }





            });



    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowDoesNotAcceptFocus | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_X11DoNotAcceptFocus,true);
    setAttribute(Qt::WA_X11NetWmWindowTypeDock,true);
    MenuX11 *menuX11 = new MenuX11();
    menuX11->setStrut(winId(),height(),width());
    ///ui->menubar->addMenu(m_search);
#if 1





#endif

    menubar = new QMenuBar(this);
    menubar->setNativeMenuBar(false);
    setMenuBar(menubar);

    menubar->setFocusPolicy(Qt::NoFocus);






#if 0

    m_menu = new QMenu(0);
#endif
#if 1
    m_searchMenu->setVisible(false);
    m_searchMenu->setTearOffEnabled(false);
    m_searchAction = menubar->addMenu(m_searchMenu);
    m_searchAction->setVisible(false);
#endif
#if 0
    QMenu *sub0 = new QMenu(m_menu);
    sub0->setTitle("asd1&212");
    QMenu *sub0sub0 = new QMenu(sub0);
    sub0sub0->setTitle("&asd121");
    sub0->addMenu(sub0sub0);
    QMenu *sub1 = new QMenu(m_menu);
    QMenu *sub1sub0 = new QMenu(sub1);
    sub1sub0->setTitle("su&bmenu");
    sub1->addMenu(sub1sub0);
    sub1->setTitle("asd1&212");
    m_menu->setTitle("as&d");
    m_menu->setFixedHeight(30);
    m_menu->addMenu(sub0);
    m_menu->addMenu(sub1);
    menubar->addMenu(m_menu);

#endif
    menubar->setCornerWidget(m_searchLineEdit,Qt::TopLeftCorner);

    menubar->adjustSize();
    QDBusConnection::sessionBus().registerService(DBUS_SERVICE);
    qDebug()<<QDBusConnection::sessionBus().lastError();
    m = new MenuImporter(this);
    new MenuImporterAdaptor(m);
    QDBusConnection::sessionBus().registerObject(DBUS_OBJECT_PATH, m);
    qDebug()<<QDBusConnection::sessionBus().lastError();


#if 1

    connect(m,&MenuImporter::windowRegistered,this,[this](uint winId,QString service,QString path) {

        m_windowMenus[winId]=QStringList({service,path});

    });
    connect(this,&MainWindow::windowChanged,this,[this,menuX11]() {

        unsigned long winId = menuX11->getActiveWindow();

        qDebug() << winId << this->window()->winId();
        if(winId == this->window()->winId()) {
            return;
        }
        qDebug() << (m_menu != m_menuDbusMenus[winId]) << __LINE__;
        if(!m_windowMenus[winId].isEmpty() && (m_menu ==nullptr || (m_menu != m_menuDbusMenus[winId]))) {
            for(QAction *a :menubar->actions()) {
                if(a==m_searchAction) {
                    continue;
                }
                menubar->removeAction(a);

            }


            m_menuImporter = new HDBusMenuImporter(m_windowMenus[winId][0],m_windowMenus[winId][1],DBusMenuImporterType::ASYNCHRONOUS,menubar);
            m_menu = m_menuImporter->menu();
            m_menuDbusMenus[winId]=m_menu;






        }







    });
    connect(m,&MenuImporter::windowUnregistered,this,[this](uint winId) {
        m_windowMenus[winId].clear();
        for(QAction *a :menubar->actions()) {
            if(a==m_searchAction) {
                continue;
            }
            menubar->removeAction(a);

        }
        if(m_menu)  {
            m_menu->clear();
        }

    });
}

#endif

MainWindow::~MainWindow()
{

}
void MainWindow::beforeSearch(QMenu *item) {

    //if(item!=m_menu)
    item->aboutToShow();
    QTimer::singleShot(250,this,[this,item] {
    for(QAction *a: item->actions()) {

            if(a->menu())
                beforeSearch(a->menu());
    }
        });






}




void MainWindow::searchMenu(QMenu * item,QStringList names,QString searchTerm) {
    if(item && !item->title().isEmpty() )
        names << item->title();
    if(item==nullptr) {
        return;
    }

    for(QAction *a: item->actions())    {




        if(!a->menu() && (a->text().contains(searchTerm,Qt::CaseInsensitive) || a->toolTip().contains(searchTerm,Qt::CaseInsensitive))) {
            CloneAction * cpy = new CloneAction(a);
            QString txt = names.join(QString(" ")+ QChar(0x25b8)+QString(" "))+QString(" ")+ QChar(0x25b8)+QString(" ")+a->text();
            cpy->setText(txt.replace("&",""));
            cpy->setEnabled(a->isEnabled());
            cpy->setVisible(a->isVisible());
            m_searchMenu->addAction(cpy);


        } else if(a->menu()){


        searchMenu(a->menu(),names,searchTerm);

        }


    }

}
