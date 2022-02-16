# TrickyPrototyping

UE4 plugin for rapid prototyping. It contains the most basic systems and mechanics which can be found in majority of games.

## Installation

1. Create Plugins folder in your project directory;
2. Download and unzip archive in this folder. You should get such a path to a plugin `Plugins/TrickyPrototyping`;
3. Alternatively you can clone this repository into the `Plugins` directory;

## What does it contain?

The plugin contains the most frequently used things while prototyping various games.

All elements work with blueprints.

## Animated actors

`AnimatedActor` can animate its scene components using a timeline component. It was used to create doors and buttons for the plugin.

However, it is possible to make your custom actor using blueprints.

### Doors

There are two classes of doors:

1. `DoorBase`. Great to make simple sliding doors;
2. `DoorSwing`. Specifically made for sliding doors. It calculates door leaves rotation relative to the player position;

Doors can be controlled manually. However, it’s easy to make them open automatically through interaction.

### Buttons

There is only one button class `ButtonBase`. However, it is possible to make a pressure plate, a simple button, and a switch out of this class.

## FloatingActors

These actors float between given points.

There are two different classes:

1. `FloatingActorSpline`. This actor moves along a given spline component. Great for complicated movement trajectory;
2. `FloatingActorPoints`. This actor moves between given actors in the level. Simpler, but takes less time to adjust;

These classes are made to create floating platforms, elevators, etc.

### Spline actor

A simple actor with a spline component which is good to use with `FloatingActorSpline`.

## Pickups

`PickupBase` class is made for creating any pickup for a prototype with ease through rewriting one function.

There are already 2 custom pickups:

1. `PickupHealth`. It restores some health;
2. `PickupKey`. It gives a specific key class;
