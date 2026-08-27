/*
 * SPDX-FileCopyrightText: 2021-2026 LAT Project Authors
 *
 * SPDX-License-Identifier: GPL-2.0-only
 */

#ifndef _PROFILE_H_
#define _PROFILE_H_

#include "common.h"
#include "env.h"
#include "reg-alloc.h"
#include "translate.h"
#include "profile-runtime.h"

void latx_profile_init(CPUX86State *env);
void per_tb_count(target_ulong pc, LatxTBProfileCounter counter, int inc);

#ifdef CONFIG_LATX_PROFILER
#define PER_TB_COUNT(tb, counter, inc) \
    per_tb_count((tb)->pc, (counter), (inc))
#else
#define PER_TB_COUNT(tb, counter, inc) ((void)0)
#endif

#endif
