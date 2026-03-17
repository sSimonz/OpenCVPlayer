#include "widget.h"
#include "FindDiffer/finddifferwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Widget w;
    FindDifferWidget w;
    w.show();
    return a.exec();
}
