#include "irenderdata.h"

namespace IRenderData
{
    IRenderData::IRenderData(QObject *parent) : QObject(parent)
    {
        // Initialise the array of render categories.
        for ( int i = 0; i < CAT_MAX; i++ )
        {
            m_pRenderCategories[i] = new RenderCategoryList();
        }
        
        // Initialise array of data groups.
        for ( int i = 0; i < GROUPS_MAX; i++ )
        {
            m_pDataGroups[i] = new IRenderDataGroup();
        }
    }
    
    IRenderData::~IRenderData()
    {
        // Delete all render categories.
        for ( int i = 0; i < CAT_MAX; i++ )
        {
            if ( m_pRenderCategories[i] )
            {
                delete m_pRenderCategories[i];
                m_pRenderCategories[i] = NULL;
            }
        }
        
        // Delete all data groups.
        for ( int i = 0; i < GROUPS_MAX; i++ )
        {
            if ( m_pDataGroups[i] )
            {
                delete m_pDataGroups[i];
                m_pDataGroups[i] = NULL;
            }
        }
    }
    
    void IRenderData::draw(QGLPainter *painter, int start, int count, DataGroups group, RenderCategories category, GLenum mode, qreal drawWidth)
    {
        bool allGroups = (group >= GROUPS_MAX);
        bool allCats = (category >= CAT_MAX);
        
        // If we draw all, we can optimise a little.
        if ( allCats && allGroups )
        {
            // Iterate over the global category list.
            for ( int i = 0; i < CAT_MAX; i++ )
            {
                RenderCategoryList* list = m_pRenderCategories[i];
                if ( !list ) continue;
                
                
            }
        }
    }
    
    IRenderDataGroup::IRenderDataGroup(QObject *parent) : QObject(parent)
    {
        // Initialise all render categories.
        for ( int i = 0; i < IRenderData::CAT_MAX; i++ )
        {
            categories[i] = NULL;
        }
    }
    
    IRenderDataGroup::~IRenderDataGroup()
    {
        // Delete all render categories.
        for ( int i = 0; i < IRenderData::CAT_MAX; i++ )
        {
            if ( categories[i] )
            {
                delete categories[i];
                categories[i] = NULL;
            }
        }
    }
}
