//
// Created by strongheart on 7/9/18.
//

#ifndef PARTICLE_ATTRACTOR_OPENGL_KERNEL_H
#define PARTICLE_ATTRACTOR_OPENGL_KERNEL_H

void kernel_reset(float4* verts, float4* states,
                  int ww, int wh,
                  int mesh_width, int mesh_height,
                  unsigned int numSMs);

void kernel_advance(float4* verts, float4* states,
                    float mx, float my,
                    int mesh_count,
                    float speed,
                    unsigned int numSMs,
                    double delta);

#endif //PARTICLE_ATTRACTOR_OPENGL_KERNEL_H
