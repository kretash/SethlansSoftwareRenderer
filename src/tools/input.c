#include "tools/input.h"

void key_down( WPARAM key ) {
  switch( key ) {
  case 'W':
    keys_down[k_W] = true;
    break;
  case 'A':
    keys_down[k_A] = true;
    break;
  case 'S':
    keys_down[k_S] = true;
    break;
  case 'D':
    keys_down[k_D] = true;
    break;
  case 'Q':
    keys_down[k_Q] = true;
    break;
  case 'E':
    keys_down[k_E] = true;
    break;
  }
}

void key_up( WPARAM key ) {
  switch( key ) {
  case 'W':
    keys_down[k_W] = false;
    break;
  case 'A':
    keys_down[k_A] = false;
    break;
  case 'S':
    keys_down[k_S] = false;
    break;
  case 'D':
    keys_down[k_D] = false;
    break;
  case 'Q':
    keys_down[k_Q] = false;
    break;
  case 'E':
    keys_down[k_E] = false;
    break;
  }
}

void handle_events( bool* app_running ) {

  MSG msg;
  while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
    TranslateMessage( &msg );
    DispatchMessage( &msg );

    switch( msg.message ) {
    case WM_KEYDOWN:
      key_down( msg.wParam );
      break;
    case WM_KEYUP:
      key_up( msg.wParam );
      break;
    case WM_QUIT:
      *app_running = false;
      break;
    }
  }
}