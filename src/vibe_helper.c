#include <pebble.h>
#include "vibe_helper.h"

struct CustomVibe setup_vibe_pattern(int num_vibes, uint32_t play_time, uint32_t pause_time)
{
  struct CustomVibe cv;
  cv.play_time = play_time;
  cv.pause_time = pause_time;
  
  // Get the total number of elements we'll need
  cv.pattern_length = (num_vibes*2) - 1;
  
  // Allocate space for the array
  cv.pattern_array = (uint32_t*)malloc(cv.pattern_length * sizeof(uint32_t));
  
  // Build the array
  for (int i = 0; i<cv.pattern_length; i++)
  {
    // If it's an even element, we use our vibe time
    if (i % 2 == 0)
    {
      cv.pattern_array[i] = cv.play_time;
    }
    // If its an odd element, we use our quiet time
    else
    {
      cv.pattern_array[i] = cv.pause_time;
    }
  }
  
  return cv;
}

struct CustomVibe init_vibe_pattern(int num_vibes)
{
  return setup_vibe_pattern(num_vibes, (uint32_t)100, (uint32_t)200);  
}

void play_pattern(struct CustomVibe cv)
{
  VibePattern pat = {
    .durations = cv.pattern_array,
    .num_segments = cv.pattern_length,
  };
 
  vibes_enqueue_custom_pattern(pat); 
}

void dump_cv(struct CustomVibe cv)
{
  for (int i =0; i < cv.pattern_length; i++)
  {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "pattern_array[%d]: %d", i, (int)cv.pattern_array[i]);
  }
}

void teardown(struct CustomVibe cv)
{
  free(cv.pattern_array);
}
