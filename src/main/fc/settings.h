#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "config/parameter_group.h"

#include "fc/settings_generated.h"

typedef struct lookupTableEntry_s {
    const char * const *values;
    const uint8_t valueCount;
} lookupTableEntry_t;

extern const lookupTableEntry_t settingLookupTables[];

#define SETTING_TYPE_OFFSET 0
#define SETTING_SECTION_OFFSET 4
#define SETTING_MODE_OFFSET 6

typedef enum {
    // value type, bits 0-3
    VAR_UINT8 = (0 << SETTING_TYPE_OFFSET),
    VAR_INT8 = (1 << SETTING_TYPE_OFFSET),
    VAR_UINT16 = (2 << SETTING_TYPE_OFFSET),
    VAR_INT16 = (3 << SETTING_TYPE_OFFSET),
    VAR_UINT32 = (4 << SETTING_TYPE_OFFSET),
    VAR_FLOAT = (5 << SETTING_TYPE_OFFSET), // 0x05

    // value section, bits 4-5
    MASTER_VALUE = (0 << SETTING_SECTION_OFFSET),
    PROFILE_VALUE = (1 << SETTING_SECTION_OFFSET),
    CONTROL_RATE_VALUE = (2 << SETTING_SECTION_OFFSET), // 0x20
    // value mode, bits 6-7
    MODE_DIRECT = (0 << SETTING_MODE_OFFSET),
    MODE_LOOKUP = (1 << SETTING_MODE_OFFSET), // 0x40
} settingFlag_e;

#define SETTING_TYPE_MASK (0x0F)
#define SETTING_SECTION_MASK (0x30)
#define SETTING_MODE_MASK (0xC0)

#define SETTING_TYPE(s)       ((s)->type & SETTING_TYPE_MASK)
#define SETTING_SECTION(s)    ((s)->type & SETTING_SECTION_MASK)
#define SETTING_MODE(s)       ((s)->type & SETTING_MODE_MASK)

typedef struct settingMinMaxConfig_s {
    const uint8_t indexes[SETTING_MIN_MAX_INDEX_BYTES];
} settingMinMaxConfig_t;

typedef struct settingLookupTableConfig_s {
    const uint8_t tableIndex;
} settingLookupTableConfig_t;

typedef union {
    settingLookupTableConfig_t lookup;
    settingMinMaxConfig_t minmax;
} settingConfig_t;

typedef struct {
    const uint8_t encoded_name[SETTING_ENCODED_NAME_MAX_BYTES];
    const uint8_t type; // see settingFlag_e
    const settingConfig_t config;
    const setting_offset_t offset; // TODO: make const?

} __attribute__((packed)) setting_t;

extern const setting_t settingsTable[];

void setting_get_name(const setting_t *val, char *buf);
bool setting_name_contains(const setting_t *val, char *buf, const char *cmdline);
bool setting_name_exact_match(const setting_t *val, char *buf, const char *cmdline, uint8_t var_name_length);
pgn_t setting_get_pgn(const setting_t *val);
// Returns the minimum valid value for the given setting_t. setting_min_t
// depends on the target and build options, but will always be a signed
// integer (e.g. intxx_t,)
setting_min_t setting_get_min(const setting_t *val);
// Returns the maximum valid value for the given setting_t. setting_max_t
// depends on the target and build options, but will always be an unsigned
// integer (e.g. uintxx_t,)
setting_max_t setting_get_max(const setting_t *val);