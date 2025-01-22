#pragma once
#include "engine.h"

static class ParticleMan {
public:
	static std::vector< std::vector<bool>> particleGrid;

	struct particleObject : public Engine::engineObject
	{
		SDL_FPoint vel;

		bool unoccupiedSpace(SDL_FPoint changePos) {
			//Apply changePos to hull
			SDL_FPoint checkPos = { hull.x + changePos.x, hull.y + changePos.y };

			if (checkPos.x < 0 ||
				checkPos.x >= Engine::baseRes.x ||
				checkPos.y < 0 ||
				checkPos.y >= Engine::baseRes.y) {
				return false;
			}
				return !particleGrid[checkPos.x][checkPos.y];
		}

		bool tryMove(SDL_FPoint changePos)
		{
			if (unoccupiedSpace(changePos))
			{
				SDL_FPoint newPos = { hull.x + changePos.x, hull.y + changePos.y };

				//Adjust Particle Position
				ParticleMan::particleGrid[hull.x][hull.y] = false; //Particle Free
				hull.x = newPos.x;
				hull.y = newPos.y;
				ParticleMan::particleGrid[hull.x][hull.y] = true; //Particle Bind

				return true;
			}
			return false;
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