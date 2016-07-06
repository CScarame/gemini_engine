
#include "RpiKeyboard.h"

void RpiKeyboard::update_keys(short, char)
{
	if (remap == -1) {
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

	// ONLY FOR REMAPPING KEYS
	else if (down) {
		keycodes[remap] = keycode;
		remap = -1;
	}
}

RpiKeyboard::RpiKeyboard()
{
	debug("Initializing RpiKeyboard");

	fd = 0;
	oldkbmode = K_RAW;
	
	active = 0;

	keys = 0;

	remap = -1;

	// Open and config the keyboard */
	fd = open("/dev/tty", O_RDONLY | O_NDELAY);
	tcgetattr(fd, &orig_kb);
	tcgetattr(fd, &new_kb);
	new_kb.c_lflag &= ~(ECHO | ICANON | ISIG);
	new_kb.c_iflag = IXOFF;
	new_kb.c_cc[VTIME] = 0;
	tcsetattr(fd, TCSAFLUSH, &new_kb);

	// Set med raw
	ioctl(fd, KDGKBMODE, &oldkbmode);
	ioctl(fd, KDSKBMODE, K_MEDIUMRAW);

	// Perform a sys call to prevent repeats
	system("sudo kbdrate -r 2.0 -d 1000000 -s");

	debug("Successfully initialized keyboard");

	active = 1; // Shows the keyboard is active
}

RpiKeyboard::~RpiKeyboard()
{
	ioctl(fd, KDSKBMODE, oldkbmode);

	// Perform a sys call to restore defaults
	system("sudo kbdrate -s");

	tcsetattr(fd, 0, &orig_kb);
	close(fd);

	debug("Cleaned up keyboard");
	active = 0; // Shows the keyboard is inactive
}

void RpiKeyboard::update()
{
	if (!active) return;

	debug("Update keyboard",3);

	char down; // Used to hold whether the key was moved to up or down
	unsigned char keycode; // Used to hold the keycode of the presed key

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

char RpiKeyboard::check_key(keyflag_t)
{
	char result = (keys & (1 << k)) ? 1 : 0;
}

int RpiKeyboard::remap_key(keyflag_t)
{
	remap = (int)k;
	int temp = remap;

	// The following is used to freeze everything while the key is being remapped
	while (remap != -1) {
		tick();
	}
	return (int)keycodes[temp];
}
