#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMenu>
#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QLayout>
#include <QMenuBar>
#include <QActionEvent>
#include <QLineEdit>
#include <QWidgetAction>
#include <QTimer>
#include "menuimporter.h"
class HDBusMenuImporter;
#include "dbusmenu-qt6/dbusmenuimporter.h"
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
    //void close();
    QMetaObject::Connection m_con;
    std::chrono::high_resolution_clock::time_point lastOpened;
    int reopencnt;





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
signals:
    void escapeFromSearch();
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
                                                << "visible"
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
    void convertFromQMenu(QMenu *menu);
    QMenuBar * menubar;
    //bool event(QEvent *event) override;
    QMap<uint,QStringList> m_windowMenus;
    QMap<uint,QMenu*> m_menuDbusMenus;
    HDBusMenuImporter * m_menuImporter;

public: signals:
    void setsearchfocus();
    void windowReg();
    void windowChanged();
private:
    void searchMenu(QMenu *item, QStringList names, QString searchTerm);
    void beforeSearch(QMenu *item);
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
    }
    QMenu *createMenu(QWidget *parent) override
    {
        if(parent==0) {
            parent = qobject_cast<QWidget*>(this->parent());
    }
        HsMenu *menu  = new HsMenu(parent);
        return menu;
    }
    public:
        QTimer *m_reshowTimer;
        HsMenu *recent;





};
