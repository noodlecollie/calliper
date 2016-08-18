#ifndef RENDERER_GLOBAL_H
#define RENDERER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RENDERER_LIBRARY)
#  define RENDERERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RENDERERSHARED_EXPORT Q_DECL_IMPORT
#endif

/*
 * Ideally, we want to be able to get to a scenario where each batch is self-
 * contained with a group of vertex/index/etc. buffers, a shader name and any
 * other required attributes. We can then just draw each batch one after another.
 *
 * This is an initial reckoning of the frequency that attributes will change,
 * with the most frequently changing at the top:
 *
 * | Attribute          | When it changes
 * ---------------------------------------------------------------
 * | Modelworld matrix  | Every object (without prior optimisation)
 * | Textures           | Moderately often but lots of re-use across draws (also can be bindless)
 * | Draw mode/width    | Moderately often but vast majority drawn will be triangles
 * | Shader             | Few times per phase, many objects drawn per shader
 *
 * The camera, coord transform and projection matrices don't change within a phase.
 *
 * One possible route would be to traverse the scene graph and create one batch
 * per unique group of the four attributes in the table above. This would require
 * premultiplying all the vertices with the modelworld matrix; however, as the graph
 * is being traversed at the same time, the modelworld matrix would be up-to-date every
 * time it was needed. Perhaps threaded graph traversal would help too? At the end of
 * the day it depends on whether stalling the pipeline to set different modelworld
 * matrices wastes more or less time than multiplying every vertex with the matrix
 * beforehand. Stalling is only a single operation, whereas vertex preprocessing would
 * have to happen for every single vertex (and it's CPU-bound).
 *
 * The second answer from
 * http://stackoverflow.com/questions/30834945/render-multiple-models-in-opengl-with-a-single-draw-call
 * is interesting - batch the uploads of the matrix uniforms with a Uniform Buffer Object.
 * That would at least help to increase the number of objects per batch.
 *
 * Crucially, it should not be forgotten that once everything has been uploaded to the
 * GPU, it shouldn't need to be changed until/unless the object changes. Although things like
 * the textures being used, the modelworld matrix, etc. will change frequently throughout
 * one phase of the renderer, the data itself will seldom change between frames. Obviously
 * there'll be some things animating, some brushes being translated, etc, but the vast
 * majority of the world will stay static from one frame to the next. The only thing that
 * might make this slightly more complicated is when objects are culled as the camera
 * moves around - this is something we need to consider.
 *
 * One way of indexing a buffer of matrix uniforms without adding any more bytes to the
 * input attributes of the shader might be to hide the index in another attribute such
 * as the normal. The LSB from each of the X, Y and Z mantissas could be used to encode
 * an index between 0 and 7 - 8 matrices per batch would be quite a significant
 * improvement over just 1. We would however need to be careful of causing very small
 * floats to be interpreted as zero.
 */

#endif // RENDERER_GLOBAL_H
