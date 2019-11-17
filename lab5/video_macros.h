#pragma once

#include <lcom/lcf.h>

#define BASE_PHYS_ADDRESS 0
#define MiB BIT(20)
#define VG_MODE_INDEXED 4
#define VG_MODE_DIRECT 6


/* VBE FUNCTIONS */

#define VBE_AH_FUNCTION_INDICATOR 0x4F
#define VBE_AL_CONTROLLER_INFO 0x00
#define VBE_AL_MODE_INFO 0x01
#define VBE_AL_SET_MODE 0x02
#define VBE_BX_SET_MODE_ARGUMENT BIT(14)

#define VBE_INTERRUPT_NUMBER 0x10

// Returns are on the AX
#define VBE_FUNCTION_RET_SUCCESS 0x00
#define VBE_FUNCTION_RET_FAIL 0x01
#define VBE_FUNCTION_RET_NOT_SUPPORTED 0x02
#define VBE_FUNCTION_RET_INVALID_IN_CURRENT_MODE 0x03

