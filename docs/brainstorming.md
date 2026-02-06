# Notes/Brainstorming Document
## 1. What do I want in my game? What do these things do?
- Player
Main controllable character through which the user interacts with the world.
    - Texture/animations 
    - Sound effects
    - Moves around
    - Controlled by a user
    - Inventory
    - Special attributes/data
- Enemy
Creature, can vary greatly in terms of what it can do, size, look, etc, but generally tries to kill player
    - Texture/animations
    - Sound effects
    - Moves around
    - Controlled by AI
    - Inventory
    - Special attributes/data
- Item
Can sit on the ground, or be held in an inventory
    - Texture/animations
    - Sound effects
    - Physics, but doesn't move
    - Special attributes/data
- Tile
Floor of the world, can interact with environment such as mud slowing the player down
    - Texture/animations
    - Sound effects
    - Special attributes/data
- Object
Sits on top of tiles, can be decorative or interactable/utilizable
    - Texture/animations
    - Sound effects
    - Physics, but doesn't move
    - Special attributes/data
    - Sometimes inventory (in the case of a chest, for example)
## 2. Translate to actual (pseudo)code
- Component
    - : Graphics
    - : Audio
    - : Physics
    - : Inventory
    - : Controller
        - : Input Controller
        - : AI Controller
- Game Object
    - 
    - Graphics
    - Audio
    - Physics
    - : Entity
        - Inventory
        - Controller (Input for player, AI for enemy/npc)
    - : Item
        - Item data
- Tile
    - Graphics
- Tilemap
    - vector<vector<Tile>>
- World
    - vector<GameObject>
- Game
    - World
    - *More work will need to be done later to manage loaded tiles/chunks, but for now we will just load everything at once*
