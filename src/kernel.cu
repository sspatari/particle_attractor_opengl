#include <cuda.h>
#include <cuda_runtime.h>

/* initialize grid
 * we'll distribute all particles evenly on the screen
 */
__global__ void d_reset(float4* verts, float4* states,
                         float ww, float wh,
                         unsigned int mesh_width, unsigned int mesh_height)
{
    //grid-stride loop
    for (unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
         y < mesh_height;
         y += blockDim.y * gridDim.y)
    {
        for (unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
             x < mesh_width;
             x += blockDim.x * gridDim.x)
        {
            int vi = y*mesh_width + x;
            float u, v, w;
            u = ww * (x*1.f/mesh_width);
            v = wh * (y*1.f/mesh_height);
            w = 0.0f;
            verts[vi].x = u;
            verts[vi].y = v;
            verts[vi].z = w;
            states[vi].x = 0.0f;
            states[vi].y = 0.0f;
            states[vi].z = 0.0f;
            /* color information is uploaded by host, no init here */
        }
    }
}

/*
 * calculate particle forces & new positions & colors
 */
__global__ void d_advance(float4* verts, float4* states,
                           float mx, float my,
                           unsigned int mesh_count,
                           float speed,
                           float delta)
{
    for (unsigned int vi = blockIdx.x * blockDim.x + threadIdx.x;
         vi < mesh_count;
         vi += blockDim.x * gridDim.x)
    {
        // colors are stored after all vertices
        unsigned int ci = mesh_count + vi;

        float3 state  = {states[vi].x, states[vi].y, 0.f/*states[vi].z*/};

        // calculate vector between mouse and particle
        float3 dir_force = {verts[vi].x-mx, verts[vi].y-my, 0.f};
        /* steering:
         * the new particle state vector is given by current directional force
         * and the old state scaled by the particle mass
         */
        float flen = rsqrtf( dir_force.x*dir_force.x
                             +dir_force.y*dir_force.y
            /*+dir_force.z*dir_force.z*/);
        dir_force.x *= flen;
        dir_force.y *= flen;
        /*dir_force.z *= flen;*/
        state.x += delta * dir_force.x;
        state.y += delta * dir_force.y;
        /*state.z += dir_force.z;*/

        // speed: base speed scale by particle mass
        float v = speed / states[vi].w;
        /* update particle position:
         * new_position = old_position - state_vector*speed;
         */
        float3 dv = {state.x * v, state.y * v, 0.f/*state.z * v*/};

        verts[vi].x -= delta * dv.x;
        verts[vi].y -= delta * dv.y;
        /*verts[vi].z -= delta * dv.z;*/

        // update colors depending on particles force
        float dx = 0.5f*dv.x;
        float dy = 0.5f*dv.y;

        verts[ci].x = 0.1f+fabs(dx);
        verts[ci].y = 0.1f+fabs(dy);
        verts[ci].z = 0.1f+fabs(dx+dy);
        states[vi].x = state.x;
        states[vi].y = state.y;
        /*states[vi].z = state.z;*/
    }
}

void kernel_reset(float4* verts, float4* states,
                  int ww, int wh,
                  unsigned int mesh_width, unsigned int mesh_height,
                  unsigned int numSMs)
{
    dim3 threads(32,32);
    dim3 blocks(32*numSMs);

    d_reset<<<blocks, threads>>>( verts, states,
        ww, wh,
        mesh_width, mesh_height );
}

void kernel_advance(float4* verts, float4* states,
                    float mx, float my,
                    unsigned int mesh_count,
                    float speed,
                    unsigned int numSMs,
                    double delta)
{
    dim3 threads(1024);
    dim3 blocks( 32*numSMs );

    d_advance<<<blocks, threads>>>( verts, states,
        mx, my,
        mesh_count,
        speed,
        static_cast<float>(delta));
}