# Menu2
attempt revamp of helloSystem Menu
Still very experimental. Only imports menu on registerwindow atm.
I patched QtPlugin as follows
```diff
diff --git a/platformthemeplugin/qdbusmenubar.cpp b/platformthemeplugin/qdbusmenubar.cpp
index a0acc54..dd7e2ef 100644
--- a/platformthemeplugin/qdbusmenubar.cpp
+++ b/platformthemeplugin/qdbusmenubar.cpp
@@ -101,7 +101,7 @@ void QDBusMenuBar::handleReparent(QWindow *newParentWindow)
             return;
 
         QDBusMenuRegistrarInterface registrar(REGISTRAR_SERVICE, REGISTRAR_PATH, connection, this);
-        QDBusPendingCall async = registrar.asyncCall("RegisterWindow",static_cast<uint>(window()->winId()), QVariant(m_objectPath));
+        QDBusPendingCall async = registrar.asyncCall("RegisterWindow",static_cast<uint>(window()->winId()), QDBusObjectPath(m_objectPath));
         QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(async, this);
 
               QObject::connect(watcher,&QDBusPendingCallWatcher::finished,
```
