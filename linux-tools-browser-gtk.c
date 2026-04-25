/*
 * linux-tools-browser-gtk.c
 *
 * A GTK 3 GUI that lists Linux system tools and software,
 * organised by category. Resizable, scrollable, with two columns.
 *
 * Compile:
 *   gcc -o linux-tools-browser-gtk linux-tools-browser-gtk.c $(pkg-config --cflags --libs gtk+-3.0)
 *
 * If that fails, try:
 *   gcc -o linux-tools-browser-gtk linux-tools-browser-gtk.c \
 *       -I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 \
 *       -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
 *       -I/usr/include/pango-1.0 -I/usr/include/cairo \
 *       -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0 \
 *       -lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 \
 *       -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 \
 *       -lgio-2.0 -lgobject-2.0 -lglib-2.0
 *
 * Run:
 *   ./linux-tools-browser-gtk
 *
 * Author : Assistant
 * License: Public Domain
 */

#include <gtk/gtk.h>
#include <stdlib.h>

/* ---------- Tool entry definition ---------- */
typedef struct {
    const char *name;
    const char *desc;
} ToolEntry;

/* ---------- All tool lists (9 categories) ---------- */
static const ToolEntry sysmon_tools[] = {
    { "top",      "Dynamic real-time process and resource monitor" },
    { "htop",     "Interactive process viewer (improved top)" },
    { "atop",     "Advanced system & process monitor with daily logging" },
    { "vmstat",   "Report virtual memory statistics" },
    { "iostat",   "CPU and I/O statistics for devices and partitions" },
    { "mpstat",   "Per-processor utilisation report" },
    { "free",     "Display amount of free and used memory" },
    { "uptime",   "Tell how long the system has been running" },
    { "dstat",    "Versatile replacement for vmstat/iostat/netstat" },
    { "glances",  "Cross-platform curses-based monitoring tool" },
    { "nmon",     "Nigel's performance monitor (CPU, memory, network, disk)" },
    { "sar",      "System Activity Reporter (collect & report stats)" },
    { "pidstat",  "Statistics per task (CPU, memory, I/O)" },
    { NULL, NULL }
};

static const ToolEntry sysadmin_tools[] = {
    { "systemctl", "Systemd service and unit manager" },
    { "journalctl","Query the systemd journal" },
    { "dmesg",     "Print kernel ring buffer messages" },
    { "lsmod",     "Show loaded kernel modules" },
    { "modprobe",  "Add/remove kernel modules" },
    { "fdisk",     "Manipulate disk partition table" },
    { "parted",    "GNU disk partition manipulator" },
    { "lsblk",     "List block devices" },
    { "blkid",     "Locate/print block device attributes" },
    { "mount",     "Mount a filesystem" },
    { "umount",    "Unmount a filesystem" },
    { "df",        "Report filesystem disk space usage" },
    { "du",        "Estimate file space usage" },
    { "lscpu",     "Display CPU architecture information" },
    { "lsusb",     "List USB devices" },
    { "lspci",     "List PCI devices" },
    { "sensors",   "Read hardware sensor data (lm-sensors)" },
    { "uname",     "Print system information" },
    { "hostnamectl","Query and change system hostname" },
    { "timedatectl","Control system time and date" },
    { "useradd",   "Create a new user" },
    { "passwd",    "Update user password" },
    { "kill",      "Send a signal to a process" },
    { "nice",      "Run a program with modified scheduling priority" },
    { "renice",    "Alter priority of a running process" },
    { NULL, NULL }
};

static const ToolEntry net_tools[] = {
    { "ping",      "Test reachability of a network host" },
    { "traceroute","Print the route packets trace to network host" },
    { "mtr",       "Network diagnostic tool combining ping & traceroute" },
    { "ip",        "Show / manipulate routing, devices, policy routing" },
    { "ifconfig",  "Configure network interfaces (legacy)" },
    { "iwconfig",  "Configure wireless network interfaces" },
    { "nmcli",     "Command-line client for NetworkManager" },
    { "nmtui",     "Text user interface for NetworkManager" },
    { "netstat",   "Print network connections, routing tables, etc." },
    { "ss",        "Investigate sockets (replacement for netstat)" },
    { "curl",      "Transfer data from or to a server (many protocols)" },
    { "wget",      "Non-interactive network downloader" },
    { "ssh",       "OpenSSH remote login client" },
    { "scp",       "Secure copy (remote file copy program)" },
    { "rsync",     "Fast, versatile file copying tool (local & remote)" },
    { "nslookup",  "Query Internet name servers interactively" },
    { "dig",       "DNS lookup utility" },
    { "tcpdump",   "Dump traffic on a network" },
    { "wireshark", "GUI network protocol analyser (CLI: tshark)" },
    { "nmap",      "Network exploration tool and security scanner" },
    { "iptables",  "Administration tool for IPv4 packet filtering" },
    { "firewalld", "Dynamic firewall manager" },
    { NULL, NULL }
};

static const ToolEntry file_tools[] = {
    { "ls",        "List directory contents" },
    { "cd",        "Change the working directory" },
    { "pwd",       "Print current working directory" },
    { "cp",        "Copy files and directories" },
    { "mv",        "Move (rename) files" },
    { "rm",        "Remove files or directories" },
    { "mkdir",     "Create directories" },
    { "rmdir",     "Remove empty directories" },
    { "touch",     "Change file timestamps / create empty file" },
    { "chmod",     "Change file mode bits" },
    { "chown",     "Change file owner and group" },
    { "ln",        "Create links (symbolic or hard)" },
    { "stat",      "Display file or filesystem status" },
    { "find",      "Search for files in a directory hierarchy" },
    { "locate",    "Find files by name (uses mlocate DB)" },
    { "tar",       "Tape archiver (create/extract tarballs)" },
    { "gzip",      "Compress or expand files (GNU zip)" },
    { "bzip2",     "Block-sorting file compressor" },
    { "xz",        "High-ratio LZMA compressor" },
    { "zip",       "Package and compress files" },
    { "unzip",     "List, test and extract compressed ZIP archives" },
    { "mc",        "Midnight Commander - visual file manager" },
    { "ranger",    "Console file manager with VI key bindings" },
    { "Nautilus",  "GNOME default file manager (GUI)" },
    { "Dolphin",   "KDE file manager (GUI)" },
    { "Thunar",    "XFCE file manager (GUI)" },
    { "PCManFM",   "LXDE lightweight file manager (GUI)" },
    { NULL, NULL }
};

static const ToolEntry editor_tools[] = {
    { "vim",       "Vi IMproved - highly configurable text editor" },
    { "neovim",    "Hyperextensible Vim-based text editor" },
    { "emacs",     "GNU Emacs - extensible, customisable editor" },
    { "nano",      "Simple curses-based text editor" },
    { "gedit",     "GNOME text editor (GUI)" },
    { "Kate",      "KDE advanced text editor (GUI)" },
    { "mousepad",  "Simple text editor for Xfce (GUI)" },
    { "leafpad",   "Notepad-like simple text editor (GUI)" },
    { "Sublime Text", "Sophisticated text editor (GUI, proprietary)" },
    { "VS Code",   "Microsoft's code editor (GUI)" },
    { "joe",       "Joe's Own Editor - WordStar-like console editor" },
    { "micro",     "Modern and intuitive terminal-based editor" },
    { NULL, NULL }
};

static const ToolEntry dev_tools[] = {
    { "gcc",       "GNU C compiler" },
    { "g++",       "GNU C++ compiler" },
    { "make",      "Build automation tool" },
    { "cmake",     "Cross-platform build system generator" },
    { "gdb",       "GNU Debugger" },
    { "valgrind",  "Memory debugging, profiling and analysis" },
    { "strace",    "Trace system calls and signals" },
    { "ltrace",    "Library call tracer" },
    { "git",       "Distributed version control system" },
    { "diff",      "Compare files line by line" },
    { "patch",     "Apply a diff file to an original" },
    { "ctags",     "Generate index (tag) files for source code" },
    { "cscope",    "Source code browsing tool" },
    { "bear",      "Generate compilation database for clang tools" },
    { "perf",      "Linux profiling with performance counters" },
    { "objdump",   "Display information from object files" },
    { "nm",        "List symbols from object files" },
    { "ldd",       "Print shared library dependencies" },
    { "pkg-config","Helper tool for compiling with libraries" },
    { "autoconf",  "Generate configuration scripts" },
    { "automake",  "Automatically generate Makefile.in files" },
    { NULL, NULL }
};

static const ToolEntry media_tools[] = {
    { "ffmpeg",    "Multimedia framework for encoding/decoding/streaming" },
    { "vlc",       "VLC media player (GUI & CLI: cvlc)" },
    { "mpv",       "Command-line video player based on MPlayer/mplayer2" },
    { "gimp",      "GNU Image Manipulation Program (GUI)" },
    { "imagemagick","Image manipulation suite (convert, identify, mogrify)" },
    { "inkscape",  "Vector graphics editor (GUI)" },
    { "audacity",  "Audio editor and recorder (GUI)" },
    { "sox",       "Sound eXchange - audio processing tool" },
    { "pavucontrol","PulseAudio volume control (GUI)" },
    { "alsamixer", "ALSA sound card mixer (ncurses)" },
    { "youtube-dl / yt-dlp", "Download videos from websites" },
    { "obs-studio","Open Broadcaster Software - streaming/recording" },
    { "handbrake", "Video transcoder (GUI & CLI)" },
    { NULL, NULL }
};

static const ToolEntry util_tools[] = {
    { "grep",      "Print lines matching a pattern" },
    { "sed",       "Stream editor for filtering and transforming text" },
    { "awk",       "Pattern scanning and processing language" },
    { "sort",      "Sort lines of text files" },
    { "uniq",      "Report or omit repeated lines" },
    { "wc",        "Word, line, character, and byte count" },
    { "cut",       "Remove sections from each line of files" },
    { "head",      "Output the first part of files" },
    { "tail",      "Output the last part of files" },
    { "cat",       "Concatenate files and print on standard output" },
    { "tac",       "Concatenate and print files in reverse" },
    { "less",      "Opposite of more - file pager" },
    { "more",      "File perusal filter for crt viewing" },
    { "tee",       "Read from stdin and write to stdout and files" },
    { "xargs",     "Build and execute command lines from standard input" },
    { "watch",     "Execute a program periodically, showing output" },
    { "time",      "Time a simple command or give resource usage" },
    { "date",      "Display or set the system date and time" },
    { "cal",       "Display a calendar" },
    { "which",     "Locate a command" },
    { "whereis",   "Locate binary, source, and manual page files" },
    { "whatis",    "Display one-line manual page descriptions" },
    { NULL, NULL }
};

static const ToolEntry office_tools[] = {
    { "LibreOffice Writer", "Word processor" },
    { "LibreOffice Calc",   "Spreadsheet" },
    { "LibreOffice Impress","Presentation" },
    { "evince",    "GNOME document viewer (PDF, PS, etc.)" },
    { "okular",    "KDE universal document viewer" },
    { "qpdfview",  "Lightweight PDF viewer" },
    { "Zathura",   "Minimalistic document viewer (vim-like keys)" },
    { "calibre",   "E-book management and conversion" },
    { "scribus",   "Desktop publishing" },
    { NULL, NULL }
};

/* ---------- GUI construction ---------- */
static GtkWidget *
create_category_view(const ToolEntry *tools)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GtkWidget *tree_view, *scrolled;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *col;

    store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);

    for (const ToolEntry *t = tools; t->name != NULL; ++t) {
        gtk_list_store_append(store, &iter);
        gtk_list_store_set(store, &iter,
                           0, t->name,
                           1, t->desc,
                           -1);
    }

    tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
    g_object_unref(store);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Tool", renderer,
                                                   "text", 0, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_min_width(col, 170);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col);

    renderer = gtk_cell_renderer_text_new();
    col = gtk_tree_view_column_new_with_attributes("Description", renderer,
                                                   "text", 1, NULL);
    gtk_tree_view_column_set_resizable(col, TRUE);
    gtk_tree_view_column_set_min_width(col, 420);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), col);

    scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scrolled), tree_view);

    return scrolled;
}

int main(int argc, char *argv[])
{
    GtkWidget *window, *notebook, *label, *view;

    gtk_init(&argc, &argv);

    /* Main window */
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window),
                         "Linux System Tools & Software Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 550);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

#define ADD_TAB(notebook, title, tools_array)                           \
    do {                                                                \
        label = gtk_label_new(title);                                   \
        view  = create_category_view(tools_array);                      \
        gtk_notebook_append_page(GTK_NOTEBOOK(notebook), view, label);  \
    } while(0)

    ADD_TAB(notebook, "System Monitoring",   sysmon_tools);
    ADD_TAB(notebook, "System Administration", sysadmin_tools);
    ADD_TAB(notebook, "Networking",          net_tools);
    ADD_TAB(notebook, "File Management",     file_tools);
    ADD_TAB(notebook, "Text Editors",        editor_tools);
    ADD_TAB(notebook, "Development",         dev_tools);
    ADD_TAB(notebook, "Multimedia",          media_tools);
    ADD_TAB(notebook, "Utilities",           util_tools);
    ADD_TAB(notebook, "Office / Productivity", office_tools);

#undef ADD_TAB

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
