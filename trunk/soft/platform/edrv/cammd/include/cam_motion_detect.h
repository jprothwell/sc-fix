/***************************
for camera motion detect
sheen.
2010.8.13
****************************/


#ifndef MMC_MOTION_DETECT_H
#define MMC_MOTION_DETECT_H

//sensitive level
typedef enum
{
SENSE_LOW,
SENSE_MID,
SENSE_HIGH
}MD_LEVEL;

int motion_detect_open(MD_LEVEL sensLevel, void(*md_callback)(int32));
int motion_detect_close();
void do_motion_detect(void);


#endif

