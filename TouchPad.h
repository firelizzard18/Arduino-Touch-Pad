#if !defined(_TOUCH_PAD_H)
#define _TOUCH_PAD_H

class TouchPad {
private:
	unsigned char		x1, x2,
						y1, y2,
						c1[2], c2[2];
	static const char	keypad[4][4];
	
public:
	TouchPad(char x1pin, char x2pin, char y1pin, char y2pin);
	
	void initialize();
	void initialize(char i1[2], char i2[2]);
	
	void test();
	void print(char x, char y, bool sgn);
	
	unsigned char readXRaw();
	unsigned char readYRaw();
	
	void blockUntilPress();
	void blockUntilRelease();
	
	bool validate(const char *coord);
	
	void readRaw(unsigned char *coord, bool block);
	void read(char *coord, bool block);
	
	char key(bool block_in, bool block_out);
};

#endif