#include "Award.h"

CAwardUI::CAwardUI(CPaintManagerUI& m_pam)
{
	CDialogBuilder Builder;
	CContainerUI* pAward = static_cast<CContainerUI*>(Builder.Create(_T("Award.xml"), NULL, NULL, &m_pam));
	if (pAward) {
		this->Add(pAward);
		m_pam.AddNotifier(this);//注册
	}
	else {
		this->RemoveAll();
		return;
	}
}

void CAwardUI::Notify(TNotifyUI& msg)
{
	

}
