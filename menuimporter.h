#ifndef MENUIMPORTER_H
#define MENUIMPORTER_H
#include <QVariant>
#include <QObject>
#include <QDebug>
#include<QtDBus/QDBusContext>
#include <QtDBus/QDBusAbstractInterface>
#include <QtDBus/QtDBus>
class MenuImporter : public QObject,  protected QDBusContext
{
    Q_OBJECT
public:
    explicit MenuImporter(QObject *parent = nullptr);
    QString m_service;
    QDBusObjectPath m_objectPath;
public slots:
    QString GetMenuForWindow(uint windowId, QDBusObjectPath &menuObjectPath);
    Q_NOREPLY void RegisterWindow(uint windowId, const QDBusObjectPath &menuObjectPath);
    Q_NOREPLY void UnregisterWindow(uint windowId);
signals:
    void windowRegistered(uint winId, QString service,QString path);
    void windowUnregistered(uint winId);
};

#endif // MENUIMPORTER_H
