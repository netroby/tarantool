#ifndef TARANTOOL_REPLICATION_RELAY_H_INCLUDED
#define TARANTOOL_REPLICATION_RELAY_H_INCLUDED
/*
 * Copyright 2010-2016, Tarantool AUTHORS, please see AUTHORS file.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above
 *    copyright notice, this list of conditions and the
 *    following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * <COPYRIGHT HOLDER> OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "evio.h"
#include "fiber.h"
#include "vclock.h"
#include "xstream.h"

struct replica;
struct tt_uuid;

/** State of a replication relay. */
struct relay {
	/** The thread in which we relay data to the replica. */
	struct cord cord;
	/** Replica connection */
	struct ev_io io;
	/* Request sync */
	uint64_t sync;
	struct recovery *r;
	struct xstream stream;
	struct vclock stop_vclock;
	ev_tstamp wal_dir_rescan_delay;
	uint32_t replica_id;
};

/**
 * Send initial JOIN rows to the replica
 *
 * @param fd        client connection
 * @param sync      sync from incoming JOIN request
 */
void
relay_initial_join(int fd, uint64_t sync);

/**
 * Send final JOIN rows to the replica.
 *
 * @param fd        client connection
 * @param sync      sync from incoming JOIN request
 */
void
relay_final_join(int fd, uint64_t sync, struct vclock *start_vclock,
	         struct vclock *stop_vclock);

/**
 * Subscribe a replica to updates.
 *
 * @return none.
 */
void
relay_subscribe(int fd, uint64_t sync, struct replica *replica,
		struct vclock *replica_vclock);

#endif /* TARANTOOL_REPLICATION_RELAY_H_INCLUDED */
