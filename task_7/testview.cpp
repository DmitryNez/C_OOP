#include "testview.h"
void TestView::TestLineEdit(){
    QLineEdit LineEdit;

    QTest::keyClicks(&LineEdit, "1");

    QCOMPARE(LineEdit.text(),"1");
}

void TestView::TestMinSize(){
    QVERIFY2(v.minimumWidth() == 700, "Minimum width is not correct");
    QVERIFY2(v.minimumHeight() == 450, "Minimum height is not correct");
}

void TestView::TestPushButton(){
   QTest::mouseClick(v.ui->pushButton, Qt::LeftButton);
   QVERIFY2(v.ui->widget->plottable(),"Button 'Calculate' does not work");
}
