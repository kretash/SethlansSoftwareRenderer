#ifndef __MATH_H__
#define __MATH_H__ 1

#include <math.h>

typedef struct float4 {
  float x, y, z, w;
} float4;

float4 make_float4( float x, float y, float z, float w);

typedef struct float3 {
  float x, y, z;
} float3;

float3 make_float3( float x, float y, float z );
float3 sum_float3( float3 a, float3 b );
void normalize( float3* d );
float3 half( float3 a, float3 b );
float3 normal( const float3 f );
float dot( const float3 d, const float3 f );
float3 cross( const float3 d, const float3 f );
float lenght( const float3 f );

typedef struct float2 {
  float x, y;
} float2;

typedef struct float4x4 {
  float m[16];
} float4x4;

void identity( float4x4* m ) ;
float* get( float4x4* m );
float4x4 mult( float4x4* l, float4x4* r );
float4 mult_v( float4x4* m, float4* v );
void translate( float4x4* m, float x, float y, float z );
void rotate_y( float4x4* m, float a ) ;
void prespective( float4x4* m, float fov, float aspect_ratio, float n, float f );


#endif