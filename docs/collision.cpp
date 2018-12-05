/*
https://tutel.me/c/gamedev/questions/29691/2d+tilebased+collision+detection



You might be okay without a quadtree:

    Collisions between projectiles and static environment tiles can be done directly on the tiles array, without the need for any additional data structure.
    In other words, you can easily find which tiles an entity (e.g. a projectile) is intersecting directly, with a few calculations based on the bounds
    of the entity and the tile size. The platformer sample (http://create.msdn.com/en-US/education/catalog/sample/platformer) has an example of this problem which I'll transcribe below:

*/

    

int leftTile = bounds.Left / TileSize;      // bounds = Rectangle of your entity
int topTile = bounds.Top / TileSize;
int rightTile = (int)Math.Ceiling((float)bounds.Right / TileSize) - 1;
int bottomTile = (int)Math.Ceiling(((float)bounds.Bottom / TileSize)) - 1;

for (int y = topTile; y <= bottomTile; ++y)
{
    for (int x = leftTile; x <= rightTile; ++x)
    {
        // Entity is touching tile[x, y] so check and handle collision
    }
}



/*
	For other dynamic objects such as characters, you could dynamically register/unregister them with the tiles that they're intersecting as they move, so that at any moment and for any tile you can directly know if any objects are there (here's an example, article about spatial hashing). If you implement that, then you can just do the same as step one to find which tiles the projectiles are interesecting, then check your grid to see if any objects are there, and if there are, iterate over them linearly.

	This data structure can be as simple as:

	var grid = new List<Entity>[width, height];

	Or:

	var grid = new Dictionary<int, List<Entity>>();

	In which case you'd convert [x,y] to a single int key like x+y*width and then store in the dictionary.
*/