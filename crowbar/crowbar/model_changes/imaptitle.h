#ifndef IMAPTITLE_H
#define IMAPTITLE_H

// Map handle is an integer; map title is a string.
// Both can be used in addressing.

#include "model_global.h"
#include <QString>

#define INVALID_TITLE   QString()

MODEL_BEGIN_NAMESPACE

class IMapTitle
{
public:
    virtual ~IMapTitle() {}
    
    virtual void setTitle(const QString &title) = 0;
    virtual QString title() const = 0;
};

MODEL_END_NAMESPACE

#endif // IMAPTITLE_H
