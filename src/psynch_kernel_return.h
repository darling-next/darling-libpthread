/*
 * Copyright (c) 2026 Darling project contributors.
 *
 * Internal helpers for Darling's psynch wait return contract.
 */

#ifndef _PTHREAD_PSYNCH_KERNEL_RETURN_H
#define _PTHREAD_PSYNCH_KERNEL_RETURN_H

#include <stdint.h>

/*
 * Darling routes emulated psynch wait failures through the syscall/cerror
 * boundary. Condvar waits may carry psynch status bits above the low errno
 * byte; pthread policy code must see both the base errno and the status bits.
 */
static const uint32_t _PTHREAD_PSYNCH_STATUS_MASK = ~0xffu;

typedef struct {
	uint32_t raw_error;
	uint32_t base_error;
	uint32_t status_bits;
	uint32_t updateval;
} _pthread_psynch_kernel_return_t;

OS_ALWAYS_INLINE
static inline uint32_t
_pthread_psynch_kernel_error_base(uint32_t err)
{
	return err & 0xff;
}

OS_ALWAYS_INLINE
static inline _pthread_psynch_kernel_return_t
_pthread_psynch_kernel_return_decode(uint32_t updateval)
{
	_pthread_psynch_kernel_return_t decoded = {
		.raw_error = 0,
		.base_error = 0,
		.status_bits = 0,
		.updateval = 0,
	};

	if (updateval == (uint32_t)-1) {
		decoded.raw_error = errno;
	} else if ((int32_t)updateval < 0) {
		decoded.raw_error = -(int32_t)updateval;
	} else {
		decoded.updateval = updateval;
		return decoded;
	}

	decoded.base_error = _pthread_psynch_kernel_error_base(decoded.raw_error);
	decoded.status_bits = decoded.raw_error & _PTHREAD_PSYNCH_STATUS_MASK;
	return decoded;
}

#endif /* _PTHREAD_PSYNCH_KERNEL_RETURN_H */
