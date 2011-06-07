////////////////////////////////////////////////////////////////////////
// EntityMovable.hpp
// Super Mario Project
// Copyright (C) 2011  
// Lionel Joseph lionel.r.joseph@gmail.com
// Olivier Guittonneau openmengine@gmail.com
////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef HPP_ENTITYMOVABLE
#define HPP_ENTITYMOVABLE

/* Physics constants */
#define SPEED_Y_MAX						0.008F
#define EJECTION_SPEED_Y				0.2F
#define GRAVITY							0.002F

/* Frictions constants */
#define SLIDE_COEFF_FRICTION			0.3F
#define CLASSIC_COEFF_FRICTION			0.005F
#define ICE_COEFF_FRICTION				0.004F
#define AIR_COEFF_FRICTION				0.0005F

/* Submissions */
#define GROUND_SUBMISSION			    0x0000001
#define GRAVITY_SUBMISSION              0x0000002
#define RETURN_SENDER_SUBMISSION        0x0000004


#include "Collisionable.hpp"

namespace Collisions
{
    class EntityMovable : public Collisionable
    {
	
	public:
		enum Side {
			LEFT_SIDE,
			RIGHT_SIDE
		};

    
		/* Constructors */
		EntityMovable(const string& textureName) : Collisionable(textureName), _previousPosition(Coord<float>()), _speed(Vector2f()), _side(RIGHT_SIDE) { }
		EntityMovable(const string& textureName, Coord<float>& position) : Collisionable(textureName, position), _previousPosition(position), _speed(Vector2f()), _side(RIGHT_SIDE) { }

		/* getters and setters */
		Coord<float> getPreviousPosition();
		Vector2f getSpeed();
		Side getSide();
		void setPreviousPosition(Coord<float>& previousPosition);
		void setSpeed(Vector2f& speed);
		void setSpeedX(float x);
		void setSpeedY(float y);
		void setSide(Side side);

		void gravity(Vector2f* speed, float time);

		/* Destructors */
        virtual ~EntityMovable();
		
    protected:
		Coord<float> _previousPosition;
		Vector2f _speed;
		Side _side;
    };
} // namespace

#endif // HPP_ENTITYMOVABLE