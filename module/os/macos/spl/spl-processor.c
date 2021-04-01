/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */

/*
 *
 * Copyright (C) 2013 Jorgen Lundman <lundman@lundman.net>
 *
 */

#include <sys/processor.h>
#include <i386/cpuid.h>

extern int cpu_number(void);

#ifdef __x86_64__
#define	cpuid(func, a, b, c, d) \
	__asm__ __volatile__( \
	"        pushq %%rbx        \n" \
	"        xorq %%rcx,%%rcx   \n" \
	"        cpuid              \n" \
	"        movq %%rbx, %%rsi  \n" \
	"        popq %%rbx         \n" : \
	"=a" (a), "=S" (b), "=c" (c), "=d" (d) : "a" (func))
#else /* Add ARM */
#define	cpuid(func, a, b, c, d) \
	a = b = c = d = 0
#endif

static uint64_t cpuid_features = 0ULL;
static uint64_t cpuid_features_leaf7 = 0ULL;
static boolean_t cpuid_has_xgetbv = B_FALSE;

uint32_t
getcpuid()
{
	// return ((uint32_t)cpu_number());
	return (0);
}

uint64_t
spl_cpuid_features(void)
{
	i386_cpu_info_t *info;

	info = cpuid_info();
	return (info->cpuid_features);
}

uint64_t
spl_cpuid_leaf7_features(void)
{
	i386_cpu_info_t *info;

	info = cpuid_info();
	return (info->cpuid_leaf7_features);
}
