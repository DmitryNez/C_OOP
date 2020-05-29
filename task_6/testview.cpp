#include "testview.h"

void TestView::TestLineEdit(){
    QLineEdit LineEdit;

    QTest::keyClicks(&LineEdit, "1");

    QCOMPARE(LineEdit.text(),"1");
}

void TestView::TestCalculateButton(){
   QTest::mouseClick(v.ui->Calculate, Qt::LeftButton);
   QVERIFY2(v.ui->PressurePlot->plottable(),"Button 'Calculate' does not work");
}

void TestView::TestMinSize(){
    QVERIFY2(v.minimumWidth() == 635, "Minimum width is not correct");
    QVERIFY2(v.minimumHeight() == 518, "Minimum height is not correct");
}
