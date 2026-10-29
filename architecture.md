# Architecture of the Engine

## main function

Program class is instantiatied, which constructs, manages, and destructs program related resources. For example, the cwd of the running process, it is not a 'game specific' thing so the Program class manages it

Game class is instantiated, which constructs, manages, and destructs game related resources. All the game is within this class, including the game loop

The game loop is called, which is infinite until the user closes the game

## Game class

- It has a public run() method, the game loop
- It holds the game loop state variables
- It holds the handles to all the windows the game owns
- It holds a vector\<Screen\> & vector\<Subscreen\> of all the game screens & subscreens
- It holds a hierarchical tree containing the Screens from the vector container as nodes
- It holds a vector of 'Special Screens', which are normal screens in the tree that can be called by anyone as they are visible to all the tree
- It holds the variable that references the current active screen. The game loop is supposed to cooperate with this active screen.
- It holds the loading screen, which is separate from all the screens that exist. This screen the game engine only renders when it is loading game resources.

## Screen class

Each screen is like a stateless function. It takes structured arguments from the caller screen. It knows the id of the screen that called it. The caller screen makes it the active screen. It does its job, and then returns structured data back to the caller and makes the caller the active screen. When its in the tree, it can only see & call its children, it can only be called by its parent to which it returns control. It can see nowhere else in the screen, except the 'Special Screens' list which lives parallel to the tree. This container vector references the normal screens in the tree that can be called by any screen from anywhere (a common example is the settings screen). Whenever a screen calls another, it registers a callback that takes input the returned data by the called screen, and does a job out of it. and then normally gives control back to the caller.

A screen is responsible for its own resources. The parent does not provide any resource; The screen manages itself. For persistent data, the whole game is supposed to have a single AppData directory. Now the relevant data should be returned to the parent, but if the screen wishes to independently keep its persistent data, it 'owns' a directory in the Game AppData dir. There should be a folder of the name of the screen id for every screen, that the screen owns.

The Screen holds the Environment Class

## Subscreen class

It does not take full control. The caller screen is still active and rendering while this renders on top of it whatever screen part it wants.

A Screen can only own a single subscreen. A Subscreen cannot be independently used anywhere; it can only be owned by a Screen. A subscreen cannot see its Screen's parent nor children. It can only see and call Special Screens. When a called Screen returns to it, it cannot have a registered callback. It does not take any arguments, though it can return structured data. It cannot have persistent data.

## Environment class

It is an abstract interface class that defines an environment/scene
An implementation of it can be a type of environment, like:
    2D Top_view
    2D Side_view
    3D
    etc..
The implementation can allow to activate/deactivate any subsystem. For example, you can disable gravity, or alter with its values in a 2D Side_view Environment.

For a screen to render on the surface, it can either do it the raw way, or it can hold an environment instance that simulates the environment and coordinates objects within.

### Environment coordination with objects

A 2D Environment holds a vector of all the sprites registered for it. In the game loop it gives all sprites chance to update their coordinates, size, texture, and then renders them accordingly. What about collision and objects' interaction with each other?

1. Each sprite should have registered a collision callback, that recieves in its parameters the ID of the object it collided with, so that it can respond respectively.
2. Each sprite should have registered an interaction callback, that recieves in its parameters the ID of the object that requests an interaction and also the signal of interaction, so that it can respond respectively.
