#include <pebble.h>
#include <pebble.h>

Window *my_window;
TextLayer *text_layer;
int g_count = 0;
char buffer [10];
bool running = false;


// Converts integer seconds into a string in the format of
// M:SS and stores in provided buffer
void format_time(char * buffer, int buf_len, int time_in_seconds)
{
  int min = time_in_seconds / 60;
  int sec = time_in_seconds % 60;

  snprintf(buffer, 10, "%02d:%02d", min, sec);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "FMT: %s", buffer);
}

void display_current_time()
{
  format_time(buffer, 10, g_count);
  text_layer_set_text(text_layer, buffer);
}

void reset_counter()
{
  running = false;
  g_count = 45 * 60; // 45 minutes in seconds 
  display_current_time();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  g_count -= 1;
  display_current_time();
}

static void long_select_handler(ClickRecognizerRef recognizer, void *context) {
  reset_counter();
}

void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
  if (!running)
  {
    running = true;
    tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  } else {
    running = false;
    tick_timer_service_unsubscribe();
  }
}

void click_config_provider(void *context)
{
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 1000, long_select_handler, NULL);
}

void window_load(Window *window)
{
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
  text_layer = text_layer_create(GRect(0, 53, window_bounds.size.w, window_bounds.size.h));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_LECO_42_NUMBERS));
  window_set_click_config_provider_with_context(my_window, click_config_provider, text_layer);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));
 
  reset_counter();
}

void window_unload(Window *window)
{
  text_layer_destroy(text_layer);
}

void handle_init(void) {
  my_window = window_create();

  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  
  window_stack_push(my_window, true);
}

void handle_deinit(void) {
  window_destroy(my_window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}