#include <Windows.h>
#include "sl/sethlans.h"

#ifndef __INPUT_H__
#define __INPUT_H__ 1

enum key {
  k_W = 0,
  k_A,
  k_S,
  k_D,
  k_Q,
  k_E,

  nn_TOTAL_KEYS,
};

static bool keys_down[nn_TOTAL_KEYS];
void key_down( WPARAM key );
void key_up( WPARAM key );
void handle_events( bool* app_running );

#endif