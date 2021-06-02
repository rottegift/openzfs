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
 * OSX Atomic functions using clang builtins.
 *
 * Jorgen Lundman <lundman@lundman.net>
 *
 */

#ifndef _SPL_ATOMIC_H
#define	_SPL_ATOMIC_H
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wsign-conversion"
#pragma GCC diagnostic warning "-Wshorten-64-to-32"
#pragma GCC diagnostic warning "-Wimplicit-int-conversion"
#pragma GCC diagnostic warning "-Weverything"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <sys/types.h>
#include <string.h>

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * Increment target
 */

#define	ATOMIC_INC(name, type)						\
	static inline							\
	void atomic_inc_##name(volatile type *target)			\
	{								\
		(void) __atomic_add_fetch(target, 1, __ATOMIC_SEQ_CST); \
	}


ATOMIC_INC(8, uint8_t)
ATOMIC_INC(16, uint16_t)
ATOMIC_INC(32, uint32_t)
ATOMIC_INC(64, uint64_t)

#define	ATOMIC_INC_NV(name, type)			    \
	static inline					    \
	type atomic_inc_##name##_nv(volatile type * target) \
	{						    \
	return (__atomic_add_fetch(target, 1, 		    \
		__ATOMIC_SEQ_CST));			    \
	    }

ATOMIC_INC_NV(32, uint32_t)
ATOMIC_INC_NV(64, uint64_t)

/*
 * Decrement target
 */

#define	ATOMIC_DEC(name, type)						\
	static inline							\
        void atomic_dec_##name(volatile type *target)			\
        {								\
		(void) __atomic_sub_fetch(target, 1,			\
		    __ATOMIC_SEQ_CST);					\
	}

ATOMIC_DEC(8, uint8_t)
ATOMIC_DEC(16, uint16_t)
ATOMIC_DEC(32, uint32_t)
ATOMIC_DEC(64, uint64_t)

#define	ATOMIC_DEC_NV(name, type)				\
	static inline						\
	type atomic_dec_##name##_nv(volatile type *target)	\
	{							\
		return (__atomic_sub_fetch(target, 1,		\
			__ATOMIC_SEQ_CST));			\
	}

ATOMIC_DEC_NV(32, uint32_t)
ATOMIC_DEC_NV(64, uint64_t)

/*
 * Add delta to target
 */

#define	ATOMIC_ADD(name, type1, type2)					\
	static inline							\
        void atomic_add_##name(volatile type1 *target, type2 bits)      \
        {                                                               \
		(void) __atomic_add_fetch((volatile type2 *) target,	\
		    bits, __ATOMIC_SEQ_CST);				\
        }

ATOMIC_ADD(8, uint8_t, int8_t)
ATOMIC_ADD(16, uint16_t, int16_t)
ATOMIC_ADD(32, uint32_t, int32_t)
ATOMIC_ADD(64, uint64_t, int64_t)

#define	ATOMIC_ADD_NV(name, type1, type2)			\
	static inline						\
	type1 atomic_add_##name##_nv(volatile type1 *target,	\
	    type2 x)						\
	{							\
		return ((type1)__atomic_add_fetch(		\
			(volatile type2 *)target, x,		\
			__ATOMIC_SEQ_CST));			\
	}

ATOMIC_ADD_NV(8, uint8_t, int8_t)
ATOMIC_ADD_NV(16, uint16_t, int16_t)
ATOMIC_ADD_NV(32, uint32_t, int32_t)
ATOMIC_ADD_NV(64, uint64_t, int64_t)

/*
 * Subtract delta to target
 */


#define	ATOMIC_SUB(name, type1, type2)					\
	static inline							\
        void atomic_sub_##name(volatile type1 *target, type2 bits)      \
        {                                                               \
		(void) __atomic_sub_fetch((volatile type2 *)target,	\
		    bits, __ATOMIC_SEQ_CST);				\
        }

ATOMIC_SUB(8, uint8_t, int8_t)
ATOMIC_SUB(16, uint16_t, int16_t)
ATOMIC_SUB(32, uint32_t, int32_t)
ATOMIC_SUB(64, uint64_t, int64_t)

static inline uint64_t
atomic_sub_64_nv(volatile uint64_t *target, int64_t delta)
{
	return ((uint64_t)__atomic_sub_fetch((volatile int64_t *)
		target, delta, __ATOMIC_SEQ_CST));
}

/*
 * logical OR bits with target
 */

#define ATOMIC_OR(name, type)						\
	static inline							\
        void atomic_or_##name(volatile type *target, type bits)         \
        {                                                               \
                (void) __atomic_or_fetch(target, bits, __ATOMIC_SEQ_CST); \
        }

ATOMIC_OR(8, uint8_t)
ATOMIC_OR(16, uint16_t)
ATOMIC_OR(32, uint32_t)
ATOMIC_OR(64, uint64_t)

/*
 * logical AND bits with target
 */

#define ATOMIC_AND(name, type)						\
	static inline							\
        void atomic_and_##name(volatile type *target, type bits)        \
        {                                                               \
                (void) __atomic_and_fetch(target, bits, __ATOMIC_SEQ_CST); \
        }

ATOMIC_AND(8, uint8_t)
ATOMIC_AND(16, uint16_t)
ATOMIC_AND(32, uint32_t)
ATOMIC_AND(64, uint64_t)

/*
 * Compare And Set
 * if *arg1 == arg2, then set *arg1 = arg3; return old value.
 */

#define ATOMIC_CAS(name, type) \
	static inline							\
        type atomic_cas_##name(volatile type *target, type exp, type des) \
        {                                                               \
		__atomic_compare_exchange_n(target,			\
		    &exp, des, B_FALSE,					\
                    __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);                \
                return (exp);                                           \
        }

ATOMIC_CAS(8, uint8_t)
ATOMIC_CAS(16, uint16_t)
ATOMIC_CAS(32, uint32_t)
ATOMIC_CAS(64, uint64_t)

#define ATOMIC_SWAP(name, type)						\
	static inline							\
        type atomic_swap_##name(volatile type *target, type bits)       \
        {                                                               \
                return (__atomic_exchange_n(target, bits, __ATOMIC_SEQ_CST)); \
        }

ATOMIC_SWAP(8, uint8_t)
ATOMIC_SWAP(16, uint16_t)
ATOMIC_SWAP(32, uint32_t)
ATOMIC_SWAP(64, uint64_t)

static inline void *
atomic_cas_ptr(void *target, void *exp, void *des)
{
        __atomic_compare_exchange_n((void **)target,
	    &exp, des, B_FALSE,
            __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
        return (exp);
}

static inline void
membar_producer(void)
{
	__c11_atomic_thread_fence(__ATOMIC_SEQ_CST);
}

#ifdef	__cplusplus
}
#endif
#pragma GCC diagnostic pop
#endif  /* _SPL_ATOMIC_H */
