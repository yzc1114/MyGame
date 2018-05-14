#ifndef _SAVECONTROL_
#define _SAVECONTROL_

#include"MagicTower.h"

class SaveControl : public Singleton<SaveControl>
{
public:
	SaveControl();
	~SaveControl();
	void save(int order);
	void load(int order);
	bool checkIfTheSaveExisted(int order);
};


#endif
