#include "commandsuggestionlist.h"
#include <QString>

CommandSuggestionList::CommandSuggestionList(QWidget *parent) :
    QListWidget(parent), m_flHeightScale(1.0f)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setUniformItemSizes(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

QString CommandSuggestionList::getCurrentSelection()
{
    QListWidgetItem* i = currentItem();
    return i == NULL ? QString("") : i->text();
}

bool CommandSuggestionList::hasSelection() const
{
    return currentItem() != NULL;
}

void CommandSuggestionList::moveUp()
{
    if ( count() < 1 ) return;
    
    int next = currentRow() - 1;
    if ( next >= count() || next < 0 ) next = count() - 1;
    setCurrentRow(next);
}

void CommandSuggestionList::moveDown()
{
    if ( count() < 1 ) return;
    
    int next = currentRow() + 1;
    if ( next >= count() || next < 0 ) next = 0;
    setCurrentRow(next);
}

void CommandSuggestionList::selectFirst()
{
    if ( count() < 1 ) return;
    
    setCurrentRow(0);
}

float CommandSuggestionList::heightScale() const
{
    return m_flHeightScale;
}

void CommandSuggestionList::setHeightScale(float scale)
{
    m_flHeightScale = scale;
}

void CommandSuggestionList::resetHeightScale()
{
    m_flHeightScale = 1.0f;
}

void CommandSuggestionList::autoHeight()
{
    // By default, a scale of 1.0 means 20 pixels per item.
    // A larger scale means more height per item, a smaller scale means less height.
    int pixelsPerItem = qRound(18.0f * m_flHeightScale);
    
    // Calculate the height and clamp.
    int newHeight = pixelsPerItem * count();
    if ( newHeight > maximumHeight() ) newHeight = maximumHeight();
    
    // Set height depending on how many items we have, plus padding.
    resize(width(), newHeight + 4);
}

void CommandSuggestionList::autoWidth()
{
//    // Find largest width.
//    int width = 0;
//    for ( int i = 0; i < count(); i++ )
//    {
//        int chars = item(i)->text().length();
//        int tempwidth = qRound((float) chars * m_flWidthScale * 10.0f);
        
//        if ( tempwidth > width ) width = tempwidth;
        
//        // If we've reached the maximum width we don't need to process any more.
//        if ( width >= maximumWidth() )
//        {
//            width = maximumWidth();
//            break;
//        }
//    }
//    // Set to largest width.
//    resize(width, height());
    
    // FIXME: Making the width static for now unless/until we find a good way of doing this.
    resize(200, height());
}
