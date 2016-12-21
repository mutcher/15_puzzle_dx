# 15 Puzzle Game
[![Build status](https://ci.appveyor.com/api/projects/status/2v5auycn8a26tun7?svg=true)](https://ci.appveyor.com/project/mutcher/15-puzzle-dx)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
## Screenshots
![Main Menu](https://github.com/mutcher/15_puzzle_dx/raw/master/.github/main_menu.png "Main Menu")
![Game Field](https://github.com/mutcher/15_puzzle_dx/raw/master/.github/game_field.png "Game Field")

## Description
This is classic 15 puzzle game.
Same as [console version](https://github.com/mutcher/15_puzzle) but I desided use DirectX 11 for rendering.

## License
Copyright (c) 2016 Illia Gorbatiuk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Used Tools
1. Visual Studio 2012 or newer.
2. GIMP 2.

## How To Compile
1. First of all you need Visual Studio 2012 or newer.
1. Compile 15_puzzle_dx project.
1. Go to *$(OutDir)*.
1. Start the *15_puzzle_dx.exe*.

## How To Run In Debug Mode
1. Make first two steps from *How To Compile* part.
1. [Set Working Directory](https://codeyarns.com/2013/12/30/how-to-change-working-directory-in-visual-studio/) to $(OutDir).
1. Run the application.

## Default Controls
### Main Menu
* Arrow keys.
* Enter button.

### Game Scene
* Arrow keys
* F5 button to restart the game.
* Esc button to back into main menu.
