#pragma once
#include "..//PlayerState.h"

class PShotManager;

/******************************************************************
*	PlayerDeadClass
**/

class Player;
class PlayerContext;

class PlayerDead final
	: public PlayerState
{
public:
	PlayerDead(Player* pOwner);
	~PlayerDead() override;
	
	//‰Šú‰»ˆ—
	void Enter() override;
	//–ˆƒtƒŒ[ƒ€“ü‚éˆ—
	void Execute() override;
	//State‚©‚ço‚é‚Æ‚«‚É“ü‚éˆ—.
	void Exit() override;

	void Update() override;
	void Draw() override;
	void Init() override;
private:
};