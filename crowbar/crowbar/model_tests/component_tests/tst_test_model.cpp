#include <QString>
#include <QtTest>
#include <QVector3D>
#include "eulerangle.h"
#include <QtDebug>
#include "model_util.h"
#include "hierarchytransform_new.h"

using namespace MODEL_NAMESPACE;

class Test_Model : public QObject
{
    Q_OBJECT
    
public:
    Test_Model();
    
private slots:
    // EulerAngle
    void testEulerAngle_EmptyConstructor();
    void testEulerAngle_Constructor();
    void testEulerAngle_SetPitch();
    void testEulerAngle_SetYaw();
    void testEulerAngle_SetRoll();
    void testEulerAngle_SetFromVector();
    void testEulerAngle_ConvertToVector();
    void testEulerAngle_Normalise();
    void testEulerAngle_Equality();
    void testEulerAngle_Opposite();
    void testEulerAngle_Assignment();
    void testEulerAngle_Addition();
    void testEulerAngle_Subtraction();
    void testEulerAngle_AdditionSelf();
    void testEulerAngle_SubtractionSelf();
};

Test_Model::Test_Model()
{
}

void Test_Model::testEulerAngle_EmptyConstructor()
{
    const EulerAngle angle;
    
    QVERIFY2(qFuzzyIsNull(angle.pitch()) &&
             qFuzzyIsNull(angle.yaw()) &&
             qFuzzyIsNull(angle.roll()),
             "Constructor does not set angles to 0 by default.");
}

void Test_Model::testEulerAngle_Constructor()
{
    const EulerAngle angle(-20.0f, 15.0f, 12.2f);
    
    QVERIFY2(qFuzzyCompare(angle.pitch(), -20.0f) &&
             qFuzzyCompare(angle.yaw(), 15.0f) &&
             qFuzzyCompare(angle.roll(), 12.2f),
             "Constructor does not set angles correctly.");
}

void Test_Model::testEulerAngle_SetPitch()
{
    EulerAngle angle;
    angle.setPitch(-15.23f);
    QVERIFY2(qFuzzyCompare(angle.pitch(), -15.23f), "Pitch not set correctly.");
}

void Test_Model::testEulerAngle_SetYaw()
{
    EulerAngle angle;
    angle.setYaw(123.8f);
    QVERIFY2(qFuzzyCompare(angle.yaw(), 123.8f), "Yaw not set correctly.");
}

void Test_Model::testEulerAngle_SetRoll()
{
    EulerAngle angle;
    angle.setRoll(-172.0f);
    QVERIFY2(qFuzzyCompare(angle.roll(), -172.0f), "Roll not set correctly.");
}

void Test_Model::testEulerAngle_SetFromVector()
{
    EulerAngle angle = EulerAngle::vectorToAngle(QVector3D(1.0f, 1.0f, qSqrt(2.0f)));
    //qDebug() << "Pitch:" << angle.pitch() << "Yaw:" << angle.yaw() << "Roll:" << angle.roll();
    QVERIFY2(qFuzzyCompare(angle.pitch(), 315.0f) &&
             qFuzzyCompare(angle.yaw(), 45.0f) &&
             qFuzzyIsNull(angle.roll()),
             "Angles not set correctly from vector.");
}

void Test_Model::testEulerAngle_ConvertToVector()
{
    EulerAngle angle(-90.0f, 0.0f, 0.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle), QVector3D(0.0f, 0.0f, 1.0f)),
             "Up angle not correctly converted to vector.");
    
    angle = EulerAngle(90.0f, 0.0f, 0.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle), QVector3D(0.0f, 0.0f, -1.0f)),
             "Down angle not correctly converted to vector.");
    
    angle = EulerAngle(0.0f, 0.0f, 0.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle), QVector3D(1.0f, 0.0f, 0.0f)),
             "East angle not correctly converted to vector.");
    
    angle = EulerAngle(0.0f, 180.0f, 0.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle), QVector3D(-1.0f, 0.0f, 0.0f)),
             "West angle not correctly converted to vector.");
    
    angle = EulerAngle(0.0f, 90.0f, 0.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle), QVector3D(0.0f, 1.0f, 0.0f)),
             "North angle not correctly converted to vector.");
    
    angle = EulerAngle(0.0f, 270.0f, 0.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle), QVector3D(0.0f, -1.0f, 0.0f)),
             "South angle not correctly converted to vector.");
    
    angle = EulerAngle(10.0f, 20.0f, 30.0f);
    QVERIFY2(Model_Util::fuzzyVectorCompare(EulerAngle::angleToVector(angle),
             QVector3D(qCos(qDegreesToRadians(20.0f)) * qCos(qDegreesToRadians(10.0f)),
                       qSin(qDegreesToRadians(20.0f)) * qCos(qDegreesToRadians(10.0f)),
                       -qSin(qDegreesToRadians(10.0f)))),
             "Arbitrary angle not correctly converted to vector.");
}

void Test_Model::testEulerAngle_Normalise()
{
    EulerAngle angle(1720.4f, -439.0f, 125.4f);
    EulerAngle angle2 = angle.normalised();
    angle.normalise();
    
    QVERIFY2(qFuzzyCompare(angle.pitch(), 280.4f) &&
             qFuzzyCompare(angle.yaw(), 281.0f) &&
             qFuzzyCompare(angle.roll(), 125.4f),
             "Angle not normalised correctly.");
    
    QVERIFY2(qFuzzyCompare(angle2.pitch(), 280.4f) &&
             qFuzzyCompare(angle2.yaw(), 281.0f) &&
             qFuzzyCompare(angle2.roll(), 125.4f),
             "Returned angle not normalised correctly.");
}

void Test_Model::testEulerAngle_Equality()
{
    EulerAngle angle(100.0f, 120.0f, 140.139f);
    EulerAngle angle2(100.0f, 120.0f, 140.139f);
    EulerAngle angle3(130.0f, 120.0f, 140.0f);
    
    QVERIFY2(angle == angle2 && angle != angle3,
             "Equality and inequality not computed correctly.");
}

void Test_Model::testEulerAngle_Opposite()
{
    EulerAngle angle(100.0f, 12.0f, 13.2f);
    EulerAngle angle2 = angle.opposite();
    angle.makeOpposite();
    
    QVERIFY2(qFuzzyCompare(angle.pitch(), -100.0f) &&
             qFuzzyCompare(angle.yaw(), 192.0f) &&
             qFuzzyCompare(angle.roll(), 13.2f),
             "Opposite angle not computed correctly.");
    
    QVERIFY2(qFuzzyCompare(angle2.pitch(), -100.0f) &&
             qFuzzyCompare(angle2.yaw(), 192.0f) &&
             qFuzzyCompare(angle2.roll(), 13.2f),
             "Opposite angle not returned correctly.");
}

void Test_Model::testEulerAngle_Assignment()
{
    EulerAngle angle(1.0f, 2.0f, 3.0f);
    EulerAngle angle2(0.5f, 1.0f, 1.34f);
    angle = angle2;
    QVERIFY2(qFuzzyCompare(angle.pitch(), angle2.pitch()) &&
             qFuzzyCompare(angle.yaw(), angle2.yaw()) &&
             qFuzzyCompare(angle.roll(), angle2.roll()),
             "Assignment operator not implemented correctly.");
}

void Test_Model::testEulerAngle_Addition()
{
    EulerAngle angle(1.0f, 1.1f, 2.3f);
    EulerAngle angle2(1.25f, -0.5f, 1.2345f);
    EulerAngle angle3 = angle + angle2;
    
    QVERIFY2(qFuzzyCompare(angle3.pitch(), angle.pitch() + angle2.pitch()) &&
             qFuzzyCompare(angle3.yaw(), angle.yaw() + angle2.yaw()) &&
             qFuzzyCompare(angle3.roll(), angle.roll() + angle2.roll()),
             "Angle addition is not computed correctly.");
}

void Test_Model::testEulerAngle_Subtraction()
{
    EulerAngle angle(1.0f, 1.1f, 2.3f);
    EulerAngle angle2(1.25f, -0.5f, 1.2345f);
    EulerAngle angle3 = angle - angle2;
    
    QVERIFY2(qFuzzyCompare(angle3.pitch(), angle.pitch() - angle2.pitch()) &&
             qFuzzyCompare(angle3.yaw(), angle.yaw() - angle2.yaw()) &&
             qFuzzyCompare(angle3.roll(), angle.roll() - angle2.roll()),
             "Angle subtraction is not computed correctly.");
}

void Test_Model::testEulerAngle_AdditionSelf()
{
    EulerAngle angle(1.0f, 1.1f, 2.3f);
    EulerAngle angle2(1.25f, -0.5f, 1.2345f);
    angle += angle2;
    
    QVERIFY2(qFuzzyCompare(angle.pitch(), 1.0f + angle2.pitch()) &&
             qFuzzyCompare(angle.yaw(), 1.1f + angle2.yaw()) &&
             qFuzzyCompare(angle.roll(), 2.3f + angle2.roll()),
             "Angle self-addition is not computed correctly.");
}

void Test_Model::testEulerAngle_SubtractionSelf()
{
    EulerAngle angle(1.0f, 1.1f, 2.3f);
    EulerAngle angle2(1.25f, -0.5f, 1.2345f);
    angle -= angle2;
    
    QVERIFY2(qFuzzyCompare(angle.pitch(), 1.0f - angle2.pitch()) &&
             qFuzzyCompare(angle.yaw(), 1.1f - angle2.yaw()) &&
             qFuzzyCompare(angle.roll(), 2.3f - angle2.roll()),
             "Angle self-subtraction is not computed correctly.");
}

QTEST_APPLESS_MAIN(Test_Model)

#include "tst_test_model.moc"
