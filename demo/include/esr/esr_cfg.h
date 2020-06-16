#pragma once

#define G_CFG_ESR	CFG_ESR::get_inst()

#define CFG_FILE_NAME	"./esr.cfg"
#define SEC_NAME		esr
#define CFG_NAME		CFG_ESR
#include "cfg_template.h"
#undef	CFG_FILE_NAME
#undef	SEC_NAME
#undef	CFG_NAME