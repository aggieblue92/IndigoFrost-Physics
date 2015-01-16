#include <irrlicht.h>
#include <driverChoice.h>
#include <IndigoFrost.h>
#include "DemoTimer.h"
#include "CollisionTestSceneNode.h"

class StuffBrokeException { public: std::string errMessage; StuffBrokeException(std::string m) : errMessage(m) {} };

void SetupDevice(irr::IrrlichtDevice*& o_device)
{
	// Ask user for driver type, create device...
	auto driverType = irr::driverChoiceConsole();
	if (irr::video::EDT_COUNT == driverType)
	{
		throw StuffBrokeException("Could not initialize driver of specified type!");
	}
	o_device = irr::createDevice(driverType, irr::core::dimension2d<irr::u32>(800, 600), 32, false);

	if (!o_device)
	{
		throw StuffBrokeException("Could not create device!");
	}

	// Create engine and camera...
	o_device->setWindowCaption(L"Basic Vanilla Test - Indigo Frost demo (powered by Irrlicht)");
	auto driver = o_device->getVideoDriver();
	auto smgr = o_device->getSceneManager();
	smgr->addCameraSceneNodeFPS();
}

void SetupNodes(irr::IrrlichtDevice* device, Frost::WorldManager& root, const std::vector<std::string>& namesList)
{
	auto smgr = device->getSceneManager();
	for (unsigned int i = 0u; i < namesList.size(); ++i)
	{
		FrostTest::CollisionSceneNode* myNode = new FrostTest::CollisionSceneNode(smgr->getRootSceneNode(), smgr, 678, root[namesList[i]]);
		myNode->drop();
		myNode = 0;
	}
}

int main()
{
	// Set up Irrlicht...
	irr::IrrlichtDevice* device = nullptr;
	SetupDevice(device);

	// <PHYSICS>
	Frost::WorldManager root(Frost::FROST_COLLISION_MANAGER_BRUTE_FORCE);

	// Keep registry of named objects
	std::vector<std::string> objNames;

	// Create objects...
	objNames.push_back("Ball");
	objNames.push_back("Table");
	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(10.f, Frost::MathConstants::MATRIX_IDENTITY, 0.f, 0.f, Frost::Vect3(0.f, 150.f, 400.f)), "Ball");
	root.addObject(std::make_shared<Frost::BasicPhysicsObject>(0.f, Frost::MathConstants::MATRIX_ZERO, 1.f, 1.f, Frost::Vect3(0.f, 0.f, 400.f)), "Table");
	root["Ball"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CollisionSphere>(10.f, Frost::Vect3(130.f, 10.f, 130.f), root["Ball"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);
	root["Table"]->getCollidableData()->addCollisionObject(
		std::make_shared<Frost::CollisionBox>(Frost::Vect3(130.f, 10.f, 130.f), root["Table"]->getPhysicsObject()->getPos(),
		Frost::MathConstants::QUATERNION_UNIT, root["Table"]->getPhysicsObject()),
		Frost::MathConstants::MATRIX_IDENTITY);

	// Add forces to the objects in our scene...
	root.addForce(std::make_shared<Frost::GravityForce>(Frost::MathConstants::VECTOR_UNIT_Y * -1.f, 98.1f), "Ball");
	
	// </PHYSICS>

	// Create our scene nodes...
	SetupNodes(device, root, objNames);

	// Main game loop
	irr::u32 frames = 0;
	DemoTimer timer;
	auto driver = device->getVideoDriver();
	auto smgr = device->getSceneManager();
	while (device->run())
	{
		timer.tick();
		root.update(timer.getFrameTime());

		driver->beginScene(true, true, irr::video::SColor(0, 100, 100, 100));
		smgr->drawAll();
		driver->endScene();

		if (++frames == 100)
		{
			irr::core::stringw str = L"Indigo Frost demo - Irrlicht [";
			str += timer.getFrameTime();
			str += L" ms";
			str += L"] FPS: ";
			str += (irr::s32)driver->getFPS();

			device->setWindowCaption(str.c_str());
			frames = 0;
		}
	}

	device->drop();
	device = 0;

	return 0;
}