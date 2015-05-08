#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer, *s_date_timelayer;
static GFont s_time_font, s_date_font;
static BitmapLayer *s_day_layer, *s_moon_layer;
static BitmapLayer *s_stars_layer, *s_sun_layer;
static BitmapLayer *s_mornLight_layer, *s_nightLight_layer;
static GBitmap *s_nightLight_bitmap, *s_mornLight_bitmap; 
static GBitmap *s_sun_bitmap, *s_stars_bitmap;
static GBitmap *s_moon_bitmap, *s_day_bitmap;


// Function to modify the background at 6 PM
static void update_night_background(Window *window) {
  layer_set_hidden((Layer *)s_stars_layer, false);  // Enable Stars Layer
  layer_set_hidden((Layer *)s_day_layer, true); // Hide Morning Layer
  layer_set_hidden((Layer *)s_nightLight_layer, false); // Enable Night Backlight
  layer_set_hidden((Layer *)s_mornLight_layer, true); // Disable Morning Backlight
  text_layer_set_text_color(s_time_layer, GColorBlack);  // Change the text color to white
  text_layer_set_text_color(s_date_timelayer, GColorWhite); // Change the date text color to Black
}

// Function to modify the background at 6 AM
static void update_morning_background(Window *window) {
  layer_set_hidden((Layer *)s_stars_layer, true);  // Hide Stars Layer
  layer_set_hidden((Layer *)s_day_layer, false); // Enable Morning Layer
  layer_set_hidden((Layer *)s_nightLight_layer, true); // Hide Night Backlight
  layer_set_hidden((Layer *)s_mornLight_layer, false); // Enable Morning Backlight
  text_layer_set_text_color(s_time_layer, GColorBlack);  // Change the text color to white
  text_layer_set_text_color(s_date_timelayer, GColorBlack);  // Change the date text color to White
}

/*
void on_animation_stopped(Animation *anim, bool finished, void *context)
{
     //Free the memoery used by the Animation
     property_animation_destroy((PropertyAnimation*) anim);
}
 */

void animate_layer(Layer *layer, GRect *start, GRect *finish)
{
    const int delay = 500;
    const int duration = 300;

    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 /*
    //Set stopped handler to free memory
    AnimationHandlers handlers = {
        //The reference to the stopped handler is the only one in the array
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    }; 
   // animation_set_handlers((Animation*) anim, handlers, NULL);
 */
    //Start animation!
    animation_schedule((Animation*) anim);   
}

static void update_trajectory_animation(int buf_hour) {
   
  GRect start;
  GRect finish;
  static int stat[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  
  // Step 1 - Sun Rise, Bottom
  if (buf_hour>=6 && buf_hour<=7 &&stat[0] == 0) { 
    stat[0] = 1;
    stat[9] = 0;
    finish = GRect(45,-9, 144, 168);
    start = GRect(45,-7, 144, 168);
    animate_layer((Layer *)s_sun_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_sun_layer, false); // Enable Sun
    layer_set_hidden((Layer *)s_moon_layer, true); // Disable Moon    
  } // Step 1 - End Rise, Inbetween 
  
  // Step 2 - Sun Rise, Inbetween
  else if (buf_hour>=8 && buf_hour<11 &&stat[1] == 0) { 
    stat[1] = 1;
    stat[0] = 0;
    finish = GRect(30,-40, 144, 168); 
    start = GRect(45,-9, 144, 168); 
    animate_layer((Layer *)s_sun_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_sun_layer, false); // Enable Sun
    layer_set_hidden((Layer *)s_moon_layer, true); // Disable Moon    
  } // Step 2 - End Rise, Inbetween 
  
  // Step 3 - Sun Rise, Peak
  else if (buf_hour>=11 && buf_hour<14 &&stat[2] == 0) { 
    stat[2] = 1;
    stat[1] = 0;
    finish = GRect(0,-60, 144, 168);  //-60
    start = GRect(30,-40, 144, 168);
    animate_layer((Layer *)s_sun_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_sun_layer, false); // Enable Sun
    layer_set_hidden((Layer *)s_moon_layer, true); // Disable Moon    
  } // Step 3 - End Rise, Peak 
  
   // Step 4 - Sun Set, Inbetween
  else if (buf_hour>=14 && buf_hour<17 &&stat[3] == 0) { 
    stat[3] = 1;
    stat[2] = 0;
    finish = GRect(-30,-40, 144, 168);
    start = GRect(0,-60, 144, 168);
    animate_layer((Layer *)s_sun_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_sun_layer, false); // Enable Sun
    layer_set_hidden((Layer *)s_moon_layer, true); // Disable Moon    
  } // Step 4 - End Set, Inbetween 
  
  // Step 5 - Sun Set, Bottom
  else if (buf_hour>=17 && buf_hour<18 &&stat[4] == 0) { 
    stat[4] = 1;
    stat[3] = 0;
    finish = GRect(-45,-9, 144, 168);
    start = GRect(-30,-40, 144, 168);
    animate_layer((Layer *)s_sun_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_sun_layer, false); // Enable Sun
    layer_set_hidden((Layer *)s_moon_layer, true); // Disable Moon    
  } // Step 5 - End Set, Bottom 
  
  // Step 6 - Moon Rise, Bottom
  else if (buf_hour>=18 && buf_hour<19 &&stat[5] == 0) { 
    stat[5] = 1;
    stat[4] = 0;
    finish = GRect(45,-9, 144, 168);
    start = GRect(45,-7, 144, 168);
    animate_layer((Layer *)s_moon_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_moon_layer, false); // Enable Moon
    layer_set_hidden((Layer *)s_sun_layer, true); // Disable Sun    
  } // Step 6 - End Rise, Bottom 
  
  // Step 7 - Moon Rise, Inbetween
  else if (buf_hour>=19 && buf_hour<23 &&stat[6] == 0) { 
    stat[6] = 1;
    stat[5] = 0;
    finish = GRect(30,-40, 144, 168); 
    start = GRect(45,-9, 144, 168);
    animate_layer((Layer *)s_moon_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_moon_layer, false); // Enable Moon
    layer_set_hidden((Layer *)s_sun_layer, true); // Disable Sun    
  } // Step 7 - End Rise, Inbetween 
  
  // Step 8 - Moon Rise, Peak
  else if ((buf_hour>=23 || buf_hour<2) &&stat[7] == 0) { 
    stat[7] = 1;
    stat[6] = 0;
    finish = GRect(0,-60, 144, 168);  //-60
    start = GRect(30,-40, 144, 168);
    animate_layer((Layer *)s_moon_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_moon_layer, false); // Enable Moon
    layer_set_hidden((Layer *)s_sun_layer, true); // Disable Sun    
  } // Step 8 - End Rise, Peak 
  
   // Step 9 - Moon Set, Inbetween
  else if (buf_hour>=2 && buf_hour<5 &&stat[8] == 0) { 
    stat[8] = 1;
    stat[7] = 0;
    finish = GRect(-30,-40, 144, 168);
    start = GRect(0,-60, 144, 168);
    animate_layer((Layer *)s_moon_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_moon_layer, false); // Enable Moon
    layer_set_hidden((Layer *)s_sun_layer, true); // Disable Sun    
  } // Step 9 - End Set, Inbetween 
  
  // Step 10 - Moon Set, Bottom
  if (buf_hour>=5 && buf_hour<6 &&stat[9] == 0) { 
    stat[9] = 1;
    stat[8] = 0;
    finish = GRect(-45,-9, 144, 168);
    start = GRect(-30,-40, 144, 168);
    animate_layer((Layer *)s_moon_layer, &start, &finish); // Move the layer from and to above points
    layer_set_hidden((Layer *)s_moon_layer, false); // Enable Moon
    layer_set_hidden((Layer *)s_sun_layer, true); // Disable Sun      
  } // Step 10 - End Set, Bottom 
  
}

static void update_background(int buf_hour){
  static bool flag = true;
  static bool first_time = true;
  
  if ((buf_hour >= 6) && (buf_hour <18)) {
    if (flag == true ) {
       update_morning_background(s_main_window); 
       flag = false;
       first_time = false;
    }
  }
  
  else if (flag == false || first_time == true) {
    update_night_background(s_main_window); 
    flag = true; 
  }
    
  first_time = false;
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  
  // Create a long-lived 
  static char buffer[] = "00:00";
  
  static char date_buf[] = "Def  00, Wednesday";
  
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 24h hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  //Write Date and Month to a buffer
  strftime(date_buf, sizeof("Def  00,  Wednesday"), "%b %d, %A", tick_time);
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
  text_layer_set_text(s_date_timelayer, date_buf);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  
  static int buf_hour = 0;
  buf_hour = tick_time->tm_hour;
  
  // Update the background based on Night and Day
  update_background(buf_hour);
  
  // Update the time on every tick
  update_time();
  
  // Update the movement of cellestial bodies
  update_trajectory_animation(buf_hour);
}

static void main_window_load(Window *window) {
  
  
  //Create GBitmap, Set and Hide Stars Layer
  s_stars_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NIGHT_BACKGROUND);
  s_stars_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_stars_layer, s_stars_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_stars_layer));  
  layer_set_hidden((Layer *)s_stars_layer, true);
  
  //Create GBitmap, then set to created BitmapLayer
  s_day_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MORNING_BACKGROUND);
  s_day_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
  bitmap_layer_set_bitmap(s_day_layer, s_day_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_day_layer));
  layer_set_hidden((Layer *)s_day_layer, true);
  
  // SUN // GRect(35, 15, 50, 45)
  s_sun_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUN_OBJECT);
  s_sun_layer = bitmap_layer_create(GRect(0, 14, 144, 168));
  bitmap_layer_set_background_color(s_sun_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_sun_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_sun_layer, s_sun_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_sun_layer));
  layer_set_hidden((Layer *)s_sun_layer, true);
  
  // MOON // GRect(15, 45, 45, 45)
  s_moon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MOON_OBJECT);
  s_moon_layer = bitmap_layer_create(GRect(0, 14, 144, 168));
  bitmap_layer_set_background_color(s_moon_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_moon_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_moon_layer, s_moon_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_moon_layer));
  layer_set_hidden((Layer *)s_moon_layer, true);
  
  // MORNING BACKLIGHT
  s_mornLight_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MORNING_BACKLIGHT);
  s_mornLight_layer = bitmap_layer_create(GRect(8, 75, 130, 65));
  bitmap_layer_set_background_color(s_mornLight_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_mornLight_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_mornLight_layer, s_mornLight_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_mornLight_layer));
  layer_set_hidden((Layer *)s_mornLight_layer, true);
  
  // NIGHT BACKLIGHT
  s_nightLight_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NIGHT_BACKLIGHT);
  s_nightLight_layer = bitmap_layer_create(GRect(8, 75, 130, 65));
  bitmap_layer_set_background_color(s_nightLight_layer, GColorClear);
  bitmap_layer_set_compositing_mode(s_nightLight_layer, GCompOpSet);
  bitmap_layer_set_bitmap(s_nightLight_layer, s_nightLight_bitmap);
  layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(s_nightLight_layer));
  layer_set_hidden((Layer *)s_nightLight_layer, true);
  

  // Create Date and Month Layer // GRect(0, 121, 144, 50)
  s_date_timelayer = text_layer_create(GRect(0, 141, 144, 50));
  text_layer_set_background_color(s_date_timelayer, GColorClear);
  text_layer_set_text_color(s_date_timelayer, GColorBlack);
  text_layer_set_text(s_date_timelayer, "Def  00");
  s_date_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ALLER_BOLD_16));
  text_layer_set_font(s_date_timelayer, s_date_font);
  text_layer_set_text_alignment(s_date_timelayer, GTextAlignmentCenter);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_date_timelayer));
  
  
  // Create time TextLayer // GRect(5, 76, 139, 50)
  s_time_layer = text_layer_create(GRect(0, 77, 144, 60));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_FRANCOIS_BOLD_48));

  //Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
  // Make sure the time is displayed from the start
  update_time();
}

static void main_window_unload(Window *window) {
  
  //Destroy GBitmap
  gbitmap_destroy(s_day_bitmap);
  gbitmap_destroy(s_mornLight_bitmap);
  gbitmap_destroy(s_nightLight_bitmap);
  gbitmap_destroy(s_stars_bitmap);
  gbitmap_destroy(s_sun_bitmap);
  gbitmap_destroy(s_moon_bitmap);
  
  //Destroy BitmapLayer
  bitmap_layer_destroy(s_day_layer);
  bitmap_layer_destroy(s_mornLight_layer);
  bitmap_layer_destroy(s_nightLight_layer);
  bitmap_layer_destroy(s_stars_layer);
  bitmap_layer_destroy(s_sun_layer);
  bitmap_layer_destroy(s_moon_layer);
  
  //Unload GFont
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_date_font);
  
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_date_timelayer);  
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
