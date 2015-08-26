#ifndef __SINGLE_INSTANCE_MAP_H__
#define __SINGLE_INSTANCE_MAP_H__

#include "Util.h"
#include <unordered_map>

using namespace std;

	template<typename T>
	class SingleInstanceMap
	{

	public: //Construction

		SingleInstanceMap(){}

		~SingleInstanceMap()
		{

		}

		SingleInstanceMap(const SingleInstanceMap&) = delete;
		SingleInstanceMap& operator=(const SingleInstanceMap&) = delete;

	private: //Data

		unordered_map<string, T*> m_items{};

	public: //Interface

		bool containsItem(string _sFileName)
		{

			unordered_map<string, T*>::iterator _iter = m_items.find(_sFileName);

			if (_iter == m_items.end())
				return false;
			else
				return true;
		}

		bool addItem(string _sFileName, T* _pItem)
		{
			if (containsItem(_sFileName))
				return false;

			m_items[_sFileName] = _pItem;

			return true;
		}

		T* getItem(string _sFileName)
		{
			assert(containsItem(_sFileName) && "Tried to get an item that does not exist");

			return m_items[_sFileName];
		}

		void shutDownRelease()
		{
			for (auto& pItem : m_items)
				SAFE_RELEASE(pItem.second);

			m_items.clear();
		}

		void shutDownDelete()
		{
			for (auto& pItem : m_items)
				SAFE_DELETE(pItem.second);

			m_items.clear();
		}
	};

#endif //__SINGLE_INSTANCE_MAP_H__