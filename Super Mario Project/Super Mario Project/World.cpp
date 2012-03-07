////////////////////////////////////////////////////////////////////////
// World.cpp
// Super Mario Project
// Copyright (C) 2011  
// Lionel Joseph lionel.r.joseph@gmail.com
// Olivier Guittonneau openmengine@gmail.com
////////////////////////////////////////////////////////////////////////

#include "World.hpp"
#include "XMLParser.hpp"
#include "CollisionManager.hpp"
#include <fstream>
#include <iostream>

namespace SuperMarioProject
{
	World::World(const Window* window) : _inputState(InputState(window)),
		_level(new Level()),
		_actualTime(0),
		_previousTime(0),
		_elapsedTime(0)
		{
			loadWorld();

			if(_levelNames.size() > 0)
			{
				_level->loadLevel(_levelNames[0]);
				_levelTree = new QuadTree(Vector2f(_level->getSize().x / 2,
					_level->getSize().y / 2), _level->getSize(), 0);
				_levelTree->buildTree(_level->getBlocksOccurrences());
			}

			/* Add default perso */
			_persos.push_back(new Perso("small_mario", Vector2f(0,0)));
		}

	Level* World::getLevel()
	{
		return _level;
	}

	float World::getElapsedTime()
	{
		return _clock.GetElapsedTime();
	}

	void World::addLevelName(string levelName)
	{
		_levelNames.push_back("levels/" + levelName);
	}

	void World::update(RenderWindow& app)
	{
		//_level->updatePhysicData(app);

		_inputState.update();
		
		for (vector<Perso*>::iterator it = this->_persos.begin(); it != this->_persos.end(); ++it)
		{
			(*it)->updatePerso(_elapsedTime, _inputState);

			vector<BlockOccurrence*> blocks = _levelTree->getBlocks((*it)->getHitboxPosition(), (*it)->getHitboxSize());
			for (vector<BlockOccurrence*>::iterator itBlocks = blocks.begin(); itBlocks != blocks.end(); ++itBlocks)
			{
				Collisions::CollisionManager::solveCollisions((*it), (*itBlocks), _level, app);
			}

			(*it)->updateGraphicData(app);
		}

		_level->updateGraphicData(app);

		updateTime();
	}

	void World::updateTime()
	{
		_previousTime = _actualTime;
		_actualTime = _clock.GetElapsedTime();
		_previousElapsedTime = _elapsedTime;
		_elapsedTime = (_actualTime - _previousTime);

		/* FPS */
		if(_actualTime - _fpsTime >= 250)
		{
			_fpsTime = _actualTime;
			_fps = _nbFramesCalculated * 4;
			_nbFramesCalculated = 0;
		}

		/* If difference beetween actual and previous frame is greater than 0.5s, 
			then we assign previousTime to actual time*/
        if(_elapsedTime > 500)
        {
			_previousTime = _clock.GetElapsedTime();
            _actualTime = _previousTime;
           _elapsedTime = 0;
        }

		vector<Perso*>::iterator it;
		
		for (it = this->_persos.begin(); it < this->_persos.end(); ++it)
		{
			if((*it)->getState() != Perso::FINISH && (*it)->getState() != Perso::FINISH_CASTLE && (*it)->getState() != Perso::DEAD
				&& (*it)->getHUD()->getTime() > 0)
			{
				(*it)->getHUD()->setTime((*it)->getHUD()->getTime() - _elapsedTime / 10);
				if((*it)->getHUD()->getTime() < 0)
					(*it)->getHUD()->setTime(0);
			}
		}

		_nbFramesCalculated++;
	}

	void World::render(RenderWindow& app)
	{
		this->_level->render(app);
		//_levelTree->render();

		for (vector<Perso*>::iterator it = this->_persos.begin(); it < this->_persos.end(); ++it)
		{
			(*it)->render(app);
		}
	}

	void World::loadWorld()
	{
		XMLParser::loadWorld("worlds/world1.xml", this);
	}

    World::~World()
    {
		if(_level != nullptr)
			delete _level;

		/* Persos */
		for(vector<Perso*>::iterator itPersos = this->_persos.begin(); itPersos < this->_persos.end(); ++itPersos)
		{
			delete (*itPersos);
		}

    }
} // namespace
