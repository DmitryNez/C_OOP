#ifndef TESTVIEW_H
#define TESTVIEW_H
#include <QtWidgets>
#include <QtTest/QtTest>
#include "view.h"

class TestView: public QObject
{
    Q_OBJECT

private slots:
    void TestLineEdit();
    void TestMinSize();
    void TestPushButton();
private:
    view v;
};
#endif // TESTVIEW_H
