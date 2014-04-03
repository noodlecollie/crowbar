#include <QGuiApplication>
#include <Qt3D/QBox3D>
#include <QVector3D>
#include <QTime>
#include "worldculltreenode.h"
#include "renderbox.h"

using namespace NOctree;
using namespace NGeometry;

class CustomObject
{
public:
	  CustomObject() {}
	  
	  void customFunction()
	  {
		   qDebug("Custom function called!");
	  }
};

template<typename T>
class A
{
public:
	   A() {}
	   
	   void execute(T obj)
	   {
			// Call object's custom function.
			obj.customFunction();
	   }
};

template<typename T>
class B
{
public:
	   B() {}
	   
	   void execute(T obj)
	   {
			helper(obj);
	   }
	   
protected:
		  virtual void helper(T obj)
		  {
               Q_UNUSED(obj)
			   qDebug("WRONG FUNCTION CALLED!");
               Q_ASSERT(false);
		  }
};

template<typename T>
class C : public B<T>
{
public:
	   C() : B<T>() {}
	   
protected:
		  virtual void helper(T obj)
		  {
				  qDebug("Correct helper called for object template.");
				  obj.customFunction();
		  }
};

template<typename T>
class C<T*> : public B<T*>
{
public:
	   C() : B<T*>() {}

protected:
		  virtual void helper(T* obj)
		  {
				  qDebug("Correct helper called for pointer template.");
				  obj->customFunction();
		  }
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    WorldCullTreeNode<IConstBBoxVolume*, 1024, 2> tree(QBox3D(QVector3D(-1024.0f, -1024.0f, -1024.0f), QVector3D(1024.0f, 1024.0f, 1024.0f)));
    qDebug("Tree stats: %d %d", tree.splitMinDimensions(), tree.splitMinObjects());
    
    RenderBox box(QVector3D(5.0f, 3.0f, 2.0f), QVector3D(-1.0f, -1.0f, -1.0f), QVector3D(1.0f, 1.0f, 1.0f));
    IConstBBoxVolume* cbbv =  dynamic_cast<IConstBBoxVolume*>(&box);
    Q_ASSERT(cbbv);
    
    tree.addObjectRecurse(cbbv);
    qDebug("First node that contains box: %p", tree.findObjectRecurse(cbbv));

#if 0
    CustomObject obj;
        
        // This works
        A<CustomObject> instance1;
        instance1.execute(obj);
        
        // This doesn't
        //A<CustomObject*> instance2;
        //instance2.execute(obj);    // <- This won't compile.
        
        // This works correctly.
        C<CustomObject> instance3;
        instance3.execute(obj);
        
        C<CustomObject*> instance4;
        instance4.execute(&obj);
#endif
    
    //return app.exec();
    return 0;
}
