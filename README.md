# th3d

bullethell with 3 dimensions

## Installation

Currently, th3d was only tested to run on linux, but it should run on windows and mac too.

With this commands, you will build th3d (make sure you also are located in the th3d directory)
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

To start th3d, you just do
```
$ ./th3d resources/bullets/test.bm.json
```

th3d takes one parameter, and that is the bullet map, that its going to load.

If you want to make your own, please proceed further to the bullet map section.

# Bullet Maps

Everything is defined into a json file, lets first examine its structure

At the very top of the hierachy, we have a single object. It represents the world, where everything is contained in.

The most importand thing the world has are its units. They consist of three other components.

- shape
- position
- engines

__Shape__ defines how the unit is going to be perceived. For example, as a cube.

__Position__ defines where the unit is located with the x, y and z coordinated.

__Engines__ are the bullet producers the unit has available.

When looking closer to position, you will notice, that there not only numbers, but also addition, subtractions, maybe some functions and everything is contained in a string. From now on, this will be refered as a __mathematical expression__. In th3d, you define positions with mathematical expression, since you are able to not only define a fixed position, but also make the position change after time. This is defined as __movement__. So with mathematical expression, you are easily able to move .
