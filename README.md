# 2048

A command line clone of the 2048 game. The game can be played with WASD keys. Supports custom-sized game grids and custom starting tile values.

### Building
```
make
```
### Running 
```
./2048 [options]
```
### Command line arguments
```
-x <value>    Set horizontal size (default 4, min 1)
-y <value>    Set vertical size (default 4, min 1)
-n <value>    Set start tile value (default 2, min 1)
-h            Show help
```
### Clean build directory
```
make clean
```
### Planned features
- [ ] Save game state and high scores on exit
- [ ] GUI
