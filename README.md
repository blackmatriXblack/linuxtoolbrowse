# linuxtoolbrowse
a tool of linux for everyone in c

## 1. Project Overview

**Linux Tools Browser GTK** is a graphical user interface (GUI) application developed in C using the GTK 3 library. Its primary purpose is to serve as an interactive reference guide and browser for common Linux system tools, commands, and software.

Unlike execution-focused launchers, this application is designed for **discovery and education**. It presents a categorized list of essential Linux utilities along with concise descriptions, helping users identify the right tool for specific tasks such as system monitoring, network diagnostics, file management, and software development.

### Key Features
- **Categorized Reference**: Tools are organized into 9 logical categories (e.g., System Monitoring, Networking, Development).
- **Descriptive Interface**: Each tool is accompanied by a brief, human-readable description of its function.
- **Resizable & Scrollable**: The interface uses scrollable views, allowing users to browse long lists comfortably.
- **Tabbed Navigation**: Uses a `GtkNotebook` to switch between categories easily without cluttering the screen.
- **Lightweight**: Written in pure C with minimal dependencies (GTK 3 only), ensuring fast startup and low resource usage.

---

## 2. Technology Stack & Dependencies

### Programming Language
- C (C99/C11 standard)

### Core Libraries
- **GTK+ 3.0**: Used for creating the main window, notebook tabs, tree views, and handling UI events.
- **GLib**: Used for data structures (though minimal here, mostly for GTK integration).

### Build Dependencies
GTK 3 development libraries are required. On Debian/Ubuntu-based distributions:
```bash
sudo apt-get install libgtk-3-dev
```

### Runtime Dependencies
- None specific to the application itself. It is a static reference tool and does not execute the listed commands. However, the tools listed (e.g., `htop`, `git`, `vim`) must be installed on the system to be useful outside this browser.

---

## 3. Architecture Design

### 3.1 Data Structure
The application uses a simple struct to define each tool entry:
```c
typedef struct {
    const char *name;   // Name of the tool/command
    const char *desc;   // Brief description of its function
} ToolEntry;
```
Arrays of `ToolEntry` structs are defined for each category, terminated by `{ NULL, NULL }` to mark the end of the list. This allows for easy iteration and dynamic GUI generation.

### 3.2 GUI Components
1.  **Main Window (`GtkWindow`)**: The top-level container.
2.  **Notebook (`GtkNotebook`)**: Acts as a tabbed pane, holding one tab per category.
3.  **Scrolled Window (`GtkScrolledWindow`)**: Wraps the tree view in each tab to enable scrolling if the list exceeds the window height.
4.  **Tree View (`GtkTreeView`)**: Displays the list of tools in a two-column format:
    -   **Column 1**: Tool Name (e.g., `htop`)
    -   **Column 2**: Description (e.g., "Interactive process viewer")
5.  **List Store (`GtkListStore`)**: The underlying data model that holds the `ToolEntry` data for each tab.

### 3.3 Dynamic UI Generation
The function `create_category_view()` is a reusable factory that:
1.  Creates a `GtkListStore` with two string columns.
2.  Iterates through the provided `ToolEntry` array and populates the store.
3.  Creates a `GtkTreeView` with two resizable columns.
4.  Embeds the view in a `GtkScrolledWindow`.
5.  Returns the scrolled window widget to be added to the notebook.

This approach minimizes code duplication and makes it easy to add new categories by simply defining a new `ToolEntry` array and adding one line to the `main()` function.

---

## 4. Functional Categories

The application organizes over 100 Linux tools into the following 9 tabs:

| Tab Category | Description | Key Tools Included |
| :--- | :--- | :--- |
| **System Monitoring** | Tools for observing real-time system performance and resource usage. | `top`, `htop`, `atop`, `vmstat`, `iostat`, `free`, `glances`, `nmon`, `sar` |
| **System Administration** | Essential commands for managing services, users, disks, and kernel modules. | `systemctl`, `journalctl`, `dmesg`, `fdisk`, `lsblk`, `mount`, `useradd`, `passwd`, `lspci`, `lsusb` |
| **Networking** | Utilities for network configuration, diagnostics, and security scanning. | `ping`, `traceroute`, `ip`, `ifconfig`, `nmcli`, `netstat`, `ss`, `curl`, `wget`, `ssh`, `nmap`, `tcpdump`, `wireshark`, `iptables` |
| **File Management** | Commands for navigating, manipulating, and archiving files and directories. | `ls`, `cd`, `cp`, `mv`, `rm`, `find`, `locate`, `tar`, `gzip`, `zip`, `mc`, `ranger`, `Nautilus`, `Dolphin` |
| **Text Editors** | Both terminal-based and graphical text editors for code and configuration. | `vim`, `neovim`, `emacs`, `nano`, `gedit`, `Kate`, `VS Code`, `Sublime Text`, `micro` |
| **Development** | Compilers, debuggers, version control, and build tools for software development. | `gcc`, `g++`, `make`, `cmake`, `gdb`, `valgrind`, `git`, `diff`, `strace`, `pkg-config`, `autoconf` |
| **Multimedia** | Tools for audio/video playback, editing, and conversion. | `ffmpeg`, `vlc`, `mpv`, `gimp`, `imagemagick`, `inkscape`, `audacity`, `obs-studio`, `handbrake`, `pavucontrol` |
| **Utilities** | General-purpose command-line utilities for text processing and system info. | `grep`, `sed`, `awk`, `sort`, `uniq`, `wc`, `head`, `tail`, `less`, `watch`, `date`, `which`, `whereis` |
| **Office / Productivity** | Office suites, document viewers, and e-book managers. | `LibreOffice` (Writer, Calc, Impress), `evince`, `okular`, `calibre`, `scribus`, `Zathura` |

---

## 5. Compilation & Execution

### 5.1 Standard Compilation
In most modern Linux distributions with GTK 3 installed, use `pkg-config` to handle flags automatically:

```bash
gcc -o linux-tools-browser-gtk linux-tools-browser-gtk.c $(pkg-config --cflags --libs gtk+-3.0)
```

### 5.2 Manual Compilation (Fallback)
If `pkg-config` is unavailable or fails, you can manually specify the include paths and libraries:

```bash
gcc -o linux-tools-browser-gtk linux-tools-browser-gtk.c \
  -I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 \
  -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
  -I/usr/include/pango-1.0 -I/usr/include/cairo \
  -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0 \
  -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 \
  -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 \
  -lgio-2.0 -lgobject-2.0 -lglib-2.0
```

### 5.3 Execution
```bash
./linux-tools-browser-gtk
```

### 5.4 Usage Instructions
1.  Launch the application.
2.  Use the tabs at the top to switch between categories (e.g., click "Networking").
3.  Scroll through the list to find a tool.
4.  Read the description in the right column to understand what the tool does.
5.  Open a separate terminal to execute the desired command (the app itself does not run the tools).

---

## 6. Code Structure & Key Functions

### 6.1 Data Definitions
-   `ToolEntry` struct: Defines the schema for each tool.
-   `sysmon_tools[]`, `net_tools[]`, etc.: Static arrays containing the hardcoded data for each category.

### 6.2 UI Construction
-   **`create_category_view(const ToolEntry *tools)`**:
    -   Creates a `GtkListStore` with 2 columns (String, String).
    -   Loops through the `tools` array until `name` is `NULL`.
    -   Populates the store with `gtk_list_store_set`.
    -   Creates a `GtkTreeView` and adds two `GtkTreeViewColumn`s with `GtkCellRendererText`.
    -   Wraps the view in a `GtkScrolledWindow` and returns it.

### 6.3 Main Logic
-   **`main()`**:
    -   Initializes GTK.
    -   Creates the main window and sets title/size.
    -   Creates a `GtkNotebook`.
    -   Uses a macro `ADD_TAB` to simplify adding each category:
        ```c
        #define ADD_TAB(notebook, title, tools_array)
        do {
            label = gtk_label_new(title);
            view  = create_category_view(tools_array);
            gtk_notebook_append_page(GTK_NOTEBOOK(notebook), view, label);
        } while(0)
        ```
    -   Shows all widgets and enters the GTK main loop.

---

## 7. Known Limitations & Considerations

1.  **Read-Only Reference**:
    -   This application is **not** a launcher. Clicking on a tool does not execute it. It is purely an informational browser. Users must open a terminal separately to run the commands.

2.  **Hardcoded Data**:
    -   All tool lists are hardcoded in the source code. Adding new tools requires editing the C file and recompiling. There is no external configuration file support (e.g., JSON/XML).

3.  **Desktop Environment Agnostic**:
    -   The list includes tools from various desktop environments (GNOME, KDE, XFCE). For example, it lists both `Nautilus` and `Dolphin`. Users should be aware that some GUI tools may not be installed or may look out of place depending on their current DE.

4.  **No Search Functionality**:
    -   Currently, users must manually browse tabs and scroll to find tools. A search/filter bar would significantly improve usability for large lists.

5.  **Static Descriptions**:
    -   Descriptions are brief and static. They do not pull `man` pages or live help data.

---

## 8. Extension & Maintenance Suggestions

### 8.1 Add Search/Filter
Implement a `GtkSearchEntry` above the tree view. Connect its `search-changed` signal to a filter function that hides rows in the `GtkListStore` that do not match the search text.

### 8.2 Add "Copy to Clipboard" Feature
Add a right-click context menu to the `GtkTreeView` that allows users to copy the selected tool name to the clipboard for easy pasting into a terminal.

### 8.3 External Configuration
Move the `ToolEntry` arrays to a JSON or CSV file. Parse this file at runtime to populate the UI. This would allow users to customize the list without recompiling.

### 8.4 Link to Man Pages
Add a button or double-click action that opens the `man` page for the selected tool in a terminal or a GUI man-page viewer (e.g., `yelp` or `gnome-help`).

### 8.5 Icons
Add a third column for icons. Use `gtk_cell_renderer_pixbuf` to display standard GTK icons or custom icons for each tool, enhancing visual recognition.

---

## 9. License

This project code is licensed under the **Public Domain**. You are free to use, modify, and distribute it without attribution.

---

## 10. Appendix: Comparison with `linux-real-tools-menu`

| Feature | Linux Tools Browser GTK | Linux Real Tools Menu |
| :--- | :--- | :--- |
| **Primary Goal** | Reference & Education | Execution & Launching |
| **Interaction** | Browse lists, read descriptions | Right-click menu, run commands |
| **UI Style** | Tabbed Notebook with Tree Views | Context Menu (Popup) |
| **Command Execution** | No (Read-only) | Yes (Async spawn) |
| **Customization** | Edit source code | Edit source code |
| **Best For** | Learning new commands, finding tools | Quick access for power users |

---

**Author**: zerowjh 
**Last Updated**: 2026-04-25  
**Version**: 1.0
