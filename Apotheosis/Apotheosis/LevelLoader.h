// Bachelor of Software Engineering 
// Media Design School 
// Auckland
// New Zealand
// 
// (c) 2005 - 2099 Media Design School 
// 
// File Name : LevelLoader.h
// Description :  LevelLoader header
// Author : Serge Radinovich
// Mail : srad015@hotmail.com

#ifndef __LEVEL_LOADER_H__
#define __LEVEL_LOADER_H__

#include "pugiconfig.hpp"
#include "pugixml.hpp"
#include "Util.h"
#include "IGameScene.h"


class LevelLoader
{
public:

	static void fromXML(const string& _rkXMLFileName, array<Platform, 10>& _rPlatforms)
	{
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(_rkXMLFileName.c_str());

		pugi::xml_node _levelNode = doc.child("Profile").child("Level");

		//Platforms
		pugi::xml_node _platforms = _levelNode.child("Platforms");
		if (_platforms)
		{
			UINT i = 0;
			for (pugi::xml_node platform = _platforms.first_child(); platform; platform = platform.next_sibling())
			{
				if (i >= _rPlatforms.size())
					break;

				_rPlatforms[i].setActive(true);
				_rPlatforms[i].setPosition(nodePositionX(platform), nodePositionY(platform));
				pugi::xml_node _rotation = platform.child("Rotation");
				if (_rotation)
					_rPlatforms[i].setRotation(_rotation.text().as_float());
				i++;
			}
		}

	}

	static float nodePositionX(pugi::xml_node _node)
	{
		return _node.child("Position").child("X").text().as_float();
	}

	static float nodePositionY(pugi::xml_node _node)
	{
		return _node.child("Position").child("Y").text().as_float();
	}

	static void setNodePos(pugi::xml_node _node, b2Vec2 _pos)
	{
		stringstream _ss1; _ss1 << _pos.x;
		stringstream _ss2; _ss2 << _pos.y;

		_node.append_child().set_name("Position");
		_node.child("Position").append_child("X");
		_node.child("Position").child("X").append_child(pugi::node_pcdata).set_value(_ss1.str().c_str());
		_node.child("Position").append_child("Y");
		_node.child("Position").child("Y").append_child(pugi::node_pcdata).set_value(_ss2.str().c_str());
	}


	static void toXML(const string& _rkXMLFileName, array<Platform,10>& _rPlatforms)
	{
		pugi::xml_document doc;

		pugi::xml_node _profile = doc.append_child();
		_profile.set_name("Profile");

		_profile.append_attribute("Format") = "1";
		_profile.append_child().set_name("Level");

		pugi::xml_node _level = _profile.child("Level");

		//Platforms
		_level.append_child().set_name("Platforms");
		pugi::xml_node _platforms = _level.child("Platforms");

		//Create the nodes 
		for (int i = _rPlatforms.size() - 1; i >= 0; i--)
		{
			if (!_rPlatforms[i].isActive())
				break;

			_platforms.append_child().set_name("Platform");
		}
		//Create the positions for each new node
		UINT j = _rPlatforms.size() - 1;
		for (pugi::xml_node newNode = _platforms.first_child(); newNode; newNode = newNode.next_sibling())
		{
			setNodePos(newNode, _rPlatforms[j].getPosition());
			j--;
		}
		

		doc.save_file((_rkXMLFileName).c_str());


	}
};

#endif //__LEVEL_LOADER_H__