#include "view.h"
#include <QApplication>
#include<gtest/gtest.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    view w;
    w.show();
    //return a.exec();
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();


}

TEST(TestCase,TestName){
    ASSERT_EQ(1,1);
}


