// Example file showing the setup for the small screen.
// Needs more work

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/mman.h>

#include "../Screen.cpp"

const int DEBUG = 1;
const int CTLFLOW = 2;
const char* FRAMEBUFFER = "/dev/fb1";
const char* CONSOLE = "/dev/tty1";
const int PAGES = 1;


class SmallRpiScreen : public Screen
{
  private:

    int fbfd; // Framebuffer file descriptor
    int kbfd; // Console text file descriptor

    struct fb_fix_screeninfo finfo;
    struct fb_var_screeninfo vinfo;
    struct fb_var_screeninfo orig_vinfo;

    long int len_fb_mem; // Length of fb mem
    char *fbp;

    char* fbpages;
    int page;

  public:
    // Start the framebuffer access
    SmallRpiScreen() : Screen::Screen()
    {

      fbfd = 0; // Framebuffer file descriptor
      kbfd = 0; // Console text file descriptor

      len_fb_mem = 0; // Length of fb mem
      fbp = 0;

      fbpages = 0;
      page = 0;


      if (DEBUG >= CTLFLOW) printf("Starting SmallRpiScreen init\n");

      // Open the framebuffer device file
      fbfd = open(FRAMEBUFFER,O_RDWR);
      if (fbfd == -1) {
        if (DEBUG >= 1) printf("Error: cannot open framebuffer device.\n");
      }
      if (DEBUG >= 1) printf("Framebuffer device opened.\n");

      // Hide Cursor
      kbfd = open(CONSOLE, O_WRONLY);
      if (kbfd >= 0) {
        ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
      }
      else {
        if (DEBUG >= 1) printf("Could not open console to diable cursor\n");
      }

      // Get orig var screen info
      if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
        if (DEBUG >= 1) printf("Error reading orig var screen info\n");
      }
      if (DEBUG >= 1) printf("Original info: %dx%d, %dbpp\n",
                         vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

      // Store orig var info
      memcpy(&orig_vinfo,&vinfo, sizeof(struct fb_var_screeninfo));


      if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
        if (DEBUG >= 1) printf("Error setting var info.\n");
      }
      if (DEBUG >= 1) printf("New info: %dx%d, %dbpp\n          %dx%d virtual\n",
                             vinfo.xres, vinfo.yres, vinfo.bits_per_pixel,
                             vinfo.xres_virtual, vinfo.yres_virtual);

      // Get fix screen info
      if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
        if (DEBUG >= 1) printf("Error reading fix screen info\n");
      }
      if (DEBUG >= 1) printf("Fixed info: %d smem_len, %d line_length\n",
                         finfo.smem_len, finfo.line_length);


      // Map framebuffer to user memory
      len_fb_mem = finfo.smem_len;

      fbp = (char*)mmap(0, len_fb_mem,
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        fbfd, 0);
      if ((int)fbp == -1) {
        if (DEBUG >= 1) printf("Failed to map fb.\n");
      }

      fbpages = (char*)malloc(len_fb_mem*PAGES);

      if ((int)fbpages == -1){
        if (DEBUG >= 1) printf("Failed to allocate pages\n");
      }

      if (DEBUG >= CTLFLOW) printf("Finished init\n");
    }

    // Cleanup the framebuffer access
    ~SmallRpiScreen()
    {
      if (DEBUG >= CTLFLOW) printf("Starting cleanup\n");

      // Unmap the framebuffer
      munmap(fbp, len_fb_mem);
      // Restore original screen
      if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo)){
        if (DEBUG >=  1) printf("Error restoring original screen info\n");
      }

      // Unmap pages
      munmap(fbpages, PAGES*len_fb_mem);

      if (kbfd >= 0) {
        ioctl(kbfd, KDSETMODE, KD_TEXT);
        close(kbfd);
      }

      // Close framebuffer
      close(fbfd);
      if (DEBUG >= 1) printf("Closed framebuffer device\n");

      if (DEBUG >= CTLFLOW) printf("Ending cleanup\n");
    }


    void draw_pixel(int x, int y, int c)
    {
      if (DEBUG >= CTLFLOW+1) printf("Starting put_pixel\n");

      // Calculate pixel's byte offset inside the buffer
      unsigned int pix_offset = (x * vinfo.bits_per_pixel / 8) + y * finfo.line_length;

      // Similar to fbpage[pix_offset] = c
      *((short*)(fbpages + pix_offset + (page * len_fb_mem))) = c;
    }

    void clear_screen(int c)
    {
      if (DEBUG >= CTLFLOW+1) printf("Starting clear_screen\n");

      memset(fbpages + (page * len_fb_mem),c,len_fb_mem);
    }

    // Writes page to fbp and incr page
    void switch_page()
    {
      if (DEBUG >= CTLFLOW) printf("Starting draw\n");

      memcpy(fbp,fbpages + (page * len_fb_mem),len_fb_mem);
      page = (page + 1)%PAGES;
    }
};


int main(int argc, char* argv[])
{
  SmallRpiScreen sc = SmallRpiScreen();
  long c;
  for(c=0; c < 0b1111111111011111; c=c+0b0000100001000001) {
    sc.clear_screen(c);
    usleep(1000000);
    sc.switch_page();
  }
  return(0);
}
