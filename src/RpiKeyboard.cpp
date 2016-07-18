
#include "RpiKeyboard.h"

RpiKeyboard::RpiKeyboard()
{
	debug("Initializing RpiKeyboard");

	fd = 0;
	oldkbmode = K_RAW;

	active = 0;

	keys = 0;

	remap = -1;

	keycodes[0] =  17; // W
	keycodes[1] =  30; // A
	keycodes[2] =  31; // S
	keycodes[3] =  32; // D
	keycodes[4] =  57; // Space
	keycodes[5] =  42; // LShift
	keycodes[6] =  36; // J
	keycodes[7] =  37; // K
	keycodes[8] =  38; // L
	keycodes[9] =  39; // :
	keycodes[10] = 28; // Enter
	keycodes[11] = 1;  // Esc

	// Open and config the keyboard */
	fd = open("/dev/tty", O_RDONLY | O_NDELAY);
	check(fd == 0, "Failed to open tty");
	tcgetattr(fd, &orig_kb);
	tcgetattr(fd, &new_kb);
	new_kb.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_kb.c_iflag = IXOFF;
	new_kb.c_cc[VTIME] = 0;
	tcsetattr(fd, TCSAFLUSH, &new_kb);

	// Set med raw
	check( ioctl(fd, KDGKBMODE, &oldkbmode) , "Failed to get oldkbmode");
	check( ioctl(fd, KDSKBMODE, K_MEDIUMRAW), "Failed to set keyboard to med raw");

	// Perform a sys call to prevent repeats
	system("sudo kbdrate -r 2.0 -d 1000000 -s");

	debug("Successfully initialized keyboard");

	active = 1; // Shows the keyboard is active

error:
	if(fd){
		if(oldkbmode != K_RAW){
			ioctl(fd, KDSKBMODE, oldkbmode);
		}

		tcsetattr(fd, 0, &orig_kb);
		close(fd);
	}
	// Perform a sys call to restore defaults
	system("sudo kbdrate -r 20.0 -d 500 -s");

	debug("Cleaned up keyboard");
	active = 0; // Shows the keyboard is inactive
}

RpiKeyboard::~RpiKeyboard()
{
	ioctl(fd, KDSKBMODE, oldkbmode);

	// Perform a sys call to restore defaults
	system("sudo kbdrate -r 20.0 -d 500 -s");

	tcsetattr(fd, 0, &orig_kb);
	close(fd);

	debug("Cleaned up keyboard");
	active = 0; // Shows the keyboard is inactive
}

void RpiKeyboard::update()
{
	if (!active) return;

	debug("Update keyboard");

	int down; // Used to hold whether the key was moved to up or down
	int keycode; // Used to hold the keycode of the presed key

	unsigned char buf;  // Characters are read one at a time to this buffer.

	while (read(fd, &buf, 1) >= 0) {
		down = !(buf & 0x80); // bit is 0 for down, 1 for up
		keycode = buf & 0x7F;    // The rest is put into keycode
		update_keys(keycode, down);
	}
}

int RpiKeyboard::get_keys()
{
	return keys;
}

int RpiKeyboard::check_key(keyflag_t k)
{
	return keys & (1 << k);
}

/*int RpiKeyboard::remap_key(keyflag_t k)
{
	remap = (int)k;
	int temp = remap;

	// The following is used to freeze everything while the key is being remapped
	while (remap != -1) {
		update();
	}
	return (int)keycodes[temp];
}*/

void RpiKeyboard::update_keys(int keycode, int down)
{
	int i;
	for (i = 0; i<num_keys; i++) { // For each key, check if this is its keycode
		if (keycode == keycodes[i]) {
			if (down) {
				keys |= (1 << i); // Mask the bit on
			}
			else {
				keys &= ~(1 << i);// Mask the bit off
			}
		}
	}
}
