#ifndef KEYBOARD_H
#define KEYBOARD_H

// Enum for key inputs
typedef enum
{
	k_up,
	k_left,
	k_down,
	k_right,
	k_jump,
	k_stance,
	k_a,
	k_b,
	k_x,
	k_y,
	k_enter,
	k_esc,
	num_keys
} keyflag_t;

class Keyboard
{
public:
	Keyboard() {};

	~Keyboard() {};

	virtual void update() = 0;
	// Updates the internal key variables to reflect the status of the keyboard

	virtual int get_keys() = 0;
	// Returns an int with bits for each key

	virtual int check_key(keyflag_t) = 0;
	// Returns boolean "if key is down"
};



#endif
