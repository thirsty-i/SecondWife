#ifndef _H_LOCK_H_
#define _H_LOCK_H_

struct Lock {};
struct NoLock {};


#include "mutex_lock.hpp"
#include "null_lock.hpp"
#include "spin_lock.hpp"
#include "lock_guard.h"

#endif // _H_LOCK_H_