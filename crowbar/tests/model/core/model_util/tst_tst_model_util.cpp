#include <QString>
#include <QtTest>

class tst_model_util : public QObject
{
    Q_OBJECT

public:
    tst_model_util();

private Q_SLOTS:
    void implement_tests();
};

tst_model_util::tst_model_util()
{
}

void tst_model_util::implement_tests()
{
    QFAIL("Not implemented yet.");
}

QTEST_APPLESS_MAIN(tst_model_util)

#include "tst_tst_model_util.moc"
