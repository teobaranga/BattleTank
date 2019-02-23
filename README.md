# BattleTank

![](Screenshots/screenshot.png)

An open world head-to-head tank fight game with simple AI, terrain, and advanced control system in Unreal Engine 4.

This is my personal implementation of the project with the same name from "The Unreal Engine Developer Course" on Udemy.

**Last completed lecture:** Creating physics constraints (121/134)

### Features:
- Main menu with "Start" and "Quit" buttons
- Basic textured landscape sculpted using Unreal
- High quality tank model ([T90 by milosbaskic](https://www.cgtrader.com/free-3d-models/military/vehicle/russian-military-vehicles-t90))
- Basic movement functionality for player and AI: forwards/backwards, left/right (NOTE: movement is glitchy at this stage possibly due to the tank collision volumes)
- Aiming and firing projectiles, along with muzzle flash, smoke trails, and impact blasts done using particle effects 
- Mortar that fires projectiles like the AI tanks, used to demonstrate re-usability
- Basic UI
	- Crosshairs which are colored according to the firing state of the tank
	- Ammo display
- NavMesh used by AI tanks to navigate towards the player 

### Resources:
- Main menu background: [Jumping off Tanks](https://unsplash.com/photos/xO5nNl8QCS4) by Chris Geirman
- Main menu font: [Grunge Tank](https://www.dafont.com/grunge-tank.font)