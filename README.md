# Unreal Engine 5.6 Multiplayer Game

A complete Unreal Engine 5.6 game project with modern multiplayer networking support for testing across multiple computers.

## Features

- **UE 5.6 Compatible**: Built for the latest Unreal Engine version
- **Modern Multiplayer**: Enhanced networking with latest UE5 features
- **First-Person Character**: Customizable player character with movement and interaction
- **Weapon System**: Basic weapon mechanics with projectiles
- **AI Enemies**: Simple AI enemies that can be spawned and fought
- **Game Modes**: Multiple game modes including Deathmatch and Team Deathmatch
- **UI System**: Complete HUD and menu system using CommonUI
- **Level Design**: Sample levels with different environments
- **Enhanced Input**: Modern input system using Enhanced Input plugin
- **Mass AI**: Optional Mass Entity AI system for performance

## Project Structure

```
unreal/
├── Content/           # Unreal Engine content (Blueprints, Meshes, Materials, etc.)
├── Source/            # C++ source code
├── Config/            # Engine and project configuration
├── Build/             # Build scripts and tools
└── Documentation/     # Project documentation
```

## Setup Instructions

### Prerequisites
- **Unreal Engine 5.6** (latest version)
- Visual Studio 2022 (for C++ development)
- Git for version control

### Installation
1. Clone this repository
2. Open the `.uproject` file with Unreal Engine 5.6
3. Build the project when prompted
4. Configure networking settings in Project Settings

### Multiplayer Testing
1. **Host Setup**: 
   - Open the project in Unreal Engine 5.6
   - Go to Play → Advanced Settings
   - Set "Number of Players" to 2 or more
   - Set "Net Mode" to "Play as Listen Server"
   - Click Play

2. **Client Setup**:
   - On the second computer, open the same project
   - Go to Play → Advanced Settings
   - Set "Net Mode" to "Play as Client"
   - Enter the host computer's IP address
   - Click Play

## Game Controls

- **WASD**: Move
- **Mouse**: Look around
- **Left Click**: Fire weapon
- **R**: Reload
- **E**: Interact
- **Space**: Jump
- **Shift**: Sprint
- **Tab**: Scoreboard

## UE 5.6 Features Used

- **Enhanced Input System**: Modern input handling
- **CommonUI**: Cross-platform UI framework
- **Modular Gameplay**: Component-based gameplay systems
- **Mass Entity**: High-performance AI system
- **Gameplay Abilities**: Advanced ability system
- **Gameplay Debugger**: Built-in debugging tools
- **Gameplay Insights**: Performance monitoring

## Development

### Adding New Features
- Create Blueprints in the Content folder
- Add C++ classes in the Source folder
- Use Enhanced Input for new controls
- Leverage CommonUI for new interfaces
- Update this README with new features

### Networking Considerations
- All gameplay logic should be replicated
- Use Gameplay Framework classes for networking
- Test multiplayer functionality regularly
- Use Gameplay Debugger for network debugging

## Contributing

This is a personal project for learning Unreal Engine 5.6 multiplayer development.

## License

Personal use only - not for commercial distribution. 