#ifndef __C_LIBRARY_H
#define __C_LIBRARY_H

#pragma systemFile

// Given a structure, you can figure out how many bytes-offset
//  a specific member of a struct is
// The "C_" is needed as RobotC doesn't allow the "offsetof" keyword
#define C_offsetof(st, m) ((int)(&((st *)0)->m))

// Fixed-width variables
#define int8_t 					byte
#define uint8_t 				ubyte
#define int16_t 				int
#define uint16_t 				unsigned int
#define int32_t 				long
#define uint32_t 				unsigned long
#define intptr_t				int

// RobotC doesn't allow these to be defined
// #define intmax_t 				long
// #define intmax_t 				unsigned long

// int_leastN_t is guarenteed to be atleast N bits
#define int_least8_t 		long
#define uint_least8_t 	unsigned long
#define int_least16_t 	long
#define uint_least16_t 	unsigned long
#define int_least32_t 	long
#define uint_least32_t 	unsigned long

// RobotC doesn't allow these to be defined
// #define int_fast8_t 		long
// #define int_fast8_t 		unsigned long
// #define int_fast16_t 	long
// #define int_fast16_t 	unsigned long
// #define int_fast32_t 	long
// #define int_fast32_t 	unsigned long

// Min/Max of variables as defined on the RobotC webpage:
//   http://help.robotc.net/WebHelpArduino/index.htm#page=general_programming/Data%20Types/Data_Types.htm
#define INTMAX_MIN -2147483648
#define INTMAX_MAX 2147483647
#define UINTMAX_MAX 4294967295
#define INT8_MIN -128
#define INT8_MAX 127
#define UINT8_MAX 255
#define INT16_MIN -32768
#define INT16_MAX 32767
#define UINT16_MAX 65535
#define INT32_MIN -2147483648
#define INT32_MAX 2147483647
#define UINT32_MAX 4294967295

// Checks if a character is an alphabetical letter
bool isalpha(char c)
{
	return (c >= 'a' && c <= 'z')||(c >= 'A' && c <= 'Z');
}

// Returns true if a character is a blank space
bool isblank(char c)
{
	return (c == ' ') || (c == '\t');
}

// Returns true if the character is a control character
// (Other wise defined as in between 0x00 and 0x1F)
bool iscntrl(char c)
{
	return (c >= 0x00 && c <= 0x1F);
}

// Returns true if the character is a number
bool isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

// Returns true if the character is a hexidecimal number
bool isxdigit(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}


#endif
