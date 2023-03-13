# TrickyPrototyping (DEPRECATED)

**THIS PLUGIN IS DEPRECATED, PLEASE USE [THIS COLLECTION OF PLUGINS](https://tricky-fat-cat.itch.io/tricky-prototyping-collection)**

UE4 plugin for rapid prototyping. It contains the most basic systems and mechanics which can be found in the majority of games.

I used this plugin in my personal projects:

1. [**HONK III**](https://tricky-fat-cat.itch.io/honk-iii);
2. [**Game designer test**](https://github.com/TrickyFatCat/AssignmentVNG);

## Installation

1. Create Plugins folder in your project directory;
2. Download and unzip the archive in this folder. You should get such a path to a plugin `Plugins/TrickyPrototyping`;
3. Alternatively you can clone this repository into the `Plugins` directory;

## What does it contain?

The plugin contains the most frequently used things while prototyping various games.

All elements work with blueprints.

  * [Animated actors](#animated-actors)
    + [Doors](#doors)
    + [Buttons](#buttons)
  * [FloatingActors](#floatingactors)
    + [Spline actor](#spline-actor)
  * [Pickups](#pickups)
  * [Interaction system](#interaction-system)
  * [Keyring system](#keyring-system)
  * [Health system](#health-system)
  * [Entity resource object](#entity-resource-object)
  * [Core elements](#core-elements)
  * [User interface](#user-interface)

## Animated actors

`AnimatedActor` can animate its scene components using a timeline component. It was used to create doors and buttons for the plugin.

However, it is possible to make your custom actor using blueprints.

Animated actors do not know what scene components to animate.

To make them work, call functions `FillAnimatedComponents` or `AddAnimatedComponent` in the construction script. It must be done for doors and buttons too.

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

## Interaction system

There is a simple interaction system in the plugin.

`InteractionQueueComponent` must be added to a player character to make this system work.

All actors mentioned above can be easily made interactive through their properties.

However, it's easy turn any actor into interactive one:

1. Add `InteractionBoxComponent`, `InteractionSphereComponent` or `InteractionCapsuleComponent` to a chosen actor;
2. Add `InteractionInterface` to a chosen actor;
3. Override `ProcessInteraction` function;

## Keyring system

This system is used for opening doors, however, you can use them everywhere.

To make it work:

1. Add `KeyRingComponent` to a player character;
2. Create a new Blueprint class inherited from the `KeyType` C++ class;
3. Give this key to the player character using pickups or through other systems;

## Health system

The description will be added soon

## Entity resource object

The description will be added soon

## Core elements

The description will be added soon

## User interface

The description will be added soon
