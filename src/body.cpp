#include <lspe/body.h>

namespace lspe
{

RigidBodyProperty::RigidBodyProperty()
{
	type            = BodyType::eStatic;
	shape           = { nullptr, ShapeType::eNull };

	linearVelocity  = { 0.0f, 0.0f };
	angularVelocity = 0.0f;

	linearDamping   = 0.0f;
	angularDamping  = 0.0f;

	gravityScale    = 1.0f;

	world.location = { 0.0f, 0.0f };
	world.angle    = 0.0f;

	enabled        = true;
	awake          = true;
	allowSleep     = true;
	fixedRotation  = false;
	enableCCD      = false;

	userdata       = nullptr;
}

RigidBody::RigidBody()
	: property()
{

}

RigidBody::~RigidBody()
{

}

BodyType RigidBody::getBodyType() const
{
	return property.type;
}

Shape RigidBody::getShape() const
{
	return property.shape;
}

void RigidBody::setBodyType(BodyType type)
{
	if (property.type == type) return;

	property.type = type;

	if (type == BodyType::eStatic)
	{
		property.linearVelocity  = { 0.0f, 0.0f };
		property.angularVelocity = 0.0f;
		flags &= ~RigidBodyFlag::eAwake;
	}

	force  = { 0.0f, 0.0f };
	torque = 0.0f;
}

const RigidBodyProperty& RigidBody::getProperty() const
{
	return property;
}

bool RigidBody::setShape(Shape shape)
{
	if (shape.type != ShapeType::eNull
		&& shape.data != nullptr)
	{
		property.shape = shape;
		return true;
	}
	return false;
}

void RigidBody::applyForce2Center(vec2 force, bool wake)
{
	if (property.type != BodyType::eDynamic) return;

	if (wake && (flags & RigidBodyFlag::eAwake))
	{
		setAwake(true);
	}

	if (flags & RigidBodyFlag::eAwake)
	{
		this->force += force;
	}
}

void RigidBody::applyForce(vec2 force, vec2 point, bool wake)
{
	if (property.type != BodyType::eDynamic) return;

	if (wake && (flags & RigidBodyFlag::eAwake))
	{
		setAwake(true);
	}

	if (flags & RigidBodyFlag::eAwake)
	{
		this->force += force;
		this->torque += cross(point, force);
	}
}

void RigidBody::applyTorque(float torque, bool wake)
{
	if (property.type != BodyType::eDynamic) return;

	if (wake && (flags & RigidBodyFlag::eAwake))
	{
		setAwake(true);
	}

	if (flags & RigidBodyFlag::eAwake)
	{
		this->torque += torque;
	}
}

void RigidBody::applyLinearImpulse2Center(vec2 linearImpulse, bool wake)
{
	if (property.type != BodyType::eDynamic) return;

	if (wake && (flags & RigidBodyFlag::eAwake))
	{
		setAwake(true);
	}

	if (flags & RigidBodyFlag::eAwake)
	{
		property.linearVelocity += linearImpulse * inv_mass;
	}
}

void RigidBody::applyLinearImpulse(vec2 linearImpulse, vec2 point, bool wake)
{
	if (property.type != BodyType::eDynamic) return;

	if (wake && (flags & RigidBodyFlag::eAwake))
	{
		setAwake(true);
	}

	if (flags & RigidBodyFlag::eAwake)
	{
		property.linearVelocity += linearImpulse * inv_mass;
		property.angularVelocity += cross(point, linearImpulse) * inv_inertia;
	}
}

void RigidBody::applyAngularImpulse(float angularImpulse, bool wake)
{
	if (property.type != BodyType::eDynamic) return;

	if (wake && (flags & RigidBodyFlag::eAwake))
	{
		setAwake(true);
	}

	if (flags & RigidBodyFlag::eAwake)
	{
		property.angularVelocity += angularImpulse * inv_inertia;
	}
}

float RigidBody::getMass() const
{
	return mass;
}

float RigidBody::getInertia() const
{
	return inertia;
}

void RigidBody::setMass(float mass)
{
	if (property.type != BodyType::eDynamic) return;

	this->mass = mass <= FLT_EPSILON ? 1.0f : mass;
	this->inv_mass = 1.0f / this->mass;
}

void RigidBody::setInertia(float inertia)
{
	if (property.type != BodyType::eDynamic) return;

	this->inertia = inertia;
	this->inv_inertia = 1.0f / this->inertia;
}

vec2 RigidBody::getLinearVelocity() const
{
	return property.linearVelocity;
}

float RigidBody::getAngularVelocity() const
{
	return property.angularVelocity;
}

float RigidBody::getLinearDamping() const
{
	return property.linearDamping;
}

float RigidBody::getAngularDamping() const
{
	return property.angularDamping;
}

float RigidBody::getGravityScale() const
{
	return property.gravityScale;
}

void RigidBody::setLinearVelocity(const vec2 &linearVelocity)
{
	property.linearVelocity = linearVelocity;
}

void RigidBody::setAngularVelocity(float angularVelocity)
{
	property.angularVelocity = angularVelocity;
}

void RigidBody::setLinearDamping(float linearDamping)
{
	property.linearDamping = linearDamping;
}

void RigidBody::setAngularDamping(float angularDamping)
{
	property.angularDamping = angularDamping;
}

void RigidBody::setGravityScale(float gravityScale)
{
	property.gravityScale = gravityScale;
}

bool RigidBody::isEnabled() const
{
	return property.enabled;
}

bool RigidBody::isAwake() const
{
	return property.awake;
}

bool RigidBody::isAllowSleep() const
{
	return property.allowSleep;
}

bool RigidBody::isEnableCCD() const
{
	return property.enableCCD;
}

bool RigidBody::isFixedRotation() const
{
	return property.fixedRotation;
}

void RigidBody::setEnabled(bool flag)
{
	if (flag)
	{
		flags |= RigidBodyFlag::eEnabled;
	} else
	{
		flags &= ~RigidBodyFlag::eEnabled;
	}
}

void RigidBody::setAwake(bool flag)
{
	if (property.type == BodyType::eStatic) return;

	if (flag)
	{
		flags |= RigidBodyFlag::eAwake;
	} else
	{
		flags &= ~RigidBodyFlag::eAwake;

		property.linearVelocity = { 0.0f, 0.0f };
		property.angularVelocity = 0.0f;
		force = { 0.0f, 0.0f };
		torque = 0.0f;
	}

	sleepTime = 0;
}

void RigidBody::setAllowSleep(bool flag)
{
	if (flag)
	{
		flags |= RigidBodyFlag::eAutoSleep;
	} else
	{
		flags &= ~RigidBodyFlag::eAutoSleep;

		setAwake(true);
	}
}

void RigidBody::setEnableCCD(bool flag)
{
	if (flag)
	{
		flags |= RigidBodyFlag::eEnableCCD;
	} else
	{
		flags &= ~RigidBodyFlag::eEnableCCD;
	}
}

void RigidBody::setFixedRotation(bool flag)
{
	if (flag)
	{
		flags |= RigidBodyFlag::eFixedRotation;
	} else
	{
		flags &= ~RigidBodyFlag::eFixedRotation;
	}

	property.angularVelocity = 0.0f;
}

};
