#include "SmallRpiScreen.h"

SmallRpiScreen::SmallRpiScreen()
{
	int w, h, bpp;
	init(&w, &h, &bpp);
}

SmallRpiScreen::SmallRpiScreen(int *w, int *h, int *bpp)
{
	init(w,h,bpp);
}

SmallRpiScreen::~SmallRpiScreen()
{
	debug("Starting SmallRpiScreen cleanup");

	// Unmap framebuffer
	munmap(fbp, len_fb);

	// Restore orig screen
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo))
		debug("Error restoring orig screen info");

	debug("Restored orig screen");

	// Unmap pages (sm screen specific)
	munmap(fbpage, PAGES*len_fb);

	// Restore cursor blink; close keyboard
	if (kbfd >= 0) {
		ioctl(kbfd, KDSETMODE, KD_TEXT);
		close(kbfd);
		debug("Restored orig keyboard");
	}

	// Close framebuffer
	close(fbfd);
	debug("Closed framebuffer device");

	debug("Finished cleanup");
}

void SmallRpiScreen::draw_pixel(int x, int y, int c)
{
	debug("Drawing pixel", 3);

	// Calculate pixel's offset inside the buffer
	unsigned int pix_offset = (x * vinfo.bits_per_pixel / 8)+ y * finfo.line_length;

	//Do page calc (not in this implementation)

	// Similar to fbpage[pix_offset] = c 
	//(Normally would use fbp and page offset)
	*((short*)(fbpage + pix_offset + (page * len_fb))) = c;
}

void SmallRpiScreen::draw_rect(int x,int y, int w, int h, int c)
{

	debug("Drawing rect",2);

	int i,j;
	for(j=0;j<h;j++){
	  for(i=0;i<w;i++){
	    draw_pixel(x+i,y+j,c);
	  }
	}
}

void SmallRpiScreen::clear_screen(int c)
{
	debug("Clearing Screen", 2);

	int i,j;
	for(i = 0; i < vinfo.xres; i++){
	  for(j = 0; j < vinfo.yres; j++){
	    draw_pixel(i,j+(page * vinfo.yres),c);
	  }
	}
}

void SmallRpiScreen::switch_page()
{
	debug("Switching page", 2);

	// Naive implementation because we don't have virtual fbp space
	memcpy(fbp, fbpage + (page * len_fb), len_fb);
	page = (page + 1) % PAGES;
}

int SmallRpiScreen::init(int *w, int *h, int *bpp)
{
	FRAMEBUFFER = "/dev/fb1";
	CONSOLE = "/dev/tty1";

        PAGES = 1;

        fbfd = 0; // Framebuffer file descriptor
        kbfd = 0; // Keyboard file descriptor

        len_fb = 0; // Length of framebuffer memory
        fbp = 0; // Framebuffer pointer

        fbpage = 0; // Pointer to the page memory location, because
                                          // The small screen can't load in any virtua$
        page = 0;  // Current page being written to (always 0 for sm screen)


	debug("Starting SmallRpiScreen init");

	// Open framebuffer device file
	fbfd = open(FRAMEBUFFER, O_RDWR);
	if (fbfd == -1){
	  debug("Error: cannot open framebuffer device.");
	  return (-1);
	}

	printf("%d <<< FBFD\n",(int)fbfd);


	debug("Framebuffer device opened");

	// Open console and hide cursor (Do this in keyboard???)
	kbfd = open(CONSOLE, O_WRONLY);
	if (kbfd >= 0) {
		ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
	}
	else{
	  debug("Could not open console to disable cursor.");
	  return -1;
	}

	debug("Diabled cursor blink.");

	// Get orig variable screen info
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)){
	  debug("Error reading orig vinfo");
	  return -1;
	}

	if(DEBUG >= 1) printf("Original info: %dx%d, %dbpp\n",
		       vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Store orig vinfo
	memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

	//Change vinfo (not in this implementation... but we can try)
	/*vinfo.xres = *w;
	vinfo.yres = *h;
	vinfo.bits_per_pixel = *bpp;
	vinfo.xres_virtual = *w;*/
	//vinfo.yres_virtual = vinfo.yres * 2;

	// Set vinfo
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)){
	  debug("Error setting vinfo");
	  return -1;
	}

	if(DEBUG >= 1) printf("New info: %dx%d, %dbpp\n          %dx%d virtual\n",
		       vinfo.xres, vinfo.yres, vinfo.bits_per_pixel,
		       vinfo.xres_virtual, vinfo.yres_virtual);

	// Get fix screen info
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)){
	  debug("Error reading finfo");
	  return -1;
	}

	if (DEBUG >= 1) printf("Fixed info: %d smem_len, %d line_length\n",
		                   finfo.smem_len, finfo.line_length);

	// Map framebuffer to user memory
	len_fb = finfo.smem_len;

	fbp = (char*) mmap(0, len_fb,
				  PROT_READ | PROT_WRITE,
				  MAP_SHARED,
				  fbfd, 0);
	if ((int)fbp == -1){
	  debug("Failed to map a pointer to fb");
	  return -1;
	}

	fbpage = (char*)malloc(len_fb*PAGES);

	if ((int)fbpage == -1){
	  debug("Failed to allocate pages");
	  return -1;
	}

	*w = vinfo.xres;
	*h = vinfo.yres;
	*bpp = vinfo.bits_per_pixel;

	debug("Finished init");

	return 0;
}
/*
int main(int argc, char *argv[])
{
  int w = 320, h = 240, bpp = 16;
  SmallRpiScreen sc(&w,&h,&bpp);
  printf("\n-------------------\nDisplay: %dx%d, %dbpp\n", w, h,bpp);
  int s = 10;
  int i,j;
  for(i = 0; i < s; i++){
    for(j = 0; j < s; j++){
      sc.draw_pixel(i,j,0xF81F);
    }
  }
  sc.switch_page();
  sleep(1);
  sc.clear_screen(0xF000);
  sc.draw_pixel(100,100,0x07E0);
  sc.switch_page();
  sleep(3);
}
*/
