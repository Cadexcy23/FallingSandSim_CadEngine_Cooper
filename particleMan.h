#pragma once
#include "engine.h"

static class ParticleMan {
public:
	static std::vector< std::vector<bool>> particleGrid;

	struct particleObject : public Engine::engineObject
	{
		SDL_FPoint vel;

		bool unoccupiedSpace(int direction) {
			//Occupancy Check Origin
			SDL_FPoint occupancyCheck = { hull.x, hull.y };

			//Adjust Direction Checked
			switch (direction) {
			case 0: //Up
				occupancyCheck.y--;
				break;
			case 1: //Up Right
				occupancyCheck.x++;
				occupancyCheck.y--;
				break;
			case 2: //Right
				occupancyCheck.x++;
				break;
			case 3: //Down Right
				occupancyCheck.x++;
				occupancyCheck.y++;
				break;
			case 4: //Down
				occupancyCheck.y++;
				break;
			case 5: //Down Left
				occupancyCheck.x--;
				occupancyCheck.y++;
				break;
			case 6: //Left
				occupancyCheck.x--;
				break;
			case 7: //Up Left
				occupancyCheck.x--;
				occupancyCheck.y--;
				break;
			}
			if (occupancyCheck.x < 0 ||
				occupancyCheck.x >= Engine::baseRes.x ||
				occupancyCheck.y < 0 ||
				occupancyCheck.y >= Engine::baseRes.y) {
				return false;
			}
				return !particleGrid[occupancyCheck.x][occupancyCheck.y];
		}

		particleObject(const SDL_FRect& hull = { 0, 0, 1, 1 }, double rot = 0,
			bool centered = false, SDL_FlipMode flip = SDL_FLIP_NONE, float scale = 1.0,
			SDL_FPoint vel = { 0, 0 }, double spin = 0, int depth = 0)
			: engineObject(hull, NULL, rot,
				centered, flip, scale,
				depth),
			vel(vel) {
			tex = Engine::loadTargetTex({ 1,1 });
			SDL_Texture* texture = Engine::setRenderTarget(tex);
			SDL_Color sand = { 235, 235, 180, 255 };
			Engine::drawRect(hull, sand);
			Engine::setRenderTarget(texture);
		}
	};
	static void initParticle();

	static std::vector<std::shared_ptr <Engine::engineObject>> particleTypes;
};