#include <pebble.h>
#include "location_menu.h"

#define NUM_MENU_SECTIONS 2
#define NUM_FIRST_MENU_ITEMS 4

//Location Menu Window
static Window *s_location_menu_window;
static BitmapLayer *s_background_layer;

//Menu stuff
MenuLayer *menu_layer;
static SimpleMenuLayer *s_simple_menu_layer;
static SimpleMenuSection s_menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem s_first_menu_items[NUM_FIRST_MENU_ITEMS];
static GBitmap *s_menu_icon_image;

static void location_select_callback(int index, void *ctx) {
  
}

static void location_menu_window_load(Window *window) {
  
    // Although we already defined NUM_FIRST_MENU_ITEMS, you can define
  // an int as such to easily change the order of menu items later
  int num_a_items = 0;

  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "oh shit whattup",
   // .subtitle = "Say hello!",
    .callback = location_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "What time is it?",
   // .subtitle = "What time IS it??",
    .callback = location_select_callback,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "Weather",
   // .subtitle = "Is it hot in here?",
    .callback = location_select_callback,
    .icon = s_menu_icon_image,
  };
  s_first_menu_items[num_a_items++] = (SimpleMenuItem) {
    .title = "IFTTT",
   // .subtitle = "Fire and IFTTT recipe",
    .callback = location_select_callback,
    .icon = s_menu_icon_image,
  };

  s_menu_sections[0] = (SimpleMenuSection) {
    .title = "Saved Locations",
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = s_first_menu_items,
  };
  
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);

  s_simple_menu_layer = simple_menu_layer_create(bounds, window, s_menu_sections, NUM_MENU_SECTIONS, NULL);

  layer_add_child(window_layer, simple_menu_layer_get_layer(s_simple_menu_layer));
}

void show_location_menu(void){
   s_location_menu_window = window_create();
  
  window_set_window_handlers(s_location_menu_window, (WindowHandlers) {
    .load = location_menu_window_load,
  //  .unload = handle_window_unload,
  });
  window_stack_push(s_location_menu_window, true);
}