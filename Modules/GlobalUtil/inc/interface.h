#ifndef INTERFACE_H
#define INTERFACE_H

// Taken from http://www.codeproject.com/Articles/10553/Using-Interfaces-in-C - these defines look pretty!

#define INTERFACE class

#define DECLARE_INTERFACE(name) INTERFACE name { \
          public: \
          virtual ~name() {}

#define DECLARE_BASED_INTERFACE(name, base) class name : public base { \
          public: \
          virtual ~name() {}

#define INTERFACE_FUNC virtual

#define END_INTERFACE };

#define implements public

#endif // INTERFACE_H
