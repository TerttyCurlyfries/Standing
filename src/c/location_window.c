#include <pebble.h>
#include "location_window.h"

//Location Window
static Window *s_location_window;
static BitmapLayer *s_background_layer;
static TextLayer *s_splash_text;

//Title char
static char *realTitle;

//Thing for timer to do once it runs out? Idk it works though
void test(void * data){
  window_stack_remove(s_location_window, false);
}

static void location_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_location_window, GColorRed);
  
  s_background_layer = bitmap_layer_create(bounds);

  //Create the text
  s_splash_text = text_layer_create(GRect(0, 110, 144, 30));
  text_layer_set_text_color(s_splash_text, PBL_IF_COLOR_ELSE(GColorBlack, GColorWhite));
  text_layer_set_background_color(s_splash_text, GColorClear);
  text_layer_set_text(s_splash_text, "Location saved!");
  text_layer_set_text_alignment(s_splash_text, GTextAlignmentCenter);
  text_layer_set_font(s_splash_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  // Add Text to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_splash_text));
}

void show_location_window(char* title){
  realTitle = title;
  
  s_location_window = window_create();
  
  window_set_window_handlers(s_location_window, (WindowHandlers) {
    .load = location_window_load,
  //  .unload = handle_window_unload,
  });
  window_stack_push(s_location_window, true);
  
  typedef struct {
    int lat;
    int lon;
  } Data;

  uint32_t key = 0;
    Data data = (Data) {
    .lat = 32,
    .lon = 45
  };
  
  // Write the data structure
  persist_write_data(key, &data, sizeof(Data));
  
  APP_LOG(APP_LOG_LEVEL_ERROR, "Data WROTED");
  
  //TIMER
  app_timer_register(2000, test, NULL);
}
