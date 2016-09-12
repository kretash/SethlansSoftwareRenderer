
#include "tools/math.h"

float4 make_float4( float x, float y, float z, float w) {
  float4 r;
  r.x = x; r.y = y; r.z = z; r.w = w;
  return r;
}

float3 make_float3( float x, float y, float z ){
  float3 r;
  r.x = x; r.y = y; r.z = z;
  return r;
}

float3 sum_float3( float3 a, float3 b ) {
  float3 r;
  r.x = a.x + b.x; 
  r.y = a.y + b.y;
  r.z = a.z + b.z;
  return r;
}

void normalize( float3* d ) {
  float m = sqrtf( d->x * d->x + d->y * d->y + d->z * d->z );
  d->x /= m;
  d->y /= m;
  d->z /= m;
}

float3 half( float3 a, float3 b ){
  float3 sum = sum_float3( a, b );
  normalize( &sum );
  return sum;
}

float3 normal( const float3 f ) {
  float3 normal = f;
  float m = f.x * f.x + f.y * f.y + f.z * f.z;
  if( m == 0.0f ) return f;
  m = 1.0f / sqrtf( m );
  normal.x *= m;
  normal.y *= m;
  normal.z *= m;
  return normal;
}

float dot( const float3 d, const float3 f ) {
  float dot = d.x * f.x + d.y * f.y + d.z * f.z;
  return dot;
}

float3 cross( const float3 d, const float3 f ) {
  float3 r;
  r.x = d.y * f.z - d.z * f.y;
  r.y = d.z * f.x - d.x * f.z;
  r.z = d.x * f.y - d.y * f.x;
  return r;
}

float lenght( const float3 f ) {
  float square_me =
    f.x * f.x +
    f.y * f.y +
    f.z * f.z;
  return sqrtf( square_me );
}


void identity( float4x4* m ) {
  float tmp[16] =
  {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
  memcpy( m, tmp, sizeof( float ) * 16 );
}

float* get( float4x4* m ){
  return m->m;
}

float4x4 mult( float4x4* l, float4x4* r ){
  float4x4 result;

  result.m[0] = l->m[0] * r->m[0] + l->m[4] * r->m[1] + l->m[8] * r->m[2] + l->m[12] * r->m[3];
  result.m[1] = l->m[1] * r->m[0] + l->m[5] * r->m[1] + l->m[9] * r->m[2] + l->m[13] * r->m[3];
  result.m[2] = l->m[2] * r->m[0] + l->m[6] * r->m[1] + l->m[10] * r->m[2] + l->m[14] * r->m[3];
  result.m[3] = l->m[3] * r->m[0] + l->m[7] * r->m[1] + l->m[11] * r->m[2] + l->m[15] * r->m[3];

  result.m[4] = l->m[0] * r->m[4] + l->m[4] * r->m[5] + l->m[8] * r->m[6] + l->m[12] * r->m[7];
  result.m[5] = l->m[1] * r->m[4] + l->m[5] * r->m[5] + l->m[9] * r->m[6] + l->m[13] * r->m[7];
  result.m[6] = l->m[2] * r->m[4] + l->m[6] * r->m[5] + l->m[10] * r->m[6] + l->m[14] * r->m[7];
  result.m[7] = l->m[3] * r->m[4] + l->m[7] * r->m[5] + l->m[11] * r->m[6] + l->m[15] * r->m[7];

  result.m[8] = l->m[0] * r->m[8] + l->m[4] * r->m[9] + l->m[8] * r->m[10] + l->m[12] * r->m[11];
  result.m[9] = l->m[1] * r->m[8] + l->m[5] * r->m[9] + l->m[9] * r->m[10] + l->m[13] * r->m[11];
  result.m[10] = l->m[2] * r->m[8] + l->m[6] * r->m[9] + l->m[10] * r->m[10] + l->m[14] * r->m[11];
  result.m[11] = l->m[3] * r->m[8] + l->m[7] * r->m[9] + l->m[11] * r->m[10] + l->m[15] * r->m[11];

  result.m[12] = l->m[0] * r->m[12] + l->m[4] * r->m[13] + l->m[8] * r->m[14] + l->m[12] * r->m[15];
  result.m[13] = l->m[1] * r->m[12] + l->m[5] * r->m[13] + l->m[9] * r->m[14] + l->m[13] * r->m[15];
  result.m[14] = l->m[2] * r->m[12] + l->m[6] * r->m[13] + l->m[10] * r->m[14] + l->m[14] * r->m[15];
  result.m[15] = l->m[3] * r->m[12] + l->m[7] * r->m[13] + l->m[11] * r->m[14] + l->m[15] * r->m[15];

  return result;
}

float4 mult_v( float4x4* m, float4* v ){
  float r[4];

  for( unsigned int i = 0; i < 4; ++i ) {
    r[i] = ( m->m[0 + i] * v->x ) + ( m->m[4 + i] * v->y ) +
      ( m->m[8 + i] * v->z ) + ( m->m[12 + i] * v->w );
  }

  return make_float4( r[0], r[1], r[2], r[3]);
}

void translate( float4x4* m, float x, float y, float z ) {
  m->m[12] = x;
  m->m[13] = y;
  m->m[14] = z;
}

void rotate_y( float4x4* m, float a ) {
  m->m[0] = cosf( a );
  m->m[2] = sinf( a );
  m->m[8] = -sinf( a );
  m->m[10] = cosf( a );
}

void prespective( float4x4* m, float fov, float aspect_ratio, float n, float f ) {
  float tan_fov = tanf( fov / 2.0f );
  identity( m );
  m->m[0] = 1.0f / ( aspect_ratio*tan_fov );
  m->m[5] = 1.0f / tan_fov;
  m->m[10] = -( f + n ) / ( f - n );
  m->m[11] = -1.0f;
  m->m[14] = -( 2.0f * f * n ) / ( f - n );
}