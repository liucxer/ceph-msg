#include "shared_mutex_debug.h"

#include <system_error>

#include "include/acconfig.h"
#include "common/valgrind.h"

namespace ceph {

shared_mutex_debug::shared_mutex_debug(const std::string& n,
                                       bool track_lock,
                                       bool enable_lock_dep,
                                       bool prioritize_write)
  : mutex_debugging_base{n, false /* backtrace */},
    track(track_lock),
    lockdep(enable_lock_dep)
{
#ifdef HAVE_PTHREAD_RWLOCKATTR_SETKIND_NP
  if (prioritize_write) {
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    // PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP
    //   Setting the lock kind to this avoids writer starvation as long as
    //   long as any read locking is not done in a recursive fashion.
    pthread_rwlockattr_setkind_np(&attr,
                                  PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
    pthread_rwlock_init(&rwlock, &attr);
    pthread_rwlockattr_destroy(&attr);
  } else
#endif
  // Next block is in {} to possibly connect to the above if when code is used.
  {
    pthread_rwlock_init(&rwlock, NULL);
  }
  ANNOTATE_BENIGN_RACE_SIZED(&id, sizeof(id), "shared_mutex_debug lockdep id");
  ANNOTATE_BENIGN_RACE_SIZED(&nlock, sizeof(nlock), "shared_mutex_debug nwlock");
  ANNOTATE_BENIGN_RACE_SIZED(&nrlock, sizeof(nrlock), "shared_mutex_debug nrlock");
}

// exclusive
void shared_mutex_debug::lock()
{
  if (g_lockdep && lockdep) {
    _will_lock();
  }
  if (int r = pthread_rwlock_wrlock(&rwlock); r != 0) {
    throw std::system_error(r, std::generic_category());
  }
  if (lockdep && g_lockdep) {
    _locked();
  }
  _post_lock();
}

bool shared_mutex_debug::try_lock()
{
  int r = pthread_rwlock_trywrlock(&rwlock);
  switch (r) {
  case 0:
    if (lockdep && g_lockdep) {
      _locked();
    }
    _post_lock();
    return true;
  case EBUSY:
    return false;
  default:
    throw std::system_error(r, std::generic_category());
  }
}

void shared_mutex_debug::unlock()
{
  _pre_unlock();
  if (lockdep && g_lockdep) {
    _will_unlock();
  }
  if (int r = pthread_rwlock_unlock(&rwlock); r != 0) {
    throw std::system_error(r, std::generic_category());
  }
}

// shared locking
void shared_mutex_debug::lock_shared()
{
  if (lockdep && g_lockdep) {
    _will_lock();
  }
  if (int r = pthread_rwlock_rdlock(&rwlock); r != 0) {
    throw std::system_error(r, std::generic_category());
  }
  if (lockdep && g_lockdep) {
    _locked();
  }
  _post_lock_shared();
}

bool shared_mutex_debug::try_lock_shared()
{
  if (lockdep && g_lockdep) {
    _will_unlock();
  }
  switch (int r = pthread_rwlock_rdlock(&rwlock); r) {
  case 0:
    if (lockdep && g_lockdep) {
      _locked();
    }
    _post_lock_shared();
    return true;
  case EBUSY:
    return false;
  default:
    throw std::system_error(r, std::generic_category());
  }
}

void shared_mutex_debug::unlock_shared()
{
  _pre_unlock_shared();
  if (lockdep && g_lockdep) {
    _will_unlock();
  }
  if (int r = pthread_rwlock_unlock(&rwlock); r != 0) {
    throw std::system_error(r, std::generic_category());
  }
}

// exclusive locking
void shared_mutex_debug::_pre_unlock()
{
  if (track) {
    ceph_assert(nlock > 0);
    --nlock;
    ceph_assert(locked_by == std::this_thread::get_id());
    ceph_assert(nlock == 0);
    locked_by = std::thread::id();
  }
}

void shared_mutex_debug::_post_lock()
{
  if (track) {
    ceph_assert(nlock == 0);
    locked_by = std::this_thread::get_id();
    ++nlock;
  }
}

// shared locking
void shared_mutex_debug::_pre_unlock_shared()
{
  if (track) {
    ceph_assert(nrlock > 0);
    nrlock--;
  }
}

void shared_mutex_debug::_post_lock_shared()
{
  if (track) {
    ++nrlock;
  }
}

} // namespace ceph
