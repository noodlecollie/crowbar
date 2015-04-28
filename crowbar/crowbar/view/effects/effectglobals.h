#ifndef EFFECTGLOBALS_H
#define EFFECTGLOBALS_H

#include "view_global.h"
#include <QByteArray>
#include <QVector4D>
#include <QColor>
#include <QGL>

VIEW_BEGIN_NAMESPACE

static char const litMaterialLightingShader[] =
#if !defined(QT_OPENGL_ES)
"uniform mediump vec3 sdli;\n"   // Direction of the light (must be normalized).
"uniform mediump vec3 pli;\n"    // Position of the light
"uniform mediump float pliw;\n"  // 0 for directional, 1 for positional.
"uniform mediump float srli;\n"  // Spotlight exponent for the light
"uniform mediump float crli;\n"  // Spotlight cutoff for the light
"uniform mediump float ccrli;\n" // Cosine of spotlight cutoff for the light
"uniform mediump float k0;\n"    // Constant attentuation factor for the light
"uniform mediump float k1;\n"    // Linear attentuation factor for the light
"uniform mediump float k2;\n"    // Quadratic attentuation factor for the light
"uniform mediump vec4 acm[2];\n" // Ambient color of the material and light
"uniform mediump vec4 dcm[2];\n" // Diffuse color of the material and light
"uniform mediump vec4 scm[2];\n" // Specular color of the material and light
"uniform mediump vec4 ecm[2];\n" // Emissive color and ambient scene color
"uniform mediump float srm[2];\n"// Specular exponent of the material
"uniform bool viewerAtInfinity;\n" // Light model indicates viewer at infinity
"uniform bool twoSided;\n"       // Light model indicates two-sided lighting

"varying mediump vec4 qColor;\n"
"varying mediump vec4 qSecondaryColor;\n"

"void qLightVertex(vec4 vertex, vec3 normal)\n"
"{\n"
"    int i, material;\n"
"    vec3 toEye, toLight, h;\n"
"    float angle, spot, attenuation;\n"
"    vec4 color, scolor;\n"
"    vec4 adcomponent, scomponent;\n"

    // Determine which material to use.
"    if (!twoSided || normal.z >= 0.0) {\n"
"        material = 0;\n"
"    } else {\n"
"        material = 1;\n"
"        normal = -normal;\n"
"    }\n"

    // Start with the material's emissive color and the ambient scene color,
    // which have been combined into the ecm parameter by the C++ code.
"    color = ecm[material];\n"
"    scolor = vec4(0, 0, 0, 0);\n"

    // Vector from the vertex to the eye position (i.e. the origin).
"    if (viewerAtInfinity)\n"
"        toEye = vec3(0, 0, 1);\n"
"    else\n"
"        toEye = normalize(-vertex.xyz);\n"

    // Determine the cosine of the angle between the normal and the
    // vector from the vertex to the light.
"    if (pliw == 0.0)\n"
"        toLight = normalize(pli);\n"
"    else\n"
"        toLight = normalize(pli - vertex.xyz);\n"
"    angle = max(dot(normal, toLight), 0.0);\n"

    // Calculate the ambient and diffuse light components.
"    adcomponent = acm[material] + angle * dcm[material];\n"

    // Calculate the specular light components.
"    if (angle != 0.0) {\n"
"        h = normalize(toLight + toEye);\n"
"        angle = max(dot(normal, h), 0.0);\n"
"        if (srm[material] != 0.0)\n"
"            scomponent = pow(angle, srm[material]) * scm[material];\n"
"        else\n"
"            scomponent = scm[material];\n"
"    } else {\n"
"        scomponent = vec4(0, 0, 0, 0);\n"
"    }\n"

    // Apply the spotlight angle and exponent.
"    if (crli != 180.0) {\n"
"        spot = max(dot(normalize(vertex.xyz - pli), sdli), 0.0);\n"
"        if (spot < ccrli) {\n"
"            adcomponent = vec4(0, 0, 0, 0);\n"
"            scomponent = vec4(0, 0, 0, 0);\n"
"        } else {\n"
"            spot = pow(spot, srli);\n"
"            adcomponent *= spot;\n"
"            scomponent *= spot;\n"
"        }\n"
"    }\n"

    // Apply attenuation to the colors.
"    if (pliw != 0.0) {\n"
"        attenuation = k0;\n"
"        if (k1 != 0.0 || k2 != 0.0) {\n"
"            float len = length(pli - vertex.xyz);\n"
"            attenuation += k1 * len + k2 * len * len;\n"
"        }\n"
"        color += adcomponent / attenuation;\n"
"        scolor += scomponent / attenuation;\n"
"    } else {\n"
"        color += adcomponent;\n"
"        scolor += scomponent;\n"
"    }\n"

    // Generate the final output colors.
"    float alpha = dcm[material].a;\n"
"    qColor = vec4(clamp(color.rgb, 0.0, 1.0), alpha);\n"
"    qSecondaryColor = clamp(scolor, 0.0, 1.0);\n"
"}\n";
#else
"uniform mediump vec3 sdli;\n"   // Direction of the light (must be normalized).
"uniform mediump vec3 pli;\n"    // Position of the light
"uniform mediump float pliw;\n"  // 0 for directional, 1 for positional.
"uniform mediump float srli;\n"  // Spotlight exponent for the light
"uniform mediump float crli;\n"  // Spotlight cutoff for the light
"uniform mediump float ccrli;\n" // Cosine of spotlight cutoff for the light
"uniform mediump vec4 acm;\n"    // Ambient color of the material and light
"uniform mediump vec4 dcm;\n"    // Diffuse color of the material and light
"uniform mediump vec4 scm;\n"    // Specular color of the material and light
"uniform mediump vec4 ecm;\n"    // Emissive color and ambient scene color
"uniform mediump float srm;\n"   // Specular exponent of the material
"uniform bool viewerAtInfinity;\n" // Light model indicates viewer at infinity

"varying mediump vec4 qColor;\n"
"varying mediump vec4 qSecondaryColor;\n"
"varying mediump vec4 qCombinedColor;\n"

"void qLightVertex(vec4 vertex, vec3 normal)\n"
"{\n"
"    vec3 toEye, toLight, h;\n"
"    float angle, spot;\n"
"    vec4 color, scolor;\n"

    // Vector from the vertex to the eye position (i.e. the origin).
"    if (viewerAtInfinity)\n"
"        toEye = vec3(0, 0, 1);\n"
"    else\n"
"        toEye = normalize(-vertex.xyz);\n"

    // Determine the cosine of the angle between the normal and the
    // vector from the vertex to the light.
"    if (pliw == 0.0)\n"
"        toLight = normalize(pli);\n"
"    else\n"
"        toLight = normalize(pli - vertex.xyz);\n"
"    angle = max(dot(normal, toLight), 0.0);\n"

    // Calculate the ambient and diffuse light components.
"    color = acm + angle * dcm;\n"

    // Calculate the specular light components.
"    if (angle != 0.0) {\n"
"        h = normalize(toLight + toEye);\n"
"        angle = max(dot(normal, h), 0.0);\n"
"        if (srm != 0.0)\n"
"            scolor = pow(angle, srm) * scm;\n"
"        else\n"
"            scolor = scm;\n"
"    } else {\n"
"        scolor = vec4(0, 0, 0, 0);\n"
"    }\n"

    // Apply the spotlight angle and exponent.
"    if (crli != 180.0) {\n"
"        spot = max(dot(normalize(vertex.xyz - pli), sdli), 0.0);\n"
"        if (spot < ccrli) {\n"
"            color = vec4(0, 0, 0, 0);\n"
"            scolor = vec4(0, 0, 0, 0);\n"
"        } else {\n"
"            spot = pow(spot, srli);\n"
"            color *= spot;\n"
"            scolor *= spot;\n"
"        }\n"
"    }\n"

    // Generate the final output colors.
"    color += ecm;\n"
"    float alpha = dcm.a;\n"
    // Note: Normally, the qCombinedColor is ignored, and per-pixel
    // value is calculated.
    // If OPENGL_ES is defined, qColor and qSecondaryColor are ignored,
    // and qCombinedColor is calculated here to speed up the fragment shader.
"    qColor = vec4(clamp(color.rgb, 0.0, 1.0), alpha);\n"
"    qSecondaryColor = clamp(scolor, 0.0, 1.0);\n"
"    qCombinedColor = clamp(qColor + vec4(qSecondaryColor.xyz, 0.0), 0.0, 1.0);\n"
"}\n";
#endif

QByteArray VIEWSHARED_EXPORT createVertexSource(const char *lighting, const char *extra);
QVector4D VIEWSHARED_EXPORT colorToVector4(const QColor& color);
QVector4D VIEWSHARED_EXPORT colorToVector4 (const QColor &color, const QColor &lightColor);

void qt_gl_ClientActiveTexture(GLenum texture);

VIEW_END_NAMESPACE

#endif // EFFECTGLOBALS_H
