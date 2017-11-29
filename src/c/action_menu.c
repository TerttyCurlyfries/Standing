#include <pebble.h>
#include "action_menu.h"
#include "location_window.h"

//Speech global? Idk
static DictationSession *voice;
static char s_last_text[512];

//Title
static char* title;

//Voice
static Window *s_voice_window;
static BitmapLayer *s_background_layer;

//Action Menu
static ActionMenu *s_action_menu;
static ActionMenuLevel *default_menu;
static ActionMenuLevel *action_menu;
static ActionMenuLevel *custom_menu;

typedef enum {
  speechChoice,
  
  carChoice,
  houseChoice,
  workChoice
} helloType;

typedef struct {
  helloType type;
} Context;

static helloType s_current_type;

static void hello_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {

  s_current_type = (helloType)action_menu_item_get_action_data(action);
  
  // Play this vibration
  switch(s_current_type) {
    case speechChoice:  show_voiceMenu();  break;
    
    case carChoice: title = "Car"; show_location_window(title);  break;
    case houseChoice:   title = "House"; show_location_window(title);   break;
    case workChoice: title = "Work"; show_location_window(title); break;
  }
}

//D-bug voice stuff
static void dictation_session_callback(DictationSession *session, DictationSessionStatus status,
                                       char *transcription, void *context) {
if(status == DictationSessionStatusSuccess) {
    // Display the dictated text
    snprintf(s_last_text, sizeof(s_last_text), "Transcription:\n\n%s", transcription);
    title = transcription;
    show_location_window(title);
  } else {
    // Display the reason for any error
    static char s_failed_buff[128];
    snprintf(s_failed_buff, sizeof(s_failed_buff), "Transcription failed.\n\nError ID:\n%d", (int)status);
  }
}

static void voice_window_load(Window *window) {
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  window_set_background_color(s_voice_window, GColorRed);
  
  s_background_layer = bitmap_layer_create(bounds);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  
  voice = dictation_session_create(sizeof(s_last_text), dictation_session_callback, NULL);
  
  dictation_session_start(voice);
  
}

static void handle_window_unload(Window* window) {
  
}


void show_voiceMenu(void){
  s_voice_window = window_create();
  
  window_set_window_handlers(s_voice_window, (WindowHandlers) {
    .load = voice_window_load,
    .unload = handle_window_unload,
  });
  window_stack_push(s_voice_window, true);
}

void hide_voiceMenu(void){
  window_stack_remove(s_voice_window, true);
}

void show_THEmenu(void){
  action_menu = action_menu_level_create(3);

  action_menu_level_add_action(action_menu, "Speech", hello_performed_callback, (void *)speechChoice);
  
  default_menu = action_menu_level_create(3);
  action_menu_level_add_child(action_menu, default_menu, "Default");

  action_menu_level_add_action(default_menu, "Car", hello_performed_callback, (void *)carChoice);
  action_menu_level_add_action(default_menu, "House", hello_performed_callback, (void *)houseChoice);
  action_menu_level_add_action(default_menu, "Work", hello_performed_callback, (void *)workChoice);
  
  action_menu_level_add_child(action_menu, default_menu, "Custom");
  
  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig config = (ActionMenuConfig) {
    .root_level = action_menu,
    .colors = {
      .background = GColorRed,
      .foreground = GColorBlack,
    },
    .align = ActionMenuAlignCenter
  };
  
   // Show the ActionMenu
  s_action_menu = action_menu_open(&config);
}

void show_defaultMenu(void){
  default_menu = action_menu_level_create(3);
  
  action_menu_level_add_child(action_menu, default_menu, "DEFAULT");

  action_menu_level_add_action(default_menu, "Car", hello_performed_callback, (void *)carChoice);
  action_menu_level_add_action(default_menu, "House", hello_performed_callback, (void *)houseChoice);
  action_menu_level_add_action(default_menu, "Work", hello_performed_callback, (void *)workChoice);
}