# SuperTux

SuperTux is a jump'n'run game with strong inspiration from the
Super Mario Bros. games for the various Nintendo platforms.

Run and jump through multiple worlds, fighting off enemies by jumping
on them or bumping them from below, grabbing power-ups and other stuff on the way.

This is a **fork** of the latest version of Milestone 1 containing changes that fix gripes and oddities within the version while also adding newer (and sometimes older) features whilst reasonably keeping the original direction.

![Screenshot](https://www.supertux.org/images/supertux-0.1.3-6.png)


## Story: Penny gets captured!

Tux and Penny were out having a nice picnic on the ice fields of
Antarctica. Suddenly, a creature jumped from behind an ice bush, there
was a flash, and Tux fell asleep!

When Tux wakes up, he finds that Penny is missing. Where she lay
before now lies a letter:
>Tux, my arch enemy! I have captured your beautiful Penny and have
>taken her to my fortress. The path to my fortress is littered with my
>minions. Give up on the thought of trying to reclaim her, you haven't
>got a chance!
>
>-Nolok

Tux looks and sees Nolok's fortress in the distance. Determined to
save his beloved Penny, he begins his journey.

## Installation

This fork currently does not have any binary builds. Refer to `INSTALL` for
compilation instructions.

## Documentation

Important documentation for SuperTux is contained in multiple files.
Please see them:

* `README.md` - This file
* `INSTALL` - Requirements, compiling and installing.
* `LICENSE` - The GNU General Public License, under whose terms SuperTux is
licensed. (Most of the data subdirectory is also licensed under
CC-by-SA)
* `LEVELDESIGN` - Very useful information for those that want to design levels.
* `data/CREDITS` - Credits for people that contributed to the creation of this
version of SuperTux. (You can view these in the game menu as well.)


## Playing the game

### Controls
Both keyboards and joysticks/gamepads are supported. You can view/change the
controls via **Options > Keyboard/Joystick Setup**. <br>
Basically, the only controls you will need
to use in-game are the following: jump, duck, move left & right and power,
"Power" allows you to run, pick up objects and use any
power-up you have, such as shooting fireballs with the fire flower (which is
currently the only implemented power-up).

Other useful keys include the Esc key, which is used to go to the menu
or to go up a level in the menu. The menu can be navigated using the
arrow keys or the mouse.

In the world map, the arrow keys are used to navigate and Left Ctrl/Enter
enters the current level.

The level editor is controled via the mouse, but there are shortcuts that
may make your life easier. To see the shortcuts for the buttons, just
do a Right-click on them. If you are interested in creating SuperTux
levels, we advise you to read `LEVELDESIGN` to learn more about the editor
and creating new levels.

### Scoring
If you kill more than one enemy at a time without landing on the ground,
each enemy is worth 50 more points.

You can also score by shooting at the enemies with fireballs from the
fire flower or by using other enemies, such as Mr. Bomb's explosion, a
kicked Mr. Iceblock or a falling stalactite).