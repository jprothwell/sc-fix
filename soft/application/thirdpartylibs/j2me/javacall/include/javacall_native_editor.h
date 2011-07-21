

#ifndef __JAVACALL_NATIVE_EDITOR_H
#define __JAVACALL_NATIVE_EDITOR_H


#ifdef __cplusplus
extern "C" {
#endif

#define ANY 0
#define EMAILADDR 1
#define NUMERIC 2
#define PHONENUMBER 3
#define URL 4
#define DECIMAL 5
#define PASSWORD 0x10000
#define UNEDITABLE 0x20000
#define SENSITIVE 0x40000
#define NON_PREDICTIVE 0x80000
#define INITIAL_CAPS_WORD 0x100000
#define INITIAL_CAPS_SENTENCE 0x200000
#define CONSTRAINT_MASK 0xFFFF

int javacall_launch_native_editor(int Constraints, unsigned short* in, int inlen, unsigned short* title, int titlelen, unsigned short* out, int maxoutlen);



#ifdef __cplusplus
}
#endif

#endif

