#include "menuimporter.h"

MenuImporter::MenuImporter(QObject *parent) : QObject{parent} {

}
void MenuImporter::RegisterWindow(uint id, const QDBusObjectPath &path) {


    qDebug() << id << path.path() << __FILE__ ":" << __LINE__ << __func__<< message().service();
    m_service = message().service();
    m_objectPath = path.path();
    emit windowRegistered();
}

void MenuImporter::UnregisterWindow(uint windowId) {
    m_objectPath = "";
    m_service ="";
    emit windowUnregistered();

}


QString MenuImporter::GetMenuForWindow(uint windowId,
                                       QDBusObjectPath &menuObjectPath) {
    qDebug() << __func__;
  return m_objectPath;
}

