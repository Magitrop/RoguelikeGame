#pragma once

#define Initializer GameInitializer::Instance()
#define Controller GameController::Instance()

#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "TilemapMath.h"
#include "Tilemap.h"
#include "Time.h"
#include "Creature.h"
#include "Camera.h"
#include "MapController.h"
#include "GameController.h"
#include "GameInitializer.h"
#include "Animation.h"
#include "MapHandler.h"

class MapController;
class CreatureSpiderling : public Creature
{
private:
	int curPathIndex = 0;
	List<Tile*>* curPath;
	Tile* tileToMove;

	Animation** idleAnim;
	Animation** movementAnim;
	Animation** attackAnim;
	Animation** deathAnim;
	Animation** deadAnim;
	Animation* _lastPlayedAnimation;

	double curHealthInHealthbar;
protected:
	virtual void InitializeAnimations(void) override
	{
		idleAnim = new Animation * [4]
		{
			// facing left
			new Animation(0.25, 2,
			{
				new AnimationFrame(16, 16, 0 * 16, 0 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 1 * 16, 0 * 16, 0.5, 0.5)
			}),
			// facing right
			new Animation(0.25, 2,
			{
				new AnimationFrame(16, 16, 0 * 16, 1 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 1 * 16, 1 * 16, 0.5, 0.5)
			}),
			// facing down
			new Animation(0.25, 2,
			{
				new AnimationFrame(16, 16, 0 * 16, 2 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 1 * 16, 2 * 16, 0.5, 0.5)
			}),
			// facing up
			new Animation(0.25, 2,
			{
				new AnimationFrame(16, 16, 0 * 16, 3 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 1 * 16, 3 * 16, 0.5, 0.5)
			}),
		};
		movementAnim = new Animation * [4]
		{
			// facing left
			new Animation(0.1, 3,
			{
				new AnimationFrame(16, 16, 2 * 16, 0 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 3 * 16, 0 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 4 * 16, 0 * 16, 0.5, 0.5)
			}),
			// facing right
			new Animation(0.1, 3,
			{
				new AnimationFrame(16, 16, 2 * 16, 1 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 3 * 16, 1 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 4 * 16, 1 * 16, 0.5, 0.5)
			}),
			// facing down
			new Animation(0.1, 3,
			{
				new AnimationFrame(16, 16, 2 * 16, 2 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 3 * 16, 2 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 4 * 16, 2 * 16, 0.5, 0.5)
			}),
			// facing up
			new Animation(0.1, 3,
			{
				new AnimationFrame(16, 16, 2 * 16, 3 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 3 * 16, 3 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 4 * 16, 3 * 16, 0.5, 0.5)
			})
		};
		attackAnim = new Animation * [4]
		{
			// facing left
			new Animation(0.15, 8,
			{
				new AnimationFrame(16, 16, 5 * 16, 0 * 16, 0.5, 0.5),
				new AnimationFrame(32, 16, 6 * 16, 0 * 16, 0.75, 0.5),
				new AnimationFrame(32, 16, 8 * 16, 0 * 16, 0.75, 0.5),
				new AnimationFrame(32, 16, 10 * 16, 0 * 16, 0.75, 0.5),
				new AnimationFrame(32, 16, 12 * 16, 0 * 16, 0.75, 0.5),
				new AnimationFrame(32, 16, 14 * 16, 0 * 16, 0.75, 0.5),
				new AnimationFrame(32, 16, 16 * 16, 0 * 16, 0.75, 0.5),
				new AnimationFrame(16, 16, 18 * 16, 0 * 16, 0.5, 0.5)
			}),
			// facing right
			new Animation(0.15, 8,
			{
				new AnimationFrame(16, 16, 5 * 16, 1 * 16, 0.5, 0.5),
				new AnimationFrame(32, 16, 6 * 16, 1 * 16, 0.25, 0.5),
				new AnimationFrame(32, 16, 8 * 16, 1 * 16, 0.25, 0.5),
				new AnimationFrame(32, 16, 10 * 16, 1 * 16, 0.25, 0.5),
				new AnimationFrame(32, 16, 12 * 16, 1 * 16, 0.25, 0.5),
				new AnimationFrame(32, 16, 14 * 16, 1 * 16, 0.25, 0.5),
				new AnimationFrame(32, 16, 16 * 16, 1 * 16, 0.25, 0.5),
				new AnimationFrame(16, 16, 18 * 16, 1 * 16, 0.5, 0.5)
			}),
			// facing down
			new Animation(0.15, 8,
			{
				new AnimationFrame(16, 16, 5 * 16, 2 * 16, 0.5, 0.5),
				new AnimationFrame(16, 32, 6 * 16, 2 * 16, 0.5, 0.25),
				new AnimationFrame(16, 32, 7 * 16, 2 * 16, 0.5, 0.25),
				new AnimationFrame(16, 32, 8 * 16, 2 * 16, 0.5, 0.25),
				new AnimationFrame(16, 32, 9 * 16, 2 * 16, 0.5, 0.25),
				new AnimationFrame(16, 32, 10 * 16, 2 * 16, 0.5, 0.25),
				new AnimationFrame(16, 32, 11 * 16, 2 * 16, 0.5, 0.25),
				new AnimationFrame(16, 16, 12 * 16, 2 * 16, 0.5, 0.5)
			}),
			// facing up
			new Animation(0.15, 8,
			{
				new AnimationFrame(16, 16, 5 * 16, 3 * 16, 0.5, 0.5),
				new AnimationFrame(16, 32, 13 * 16, 2 * 16, 0.5, 0.75),
				new AnimationFrame(16, 32, 14 * 16, 2 * 16, 0.5, 0.75),
				new AnimationFrame(16, 32, 15 * 16, 2 * 16, 0.5, 0.75),
				new AnimationFrame(16, 32, 16 * 16, 2 * 16, 0.5, 0.75),
				new AnimationFrame(16, 32, 17 * 16, 2 * 16, 0.5, 0.75),
				new AnimationFrame(16, 32, 18 * 16, 2 * 16, 0.5, 0.75),
				new AnimationFrame(16, 16, 12 * 16, 3 * 16, 0.5, 0.5)
			}),
		};
		deathAnim = new Animation * [4]
		{
			// facing left
			new Animation(0.2, 3,
			{
				new AnimationFrame(16, 16, 19 * 16, 0 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 20 * 16, 0 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 21 * 16, 0 * 16, 0.5, 0.5)
			}),
			// facing right
			new Animation(0.2, 3,
			{
				new AnimationFrame(16, 16, 19 * 16, 1 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 20 * 16, 1 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 21 * 16, 1 * 16, 0.5, 0.5)
			}),
			// facing down
			new Animation(0.2, 3,
			{
				new AnimationFrame(16, 16, 19 * 16, 2 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 20 * 16, 2 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 21 * 16, 2 * 16, 0.5, 0.5)
			}),
			// facing up
			new Animation(0.2, 3,
			{
				new AnimationFrame(16, 16, 19 * 16, 3 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 20 * 16, 3 * 16, 0.5, 0.5),
				new AnimationFrame(16, 16, 21 * 16, 3 * 16, 0.5, 0.5)
			}),
		};
		deadAnim = new Animation * [4]
		{
			// facing left
			new Animation(0.15, 1,
			{
				new AnimationFrame(16, 16, 21 * 16, 0 * 16, 0.5, 0.5),
			}),
			// facing right
			new Animation(0.15, 1,
			{
				new AnimationFrame(16, 16, 21 * 16, 1 * 16, 0.5, 0.5),
			}),
			// facing down
			new Animation(0.15, 1,
			{
				new AnimationFrame(16, 16, 21 * 16, 2 * 16, 0.5, 0.5),
			}),
			// facing up
			new Animation(0.15, 1,
			{
				new AnimationFrame(16, 16, 21 * 16, 3 * 16, 0.5, 0.5),
			}),
		};
	}
	virtual void PlayAnimations(void) override
	{
		if (isAlive)
		{
			if (isAttacking)
				isAttacking = !PlayAnimationOnce(8, attackAnim[(int)facingDirection], &_lastPlayedAnimation, true);
			else if (GetMovementState())
				PlayAnimation(2, idleAnim[(int)facingDirection], &_lastPlayedAnimation, false);
			else PlayAnimation(3, movementAnim[(int)facingDirection], &_lastPlayedAnimation, true);
		}
		else
		{
			if (!playedDeathAnimation)
				playedDeathAnimation = PlayAnimationOnce(3, deathAnim[(int)facingDirection], &_lastPlayedAnimation, true);
			else PlayAnimation(1, deadAnim[(int)facingDirection], &_lastPlayedAnimation, false);
		}
	}

	virtual bool UpdateHealthbar(double* healthToShow, double updateSpeed = 2) override
	{
		double speedDelta = updateSpeed * Time::GetDeltaTime();
		healthbarPosition->w = (int)(*healthToShow / (double)maxHealth * (Initializer.Initializer.TEXTURE_RES * 3 / 4));
		*healthToShow = MoveTowards(*healthToShow, (double)curHealth, updateSpeed);
		if (fabs(*healthToShow - curHealth) <= speedDelta)
			return true;
		return false;
	}
public:
	virtual ~CreatureSpiderling() override
	{
		/*if (curPath != nullptr)
			delete curPath;*/
		delete idleAnim;
		delete movementAnim;
		delete attackAnim;
		delete deathAnim;
		delete deadAnim;
	}

	virtual void OnTurnStart(void) override
	{
		if (Vector::GetDistance(Controller.mainHero->ToVector(), this->ToVector()) < viewDistance)
		{
			SetTarget(Controller.mainHero);
			curPath = map->tilemap->FindPath(map->tilemap->GetTile(x, y), map->tilemap->GetTile(target->x, target->y), this);
			curPathIndex = 0;
			if (curPath != nullptr && curPath->Count() <= viewDistance)
				map->camera.target = this;
			else
			{
				SetTarget(nullptr);
				delete curPath;
				curPath = nullptr;
			}
		}

		if (map->tilemap->GetTile(x, y)->isExplored &&
			map->tilemap->GetTile(x, y)->isVisible)
			Controller.SetPause(0.15);
		actionsLeft = maxActionsCount;
	}
	virtual void OnTurnEnd(void) override
	{

	}
	// return true if killed by damage
	virtual bool OnDamage(TileObject* sender = nullptr) override
	{
		if (curHealth <= 0)
		{
			isAlive = false;
			Controller.shardsCount = MoveTowards(Controller.shardsCount, Controller.shardsRequired, dropShardsCount);
			map->tilemap->GetTile(x, y)->SetCorpse(this);
			map->tilemap->GetTile(x, y)->SetTileObject(nullptr);
			Controller.RecalculateOverlay();
		}
		return !isAlive;
		//cout << "taken damage\n";
	}
	virtual void OnHealing(TileObject* sender = nullptr) override
	{
	}
	virtual void OnFullHealing(TileObject* sender = nullptr) override
	{
	}
	virtual void OnTargetChange() override
	{
		//cout << "target changed\n";
	}
	virtual void OnAttack(TileObject* target = nullptr) override
	{
		Creature::OnAttack();
	}
	virtual void OnInteractWith(TileObject* sender = nullptr) override {}
	virtual void Initialize(int withID) override
	{
		auto surf = IMG_Load("textures/creature_ui.png");
		visualMovementSpeed = 50;
		curAnimationTime = static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (0.5)));
		Creature::Initialize(withID);

		healthbarSrc->x = (factionID == 0) ? Initializer.TEXTURE_RES : 0;
		healthbarSrc->y = 0;
		healthbarSrc->w = healthbarSrc->h = Initializer.TEXTURE_RES;

		healthbarEmptySrc->x = 0;
		healthbarEmptySrc->y = Initializer.TEXTURE_RES;
		healthbarEmptySrc->w = healthbarEmptySrc->h = Initializer.TEXTURE_RES;

		healthbarPosition->w = (int)(curHealth / (double)maxHealth * (Initializer.TEXTURE_RES * 3 / 4));
		healthbarEmptyPosition->w = Initializer.TEXTURE_RES * 3 / 4;
		healthbarPosition->h = healthbarEmptyPosition->h = 1;
		healthbarTexture = SDL_CreateTextureFromSurface(Initializer.renderer, surf);
		SDL_FreeSurface(surf);

		maxActionsCount = 1;
		maxHealth = curHealth = 10 + Controller.sessionDay * 8 + (Controller.sessionDay / 2) * 8;
		dropShardsCount = 1 + Controller.sessionDay * 1;
		viewDistance = 6;
		curHealthInHealthbar = curHealth;
		damage = 3 + Controller.sessionDay * 3 + (Controller.sessionDay / 2) * 4;
		canInteractWith = false;

		target = nullptr;
		texturePosition->w = curAnimationFrame == nullptr ? Initializer.TEXTURE_RES : curAnimationFrame->resolutionX;
		texturePosition->h = curAnimationFrame == nullptr ? Initializer.TEXTURE_RES : curAnimationFrame->resolutionY;
		objectTexture = SDL_CreateTextureFromSurface(Initializer.renderer, surf = IMG_Load("textures/spiderling.png"));
		SDL_FreeSurface(surf);
	}
	virtual void Update(void) override
	{
		if (map->tilemap->GetTile(x, y)->isExplored &&
			map->tilemap->GetTile(x, y)->isVisible)
			PlayAnimations();
		Creature::Update();
		if (UpdateHealthbar(&curHealthInHealthbar, maxHealth * 0.025) && isAlive && healthbarPosition->w < 1)
		{
			actionsLeft = 0;
			curHealth = 0;
			isAlive = false;
			Controller.shardsCount = MoveTowards(Controller.shardsCount, Controller.shardsRequired, dropShardsCount);
			map->tilemap->GetTile(x, y)->SetTileObject(nullptr);
			map->tilemap->GetTile(x, y)->SetCorpse(this);
			Controller.RecalculateOverlay();
		}
		if (!endedTurn && !Controller.inSplashScreen && !isUnderPlayerControl && Controller.GetPause() <= 0)
		{
			if (GetMovementState() && map->camera.GetCameraMovementState(map))
			{
				if (actionsLeft > 0)
				{
					if (curPath != nullptr && curPathIndex < curPath->Count())
					{
						tileToMove = map->tilemap->GetTile((*curPath)[curPathIndex]->x, (*curPath)[curPathIndex]->y);
						if (MoveTo(tileToMove))
						{
							/*for (int i = 0; i < Controller.creatures[previousLayer].size(); i++)
								if (Controller.creatures[previousLayer][i]->GetID() == UUID)
								{
									Controller.creatures[previousLayer].erase(Controller.creatures[previousLayer].begin() + i);
									break;
								}
							Controller.creatures[y].push_back(this);*/
							MapHandler::Instance().RecalculateMinimap(map);
							curPathIndex++;
							actionsLeft--;
							if (map->tilemap->GetTile(x, y)->isExplored &&
								map->tilemap->GetTile(x, y)->isVisible)
								Controller.SetPause(0.25);
							Controller.RecalculateRenderLayers();
						}
						else
						{
							if (tileToMove->GetTileObject() == target)
							{
								Creature::DealDamage(damage, target, this);
								SetTarget(nullptr);
								delete curPath;
								curPath = nullptr;
								actionsLeft = 0;
								if (map->tilemap->GetTile(x, y)->isExplored &&
									map->tilemap->GetTile(x, y)->isVisible)
									Controller.SetPause(0.15);
							}
							else
							{
								SetTarget(nullptr);
								delete curPath;
								curPath = nullptr;
								actionsLeft = 0;
								if (map->tilemap->GetTile(x, y)->isExplored &&
									map->tilemap->GetTile(x, y)->isVisible)
									Controller.SetPause(0.15);
							}
						}
					}
					else
					{
						SetTarget(nullptr);
						delete curPath;
						curPath = nullptr;
						actionsLeft = 0;
						if (map->tilemap->GetTile(x, y)->isExplored &&
							map->tilemap->GetTile(x, y)->isVisible)
							Controller.SetPause(0.25);
					}
				}
				else
				{
					if (map->tilemap->GetTile(x, y)->isExplored &&
						map->tilemap->GetTile(x, y)->isVisible)
						Controller.SetPause(0.25);
					Controller.NextTurn();
				}
			}
		}
	}
};