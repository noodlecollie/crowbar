#include "commandsuggestionlist.h"
#include <QString>

CommandSuggestionList::CommandSuggestionList(QWidget *parent) :
    QListWidget(parent)
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
