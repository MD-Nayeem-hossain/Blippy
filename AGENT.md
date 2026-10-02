# Blippy Project - AGENT.md

## Overview
**Blippy** - A modern, cross-platform text expander with advanced placeholder system, designed for power users who need seamless text expansion across Windows, macOS, and Linux.

- **Version**: 0.1.0 (initial release)
- **Purpose**: Replace legacy tools like TextExpander/Beeftext with cross-platform, feature-rich alternative
- **Target Users**: Power typists, writers, developers, anyone who types repetitive text
- **License**: MIT License (free, open source)
- **Framework**: Qt 6.6+ C++

## Directory Structure
```
blippy/
├── CMakeLists.txt          # Build system configuration
├── AGENT.md               # THIS FILE - full project context
├── docs/
│   ├── ARCHITECTURE.md    # System design document
│   ├── PLACEHOLDERS.md    # Placeholder system specification
│   ├── STORAGE.md         # Storage format specification
│   └── UPDATE.md          # Update mechanism documentation
├── src/
│   ├── main.cpp           # Application entry point
│   ├── core/
│   │   ├── input/         # Keyboard hook management (cross-platform)
│   │   ├── placeholder/   # Placeholder evaluation engine
│   │   ├── substitution/  # Text expansion logic
│   │   └── backup/        # Backup and sync system
│   ├── models/            # Data models (combos, groups, history)
│   ├── types/             # Type definitions and enums
│   └── ui/                # Qt Quick/QML interface files
├── tests/
│   ├── unit/              # Unit tests
│   └── integration/       # Integration tests
└── config/
    ├── defaults.json      # Default configuration
    └── settings.json      # User preferences
```

## Key Components

### 1. Input Manager (`src/core/input/`)
- **Responsibility**: Cross-platform keyboard and mouse hook management
- **Platforms**: 
  - Windows: `WH_KEYBOARD_LL` low-level hook
  - Linux: X11/Wayland event filtering
  - macOS: Event taps (Carbon/Cocoa)
- **Critical Bug Fix**: Double-paste prevention via `substitution_active` flag and 300ms suppression window
- **Key Interfaces**:
  - `hookEvent(KeyEvent)` - Called for every keyboard event
  - `setEnabled(bool)` - Enable/disable hooking
  - `getCurrentText()` - Get currently typed text buffer

### 2. Placeholder Engine (`src/core/placeholder/`)
- **Responsibility**: Evaluate `#` placeholders with dropdown selection
- **Placeholder Types**:
  - `#{text}` → Input dialog for custom text
  - `#{time}` → Current time (HH:mm format)
  - `#{date}` → Today's date (yyyy-MM-dd)
  - `#{day}` → Day name (Monday, Tuesday, etc.)
  - `#{remaining:YYYY-MM-DD}` → Time remaining until date
  - `#{emoji}` → Emoji selector
  - `#{random:N}` → N-character random string
  - `#{shortcut:KEY}` → Trigger Blippy shortcut (e.g., `#{shortcut:pt}` → "Payments/Treasury")
- **UI**: Native dropdown on `#` press, input dialog for text placeholders
- **Critical Feature**: Shortcut resolution inside placeholders (typing `pt` expands to "Payments/Treasury" combo)

### 3. Substitution Engine (`src/core/substitution/`)
- **Responsibility**: Text expansion and insertion
- **Methods**:
  - Clipboard paste (for most applications)
  - Keystroke simulation (for sensitive apps like banks)
- **Double-Paste Prevention**:
  1. On shortcut trigger: Disable keyboard hook
  2. Set `substitution_active = true`
  3. Record clipboard content before substitution
  4. After substitution completes (or 300ms elapsed): Re-enable hook, clear flag
  5. Ctrl+v during suppression: Show toast "Blippy substituting... release to paste later"
- **Text Insertion**:
  - `insertByClipboard(text)` - Uses QGuiApplication::clipboard()
  - `insertByKeystrokes(text)` - Simulates typing via platform APIs

### 4. Backup Manager (`src/core/backup/`)
- **Responsibility**: Real-time backup and optional online sync
- **Backup Strategy**:
  - **Local**: Immediately after each substitution (JSON format)
  - **Online (optional)**: rclone integration (user pre-configures)
    - No OAuth inside app
    - User runs `rclone config` once outside Blippy
    - Blippy calls `rclone sync` using existing config
    - User controls: what, when, where to sync
- **Version History**: Keep last 5 backup snapshots
- **Restore**: One-click restore from any snapshot

### 5. Combo Model (`src/models/`)
- **Responsibility**: Manage combo/list data
- **Storage**: JSON format with schema validation
- **Format**:
```json
{
  "id": "uuid-generator",
  "keyword": "h1",
  "snippet": "Hello #{text}!",
  "placeholders": ["text"],
  "group": "work",
  "enabled": true,
  "creationDate": "2024-01-15T10:30:00Z",
  "modificationDate": "2024-01-20T14:20:00Z"
}
```
- **Import**: Beeftext JSON, Text Blaze export, manual JSON import
- **Export**: JSON, Beeftext-compatible, Text Blaze format

### 6. Types System (`src/types/`)
- **Responsibility**: Type definitions and enums
- **Key Enums**:
  - `PlaceholderType` { Text, Time, Date, Day, Remaining, Emoji, Random, Shortcut }
  - `InsertMethod` { Clipboard, Keystrokes }
  - `BackupTarget` { Local, Rclone, None }
  - `HookState` { Enabled, Disabled, Suppressed }

## Build & Run Instructions

### Prerequisites
- Qt 6.6+ (Qt 6.11.2 recommended)
- CMake 3.15+
- GCC/Clang/MSCV compiler

### Building
```bash
# From project root
mkdir -p build && cd build
cmake .. -DQt6_DIR=/usr/lib/cmake/Qt6
make -j$(nproc)
./Blippy
```

### Running Tests
```bash
cd build
ctest --output-on-failure
```

### Configuration
First run will create `~/.config/blippy/settings.json` with default values.

## Known Issues & Fixes (CRITICAL)
1. **Double-paste bug**: Fixed via `substitution_active` flag and 300ms hook suppression window
2. **Hook conflicts**: Blippy disables itself when running in VMs or remote desktops
3. **Clipboard race condition**: Backup records clipboard content before substitution, restores after

## Update Mechanism
- **On startup**: Check GitHub API for latest release
- **Change log**: Generated from git tags: `git log --oneline v0.1.0..v0.1.1`
- **Pre-push**: Husky pre-commit hook asks "What changed?" - must update AGENT.md
- **Versioning**: Semantic versioning: MAJOR.MINOR.PATCH
- **Update flow**:
  1. User runs Blippy
  2. App checks GitHub for newer version
  3. If newer: Show "Update available - v0.1.1" with changelog
  4. User clicks "Update" → downloads and replaces binary
  5. On launch: Shows "Blippy updated to v0.1.1 - see what's new"

## Development Roadmap
- [x] v0.1.0 - Initial release with core placeholder system
- [ ] v0.2.0 - Online backup via rclone (user-configured, no OAuth)
- [ ] v0.3.0 - macOS support (X11/Cocoa hooks)
- [ ] v0.4.0 - Cloud sync (user-owned server, WebDAV)
- [ ] v1.0.0 - Full 1.0 release with all smart features

## Security Model
- **Local-first**: All processing happens locally
- **No automatic cloud upload**: User explicitly configures rclone/sync
- **Clipboard privacy**: Backup records content, but user controls encryption
- **No OAuth inside app**: User pre-configures external tools
- **Sensitive apps**: Detect banking/apps and adjust insertion method

## Change Log
### v0.1.0 - Initial Release
- Core placeholder system (`#` trigger with dropdown)
- Cross-platform keyboard hooks (Windows/Linux)
- Clipboard-based text insertion
- Local backup system
- Beeftext/Text Blaze import
- Double-paste bug prevention
- AGENT.md documentation pattern

### v0.1.1 - Bug Fixes (in progress)
- Improved placeholder evaluation performance
- Enhanced shortcut resolution inside placeholders
- Better error handling for missing dependencies