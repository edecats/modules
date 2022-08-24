/**
* Copyright (c) 2020 Cosylab d.d.
* This software is distributed under the terms found
* in file LICENSE.txt that is included with this distribution.
*/

#ifndef RWLOCK_H
#define RWLOCK_H

#include <pthread.h>
#include <stdexcept>

/* Reader/writer lock implementation based on POSIX rwlocks.
 *
 * - Don't acquire locks recursively.
 * - Don't release locks if currently not holding a lock.
 * */
class RWLock {
protected:
    pthread_rwlock_t rwlock;
    pthread_rwlockattr_t rwlock_attr;

    RWLock();

public:
    static const int prefer_reader = PTHREAD_RWLOCK_PREFER_READER_NP;
    static const int prefer_writer = PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP;

    /* Flags chould be set to either prefer_reader or prefer_writer. */
    RWLock(int flags);
    ~RWLock();

    int lock_read();
    int try_lock_read();
    int unlock_read();

    int lock_write();
    int try_lock_write();
    int unlock_write();
};

#endif /* RWLOCK_H */
