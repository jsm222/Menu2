#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qmenu.h"
#include <QMainWindow>
#include <QDebug>
#include <QLayout>
#include <QMenuBar>
#include <QActionEvent>
#include <QLineEdit>
#include <QWidgetAction>
#include <QTimer>
#include "menuimporter.h"
#include <dbusmenu-qt5/dbusmenuimporter.h>
static const char *DBUS_SERVICE = "com.canonical.AppMenu.Registrar";
static const char *DBUS_OBJECT_PATH = "/com/canonical/AppMenu/Registrar";
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class HsMenu : public QMenu {

    Q_OBJECT
public:
    void actionEvent(QActionEvent *event) override;
    HsMenu(QWidget *parent = nullptr);
    std::chrono::high_resolution_clock::time_point lastOpened;





};
class HsSearchMenu : public QMenu {

    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *event) override;
    HsSearchMenu(QWidget *parent = nullptr);

};
class HsLineEdit :public QLineEdit {

    Q_OBJECT
public:
    void keyPressEvent(QKeyEvent *event) override;
    HsLineEdit(QWidget *parent = nullptr);

};
class CloneAction : public QAction
{
    Q_OBJECT
public:
    CloneAction(QAction *original, QObject *parent = 0) : QAction(parent)
    {
        m_orig = original;
        connect(m_orig, &QAction::changed, this, &CloneAction::updateMe); // update on change
        connect(m_orig, &QAction::destroyed, this, &QAction::deleteLater); // delete on destroyed
        connect(this, &QAction::triggered, m_orig, &QAction::triggered); // trigger on triggered
    }
public slots:
    void updateMe()
    {
        const QStringList props = QStringList() << "autoRepeat"
                                                << "checkable"
                                                << "checked"
                                                << "enabled"
                                                << "icon"
                                                << "iconText"
                                                << "iconVisibleInMenu"
                                                << "statusTip"
                                                << "toolTip"
                                                << "whatsThis";
        foreach (const QString prop, props) {
            setProperty(qPrintable(prop), m_orig->property(qPrintable(prop)));
        }
    }

    void resetOrigShortcutContext() { m_orig->setShortcutContext(Qt::ApplicationShortcut); }

private:
    QAction *m_orig;
    QMetaObject::Connection m_con;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QMenu *m_search;
    QMenu *m_menu = nullptr;
    HsLineEdit *m_searchLineEdit;
    ~MainWindow();
    DBusMenuImporter *m_menuImporter = nullptr;
    void convertFromQMenu(QMenu *menu);
    QMenuBar * menubar;
    void onWindowShow();
public: signals:
    void setsearchfocus();
private:
    void searchMenu(QMenu *item, QStringList names, QString searchTerm);
    Ui::MainWindow *ui;
    MenuImporter * m;
    HsSearchMenu * m_searchMenu;
    QAction * m_searchAction;
};
#endif // MAINWINDOW_H
class HDBusMenuImporter : public DBusMenuImporter
{
public:
    HDBusMenuImporter(const QString &service, const QString &path,
                      const enum DBusMenuImporterType type, QObject *parent = 0)
        : DBusMenuImporter(service, path, type, parent)

    {
        m_reshowTimer = new QTimer();
        connect(m_reshowTimer, &QTimer::timeout, this, [this] {
            if (recent) {
                recent->blockSignals(true);
                qDebug() << "reshow" << __LINE__ << recent->title();
                qobject_cast<QMenuBar *>(recent->parent()->parent())
                    ->setActiveAction(recent->menuAction());
                recent->blockSignals(false);
                recent = nullptr;
            }
        });
    }
    QMenu *createMenu(QWidget *parent) override
    {
        if(parent==0) {
            parent = qobject_cast<QWidget*>(this->parent());
        }
        HsMenu *menu  = new HsMenu(parent);
        // Make some workarounds for focus loss which  calls closeAllPoupus();
        connect(menu, &QMenu::aboutToShow, this, [this] {
                    recent = qobject_cast<HsMenu *>(sender());
                    qobject_cast<HsMenu *>(sender())->lastOpened =
                        std::chrono::high_resolution_clock::now();
                    m_reshowTimer->stop();
                });
                connect(menu, &QMenu::aboutToHide, this, [this] {
                    HsMenu *reshow = qobject_cast<HsMenu *>(sender());

                    m_reshowTimer->setSingleShot(true);
                    m_reshowTimer->setInterval(100); // if you show another menu within 100ms the reshow
                        // action is canceled.
                    std::chrono::duration<double, std::milli> dur =
                        std::chrono::high_resolution_clock::now() - reshow->lastOpened;

                    if (dur.count() < 350) // start reshow timer on fastly reclosed menus
                        m_reshowTimer->start();
                });



        return menu;
    }
    public:
        QTimer *m_reshowTimer;
        HsMenu *recent;





};
