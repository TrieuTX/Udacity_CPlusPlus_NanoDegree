# CPPND: Capstone Snake Game Checklist

## RUBIC Point
### README : New features you added to the game
1. Create a object name `Mouse` (green), have auto run and eat `food` use A* algorithm, if `mouse` eat `food` you score - 10
2. `Snake` can eat the `mouse`, add 10 score
3. Game will render wall (blue color )around the play space, if `snake` hit the wall, `snake` die and game ended

### Compiling and Testing
  1. The submission must compile and run without errors on the Udacity project workspace.
### Loops, Functions, I/O
1. The project demonstrates an understanding of C++ functions and control structures: implement function A* search algorithm
``` C++
int Game::heuristic(const SDL_Point &a, const SDL_Point &b) {
  return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

std::vector<SDL_Point> Game::AStarMouse(const SDL_Point &start,
                                        const SDL_Point &goal) {

  std::cout << "AStarMouse: " << std::endl;
  std::priority_queue<Node> openList;
  std::map<SDL_Point, SDL_Point, bool (*)(const SDL_Point &, const SDL_Point &)>
      cameFrom([](const SDL_Point &a, const SDL_Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      });
  std::map<SDL_Point, int, bool (*)(const SDL_Point &, const SDL_Point &)>
      gScore([](const SDL_Point &a, const SDL_Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      });
  std::map<SDL_Point, int, bool (*)(const SDL_Point &, const SDL_Point &)>
      fScore([](const SDL_Point &a, const SDL_Point &b) {
        return a.x == b.x ? a.y < b.y : a.x < b.x;
      });

  openList.push({start.x, start.y, 0, 0, heuristic(start, goal)});
  gScore[start] = 0;
  fScore[start] = heuristic(start, goal);

  std::vector<SDL_Point> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

  while (!openList.empty()) {
    Node current = openList.top();
    openList.pop();

    SDL_Point currentPos = {current.x, current.y};
    if (currentPos.x == goal.x && currentPos.y == goal.y) {
      std::vector<SDL_Point> path;
      while (cameFrom.find(currentPos) != cameFrom.end()) {
        mouseASearchPath.push_back(currentPos);
        currentPos = cameFrom[currentPos];
      }
      std::reverse(mouseASearchPath.begin(), mouseASearchPath.end());
      return mouseASearchPath;
    }

    for (const auto &dir : directions) {
      SDL_Point neighbor = {current.x + dir.x, current.y + dir.y};

      int tentative_gScore = gScore[currentPos] + 1;
      if (gScore.find(neighbor) == gScore.end() ||
          tentative_gScore < gScore[neighbor]) {
        cameFrom[neighbor] = currentPos;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] = gScore[neighbor] + heuristic(neighbor, goal);
        openList.push({neighbor.x, neighbor.y, fScore[neighbor],
                       gScore[neighbor], heuristic(neighbor, goal)});
      }
    }
  }

  return {};
}
```
2. The project uses data structures and immutable variables: Use vectors
```C++
 std::vector<SDL_Point> mouseASearchPath{};
```

### Object Oriented Programming
1. One or more classes are added to the project with appropriate access specifiers for class members: add new class GameObject and Mouse
```C++
class GameObject {
public:
  GameObject(int grid_width, int grid_height)
      : grid_width(grid_width), grid_height(grid_height) {}

  virtual bool ObjectCell(int x, int y) = 0;

  // Accessors
  bool IsAlive() const { return alive; }
  int GetHeadX() const { return head_x; }
  int GetHeadY() const { return head_y; }
  void SetHead(int x, int y) {
    head_x = x;
    head_y = y;
  }
  void setAlive(bool input) { alive = input; }

protected:
  bool alive{true};
  float head_x;
  float head_y;
  int grid_width;
  int grid_height;
};
```

``` C++
class Mouse : public GameObject {
public:
  Mouse(int grid_width, int grid_height) : GameObject(grid_width, grid_height) {
    NewMouse();
  }

  void UpdatePosition(int x, int y);
  bool ObjectCell(int x, int y) override;
  void NewMouse();

  int randomPosition(int lower, int upper);
};
```
2. Class constructors utilize member initialization lists
``` C++
  GameObject(int grid_width, int grid_height)
      : grid_width(grid_width), grid_height(grid_height) {}
```
```C++
  Mouse(int grid_width, int grid_height) : GameObject(grid_width, grid_height) {
    NewMouse();
  }
```
3. Classes follow an appropriate inheritance hierarchy with virtual and override functions: Class Mouse and Snake inheritane class GameObject and override method `ObjectCell`
```C++
class GameObject {

  ...
  virtual bool ObjectCell(int x, int y) = 0;
  ...
  }

class Mouse : public GameObject {
  ...
  bool ObjectCell(int x, int y) override;
  ...
}

class Snake : public GameObject {
  ...
  bool ObjectCell(int x, int y) override;
  ...
}
```
### Memory Management
1. The project makes use of references in function declarations: Function having parameters as 
references

```C++
std::vector<SDL_Point> Game::AStarMouse(const SDL_Point &start, const SDL_Point &goal);
int Game::heuristic(const SDL_Point &a, const SDL_Point &b);
```
2. The project follows the Rule of 5: in class `Mouse`, `Snake`, `GameObject`
```C++
Class Snake : public GameObject
  Snake(int grid_width, int grid_height) : GameObject(grid_width, grid_height) {
    head_x = static_cast<int>(grid_width / 2);
    head_y = static_cast<int>(grid_height / 2);
  }

  // Destructor
  ~Snake() = default;

  // Copy constructor
  Snake(const Snake &other)
      : GameObject(other), speed(other.speed), size(other.size),
        growing(other.growing), body(other.body) {}

  // Copy assignment operator
  Snake &operator=(const Snake &other) {
    if (this == &other)
      return *this;               // Self-assignment check
    GameObject::operator=(other); // Copy base class properties
    speed = other.speed;
    size = other.size;
    growing = other.growing;
    body = other.body;
    return *this;
  }

  // Move constructor
  Snake(Snake &&other) noexcept
      : GameObject(std::move(other)), speed(other.speed), size(other.size),
        growing(other.growing), body(std::move(other.body)) {
    // Reset moved-from object state if needed
    other.speed = 0.0f;
    other.size = 0;
    other.growing = false;
  }

  // Move assignment operator
  Snake &operator=(Snake &&other) noexcept {
    if (this == &other)
      return *this;                          // Self-assignment check
    GameObject::operator=(std::move(other)); // Move base class properties
    speed = other.speed;
    size = other.size;
    growing = other.growing;
    body = std::move(other.body);

    // Reset moved-from object state if needed
    other.speed = 0.0f;
    other.size = 0;
    other.growing = false;
    return *this;
  }
```

```C++
Class GameObject
  GameObject(int grid_width, int grid_height)
      : grid_width(grid_width), grid_height(grid_height) {}

  virtual ~GameObject() = default;

  // Copy constructor
  GameObject(const GameObject &other)
      : grid_width(other.grid_width), grid_height(other.grid_height),
        alive(other.alive), head_x(other.head_x), head_y(other.head_y) {}

  // Copy assignment operator
  GameObject &operator=(const GameObject &other) {
    if (this == &other)
      return *this; // Self-assignment check
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    alive = other.alive;
    head_x = other.head_x;
    head_y = other.head_y;
    return *this;
  }

  // Move constructor
  GameObject(GameObject &&other) noexcept
      : grid_width(other.grid_width), grid_height(other.grid_height),
        alive(other.alive), head_x(other.head_x), head_y(other.head_y) {
    // Optionally, reset the state of the moved-from object if necessary
    other.grid_width = 0;
    other.grid_height = 0;
    other.alive = false;
    other.head_x = 0;
    other.head_y = 0;
  }

  // Move assignment operator
  GameObject &operator=(GameObject &&other) noexcept {
    if (this == &other)
      return *this; // Self-assignment check
    grid_width = other.grid_width;
    grid_height = other.grid_height;
    alive = other.alive;
    head_x = other.head_x;
    head_y = other.head_y;

    // Reset the state of the moved-from object
    other.grid_width = 0;
    other.grid_height = 0;
    other.alive = false;
    other.head_x = 0;
    other.head_y = 0;

    return *this;
  }
```
3. The project uses smart pointers instead of raw pointers
```C++
in Class Game
  std::unique_ptr<Snake> snake;
  std::unique_ptr<Mouse> mouse;
```
### Concurrency
1. The project uses multithreading: create new thread for run Mouse in class Game, execute the `AutoRunMouse` function and stop, clean up the thread in destructor

```C++
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(std::make_unique<Snake>(grid_width, grid_height)),
      mouse(std::make_unique<Mouse>(grid_width, grid_height)), engine(dev()),
      random_w(1, static_cast<int>(grid_width - 2)),
      random_h(1, static_cast<int>(grid_height - 2)), kGridWidth(grid_width),
      kGridHeight(grid_height) {
  PlaceFood();
  auto_run_mouse = std::thread(&Game::AutoRunMouse, this);
}

Game::~Game() {
  auto_running_mouse = false; // Signal the auto-run thread to stop
  if (auto_run_mouse.joinable()) {
    auto_run_mouse.join(); // Join the thread before destruction
  }
}

```
2. A mutex or lock is used in the project: use mutex for thread safety, protect data
```C++
void Game::Update() {
  std::lock_guard<std::mutex> lock(mouse_mutex); // Lock mutex for thread
  // safety
  ....
}

void Game::AutoRunMouse() {
  while (auto_running_mouse) {

    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)); // mouse delay movement
    std::lock_guard<std::mutex> lock(
        mouse_mutex); // Lock mutex for thread safety
        ....
  }
}
```
## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.


## CC Attribution-ShareAlike 4.0 International


Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
