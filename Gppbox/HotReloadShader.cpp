#include "HotReloadShader.hpp"

#include  <istream>
#include  <iostream>


#include "Lib.hpp"

std::string HotReloadShader::getFileContent(const std::string & path) {
	std::string res;

	FILE * f = fopen(path.c_str(), "r");
	if (!f) return "";
	fseek(f, 0, SEEK_END);
	int totalLen = ftell(f);
	fseek(f, 0, SEEK_SET);

	res.resize(totalLen);

	size_t totalSize = 0;
	while (totalLen > 0) {
		size_t sizeRead = fread(&res[totalSize], 1, totalLen, f);
		if (feof(f)) {
			break;
		}
		totalSize += sizeRead;
	}

	fclose(f);
	return res;
}
void HotReloadShader::tick() {
	struct stat mt;
	bool hasOneChanged = false;

	int res = stat(vertPath.c_str(), &mt);
	if (res == -1) {
		if (!inError)
			std::cout << "no such vert shader" << std::endl;
		inError = true;
		return;
	}
	inError = false;

	if (mt.st_mtime > mtimeVert) {
		vertSrc = getFileContent(vertPath.c_str());
		mtimeVert = mt.st_mtime;
		hasOneChanged = true;
		if (vertSrc.size())
			std::cout << "vert read successfully" << std::endl;
	}

	res = stat(fragPath.c_str(), &mt);
	if (res == -1) {
		if (!inError)
			std::cout << "no such frag shader" << std::endl;
		inError = true;
		return;
	}
	inError = false;

	if (mt.st_mtime > mtimeFrag) {
		fragSrc = getFileContent(fragPath.c_str());
		mtimeFrag = mt.st_mtime;
		hasOneChanged = true;
		if (fragSrc.size())
			std::cout << "frag read successfully" << std::endl;
	}

	if (hasOneChanged) {
		bool ok = sh.loadFromMemory(vertSrc.c_str(), fragSrc.c_str());
		if (ok) {
			std::cout << "shader updated" << std::endl;
			if (onUpdate)
				onUpdate();
		}
		else
			std::cout << "unable to parse shader" << std::endl;
	}
}

void HotReloadShader::update(double dt) {
	if (!enableHotReloading)
		return;
	time += dt;
	if (time > 0.2) {
		tick();
	}
}
