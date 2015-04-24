#include "EmfClip.h"
#include "EmfOutputDevice.h"

namespace MetaFile
{
	CEmfClip::CEmfClip()
	{

	}
	CEmfClip::~CEmfClip()
	{
		Clear();
	}
	void CEmfClip::operator=(CEmfClip& oClip)
	{
		Clear();
		for (unsigned int ulIndex = 0; ulIndex < oClip.m_vCommands.size(); ulIndex++)
		{
			CEmfClipCommandBase* pCommand = oClip.m_vCommands.at(ulIndex);
			CEmfClipCommandBase* pNewCommand = NULL;
			switch (pCommand->GetType())
			{
				case EMF_CLIPCOMMAND_INTERSECT:
				{
					pNewCommand = new CEmfClipCommandIntersect(((CEmfClipCommandIntersect*)pCommand)->m_oRect);
					break;
				}
				case EMF_CLIPCOMMAND_SETPATH:
				{
					CEmfClipCommandPath* pPathCommand = (CEmfClipCommandPath*)pCommand;
					pNewCommand = new CEmfClipCommandPath(&pPathCommand->m_oPath, pPathCommand->m_unMode);
					break;
				}
			}

			if (pNewCommand)
				m_vCommands.push_back(pNewCommand);
		}
	}
	void CEmfClip::Reset()
	{
		Clear();
	}
	bool CEmfClip::Intersect(TEmfRectL& oRect)
	{
		CEmfClipCommandBase* pCommand = new CEmfClipCommandIntersect(oRect);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	bool CEmfClip::SetPath(CEmfPath* pPath, unsigned int unMode)
	{
		CEmfClipCommandBase* pCommand = new CEmfClipCommandPath(pPath, unMode);
		if (!pCommand)
			return false;

		m_vCommands.push_back(pCommand);
		return true;
	}
	void CEmfClip::ClipOnRenderer(CEmfOutputDevice* pOutput)
	{
		if (!pOutput)
			return;

		pOutput->ResetClip();
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CEmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			switch (pCommand->GetType())
			{
				case EMF_CLIPCOMMAND_INTERSECT:
				{
					CEmfClipCommandIntersect* pIntersect = (CEmfClipCommandIntersect*)pCommand;
					pOutput->IntersectClip(pIntersect->m_oRect.lLeft, pIntersect->m_oRect.lTop, pIntersect->m_oRect.lRight, pIntersect->m_oRect.lBottom);
					break;
				}
				case EMF_CLIPCOMMAND_SETPATH:
				{
					CEmfClipCommandPath* pClipPath = (CEmfClipCommandPath*)pCommand;
					pClipPath->m_oPath.Draw(pOutput, false, false, pClipPath->m_unMode);
					break;
				}
			}
		}

	}
	void CEmfClip::Clear()
	{
		for (unsigned int ulIndex = 0; ulIndex < m_vCommands.size(); ulIndex++)
		{
			CEmfClipCommandBase* pCommand = m_vCommands.at(ulIndex);
			delete pCommand;
		}
		m_vCommands.clear();
	}
}