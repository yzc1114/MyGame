#include "MagicTower.h"

class ChatRoom : public Scene
{
public:
	ChatRoom();
	~ChatRoom();
	virtual bool init();
	CREATE_FUNC(ChatRoom);
	enum {
		kZBGLayer,
		kZInputBoxBGLayer,
		kZTextBoxBGLayer
	};
};

