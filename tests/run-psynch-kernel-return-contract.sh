#!/usr/bin/env bash
# Runtime gate for dar-q95.29.2: psynch wait return decoding keeps base errno
# distinct from condvar status bits, retries only EINTR, and preserves
# condvar/mutex/rwlock handoff under signal interruption.
set -euo pipefail

here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
. "$here/guest-verdict-test-lib.sh"

GUEST_VERDICT_GUEST_PRELUDE="export PTHREAD_MUTEX_USE_ULOCK=0"

guest_verdict_run_c \
	psynch_kernel_return_contract \
	"$here/psynch_kernel_return_contract.c" \
	PSYNCH_KERNEL_RETURN_CONTRACT_OK \
	"${PSYNCH_CONTRACT_TIMEOUT:-90}" \
	-lpthread
