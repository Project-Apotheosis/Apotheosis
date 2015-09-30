#ifndef __IBUTTON_H__
#define __IBUTTON_H__

#include "Util.h"

class Executable
{
public:
	Executable(){}
	virtual ~Executable(){}

	Executable(const Executable&) = delete;
	Executable& operator=(const Executable&) = delete;

private: //Data

	std::vector<void(*)()> m_executableQueue{};

public: //Interface

	void addToExeQueue(void(*_pFunc)())
	{
		m_executableQueue.push_back(_pFunc);
	}

	virtual void execute()
	{
		for (auto pFunc : m_executableQueue)
			pFunc();
	};
};



class IButton
{

public:

	IButton(){}
	virtual ~IButton()
	{
		SAFE_DELETE(m_pExecutable);
	}

	IButton(const IButton&) = delete;
	IButton& operator=(const IButton&) = delete;

protected: //Data

	Executable* m_pExecutable{};
	UINT m_iCurrentTextureIndex{};

public: //Interface

	void initExecutable(const std::vector<void(*)()>& _krCallbacks)
	{
		assert(m_pExecutable == nullptr);

		m_pExecutable = new Executable();

		for (auto pFunc : _krCallbacks)
			m_pExecutable->addToExeQueue(pFunc);

	}

	virtual void highlight(bool highlighted) = 0;

	virtual void click()
	{	
		m_pExecutable->execute();
	}

};




#endif //__IBUTTON_SCENE_H__