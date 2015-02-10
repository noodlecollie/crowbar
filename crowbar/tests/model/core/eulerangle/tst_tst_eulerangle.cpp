#include <QString>
#include <QtTest>
#include "core/eulerangle.h"

using namespace MODEL_NAMESPACE;

class tst_EulerAngle : public QObject
{
    Q_OBJECT

public:
    tst_EulerAngle()
    {
    }

private Q_SLOTS:
    void defaultConstructorZeroesAngles()
    {
        EulerAngle angle;
        QVERIFY2(angle.pitch() == 0.0f && angle.yaw() == 0.0f && angle.roll() == 0.0f,
                  "Default constructed angle does not zero its values.");
    }

    void constructorSetsAnglesCorrectly()
    {
        EulerAngle angle(10.0f, -130.25f, 18.3f);
        QVERIFY2(angle.pitch() == 10.0f && angle.yaw() == -130.25f && angle.roll() == 18.3f,
                 "Constructor does not set angles correctly.");
    }

    void setGetPitch()
    {
        EulerAngle angle;
        angle.setPitch(123.45f);
        QVERIFY2(angle.pitch() == 123.45f, "Pitch is not set correctly.");
    }

    void setGetYaw()
    {
        EulerAngle angle;
        angle.setYaw(376.324f);
        QVERIFY2(angle.yaw() == 376.324f, "Yaw is not set correctly.");
    }

    void setGetRoll()
    {
        EulerAngle angle;
        angle.setRoll(182.54f);
        QVERIFY2(angle.roll() == 182.54f, "Roll is not set correctly.");
    }
    void equalityComparison()
    {
        EulerAngle angle1(12.0f, 34.56f, -98.7f);
        EulerAngle angle2(12.0f, 34.56f, -98.7f);
        QVERIFY2(angle1 == angle2, "Identical angles are not considered equal.");
    }

    void inequalityComparison()
    {
        EulerAngle angle1(12.0f, 34.56f, -98.7f);
        EulerAngle angle2(145.0f, -23.34f, 98.0f);
        QVERIFY2(angle1 != angle2, "Non-identical angles are not considered non-equal.");
    }

    void additionOperator()
    {
        EulerAngle angle1(10.0f, 20.0f, 33.3f);
        EulerAngle angle2(12.0f, -10.0f, 33.3f);
        EulerAngle angle3(22.0f, 10.0f, 66.6f);
        QVERIFY2(angle1 + angle2 == angle3, "Values in angles not added correctly.");
    }

    void subtractionOperator()
    {
        EulerAngle angle1(10.0f, 20.0f, 33.3f);
        EulerAngle angle2(12.0f, -10.0f, 33.3f);
        EulerAngle angle3(-2.0f, 30.0f, 0.0f);
        QVERIFY2(angle1 - angle2 == angle3, "Values in angles not subtracted correctly.");
    }

    void selfAdditionOperator()
    {
        EulerAngle angle1(10.0f, 20.0f, 33.3f);
        EulerAngle angle2(12.0f, -10.0f, 33.3f);
        angle1 += angle2;
        EulerAngle angle3(22.0f, 10.0f, 66.6f);
        QVERIFY2(angle1 == angle3, "Values in angles not self-added correctly.");
    }

    void selfSubtractionOperator()
    {
        EulerAngle angle1(10.0f, 20.0f, 33.3f);
        EulerAngle angle2(12.0f, -10.0f, 33.3f);
        angle1 -= angle2;
        EulerAngle angle3(-2.0f, 30.0f, 0.0f);
        QVERIFY2(angle1 == angle3, "Values in angles not self-subtracted correctly.");
    }

    void convertVectorToAngle()
    {
        QFAIL("Not implemented yet.");
    }

    void convertAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void normaliseAngle()
    {
        EulerAngle angle(120.0f, 720.0f, -400.36f);
        angle.normalise();
        QVERIFY2(angle.pitch() == 120.0f && angle.yaw() == 120.0f && angle.roll() == 319.64f,
                 "Angle not normalised correctly.");
    }

    void getNormalisedAngle()
    {
        EulerAngle angle1(120.0f, 720.0f, -400.36f);
        EulerAngle angle2 = angle1.normalised();
        QVERIFY2(angle2.pitch() == 120.0f && angle2.yaw() == 120.0f && angle2.roll() == 319.64f,
                 "Angle not normalised correctly.");
    }

    void normaliseAngleForCamera()
    {
        QFAIL("Not implemented yet.");
    }

    void makeAngleOpposite()
    {
        QFAIL("Not implemented yet.");
    }

    void generateRotationMatrix()
    {
        QFAIL("Not implemented yet.");
    }

    void check000AngleToVector()
    {
        EulerAngle angle(0,0,0);
        QVector3D vec = angle.toVector();
        QVERIFY2(vec == QVector3D(1,0,0), "(0,0,0) angle does not correspond to (1,0,0) vector.");
    }

    void checkNegPitchAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkPosPitchAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void check180PitchAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkNegYawAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkPosYawAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void check180YawAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkNegRollAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkPosRollAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void check180RollAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkArbitraryAngleToVector()
    {
        QFAIL("Not implemented yet.");
    }

    void checkArbitraryVectorToAngle()
    {
        QFAIL("Not implemented yet.");
    }
};

QTEST_APPLESS_MAIN(tst_EulerAngle)

#include "tst_tst_eulerangle.moc"
