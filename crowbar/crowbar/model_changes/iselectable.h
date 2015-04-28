#ifndef ISELECTABLE_H
#define ISELECTABLE_H

class ISelectable
{
public:
    virtual ~ISelectable() {}

    virtual bool selected() const = 0;
    virtual void setSelected(bool isSelected) = 0;
};

#endif // ISELECTABLE_H
