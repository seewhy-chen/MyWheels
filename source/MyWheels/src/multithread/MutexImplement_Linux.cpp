#include "MutexImplement.h"

#ifdef __LINUX__

namespace mwl {
    Mutex::Implement::Implement() {
    }

    Mutex::Implement::~Implement() {
    }

	int32_t Mutex::Implement::_Lock() {
		return -1;
	}

	int32_t Mutex::Implement::_TryLock() {
		return -1;
	}

	int32_t Mutex::Implement::_Unlock() {
		return -1;
	}
}

#endif // __LINUX__
