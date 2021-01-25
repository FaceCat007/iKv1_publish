/*��������èFaceCat��� v1.0
�Ϻ����è��Ϣ�������޹�˾
 */

#ifndef __FCLOCK__H__
#define __FCLOCK__H__
#pragma once

namespace FaceCat
{
	class FCLock
	{
	public:
		FCLock();
		~FCLock();

		void lock();
		void unLock();
	private:
		CRITICAL_SECTION m_cs;
	};
}

#endif
