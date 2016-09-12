#include <stdio.h>
#include <time.h>

#include "sl/sethlans.h"
#include "demo/teapot.h"
#include "tools/input.h"

#define MEASURE_TIME 1

int main( int argc, char **argv ) {

  bool app_running = true;
  int width = 1280;
  int height = 720;

  slCreateContext();
  slCreateWindow( width, height );
  float time = 90.0f;

#if 0
  int buffer_count = _countof( cube );
  float* static_model = cube;
#else
  int buffer_count = _countof( teapot );
  float* static_model = (float*)teapot;
#endif

  int id = 0; 
  int buffer_size = buffer_count * sizeof( float );
  slGenBuffer( &id, buffer_size );
  slBindBuffer( id );
  slBufferData( static_model, buffer_size );

  float* model = ( float* ) malloc( buffer_size );
  memset( model, 0, buffer_size );

  float4x4 model_mat;

  float4x4 view_mat;
  identity( &view_mat );
  float3 camera_position = make_float3( 0.0f, 0.0f, -20.0f );

  float4x4 proj_mat;
  identity( &proj_mat );
  prespective( &proj_mat, 75.0f, (float)width / (float)height, 0.1f, 100.0f );

#if MEASURE_TIME
  LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds, Frequency;
  int frame_num = 0;
  float fastest = 99999999.0f, msec = 0.0f, slowest = 0.0f, average = 20.0f, total = 0.0f;
#endif

  while( app_running ) {

    handle_events( &app_running );

#if MEASURE_TIME
    QueryPerformanceFrequency( &Frequency );
    QueryPerformanceCounter( &StartingTime );
#endif

    slClear( KR_COLOR );

    time += msec;

    identity( &model_mat );
    rotate_y( &model_mat, time*0.001f );

    const float s = 0.05f * (msec/16.666f);

    if( keys_down[k_W] == true ) camera_position.z += s;
    if( keys_down[k_S] == true ) camera_position.z -= s;
    if( keys_down[k_A] == true ) camera_position.x -= s;
    if( keys_down[k_D] == true ) camera_position.x += s;
    if( keys_down[k_Q] == true ) camera_position.y -= s;
    if( keys_down[k_E] == true ) camera_position.y += s;

    translate( &view_mat, camera_position.x, camera_position.y, camera_position.z );

    for( int i = 0; i < buffer_count / 3; ++i ) {

      float4 vert = make_float4( static_model[i * 3 + 0], static_model[i * 3 + 1], static_model[i * 3 + 2], 1.0f );

      float4x4 model_view_mat = mult( &view_mat, &model_mat );
      float4x4 mvp = mult( &proj_mat, &model_view_mat );

      vert = mult_v( &mvp, &vert );

      vert.x /= vert.w;
      vert.y /= vert.w;
      vert.z /= vert.w;
      vert.w /= vert.w;

      model[i * 3 + 0] = vert.x;
      model[i * 3 + 1] = vert.y;
      model[i * 3 + 2] = vert.z;
    }

    slBufferData( model, buffer_size );

    slBindBuffer( id );
    slDrawTriangles( buffer_count );

    slSwapBuffer();

#if MEASURE_TIME
    QueryPerformanceCounter( &EndingTime );
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

    ElapsedMicroseconds.QuadPart *= ( LONGLONG )1000000000.0f;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    msec = ( float ) ElapsedMicroseconds.QuadPart / 1000000.0f;

    if( msec < fastest ) fastest = msec;
    if( msec > slowest ) slowest = msec;

    total += msec;
    ++frame_num;
    average = total / ( float ) frame_num;

    printf( "frame #%d completed in %.2f ms, min %.2f ms, max %.2f ms, avg %.2f ms \n ", frame_num, msec, fastest, slowest, average );
#endif

  }

  slDestoyWindow();
  slDestroyContext();

  return 0;
}
