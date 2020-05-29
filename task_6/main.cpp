#include "view.h"
#include <QApplication>
#include <QTest>
#include "testview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    w.show();
    QTest::qExec(new TestView, argc, argv);
    return a.exec();
}
