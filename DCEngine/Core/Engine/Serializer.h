#pragma once


/*
Serializes read streams, read from different data types.
Sometimes you read different things in the order of the file
The objects themselves will decide on what order they read

SIMPLE SERIALIZATION: 

The GameObject class has  a serialize function
void GameObject::Serialize(ISerialize& stream) { 


}


file has:
100
4.3
20
bigship.png

JSON SERIALIZATION:
Object {

HP : 100,
Speed: 4.5,
Armor : 20,
Spritefile : "bigship.png"

The serializer will load those in order.
A good JSON loader will check what input it is given (error checking, depends on lib)
You could ask for them out of order. Traditional serialization is down the line for
performance reasons.

XML SERIALIZATION:
<Object>
<int name = "HP">100</int>
<float name = "Speed">100</int>


Serialization phases

1. Construct the object : built with new frm a factory, not active
2. Serialize and data setting: object is serialized and then data attributes can
   be adjusted.
3. Initialize the object: Object is now actually ready to be used now that all initial data 
    has been set.

    (!!!) Only spaces can create objects to themselves. They will do so by calling the archetype/object
          factory to create the objects then return them and add them to their gameojects vector.

DATA DRIVEN OBJECTS
Don't use "new" everywhere! You want to use an object factory. 
OBJECT MANAGEMENT (FACTORY)
Create, manage and destroy your objects in an unified way.
Don't use new whenever you need an obect.
Each object should have a clear owner.
For game objects, use a factory.

can pass in a string, or an enum, or an ID (strings are easier to read and manip now)
GameObject* ObjectFactory::BuildObject(string type) {
  switch (type) {
  case "PlayerShip" : return new PlayerShip();

  default: return NULL;

You don't need includes anywhere. The object factory header will be including the objects
themselves.  Component-based engines won't deal with this, since we use composition to create
game-logic.

GameObject* ObjectFactory::BuildObject(string type) {
return CreatorMap[type]->Create();

class GOCreator {
vitual GameObject* Create();

class CreateShip : public GOCreator
virtual

Outsource the actual creation of individual objects. Now the create ship class is put in the
same file as the new object.
We have to register objects.

void GameLogic::RegisterObjects()
  GobjectFactory->AddCreator("Ship", new ShipCreator()_


BRINGING IT TOGETHER
Now combine serialization and the data driven factory together.
Factories don't make sense until you put in serialization.
Add to the data source what creator it should use.
The factory serializes the object when it is created.

ADD TYPE INFORMATIONTO TEXT FILE
JSON:

Archetypes is classically the name of the file

GameObject* ObjectFactory::BuildObject(string& filename)
  filestream stream filename
  string objectType ... etc ...

  But wait!

Since we are using a component based engine,
a game object is just a collection of components.
Let's do data-driven composition.

The factory list has a list of creators for components.
It can then use the data source to determine what components
are on the composition and their attributes.

GOC* ObjectFactory::BuildObject(string filename) {
stream
GOC gameobject = new GOC
if GOC == NULL return NULL
while stream.IsGood())

// Get the first component name
streamread(stream, componentName)
// Get an individual pointer to the component by looking up the component creator
Component* component = CompCreators[componentName]->Create()
component->Serialzie(stream)

Now we determine components (text)

//First thing is a component type
Model 
BigGuy.bin // Because the component model knows it needs a string next, it reads the string.
Guy // Now call the component creator for guy, which needs an int, float and int
100
4.5
20

Can search outo f order or in order
JSON LOOKS PRETTY 

That was an archetype file. 
ARCHETYPES
a prototype or the original model (blueprint, recipe, etc) for an objet.
The factory uses the archetype to build the object, and then run time data is modified
as needed (such as position)
- Object = Car
- Archetype = Gray Model 2 BMW
- Instance 

ARHETYPE PROBLEMS
What data do you want to have changed per object?
- Position, Scale, Rotation all need to be different when loaded from
a level file.
Can an archetype override everything?
You need to override these. You dont' put these values in the archetype file.

DATA DRIVEN FACTORY
THe true power of the factory is when it is data drivne
Systems register their component creators to the factory
Objects are reated through archetypes which describe what components are on a 
composition and their attributes.
Run time data is modified as needed.

LEVELS
In a level you want to place an object multiple times.
Use archetypes to alias the objects so their properties can be adjusted.
The loader then overrides the position, rotation, or whatever else.
Allow properties to be overriden in the level file. 
Level loading has the extra step of overriding at the end.

Level file ex:
Camera.txt
0 0
0
Wall.txt
320 -180
0
Wall.txt
-320 -180
0






*/


class Serializer {
  //void ReadFloat(float&);
  //void ReadInt(int&);
  //void ReadString(String&);


};