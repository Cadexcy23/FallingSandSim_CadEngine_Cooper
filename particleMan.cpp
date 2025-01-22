#include "particleMan.h"

std::vector<std::shared_ptr <Engine::engineObject>> ParticleMan::particleTypes;
std::vector<std::vector<bool>> ParticleMan::particleGrid;
std::shared_ptr <Engine::engineObject> controller;

void gravity(std::shared_ptr<Engine::engineObject> obj) {
	auto particleObj = std::dynamic_pointer_cast<ParticleMan::particleObject>(obj);

	std::vector<int> checkDirections; //Create Empty List of Directions
	checkDirections.push_back(4); //Add Down to Directions List
	checkDirections.push_back(3); //Add Down Left
	checkDirections.push_back(5); //Add Down Right
	for (int i = 0; i < checkDirections.size(); i++) { //For Each Item in Direction List
		if(particleObj->unoccupiedSpace(checkDirections[i])) {

			SDL_FPoint occupancyCheck = { particleObj->hull.x, particleObj->hull.y };
			//Convert from Direction to Change in [x,y]
			switch (checkDirections[i]) {
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
			//Adjust Particle Position
			ParticleMan::particleGrid[obj->hull.x][obj->hull.y] = false; //Particle Free
			obj->hull.x = occupancyCheck.x;
			obj->hull.y = occupancyCheck.y;
			ParticleMan::particleGrid[obj->hull.x][obj->hull.y] = true; //Particle Bind
		}
	}
}

void keepInBounds(std::shared_ptr<Engine::engineObject> obj) {
	if (obj->hull.x < 0)
		obj->hull.x = 0;
	if (obj->hull.y < 0)
		obj->hull.y = 0;
	if (obj->hull.x > Engine::baseRes.x-1)
		obj->hull.x = Engine::baseRes.x-1;
	if (obj->hull.y > Engine::baseRes.y-1)
		obj->hull.y = Engine::baseRes.y-1;
}

void addParticle(std::shared_ptr<Engine::engineObject> obj) {
	if (Engine::mouseStates[0] == 2) {
		if (!ParticleMan::particleGrid[Engine::mousePos.x][Engine::mousePos.y]) { //Add Particle if Space Available
			std::shared_ptr<ParticleMan::particleObject> velObj = std::make_shared<ParticleMan::particleObject>();
			ParticleMan::particleTypes.push_back(Engine::addObject(velObj));
			ParticleMan::particleTypes.back()->hull.x = int(Engine::mousePos.x);
			ParticleMan::particleTypes.back()->hull.y = int(Engine::mousePos.y);
			ParticleMan::particleTypes.back()->updateFuncs.push_back(gravity);
			ParticleMan::particleTypes.back()->updateFuncs.push_back(keepInBounds);
			ParticleMan::particleGrid[Engine::mousePos.x][Engine::mousePos.y] = true;
			printf("Particle Total Count: %i\n", ParticleMan::particleTypes.size());
		}
	}
}

void ParticleMan::initParticle() {
	/*std::shared_ptr<ParticleMan::particleObject> velObj1 = std::make_shared<ParticleMan::particleObject>();
	particleTypes.push_back(Engine::addObject(velObj));
	particleTypes[1]->hull.x++;*/
	ParticleMan::particleGrid.resize(Engine::baseRes.x);
	for (int i = 0; i < ParticleMan::particleGrid.size(); i++) {
		ParticleMan::particleGrid[i].resize(Engine::baseRes.y);
		std::fill(ParticleMan::particleGrid[i].begin(), ParticleMan::particleGrid[i].end(), false);
	}

	std::shared_ptr<Engine::engineObject> Obj = std::make_shared<Engine::engineObject>();
	controller = Engine::addObject(Obj);
	controller->updateFuncs.push_back(addParticle);
}

/*
bool ParticleMan::particles[192][108];
std::shared_ptr<Engine::engineObject> grid;


void drawGrid(std::shared_ptr<Engine::engineObject> obj) {
	Engine::setRenderTarget(grid->tex);
	Engine::drawRect({ 0, 0, float(Engine::baseRes.x), float(Engine::baseRes.y) }, { 5, 0, 10, 255 });
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			if (ParticleMan::particles[x][y]) {
				Engine::drawRect({ float(x), float(y), 1, 1 }, { 235, 235, 180, 255 });

			}
		}
	}
	Engine::setRenderTarget(NULL);
}

void addSandParticle(std::shared_ptr<Engine::engineObject> obj) {
	bool particlesCopy[192][108];
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			particlesCopy[x][y] = ParticleMan::particles[x][y];

		}
	}
	if (Engine::mouseStates[0] == 2) {
		ParticleMan::particles[int(Engine::mousePos.x)][int(Engine::mousePos.y)] = true;
	}
		for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
			for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
				if (particlesCopy[x][y] && (y + 1 < 108)) {
					if (!particlesCopy[x][y + 1]) {
						ParticleMan::particles[x][y] = false;
						ParticleMan::particles[x][y + 1] = true;
					}
					else if (!particlesCopy[x - 1][y + 1] && (x - 1 > 0)) {
						ParticleMan::particles[x][y] = false;
						ParticleMan::particles[x - 1][y + 1] = true;
					}
					else if (!particlesCopy[x + 1][y + 1] && (x + 1 < 192)) {
						ParticleMan::particles[x][y] = false;
						ParticleMan::particles[x + 1][y + 1] = true;
					}
				}
			}
		}
	
}

void addWaterParticle(std::shared_ptr<Engine::engineObject> obj) {
	bool particlesCopy[192][108];
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			particlesCopy[x][y] = ParticleMan::particles[x][y];

		}
	}
	if (Engine::mouseStates[2] == 2) {
		ParticleMan::particles[int(Engine::mousePos.x)][int(Engine::mousePos.y)] = true;
	}
	for (int x = 0; x < (sizeof(ParticleMan::particles) / sizeof(*ParticleMan::particles)); x++) {
		for (int y = 0; y < (sizeof(ParticleMan::particles[0]) / sizeof(*ParticleMan::particles[0])); y++) {
			if (particlesCopy[x][y] && (y + 1 < 108)) {
				//If nothing below, move down
				if (!particlesCopy[x][y + 1]) { 
					ParticleMan::particles[x][y] = false;
					ParticleMan::particles[x][y + 1] = true;
				}
				else if (!particlesCopy[x - 1][y + 1] && (x - 1 > 0)) {
					ParticleMan::particles[x][y] = false;
					ParticleMan::particles[x - 1][y + 1] = true;
				}
				else if (!particlesCopy[x + 1][y + 1] && (x + 1 < 192)) {
					ParticleMan::particles[x][y] = false;
					ParticleMan::particles[x + 1][y + 1] = true;
				}
			}
		}
	}

}

void ParticleMan::initParticle() {
	for (int x = 0; x < (sizeof(particles) / sizeof(*particles)); x++) {
		for (int y = 0; y < (sizeof(particles[0]) / sizeof(*particles[0])); y++) {
			particles[x][y] = false;
		}
	}
	for (int i = 0; i < rand() % 192; i++)
		particles[rand()%192][rand()%108] = true;


	grid = std::make_shared<Engine::engineObject>(Engine::engineObject({ 0, 0, float(Engine::baseRes.x), float(Engine::baseRes.y) }, NULL, 0, false));
	grid->tex = Engine::loadTargetTex({ int(grid->hull.w), int(grid->hull.h) });
	Engine::addObject(grid);
	grid->updateFuncs.push_back(addWaterParticle);
	grid->updateFuncs.push_back(addSandParticle);
	grid->drawFuncs.push_back(drawGrid);
}
*/