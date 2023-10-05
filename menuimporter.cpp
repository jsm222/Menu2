#include "menuimporter.h"

MenuImporter::MenuImporter(QObject *parent) : QObject{parent} {

}
void MenuImporter::RegisterWindow(uint id, const QDBusObjectPath &path) {


    qDebug() << id << path.path() << __FILE__ ":" << __LINE__ << __func__<< message().service();

    emit windowRegistered(id,message().service(),path.path());
    return;
}

void MenuImporter::UnregisterWindow(uint windowId) {

    emit windowUnregistered(windowId);
    return;
}


QString MenuImporter::GetMenuForWindow(uint windowId,
                                       QDBusObjectPath &menuObjectPath) {
    qDebug() << __func__ << menuObjectPath.path();
    menuObjectPath=m_objectPath;
    return m_service;
}

