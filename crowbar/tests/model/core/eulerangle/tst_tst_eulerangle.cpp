#include <QString>
#include <QtTest>

class tst_EulerAngle : public QObject
{
    Q_OBJECT

public:
    tst_EulerAngle();

private Q_SLOTS:
    void implement_tests();
};

tst_EulerAngle::tst_EulerAngle()
{
}

void tst_EulerAngle::implement_tests()
{
    QFAIL("Not implemented yet.");
}

QTEST_APPLESS_MAIN(tst_EulerAngle)

#include "tst_tst_eulerangle.moc"
