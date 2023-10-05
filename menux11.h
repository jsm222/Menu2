#ifndef MENUX11_H
#define MENUX11_H
#include <QObject>
#include <QString>
class MenuX11 : public QObject
{
    Q_OBJECT
public:
    MenuX11();
    void setStrut(unsigned long winId, int height,int width);
    unsigned long getActiveWindow();
};

#endif // MENUX11_H
