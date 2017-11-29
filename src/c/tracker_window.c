#include <pebble.h>
#include "tracker_window.h"

//Tracker Window
static Window *s_tracker_window;
static BitmapLayer *s_background_layer;
static TextLayer *s_splash_text;

typedef struct {
  int a;
  int b;
} Data;

/*Data data = (Data) {
  .a = 32,
  .b = 45
};
*/
static void tracker_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_tracker_window, GColorGreen);
  
  s_background_layer = bitmap_layer_create(bounds);
  
  //Create the text
  s_splash_text = text_layer_create(GRect(0, 110, 144, 30));
  text_layer_set_text_color(s_splash_text, PBL_IF_COLOR_ELSE(GColorBlack, GColorWhite));
  text_layer_set_background_color(s_splash_text, GColorClear);
//  text_layer_set_text(s_splash_text, persist_read_data(0, &data, sizeof(Data)));
  text_layer_set_text_alignment(s_splash_text, GTextAlignmentCenter);
  text_layer_set_font(s_splash_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  // Add Text to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_splash_text));
}

void show_tracker_window(void){
  s_tracker_window = window_create();
  
  window_set_window_handlers(s_tracker_window, (WindowHandlers) {
    .load = tracker_window_load,
  //  .unload = handle_window_unload,
  });
  window_stack_push(s_tracker_window, true);
}