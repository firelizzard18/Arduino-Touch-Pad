#include "TouchPad.h"

#include <Arduino.h>

const char TouchPad::keypad[4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

TouchPad::TouchPad(char x1pin, char x2pin, char y1pin, char y2pin) {
	x1 = x1pin;
	x2 = x2pin;
	y1 = y1pin;
	y2 = y2pin;
}

void TouchPad::initialize() {
	do {
		readRaw(c1, true);
		blockUntilRelease();
	} while (c1[0] > 50 || c1[1] > 50);
	
	do {
		readRaw(c2, true);
		blockUntilRelease();
	} while (c2[0] < 50 || c2[1] < 50);
}

void TouchPad::initialize(char i1[2], char i2[2]) {
	c1[0] = i1[0];
	c1[1] = i1[1];
	c2[0] = i2[0];
	c2[1] = i2[1];
}

void TouchPad::test() {
	print(readXRaw(), readYRaw(), true);
}

void TouchPad::print(char x, char y, bool sgn) {
	Serial.print('(');
	if (sgn) {
		Serial.print(x, DEC);
	} else {
		Serial.print((unsigned char)x);
	}
	Serial.print(", ");
	if (sgn) {
		Serial.print(y, DEC);
	} else {
		Serial.print((unsigned char)y);
	}
	Serial.print(')');
	Serial.println();
}

unsigned char TouchPad::readXRaw() {
	pinMode(x1, INPUT);
	pinMode(x2, INPUT);
	pinMode(y1, OUTPUT);
	pinMode(y2, OUTPUT);
	
	digitalWrite(y1, LOW);
	digitalWrite(y2, HIGH);
	
	return (unsigned char) ((analogRead(x1) + analogRead(x2)) >> 3);
}

unsigned char TouchPad::readYRaw() {
	pinMode(x1, OUTPUT);
	pinMode(x2, OUTPUT);
	pinMode(y1, INPUT);
	pinMode(y2, INPUT);
	
	digitalWrite(x1, LOW);
	digitalWrite(x2, HIGH);
	
	return (unsigned char) ((analogRead(y1) + analogRead(y2)) >> 3);
}

void TouchPad::blockUntilPress() {
	while (readXRaw() < 10 || readYRaw() < 10);
}

void TouchPad::blockUntilRelease() {
	while (readXRaw() > 10 || readYRaw() > 10);
}

bool TouchPad::validate(const char* coord) {
	return coord[0] > 10 && coord[1] > 10;
}

void TouchPad::readRaw(unsigned char *coord, bool block) {
	if (block) {
		blockUntilPress();
		delay(25);
	}
	
	coord[0] = readXRaw();
	coord[1] = readYRaw();
}

void TouchPad::read(char *coord, bool block) {
	unsigned char raw[2];
	readRaw(raw, block);
	
	coord[0] = (raw[0] - c1[0]) * 100 / (c2[0] - c1[0]);
	coord[1] = (raw[1] - c1[1]) * 100 / (c2[1] - c1[1]);
}

char TouchPad::key(bool block_in, bool block_out) {
	char c[2];
	read(c, block_in);
	if (block_out) {
		blockUntilRelease();
	}
	
	if (c[0] > 0 && c[0] < 100 && c[1] > 0 && c[1] < 100)
		return keypad[c[0]/25][c[1]/33];
	else
		return '\0';
}