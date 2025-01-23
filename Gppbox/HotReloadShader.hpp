#pragma once

#include "SFML/Graphics.hpp"
#include <string>
#include <functional>



class HotReloadShader {
public:

	bool	inError = false;
	bool	enableHotReloading = true;
	std::string vertPath;
	std::string fragPath;

	std::string vertSrc;
	std::string fragSrc;

	time_t mtimeVert = 0;
	time_t mtimeFrag = 0;
	
	std::function<void(void)> onUpdate;

	HotReloadShader( std::string vertPath, std::string fragPath) {
		this->vertPath = vertPath;
		this->fragPath = fragPath;
		tick();
	}

	float	time = 0.0;
	std::string	getFileContent(const std::string & path);
	void	update(double dt);
	void	tick();

	sf::Shader sh;
};