#include "sl/sethlans.h"
#include <windowsx.h>
#include <stdio.h>

krContext* __c = NULL;

int CaptureAnImage( HWND hWnd ) {

  PAINTSTRUCT ps;
  HDC hDC;

  hDC = BeginPaint( hWnd, &ps );

  HBITMAP hbm = CreateBitmap( __c->m_width, __c->m_height, 4, 8, __c->screen_buffer );
  HDC hdcMem = CreateCompatibleDC( hDC );
  HBITMAP hbmT = SelectBitmap( hdcMem, hbm );

  BITMAP bm;
  GetObject( hbm, sizeof( bm ), &bm );

  BitBlt( hDC, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY );

  SelectBitmap( hdcMem, hbmT );
  DeleteDC( hdcMem );

  EndPaint( hWnd, &ps );

  return 0;
}

LRESULT CALLBACK WindowProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {

  switch( message ) {
  case WM_DESTROY:
    PostQuitMessage( 0 );
    return 0;
  case WM_PAINT:
    //printf( " wm_paint \n " );
    CaptureAnImage( hWnd );
    break;
  }

  return DefWindowProc( hWnd, message, wParam, lParam );
}

void slCreateWindow( int width, int height ) {

  __c->m_width = width;
  __c->m_height = height;

  __c->screen_buffer = malloc( width * height * sizeof( int ) );
  memset( __c->screen_buffer, 0, width * height * sizeof( int ) );

  __c->depth_buffer = malloc( width * height * sizeof( float ) );
  memset( __c->depth_buffer, 0, width * height * sizeof( float ) );

  float aspect_ratio = ( ( float ) width ) / ( ( float ) height );
  const char* title = "kRenderer - Software Renderer";
  WNDCLASSEX window_class;
  memset( &window_class, 0, sizeof( WNDCLASSEX ) );

  memset( &__c->window_rect, 0, sizeof( RECT ) );

  window_class.cbSize = sizeof( WNDCLASSEX );
  window_class.style = CS_HREDRAW | CS_VREDRAW;
  window_class.lpfnWndProc = &WindowProc;
  window_class.hInstance = GetModuleHandle( 0 );
  window_class.hCursor = LoadCursor( NULL, IDC_ARROW );
  window_class.lpszClassName = "ClassName1";
  RegisterClassEx( &window_class );

  __c->window_rect.left = 0;
  __c->window_rect.top = 0;
  __c->window_rect.bottom = height;
  __c->window_rect.right = width;
  AdjustWindowRect( &__c->window_rect, WS_OVERLAPPEDWINDOW, FALSE );

  RECT desktop;
  memset( &desktop, 0, sizeof( RECT ) );
  GetWindowRect( GetDesktopWindow(), &desktop );
  int x_pos = ( desktop.right - width ) / 2;
  int y_pos = ( desktop.bottom - height ) / 2;

  __c->hwnd = CreateWindow( "ClassName1", title, WS_OVERLAPPEDWINDOW,
    x_pos, y_pos,
    __c->window_rect.right - __c->window_rect.left,
    __c->window_rect.bottom - __c->window_rect.top,
    NULL, NULL, NULL, NULL );

  ShowWindow( __c->hwnd, 1 );

  return;

  for( int i = 0; i < __c->m_height; ++i ) {
    for( int e = 0; e < __c->m_width; ++e ) {

      float f_R = ( float ) i / ( float ) __c->m_height;
      unsigned char r = ( char ) ( f_R * 255.f );

      float f_G = ( float ) e / ( float ) __c->m_width;
      unsigned char g = ( char ) ( f_G * 255.f );

      unsigned char b = 0;

      __c->screen_buffer[i*__c->m_width + e] = 0xff << 24 | r << 16 | g << 8 | b;

    }
  }
}

void slClearColor( char r, char g, char b, char a ) {

}

void slClear( krMask mask ) {
  memset( __c->screen_buffer, 0, sizeof( int )*__c->m_width *__c->m_height );
  memset( __c->depth_buffer, 0, sizeof( float )*__c->m_width *__c->m_height );
}

void slSwapBuffer() {

  //for( int i = 0; i < __c->m_width *__c->m_height; ++i ) {
  //  float depth = ( float ) ( __c->depth_buffer[i] );
  //  depth /= 65535.0f;
  //  depth = clamp( depth, 0.0f, 1.0f );
  //  unsigned char c = (unsigned char)(0xff*depth);
  //  __c->screen_buffer[i] = ( unsigned int ) ( 0xff << 24 | c << 16 | c << 8 | c );
  //}


  RedrawWindow( __c->hwnd, &__c->window_rect, 0, RDW_INTERNALPAINT );
  InvalidateRect( __c->hwnd, NULL, FALSE );
  UpdateWindow( __c->hwnd );
}

void slDestoyWindow() {

  CloseWindow( __c->hwnd );
  __c->hwnd = NULL;

}