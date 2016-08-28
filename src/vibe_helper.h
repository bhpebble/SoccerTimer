#pragma once
#include <pebble.h>

struct CustomVibe
{
  uint32_t *pattern_array;
  int pattern_length;
  uint32_t play_time;
  uint32_t pause_time;
};

// Initialize a new vibration pattern
// num_vibes: The number of vibes to play
// vibe_len: How long each vibe should play
// pause_len: How long to wait between each vibe
struct CustomVibe setup_vibe_pattern(int num_vibes, uint32_t vibe_time, uint32_t pause_time);

// Initializes a new vibration pattern using defaults for vibe and pause time
// (100ms, 200ms)
// num_vibes: The number of vibes to play
struct CustomVibe init_vibe_pattern(int num_vibes);

// Play a previously initialized CustomVibe
void play_pattern(struct CustomVibe cv);

void dump_cv(struct CustomVibe cv);

// Free any used memory
void teardown(struct CustomVibe cv);
