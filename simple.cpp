#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <math.h>
#include "libfreenect.h"

freenect_context *f_ctx;
freenect_device *f_dev;


void rgb_cb(freenect_device *dev, void *rgb, uint32_t timestamp)
{
  return;
}

void depth_cb(freenect_device *dev, void *v_depth, uint32_t timestamp)
{
  std::cout << "got depth\n";
  return;
}

int main(int argc, char** argv)
{
  if (freenect_init (&f_ctx, NULL) < 0)
  {
    printf ("freenect_init() failed\n");
    return 1;
  }

  freenect_set_log_level (f_ctx, FREENECT_LOG_SPEW);
  freenect_select_subdevices(f_ctx, (freenect_device_flags)(FREENECT_DEVICE_CAMERA));

  int nr_devices = freenect_num_devices (f_ctx);
  printf ("Number of devices found: %d\n", nr_devices);

  if (nr_devices < 1)
    return 1;

  if (freenect_open_device (f_ctx, &f_dev, 0) < 0)
  {
    printf ("Could not open device\n");
    return 1;
  }

  freenect_set_video_callback(f_dev, rgb_cb);
  freenect_set_depth_callback(f_dev, depth_cb);
  freenect_set_depth_mode(f_dev, freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_10BIT));

  std::cout << "starting depth\n";

  freenect_start_depth(f_dev);

  std::cout << "started depth\n";
  while (freenect_process_events(f_ctx) >= 0) {
    //std::cout << "handled events\n";
    usleep(5);
  }

  freenect_close_device(f_dev);

  return 0;
}

