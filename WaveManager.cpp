#include "WaveManager.h"
#include <fstream>
#include <sstream>
WaveManager* WaveManager::GetInstance() {
	static WaveManager instance;
	return &instance;
}

void WaveManager::LoadFile() {
	/*std::string line;
	std::ifstream file("Resource/Wave/waveData.wave");
	assert(file.is_open());
	EnemyData* newEnemyData;
	Wave* newWave;
	waves_.clear();
	while (std::getline(file, line)) {
		std::string identifilter;
		std::istringstream s(line);
		s >> identifilter;
		if (identifilter == "wave") {
			newWave=&waves_.emplace_back();
		}
		else if (identifilter == "enemy") {
			 newEnemyData = &newWave->enemyDatas.emplace_back();
		}
		else if (identifilter == "position") {
			Vector4 position;
			s >> position.x >> position.y >> position.z;
			//マップからトランスフォーム値に変換
		}
		else if (identifilter == "velocity") {
			s >> newEnemyData->velocity.x >> newEnemyData->velocity.y >> newEnemyData->velocity.z;
		}
		else if (identifilter == "speed") {
			s >> newEnemyData->speed;
		}
		else if (identifilter == "type") {
			int32_t type;
			s >> type;
			newEnemyData->type = EnemyType(type);
		}
		else if (identifilter == "frame") {
			s >> newEnemyData->frame;
		}
		else if (identifilter == "length") {
			s >> newWave->length;
		}
	}*/

}

