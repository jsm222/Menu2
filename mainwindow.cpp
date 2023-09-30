#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include "menuimporteradaptor.h"

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
                     qobject_cast<HsLineEdit*>(parent())->setFocus();
                     qobject_cast<HsLineEdit*>(parent())->grabKeyboard();
                     return;
                 }
             }



         }
        QMenu::keyPressEvent(event);
     }
HsMenu::HsMenu(QWidget *parent) :QMenu(parent)
       {

     }
void HsMenu::actionEvent(QActionEvent* event)  {
    if(event->action()->menu() && event->type() == QEvent::ActionAdded) {
        if(qobject_cast<QMenuBar*>(parent()) != nullptr) {
            qobject_cast<QMenuBar*>(parent())->addAction(event->action());



        }

    }
    if(event->action()->menu() && event->type() == QEvent::ActionRemoved) {

        if(qobject_cast<QMenuBar*>(parent()) != nullptr) {
            qobject_cast<QMenuBar*>(parent())->removeAction(event->action());
        }

    }


        QMenu::actionEvent(event);
}
HsLineEdit::HsLineEdit(QWidget *parent) : QLineEdit(parent)
{
}
void HsLineEdit::keyPressEvent(QKeyEvent *event)
{
        if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Up) {
        emit returnPressed();
        }else {

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

        m_searchLineEdit = new HsLineEdit(0);
        m_searchMenu = new HsSearchMenu(m_searchLineEdit);
        m_searchMenu->setTitle("SearchResults");
        connect(m_searchMenu,&QMenu::aboutToHide,this,[this]{

            setWindowFlag(Qt::WindowDoesNotAcceptFocus,true);
            setAttribute(Qt::WA_X11DoNotAcceptFocus,true);
            m_searchLineEdit->clear();
            show();
        });
      connect(this,&MainWindow::setsearchfocus,this,[this] {
            setWindowFlag(Qt::WindowDoesNotAcceptFocus,false);
            setAttribute(Qt::WA_X11DoNotAcceptFocus,false);
            show();
            m_searchLineEdit->setFocus();

        });
connect(m_searchLineEdit,&QLineEdit::returnPressed,this,[this] {
          m_searchLineEdit->releaseKeyboard();
          if(!m_searchMenu->actions().isEmpty())
              m_searchMenu->setActiveAction(m_searchMenu->actions().first());


      });

        connect(m_searchLineEdit,&QLineEdit::textChanged,this,[this] {
          m_searchLineEdit->setFocus();
          if(m_searchLineEdit->text()!="") {
                m_searchMenu->clear();
                QPoint pos = this->mapToGlobal(QPoint(0,0));
                m_searchMenu->move(pos.rx(),pos.ry()+m_searchLineEdit->height());
                //m_searchMenu->setGeometry(pos.rx(),pos.ry()+m_searchLineEdit->height(),m_searchMenu->height(),m_searchMenu->width());
                if(!m_searchMenu->isVisible())
                    m_searchMenu->show();


                    QStringList names;
                if(m_menu) {
                    searchMenu(m_menu,names,m_searchLineEdit->text());
                    m_searchMenu->adjustSize();

                }

                }



            });
  //  setWindowFlags(Qt::);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowDoesNotAcceptFocus | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_X11DoNotAcceptFocus,true);


}
void MainWindow::onWindowShow() {
    ///ui->menubar->addMenu(m_search);
#if 1
    QDBusConnection::sessionBus().registerService(DBUS_SERVICE);
    qDebug()<<QDBusConnection::sessionBus().lastError();
    m = new MenuImporter();
    new MenuImporterAdaptor(m);

    QDBusConnection::sessionBus().registerObject(DBUS_OBJECT_PATH, m);
    qDebug()<<QDBusConnection::sessionBus().lastError();
#endif

    menubar = new QMenuBar(this);
    menubar->setNativeMenuBar(false);
    setMenuBar(menubar);








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




   //
    // menubar->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //m_menuImporter = new HDBusMenuImporter("","",DBusMenuImporterType::ASYNCHRONOUS,menubar);
#if 1
    connect(m,&MenuImporter::windowRegistered,this,[this] {
        qDebug() << m->m_service << m->m_objectPath;
        m_menuImporter = new HDBusMenuImporter(m->m_service,m->m_objectPath,DBusMenuImporterType::ASYNCHRONOUS,menubar);
        m_menu = m_menuImporter->menu();

    });
  connect(m,&MenuImporter::windowUnregistered,this,[this] {
        m_menuImporter->deleteLater();
    });
























#endif
}






MainWindow::~MainWindow()
{

}
void MainWindow::searchMenu(QMenu * item,QStringList names,QString searchTerm) {
    if(item && !item->title().isEmpty() )
        names << item->title();

    for(QAction *a: item->actions())    {



        if(!a->menu() && (a->text().contains(searchTerm,Qt::CaseInsensitive) || a->toolTip().contains(searchTerm,Qt::CaseInsensitive))) {
            CloneAction * cpy = new CloneAction(a);
            QString txt = names.join(QString(" ")+ QChar(0x25b8)+QString(" "))+QString(" ")+ QChar(0x25b8)+QString(" ")+a->text();
            cpy->setText(txt.replace("&",""));
            cpy->setEnabled(a->isEnabled());
            m_searchMenu->addAction(cpy);

        }

        if(a->menu()) {

            searchMenu(a->menu(),names,searchTerm);
        }
    }

}
