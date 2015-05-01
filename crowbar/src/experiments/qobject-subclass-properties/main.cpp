// Experiment to see whether subclasses of QObjects can have just their own properties
// identified, instead of including all of the superclass properties.

// Conclusions:
// propertyCount() gives all properties on all objects in hierarchy from QObject onwards.
// This will always be at least 1 because of QObject::name().
// propertyOffset() gives the offset in the list of properties at which the given object's
// properties start.
// The number of properties unique to a given object type can be determined by count - offset.
// Dynamically-added properties are not accessible via the meta-object, only via the object
// itself using QObject::property(). They do not affect the property count or offset.

#include <QCoreApplication>
#include <QtDebug>
#include "subclass.h"
#include "superclass.h"
#include "leafclass.h"
#include <QMetaProperty>

int propertiesOnClass(QObject* obj)
{
    return obj->metaObject()->propertyCount() - obj->metaObject()->propertyOffset();
}

void printAllProperties(QObject* obj)
{
    for ( int i = obj->metaObject()->propertyOffset(); i < obj->metaObject()->propertyCount(); i++ )
    {
        qDebug().nospace() << "Property " << i << ": " << QString(obj->metaObject()->property(i).name());
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Superclass super;
    Subclass sub;
    Leafclass leaf;
    const QMetaObject* mSuper = super.metaObject();
    const QMetaObject* mSub = sub.metaObject();
    const QMetaObject* mLeaf = leaf.metaObject();
    
    qDebug() << "Number of properties on superclass:" << Superclass::staticMetaObject.propertyCount()
             << "\nNumber of properties on subclass:" << Subclass::staticMetaObject.propertyCount()
             << "\nNumber of properties on leaf class:" << Leafclass::staticMetaObject.propertyCount();
    
    qDebug() << "Offset of properties on superclass:" << Superclass::staticMetaObject.propertyOffset()
             << "\nOffset of properties on subclass:" << Subclass::staticMetaObject.propertyOffset()
             << "\nOffset of properties on leaf class:" << Leafclass::staticMetaObject.propertyOffset();
    
    qDebug() << "Number of properties unique to superclass:" << propertiesOnClass(&super)
             << "\nNumber of properties unique to subclass:" << propertiesOnClass(&sub)
             << "\nNumber of propertues unique to leaf class:" << propertiesOnClass(&leaf);
    
    qDebug() << "Property at offset" << mSuper->propertyOffset() << "for superclass:" << QString(mSuper->property(mSuper->propertyOffset()).name())
             << "\nProperty at offset" << mSub->propertyOffset() << "for subclass:" << QString(mSub->property(mSub->propertyOffset()).name())
             << "\nProperty at offset" << mLeaf->propertyOffset() << "for leaf class:" << QString(mLeaf->property(mLeaf->propertyOffset()).name());
    
    qDebug() << "====================";
    
    Superclass* dynamicSuper = new Superclass();
    Subclass* dynamicSub = new Subclass();
    Leafclass* dynamicLeaf = new Leafclass();
    
    dynamicSuper->setProperty("dynamicSuperclassProperty", QVariant(2));
    dynamicSub->setProperty("dynamicSubclassProperty", QVariant(1));
    dynamicLeaf->setProperty("dynamicLeafclassProperty", QVariant(3));
    
    qDebug() << "Number of properties on dynamic superclass:" << dynamicSuper->metaObject()->propertyCount()
             << "\nNumber of properties on dynamic subclass:" << dynamicSub->metaObject()->propertyCount()
             << "\nNumber of properties on dynamic leaf class:" << dynamicLeaf->metaObject()->propertyCount();
    
    qDebug() << "Offset of properties on dynamic superclass:" << dynamicSuper->metaObject()->propertyOffset()
             << "\nOffset of properties on dynamic subclass:" << dynamicSub->metaObject()->propertyOffset()
             << "\nOffset of properties on dynamic leaf class:" << dynamicLeaf->metaObject()->propertyOffset();
    
    qDebug() << "Number of properties unique to dynamic superclass:" << propertiesOnClass(dynamicSuper)
             << "\nNumber of properties unique to dynamic subclass:" << propertiesOnClass(dynamicSub)
             << "\nNumber of properties unique to dynamic leaf class:" << propertiesOnClass(dynamicLeaf);
    
    qDebug() << "Properties on dynamic superclass:";
    printAllProperties(dynamicSuper);
    
    qDebug() << "Properties on dynamic subclass:";
    printAllProperties(dynamicSub);
    
    qDebug() << "Properties on dynamic leaf class:";
    printAllProperties(dynamicLeaf);
    
    delete dynamicSuper;
    delete dynamicSub;
    delete dynamicLeaf;
    return 0;
}
