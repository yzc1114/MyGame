#ifndef _SAVECONTROL_
#define _SAVECONTROL_

#include"MagicTower.h"
#include"sqlite3\include\sqlite3.h"
class SaveControl : public Singleton<SaveControl>
{
public:
	SaveControl();
	~SaveControl();
	void save(int order);
	void load(int order);
	bool checkIfTheSaveExisted(int order);
	Dictionary* root;
	Dictionary* Save[5];
	std::string writablePath;
	std::string fullPath;
};


#endif
