#include "bookmanage.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BookManage w;

    return a.exec();
}
