#include "RpiScreen.h"
#include <errno.h>

RpiScreen::RpiScreen()
{
	int w, h, bpp;
	RpiScreen(&w, &h, &bpp);
}

RpiScreen::RpiScreen(int *w, int *h, int *bpp)
{
	FRAMEBUFFER = "/dev/fb0";
	CONSOLE = "/dev/tty";

	PAGES = 2;

	fbfd = 0; // Framebuffer file descriptor
	kbfd = 0; // Keyboard file descriptor

	len_fb = 0; // Length of framebuffer memory
	fbp = 0; // Framebuffer pointer

	page = 0;  // Current page being written to (always 0 for sm screen)
	page_size = 0;


	debug("Starting RpiScreen init");

	// Open framebuffer device file
	fbfd = open(FRAMEBUFFER, O_RDWR);
	if (fbfd == -1) {
		debug("Error: cannot open framebuffer device.");
		return;
	}

	printf("%d <<< FBFD",(int)fbfd);

	debug("Framebuffer device opened");

	// Get orig variable screen info
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo)) {
		debug("Error reading orig vinfo");
		return;
	}

	// Store orig vinfo
	memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));

	//Change vinfo according to input params
	vinfo.xres = *w;
	vinfo.yres = *h;
	vinfo.bits_per_pixel = *bpp;
	vinfo.xres_virtual = vinfo.xres;
	vinfo.yres_virtual = vinfo.yres * PAGES;

	if (DEBUG >= 1) printf("Original info: %dx%d, %dbpp\n",
		orig_vinfo.xres, orig_vinfo.yres, orig_vinfo.bits_per_pixel);

	// Set vinfo
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo)) {
		debug("Error setting vinfo");
		return;
	}

	if (DEBUG >= 1){
		 printf("New info: %dx%d, %dbpp\n          %dx%d virtual\n",
		vinfo.xres, vinfo.yres, vinfo.bits_per_pixel,
		vinfo.xres_virtual, vinfo.yres_virtual);
	}

	printf("Test\n");

	// Open console and hide cursor (Do this in keyboard???)
	kbfd = open(CONSOLE, O_WRONLY);
	if (kbfd >= 0) {
		ioctl(kbfd, KDSETMODE, KD_GRAPHICS);
	}
	else {
		debug("Could not open console to disable cursor.");
		return;
	}

	debug("Diabled cursor blink.");

	// Get fix screen info
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
		debug("Error reading finfo");
		return;
	}

	if (DEBUG >= 1) printf("Fixed info: %d smem_len, %d line_length\n",
		finfo.smem_len, finfo.line_length);

	page_size = finfo.line_length * vinfo.yres;

	// Map framebuffer to user memory
	len_fb = finfo.smem_len;

	printf("\n%d <<< len_fb\n", len_fb);

	fbp = (char*)mmap(0,
		len_fb,
		PROT_READ | PROT_WRITE,
		MAP_SHARED,
		fbfd, 0);

	printf("%d <<< ERRNO\n",errno);

	if ((int)fbp == -1) {
		debug("Failed to map a pointer to fb");
		return;
	}

	*w = vinfo.xres;
	*h = vinfo.yres;
	*bpp = vinfo.bits_per_pixel;

	debug("Finished init");
}

RpiScreen::~RpiScreen()
{
	debug("Starting RpiScreen cleanup");

	// Unmap framebuffer
	munmap(fbp, len_fb);

	// Restore cursor blink; close keyboard
	if (kbfd >= 0) {
		ioctl(kbfd, KDSETMODE, KD_TEXT);
		close(kbfd);
		debug("Restored orig keyboard");
	}

	memcpy(&vinfo, &orig_vinfo, sizeof(struct fb_var_screeninfo));

	// Restore orig screen
	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
		debug("Error restoring orig screen info");

	debug("Restored orig screen");

	// Close framebuffer
	close(fbfd);
	debug("Closed framebuffer device");

	debug("Finished cleanup");
}

void RpiScreen::draw_pixel(int x, int y, int c)
{
	debug("Drawing pixel", 3);

	// Calculate pixel's offset inside the buffer
	unsigned int pix_offset = (x * vinfo.bits_per_pixel / 8) + y * finfo.line_length;

	//Do page calc

	pix_offset += page * page_size;

	// Similar to fbp[pix_offset+page_offset] = c 
	*((short*)(fbp + pix_offset)) = c;
}

void RpiScreen::draw_rect(int x, int y, int w, int h, int c)
{

	debug("Drawing rect", 2);

	int i, j;
	for (j = 0; j<h; j++) {
		for (i = 0; i<w; i++) {
			draw_pixel(x + i, y + j, c);
		}
	}
}

void RpiScreen::clear_screen(int c)
{
	debug("Clearing Screen", 2);

	int i, j;
	for (i = 0; i < vinfo.xres; i++) {
		for (j = 0; j < vinfo.yres; j++) {
			draw_pixel(i, j, c);
		}
	}
}

void RpiScreen::switch_page()
{
	debug("Switching page", 2);

	// Change yoffset
	vinfo.yoffset = page * vinfo.yres;
	__u32 dummy = 0;
	ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo);
	ioctl(fbfd, FBIO_WAITFORVSYNC, &dummy);

	page = (page + 1) % PAGES;
}

/*
int main(int argc, char *argv[])
{
  int w = 320, h = 240, bpp = 16;
  RpiScreen sc(&w,&h,&bpp);
  sleep(1);
  // Display a test of colors:
	sc.clear_screen(0);
	sc.switch_page();
	sc.clear_screen(0);
	unsigned int i,j,xx,yy;
	for (i = 0; i < (1 << 16); i++) {
		yy = i / w;
		xx = i % w;

		sc.draw_pixel(xx, yy, i);
	}
	sc.switch_page();
	sleep(2);
	for (i = (1 << 16); i > 0; i--) {
		j = (1<<16) - i;
		yy = j / w;
		xx = j % w;

		sc.draw_pixel(xx, yy, i);
	}
	sc.switch_page();
	sleep(2);
}

*/
