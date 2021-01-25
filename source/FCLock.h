/*基于捂脸猫FaceCat框架 v1.0
上海卷卷猫信息技术有限公司
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
