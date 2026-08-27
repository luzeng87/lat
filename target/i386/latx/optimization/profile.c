/*
 * SPDX-FileCopyrightText: 2021-2026 LAT Project Authors
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#include "profile.h"

#ifdef CONFIG_LATX_PROFILER
static uint64_t *latx_profile_counters;

uint32_t latx_profile_hash(target_ulong pc)
{
    uint64_t value = pc;

    value ^= value >> 33;
    value *= UINT64_C(0xff51afd7ed558ccd);
    value ^= value >> 33;
    value *= UINT64_C(0xc4ceb9fe1a85ec53);
    value ^= value >> 33;
    return value & (LATX_TB_PROFILE_SLOTS - 1);
}

static size_t latx_profile_offset(target_ulong pc,
                                  LatxTBProfileCounter counter)
{
    return (((size_t)counter << LATX_TB_PROFILE_BITS) |
            latx_profile_hash(pc)) * sizeof(uint64_t);
}

void latx_profile_init(CPUX86State *env)
{
    if (!latx_profile_counters) {
        latx_profile_counters = g_new0(uint64_t,
            LATX_TB_PROFILE_SLOTS * LATX_PROFILE_COUNTER_MAX);
    }
    env->latx_profile_counters = latx_profile_counters;
}

uint64_t latx_profile_read(target_ulong pc, LatxTBProfileCounter counter)
{
    size_t index = latx_profile_offset(pc, counter) / sizeof(uint64_t);

    return latx_profile_counters ? latx_profile_counters[index] : 0;
}

inline void per_tb_count(target_ulong pc, LatxTBProfileCounter counter,
                         int inc)
{
    la_profile_begin();
    IR2_OPND base = ra_alloc_itemp();
    IR2_OPND offset = ra_alloc_itemp();
    IR2_OPND value = ra_alloc_itemp();

    la_ld_d(base, env_ir2_opnd,
            offsetof(CPUX86State, latx_profile_counters));
    li_d(offset, latx_profile_offset(pc, counter));
    la_ldx_d(value, base, offset);
    la_addi_d(value, value, inc);
    la_stx_d(value, base, offset);
    ra_free_temp(base);
    ra_free_temp(offset);
    ra_free_temp(value);
    la_profile_end();
}
#endif
