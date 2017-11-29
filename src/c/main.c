#include <pebble.h>
#include "action_menu.h"
#include "location_menu.h"

//Pebble.js vars
#define KEY_LAT 0
#define KEY_LONG 1

//Hub Vars
static Window *s_hub_window;
static BitmapLayer *s_background_layer;
static TextLayer *s_splash_text;
static Layer *s_roundedrectangle_layer;
static Layer *s_triangle_layer;
int onRed = 1;

void hub_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(strcmp((text_layer_get_text(s_splash_text)), "Save my Standing!") == 0){
  show_THEmenu();
  }else{
  show_location_menu();
  }
}

void hub_down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(onRed == 0){
  //Change colour to red and text in hub
  window_set_background_color(s_hub_window, GColorRed);
  text_layer_set_text_color(s_splash_text, PBL_IF_COLOR_ELSE(GColorBlack, GColorWhite));
  text_layer_set_text(s_splash_text, "Save my Standing!"); 
  layer_set_hidden(s_roundedrectangle_layer, false);
  layer_set_hidden(s_triangle_layer, true);
  onRed = 1;
  }else{
  //Change colour to green and text in hub
  window_set_background_color(s_hub_window, GColorGreen);
  text_layer_set_text_color(s_splash_text, PBL_IF_COLOR_ELSE(GColorBlack, GColorBlack));
  text_layer_set_text(s_splash_text, "Show my Standings!");
  layer_set_hidden(s_roundedrectangle_layer, true);
  layer_set_hidden(s_triangle_layer, false);
  onRed = 0;
  }
}

void hub_config_provider(Window *window) {
// window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
 window_single_click_subscribe(BUTTON_ID_SELECT, hub_select_single_click_handler);
 window_single_click_subscribe(BUTTON_ID_DOWN, hub_down_single_click_handler);
}

static void circanvas_update_proc(Layer *layer, GContext *ctx) {
GPoint rect_bounds = GPoint(72, 60);
  
graphics_context_set_fill_color(ctx, GColorFromRGB(255, 105, 97));
graphics_fill_circle(ctx, rect_bounds, 30);
// Draw a circle
graphics_draw_circle(ctx, rect_bounds, 30);

}

static void tricanvas_update_proc(Layer *layer, GContext *ctx) {
GPoint start_line_1 = GPoint(47, 35);
GPoint end_line_1 = GPoint(110, 60);
  
GPoint start_line_2 = GPoint(110, 60);
GPoint end_line_2 = GPoint(47, 85);  
  
GPoint start_line_3 = GPoint(47, 85);
GPoint end_line_3 = GPoint(47, 35);
  
// Draw a line_1
graphics_draw_line(ctx, start_line_1, end_line_1);
  
// Draw a line_2
graphics_draw_line(ctx, start_line_2, end_line_2);  
  
//Draw a line_3
graphics_draw_line(ctx, start_line_3, end_line_3);
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_hub_window, GColorRed);
  
  // Create canvas layer
  s_roundedrectangle_layer = layer_create(bounds);
  s_triangle_layer = layer_create(bounds);
  s_background_layer = bitmap_layer_create(bounds);
  
  // Add rectangle to Window
  layer_add_child(window_get_root_layer(window), s_roundedrectangle_layer);
  layer_mark_dirty(s_roundedrectangle_layer);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  // Add triangle to Window
  layer_add_child(window_get_root_layer(window), s_triangle_layer);
  layer_mark_dirty(s_triangle_layer);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  layer_set_hidden(s_triangle_layer, true);
  
  // Add triange to Window
  
  // Assign the custom drawing procedure
  layer_set_update_proc(s_roundedrectangle_layer, circanvas_update_proc);
  layer_set_update_proc(s_triangle_layer, tricanvas_update_proc);
  s_background_layer = bitmap_layer_create(bounds);

  //Create the text
  s_splash_text = text_layer_create(GRect(0, 110, 144, 30));
  text_layer_set_text_color(s_splash_text, PBL_IF_COLOR_ELSE(GColorBlack, GColorWhite));
  text_layer_set_background_color(s_splash_text, GColorClear);
  text_layer_set_text(s_splash_text, "Save my Standing!");
  text_layer_set_text_alignment(s_splash_text, GTextAlignmentCenter);
  text_layer_set_font(s_splash_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  // Add Text to Window
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_splash_text));
  
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
  text_layer_destroy(s_splash_text);
  bitmap_layer_destroy(s_background_layer);
  layer_destroy(s_roundedrectangle_layer);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  static char lat_buffer[8];
  static char long_buffer[8];
  static char lat_long_layer_buffer[32];
  
 Tuple *lat_tuple = dict_find(iterator, KEY_LAT);
 Tuple *long_tuple = dict_find(iterator, KEY_LONG);
  
  if(lat_tuple && long_tuple) {
    snprintf(lat_buffer, sizeof(lat_buffer), "%dC", (int)lat_tuple->value->int32);
    snprintf(long_buffer, sizeof(long_buffer), "%dC", (int)long_tuple->value->int32);

    // Assemble full string and display
    snprintf(lat_long_layer_buffer, sizeof(lat_long_layer_buffer), "%s, %s", lat_buffer, long_buffer);
   // text_layer_set_text(text_layer, weather_layer_buffer);
  }
}


static void init() {
  // Create main Window element and assign to pointer
  s_hub_window = window_create();
  
  window_set_click_config_provider(s_hub_window, (ClickConfigProvider) hub_config_provider);
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_hub_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  window_stack_push(s_hub_window, true);
  
     // Register callbacks
app_message_register_inbox_received(inbox_received_callback);
//app_message_register_inbox_dropped(inbox_dropped_callback);
//app_message_register_outbox_failed(outbox_failed_callback);
//app_message_register_outbox_sent(outbox_sent_callback);
  
   // Open AppMessage
const int inbox_size = 128;
const int outbox_size = 128;
app_message_open(inbox_size, outbox_size);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_hub_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}