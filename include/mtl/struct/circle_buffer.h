#ifndef _H_CIRCLE_BUFFER_H_
#define _H_CIRCLE_BUFFER_H_

#include "type_int.h"
#include "allocator.h"
#include "bitops.h"
#include "log.h"
#include "lock.h"
#include <assert.h>

namespace mtl
{
	enum class CircleBufferLock
	{
		cblSingleWriteSingleRead,
		cblSingleWriteMutilRead,
		cblMutilWriteSingleRead,
		cblMutilWriteMutilRead,
	};

	template <CircleBufferLock>
	struct _CircleBufferLockTraits // cblSingleWriteSingleRead
	{
		using WLock = CNullLock;
		using RLock = CNullLock;
	};

	template <>
	struct _CircleBufferLockTraits<CircleBufferLock::cblSingleWriteMutilRead>
	{
		using WLock = CNullLock;
		using RLock = CSpinLock;
	};

	template <>
	struct _CircleBufferLockTraits<CircleBufferLock::cblMutilWriteSingleRead>
	{
		using WLock = CSpinLock;
		using RLock = CNullLock;
	};

	template <>
	struct _CircleBufferLockTraits<CircleBufferLock::cblMutilWriteMutilRead>
	{
		using WLock = CSpinLock;
		using RLock = CSpinLock;
	};


	template <CircleBufferLock LockModel>
	class CCircleBuffer
	{
	public:
		using value_type = BYTE;
		using pointer = value_type*;
		using size_type = UINT32;
		using void_pointer = void*;

		using LockTraits = _CircleBufferLockTraits<LockModel>;

		using read_lock_type = typename LockTraits::RLock;
		using write_lock_type = typename LockTraits::WLock;

		CCircleBuffer()
		{
			Reset(0, 0);
		}

		CCircleBuffer(void_pointer pBuffer, const size_type dwSize)
			: m_Capacity(RoundupPowOf2(dwSize))
		{
			assert(m_Capacity >= 2);
			Reset(pBuffer, dwSize);
		}

		~CCircleBuffer() = default;

		void Reset(pointer pBuffer, const size_type dwSize)
		{
			m_pBuffer = pBuffer;
			m_dwIn = m_dwOut = 0;
			m_Capacity = dwSize;
		}

		size_type Read(pointer pDst, size_type dwLen)
		{
			CLockGuard<read_lock_type> oLockGuard(m_ReadLock);
			
			UINT32 dwUsed = Used();
			if (dwLen > dwUsed)
				dwLen = dwUsed;

			_CopyOut(pDst, dwLen);
			m_dwOut += dwLen;

			return dwLen;
		}

		size_type Write(pointer pSrc, size_type dwLen)
		{
			CLockGuard<write_lock_type> oLockGuard(m_WriteLock);

			UINT32 dwUnUsed = UnUsed();
			if (dwLen > dwUnUsed)
				dwLen = dwUnUsed;

			_CopyIn(pSrc, dwLen);
			m_dwIn += dwLen;

			return dwLen;
		}

		/*Push and Pop*/
		size_type UnUsed() { return m_Capacity - (m_dwIn - m_dwOut); }
		size_type Used() { return m_dwIn - m_dwOut; }

		void _CopyIn(pointer pSrc, size_type dwLen)
		{
			UINT32 dwOff = m_dwIn & (m_Capacity - 1);
			UINT32 dwCopyLen = min(dwLen, m_Capacity - dwOff);

			memcpy(m_pBuffer + dwOff, pSrc, dwCopyLen);
			memcpy(m_pBuffer, pSrc + dwCopyLen, (dwLen - dwCopyLen));
		}

		void _CopyOut(pointer pDst, size_type dwLen)
		{
			UINT32 dwOff = m_dwOut & (m_Capacity - 1);
			UINT32 dwCopyLen = min(dwLen, m_Capacity - dwOff);

			memcpy(pDst, m_pBuffer + dwOff, dwCopyLen);
			memcpy(pDst + dwCopyLen, m_pBuffer, dwLen - dwCopyLen);
		}

	private:
		read_lock_type m_ReadLock;
		write_lock_type m_WriteLock;

		size_type m_Capacity;  /* Maximum capacity is 2^31 */   
		volatile size_type m_dwIn;
		volatile size_type m_dwOut;
		pointer m_pBuffer;
	};

}
#endif // _H_CIRCLE_BUFFER_H_