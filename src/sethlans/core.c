#include "sl/sethlans.h"

extern krContext* __c;

void slCreateContext() {

  //error handling
  __c = malloc( sizeof( krContext ) );
  memset( __c, 128, sizeof( krContext ) );

  __c->m_power_buffer_length = 16;
  __c->m_data_buffer_length = ( int ) powf( 2.0f, ( float ) __c->m_power_buffer_length );
  __c->m_data_buffer_pointer = 0;
  __c->m_data_buffer = malloc( __c->m_data_buffer_length );
#if DEBUG
  memset( __c->m_data_buffer, 0, __c->m_data_buffer_length );
#endif

}

void slGenBuffer( int* id, int lenght ) {
  int next_length = __c->m_data_buffer_pointer + lenght;

  while( next_length > __c->m_data_buffer_length ) {

    __c->m_power_buffer_length++;
    int old_lenght = __c->m_data_buffer_length;
    __c->m_data_buffer_length = ( int ) powf( 2.0f, ( float ) __c->m_power_buffer_length );

    void* new_buffer = malloc( __c->m_data_buffer_length );
#if DEBUG
    memset( new_buffer, 0, __c->m_data_buffer_length );
#endif
    memcpy( new_buffer, __c->m_data_buffer, old_lenght );

    free( __c->m_data_buffer );

    __c->m_data_buffer = new_buffer;

  }

  *id = __c->m_data_buffer_pointer;

  __c->m_data_buffer_pointer = next_length;
}

void slBindBuffer( int id ) {
  __c->m_bint_buffer = id;
}

void slBufferData( const void* data, int lenght ) {
  void* copy_to = ( char* ) __c->m_data_buffer + __c->m_bint_buffer;
  memcpy( copy_to, data, lenght );
}

float orient2d( const float3 a, const float3 b, const float2 c ) {
  return ( b.x - a.x )*( c.y - a.y ) - ( b.y - a.y )*( c.x - a.x );
}

float interpolate_floats( float3 pos, float3 x1, float3 x2, float3 x3, float v1, float v2, float v3 ) {

  float whole_area = 0.0f;
  float red_area = 0.0f;
  float green_area = 0.0f;
  float blue_area = 0.0f;

  float3 x1x2 = make_float3( x2.x - x1.x, x2.y - x1.y, 0.0f );
  float3 x1x3 = make_float3( x2.x - x3.x, x2.y - x3.y, 0.0f );
  float3 x3x1 = make_float3( x3.x - x1.x, x3.y - x1.y, 0.0f );
  float3 posx1 = make_float3( pos.x - x1.x, pos.y - x1.y, 0.0f );
  float3 posx2 = make_float3( pos.x - x2.x, pos.y - x2.y, 0.0f );
  float3 posx3 = make_float3( pos.x - x3.x, pos.y - x3.y, 0.0f );

  float l1 = abs( lenght( x1x2 ) );
  float l2 = abs( lenght( x1x3 ) );
  float l3 = abs( lenght( x3x1 ) );
  float lpos1 = abs( lenght( posx1 ) );
  float lpos2 = abs( lenght( posx2 ) );
  float lpos3 = abs( lenght( posx3 ) );

  {
    float s = ( l1 + l2 + l3 )*0.5f;
    whole_area = sqrtf( s * ( s - l1 ) * ( s - l2 ) * ( s - l3 ) );
  }
  {
    float s = ( l1 + lpos1 + lpos2 )*0.5f;
    red_area = sqrtf( s * ( s - l1 ) * ( s - lpos1 ) * ( s - lpos2 ) );
  }
  {
    float s = ( lpos2 + l2 + lpos3 )*0.5f;
    green_area = sqrtf( s * ( s - lpos2 ) * ( s - l2 ) * ( s - lpos3 ) );
  }
  {
    float s = ( lpos1 + lpos3 + l3 )*0.5f;
    blue_area = sqrtf( s * ( s - lpos1 ) * ( s - lpos3 ) * ( s - l3 ) );
  }

  float vv1 = ( ( red_area / whole_area ) * v1 );
  float vv2 = ( ( green_area / whole_area ) * v2 );
  float vv3 = ( ( blue_area / whole_area ) * v3 );

  return ( vv1 + vv2 + vv3 ) / 3.0f;
}

unsigned int get_color( float3 pos, float3 x1, float3 x2, float3 x3 ) {

  unsigned char r = 0;
  unsigned char g = 0;
  unsigned char b = 0;

  float whole_area = 0.0f;
  float red_area = 0.0f;
  float green_area = 0.0f;
  float blue_area = 0.0f;

  float3 x1x2 = make_float3( x2.x - x1.x, x2.y - x1.y, 0.0f );
  float3 x1x3 = make_float3( x2.x - x3.x, x2.y - x3.y, 0.0f );
  float3 x3x1 = make_float3( x3.x - x1.x, x3.y - x1.y, 0.0f );
  float3 posx1 = make_float3( pos.x - x1.x, pos.y - x1.y, 0.0f );
  float3 posx2 = make_float3( pos.x - x2.x, pos.y - x2.y, 0.0f );
  float3 posx3 = make_float3( pos.x - x3.x, pos.y - x3.y, 0.0f );

  float l1 = abs( lenght( x1x2 ) );
  float l2 = abs( lenght( x1x3 ) );
  float l3 = abs( lenght( x3x1 ) );
  float lpos1 = abs( lenght( posx1 ) );
  float lpos2 = abs( lenght( posx2 ) );
  float lpos3 = abs( lenght( posx3 ) );

  {
    float s = ( l1 + l2 + l3 )*0.5f;
    whole_area = sqrtf( s * ( s - l1 ) * ( s - l2 ) * ( s - l3 ) );
  }
  {
    float s = ( l1 + lpos1 + lpos2 )*0.5f;
    red_area = sqrtf( s * ( s - l1 ) * ( s - lpos1 ) * ( s - lpos2 ) );
  }
  {
    float s = ( lpos2 + l2 + lpos3 )*0.5f;
    green_area = sqrtf( s * ( s - lpos2 ) * ( s - l2 ) * ( s - lpos3 ) );
  }
  {
    float s = ( lpos1 + lpos3 + l3 )*0.5f;
    blue_area = sqrtf( s * ( s - lpos1 ) * ( s - lpos3 ) * ( s - l3 ) );
  }

  r = ( unsigned char ) ( ( red_area / whole_area ) * 0xff );
  g = ( unsigned char ) ( ( green_area / whole_area ) * 0xff );
  b = ( unsigned char ) ( ( blue_area / whole_area ) * 0xff );

  unsigned int color = 0xff << 24 | r << 16 | g << 8 | b;
  return color;
}

void slDrawTriangle( float3* x1, float3* x2, float3* x3 ) {

  float3 v1 = { x1->x, x1->y*-1.0f, x1->z };
  float3 v2 = { x2->x, x2->y*-1.0f, x2->z };
  float3 v3 = { x3->x, x3->y*-1.0f, x3->z };

  float max_x = max( v1.x, max( v2.x, v3.x ) );
  float min_x = min( v1.x, min( v2.x, v3.x ) );
  float max_y = max( v1.y, max( v2.y, v3.y ) );
  float min_y = min( v1.y, min( v2.y, v3.y ) );

  max_x = ( max_x + 1.0f ) * __c->m_width / 2.0f;
  min_x = ( min_x + 1.0f ) * __c->m_width / 2.0f;
  max_y = ( max_y + 1.0f ) * __c->m_height / 2.0f;
  min_y = ( min_y + 1.0f ) * __c->m_height / 2.0f;

  v1.x = ( v1.x + 1.0f ) *__c->m_width*0.5f;
  v1.y = ( v1.y + 1.0f ) *__c->m_height*0.5f;

  v2.x = ( v2.x + 1.0f ) *__c->m_width*0.5f;
  v2.y = ( v2.y + 1.0f ) *__c->m_height*0.5f;

  v3.x = ( v3.x + 1.0f ) *__c->m_width*0.5f;
  v3.y = ( v3.y + 1.0f ) *__c->m_height*0.5f;

  max_x = clamp( max_x, 0, __c->m_width );
  min_x = clamp( min_x, 0, __c->m_width );
  max_y = clamp( max_y, 0, __c->m_height );
  min_y = clamp( min_y, 0, __c->m_height );

  for( int i = ( int ) min_y; i < max_y; ++i ) {

    int i_m = i*__c->m_width;

    for( int e = ( int ) min_x; e < max_x; ++e ) {

      float2 p = { ( float ) e, ( float ) ( i ) };
      float3 pos = { ( float ) e, ( float ) i, 0.0f };

      float depth = 1.0f - interpolate_floats( pos, v1, v2, v3, v1.z, v2.z, v3.z );

      if( __c->depth_buffer[i_m + e] >= depth ){
        continue;
      }

      __c->depth_buffer[i_m + e] = depth;

      float w0 = orient2d( v2, v3, p );
      float w1 = orient2d( v3, v1, p );
      float w2 = orient2d( v1, v2, p );

      if( w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f ) {
        unsigned int color = get_color( pos, v1, v2, v3 );
        __c->screen_buffer[i_m + e] = color;
      } else if( w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f ) {
        unsigned int color = get_color( pos, v1, v2, v3 );
        __c->screen_buffer[i_m + e] = color;
      }

    }

  }
}

void slDrawTriangles( int count ) {
  float3* t = ( float3* ) ( ( char* ) __c->m_data_buffer + __c->m_bint_buffer );

  for( int i = 0; i < count / 3; ++i ) {
    slDrawTriangle( &t[i * 3 + 0], &t[i * 3 + 1], &t[i * 3 + 2] );
  }

}


void slDestroyContext() {
  free( __c );
  __c = NULL;
}