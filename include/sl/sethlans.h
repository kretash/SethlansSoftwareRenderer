/*
  *************************************************************************************************************
          (            (                       Carlos Martinez Romero                     (            (        
         ( ,)         ( ,)                   Sethlans Software Renderer                  ( ,)         ( ,)            
        ). ( )       ). ( )                                                             ). ( )       ). ( )     
       (, )' (.     (, )' (.                 https://github.com/kretash                (, )' (.     (, )' (.    
      \WWWWWWWW/   \WWWWWWWW/                       MIT License                       \WWWWWWWW/   \WWWWWWWW/   
       '--..--'     '--..--'                                                           '--..--'     '--..--'    
          }{           }{                                                                 }{           }{       
          {}           {}                                                                 {}           {}       
  *************************************************************************************************************
*/

#ifndef __SETHLANS_H__
#define __SETHLANS_H__ 1

#include <Windows.h>
#include "tools/math.h"

#define bool char
#define true 1
#define false 0

#define krMask int
#define KR_COLOR 0x00000001
#define KR_DEPTH 0x00000002

#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))
#define clamp(v, i, a) ( min( max(v, i), a) )
#define abs( v ) ( (v) < 0.0f ? -(v) : (v) )
#define mix( x, y, m ) ( x+((y-x)*m) )

typedef struct slContext {
  HWND hwnd;
  HDC hdc;
  RECT window_rect;
  unsigned int* screen_buffer;
  float* depth_buffer;

  void* m_data_buffer;
  int m_bint_buffer;
  int m_data_buffer_pointer;
  int m_power_buffer_length;
  int m_data_buffer_length;

  int m_width;
  int m_height;
}krContext;

void slCreateWindow( int width, int height );
void slDestoyWindow();
void slGenBuffer( int* id, int lenght );
void slBindBuffer( int id );
void slBufferData( const void* data, int lenght );
void slClearColor( char r, char g, char b, char a );
void slClear( krMask mask );
void slDrawTriangles( int count );
void slDrawTriangle( float3* x1, float3* x2, float3* x3 );
void slSwapBuffer();
void slCreateContext();
void slDestroyContext();

#endif