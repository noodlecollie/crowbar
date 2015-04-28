#ifndef QANGLE_H
#define QANGLE_H

#include <QVector3D>
#include <QtDebug>

class QAngle
{
public:
    explicit QAngle();
    explicit QAngle(float pitch, float yaw, float roll);
    explicit QAngle(const QVector3D &direction);
    QAngle(const QAngle &other);
    
    QAngle& operator=(const QAngle &other);
    bool operator==(const QAngle &other);
    bool operator!=(const QAngle &other);
    QAngle operator+(const QAngle &other);
    QAngle operator-(const QAngle &other);
    QAngle& operator+=(const QAngle &other);
    QAngle& operator-=(const QAngle &other);
    
    static QAngle vectorToAngle(const QVector3D &vector);
    static QVector3D angleToVector(const QAngle &angle);
    
    float pitch() const;
    float yaw() const;
    float roll() const;
    
    void setPitch(float f);
    void setYaw(float f);
    void setRoll(float f);
    
    void addPitch(float f);
    void addYaw(float f);
    void addRoll(float f);
    
    void flipPitch();
    
    // Assumes pitch is [0 360) (it should be!)
    inline float pitchFlipped() const;
    
    inline void subtractPitch(float f)  {addPitch(-f);}
    inline void subtractYaw(float f)    {addYaw(-f);}
    inline void subtractRoll(float f)   {addRoll(-f);}
    
    inline QVector3D toVector() const;
    QVector3D toCameraVector() const;
    void add(const QAngle &angle);
    void subtract(const QAngle &angle);
    
    // Does not modify roll.
    inline QAngle inverse() const;
    void invert();
    
private:
    static inline float clampDegrees(float deg);
    
    float m_flPitch;
    float m_flYaw;
    float m_flRoll;
};

inline float QAngle::pitchFlipped() const
{
    return 360.0f - pitch();
}

inline QVector3D QAngle::toVector() const
{
    return angleToVector(*this);
}

inline float QAngle::clampDegrees(float deg)
{
    while ( deg >= 360.0f ) deg -= 360.0f;
    while ( deg < 0.0f ) deg += 360.0f;
    return deg;
}

inline QAngle QAngle::inverse() const
{
    // To invert, we:
    // - Modify pitch to point the same angle down instead of up, or vice versa.
    // - Rotate yaw by 180 degrees.
    // - Leave roll as it is.
    
    return QAngle(pitchFlipped(), yaw() + 180.0f, roll());
}

QDebug& operator<<(QDebug &dbg, const QAngle &angle);

#endif // QANGLE_H
