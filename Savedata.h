#pragma once

//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include "input.h"

//#include "scene.h"
//#include "manager.h"
//#include "gameObject.h"

class Savedata
{
public:
	Savedata() {}

	virtual ~Savedata(){}


	void Save();	// 配置したブロックのセーブ

	void Load();	// 配置したブロックのロード


};

