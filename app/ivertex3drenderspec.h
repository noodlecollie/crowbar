/*! \file ivertex3drenderspec.h
 * \brief Defines the interface between map vertices and the vertices used for rendering.
 *
 * This file describes the requirements for a map vertex in terms of what information is required by the rendering engine.
 * In order for a vertex to be rendered, it must implement these functions.
 */

#ifndef IVERTEX3DRENDERSPEC_H
#define IVERTEX3DRENDERSPEC_H

#include <QObject>

/**
 * @brief Defines the properties required to be exposed by renderable vertices.
 */
class IVertex3DRenderSpec
{
public:
    virtual ~IVertex3DRenderSpec() {}
    
    /**
     * @brief Returns the offset of this vertex from the beginning of the VBO,
     * in V3RS_TOTAL_DATA_TRANSFER steps.
     * @return Offset for this vertex.
     */
    virtual unsigned long V3RS_Offset() = 0;

    /**
     * @brief Fills an array with the position values for this vertex.
     * @param position Array to fill. Format is XYZ.
     */
    virtual void V3RS_Position(float position[3]) = 0;

    /**
     * @brief Fills an array with the colour values for this vertex.
     * @param colour Colour values. Format is RGBA, range 0-255.
     */
    virtual void V3RS_Colour(unsigned char colour[4]) = 0;

    /**
     * @brief Fils an array with the normal vector values for this vertex.
     * @param normal Normal array to fill. Format is XYZ.
     */
    virtual void V3RS_Normal(float normal[3]) = 0;

    /**
     * @brief Fills an array with the texture co-ordinate values for this vertex.
     * @param coords Array to fill. Format is XY.
     */
    virtual void V3RS_Texture_Coords(float coords[2]) = 0;

    // 3x4 bytes for pos
    // 4x1 byte for colour
    // 3x4 bytes for normal
    // 2x4 bytes for texture co-ords
    enum { V3RS_TOTAL_DATA_TRANSFER = 36 };
};

//! \def IVertex3DRenderSpec_iid
//! \brief Unique ID for IVertex3DRenderSpec interface.
#define IVertex3DRenderSpec_iid "Crowbar.Interfaces.IVertex3DRenderSpec"
Q_DECLARE_INTERFACE(IVertex3DRenderSpec, IVertex3DRenderSpec_iid)

#endif // IVERTEX3DRENDERSPEC_H
