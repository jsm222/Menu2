#ifndef MENUIMPORTER_H
#define MENUIMPORTER_H

#include <QObject>
#include <QDebug>
#include<QtDBus/QDBusContext>
#include <QtDBus/QDBusAbstractInterface>
class MenuImporter : public QObject,  protected QDBusContext
{
    Q_OBJECT
public:
    explicit MenuImporter(QObject *parent = nullptr);
    QString m_service;
    QString m_objectPath;
    Q_INVOKABLE QString GetMenuForWindow(uint windowId, QDBusObjectPath &menuObjectPath);
    Q_INVOKABLE void RegisterWindow(uint windowId, const QDBusObjectPath &menuObjectPath);
    Q_INVOKABLE void UnregisterWindow(uint windowId);
signals:
    void windowRegistered();
    void windowUnregistered();
};

#endif // MENUIMPORTER_H
