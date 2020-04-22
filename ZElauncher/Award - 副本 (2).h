#pragma once
#include "stdafx.h"
#include <time.h>

using namespace DuiLib;

class CAwardUI :
	public CContainerUI,
	public INotifyUI
{
public:
	CAwardUI(CPaintManagerUI& m_pm);
private:
	void Notify(TNotifyUI& msg);

};
