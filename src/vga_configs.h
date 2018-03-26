#ifndef _VGA_CONFIGS_H_
#define _VGA_CONFIGS_H_

#include "inttypes.h"

#define VGA_AC_RESET            0x3DA
#define VGA_AC_INDEX            0x3C0
#define VGA_AC_WRITE            0x3C0
#define VGA_AC_READ             0x3C1

#define VGA_MISC_WRITE          0x3C2
#define VGA_MISC_READ           0x3CC

#define VGA_SEQ_INDEX           0x3C4
#define VGA_SEQ_WRITE           0x3C5
#define VGA_SEQ_READ            0x3C5

#define VGA_DAC_READ_INDEX      0x3C7
#define VGA_DAC_WRITE_INDEX     0x3C8
#define VGA_DAC_WRITE           0x3C9
#define VGA_DAC_READ            0x3C9

#define VGA_GC_INDEX            0x3CE
#define VGA_GC_READ             0x3CF
#define VGA_GC_WRITE            0x3CF

#define VGA_CRTC_INDEX          0x3D4 /* if in MTEXT mode, 0x3B4 */
#define VGA_CRTC_READ           0x3D5 /* if in MTEXT mode, 0x3B5 */
#define VGA_CRTC_WRITE          0x3D5 /* if in MTEXT mode, 0x3B5 */

typedef enum {
    AC  = VGA_AC_INDEX,
    MISC= VGA_MISC_WRITE,
    SEQ = VGA_SEQ_INDEX,
    DAC = VGA_DAC_WRITE_INDEX,
    GC  = VGA_GC_INDEX,
    CRTC= VGA_CRTC_INDEX
} vga_regs;

typedef struct
{
    uint8_t twidth;
    uint8_t theightm1;
    uint8_t cheight;
    uint8_t slength_l;
    uint8_t slength_h;
    uint8_t sequ_regs[4];
    uint8_t miscreg;
    uint8_t crtc_regs[25];
    uint8_t actl_regs[20];
    uint8_t grdc_regs[9];
} vga_config_param;

static vga_config_param config_mode_0x03 =
{
    /* tw, th-1, ch, slength */
    80, 24, 16, 0x00, 0x10,
    /* sequ_regs */
    0x00, 0x03, 0x00, 0x02,
    /* miscreg */
    0x67,
    /* crtc_regs */
    0x5f, 0x4f, 0x50, 0x82, 0x55, 0x81, 0xbf, 0x1f,
    0x00, 0x4f, 0x0d, 0x0e, 0x00, 0x00, 0x00, 0x00,
    0x9c, 0x8e, 0x8f, 0x28, 0x1f, 0x96, 0xb9, 0xa3,
    0xff,
    /* actl_regs */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x0c, 0x00, 0x0f, 0x08,
    /* grdc_regs */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0e, 0x0f, 0xff,
};

static uint8_t palette[64*3]=
{
  0x00,0x00,0x00, 0x00,0x00,0x2a, 0x00,0x2a,0x00, 0x00,0x2a,0x2a, 0x2a,0x00,0x00, 0x2a,0x00,0x2a, 0x2a,0x2a,0x00, 0x2a,0x2a,0x2a,
  0x00,0x00,0x15, 0x00,0x00,0x3f, 0x00,0x2a,0x15, 0x00,0x2a,0x3f, 0x2a,0x00,0x15, 0x2a,0x00,0x3f, 0x2a,0x2a,0x15, 0x2a,0x2a,0x3f,
  0x00,0x15,0x00, 0x00,0x15,0x2a, 0x00,0x3f,0x00, 0x00,0x3f,0x2a, 0x2a,0x15,0x00, 0x2a,0x15,0x2a, 0x2a,0x3f,0x00, 0x2a,0x3f,0x2a,
  0x00,0x15,0x15, 0x00,0x15,0x3f, 0x00,0x3f,0x15, 0x00,0x3f,0x3f, 0x2a,0x15,0x15, 0x2a,0x15,0x3f, 0x2a,0x3f,0x15, 0x2a,0x3f,0x3f,
  0x15,0x00,0x00, 0x15,0x00,0x2a, 0x15,0x2a,0x00, 0x15,0x2a,0x2a, 0x3f,0x00,0x00, 0x3f,0x00,0x2a, 0x3f,0x2a,0x00, 0x3f,0x2a,0x2a,
  0x15,0x00,0x15, 0x15,0x00,0x3f, 0x15,0x2a,0x15, 0x15,0x2a,0x3f, 0x3f,0x00,0x15, 0x3f,0x00,0x3f, 0x3f,0x2a,0x15, 0x3f,0x2a,0x3f,
  0x15,0x15,0x00, 0x15,0x15,0x2a, 0x15,0x3f,0x00, 0x15,0x3f,0x2a, 0x3f,0x15,0x00, 0x3f,0x15,0x2a, 0x3f,0x3f,0x00, 0x3f,0x3f,0x2a,
  0x15,0x15,0x15, 0x15,0x15,0x3f, 0x15,0x3f,0x15, 0x15,0x3f,0x3f, 0x3f,0x15,0x15, 0x3f,0x15,0x3f, 0x3f,0x3f,0x15, 0x3f,0x3f,0x3f 
};

#endif // _VGA_CONFIGS_H_