/*
 * SPDX-FileCopyrightText: 2021-2026 LAT Project Authors
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef _PROFILE_RUNTIME_H_
#define _PROFILE_RUNTIME_H_

typedef enum LatxTBProfileCounter {
    LATX_PROFILE_EXEC,
    LATX_PROFILE_JRRA_IN,
    LATX_PROFILE_JRRA_MISS,
    LATX_PROFILE_COUNTER_MAX,
} LatxTBProfileCounter;

/* Keep counters outside TranslationBlock so TU/AOT replacement is safe. */
#define LATX_TB_PROFILE_BITS 21
#define LATX_TB_PROFILE_SLOTS (1U << LATX_TB_PROFILE_BITS)

uint64_t latx_profile_read(target_ulong pc, LatxTBProfileCounter counter);
uint32_t latx_profile_hash(target_ulong pc);

#endif
