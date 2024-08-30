

Factorio like game 

structure

+---------------------+
|       World         |
+---------------------+
| - tiles: Tile[]     |
| - width: int        |
| - height: int       |
+---------------------+
| + GetTile(x, y): Tile|
| + UpdateWorld()     |
+---------------------+
          |
          v
+---------------------+             +---------------------+
|       Tile          |<>---------->|      Machine        |
+---------------------+             +---------------------+
| - position: Vec2    |<>---------->| - position: Vec2    |
| - is_empty: bool    |             | - input_items: Item[]|
| - machine: Machine* |             | - output_items: Item[]|
+---------------------+             | + Update()          |
                                    | + ProcessItems()    |
                                    +---------------------+
                                            |
                                            v
+---------------------+     +---------------------+     +---------------------+
|    MiningDrill      |     |    ConveyorBelt     |     |     Furnace         |
+---------------------+     +---------------------+     +---------------------+
| + Update()          |     | + Update()          |     | + Update()          |
| + MineResource()    |     | + MoveItems()       |     | + SmeltOre()        |
+---------------------+     +---------------------+     +---------------------+

+---------------------+
|       Item          |
+---------------------+
| - type: ItemType    |
| - quantity: int     |
| - weight: float     |
+---------------------+

+---------------------+
|      Inventory      |
+---------------------+
| - items: Item[]     |
| - capacity: int     |
+---------------------+
| + AddItems(item: Item)|
| + RemoveItem(item: Item)|
+---------------------+
