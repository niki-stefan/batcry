# batcry

A C tool that executes commands at specific battery levels. It is more for personal-use and learning purposes. It is very unsafe and unstable, with pretty bad code. Probably filled with bugs.

## How it works

It periodically reads the files `/sys/class/power_supply/BAT0/capacity`, which tells the current percentage of the battery, and `/sys/class/power_supply/AC0/online`, which tells if it is connected to AC. They are configurable tho.

## Building

```bash
git clone https://github.com/niki-stefan/batcry
cd batcry
chmod +x build.sh
./build.sh
```

The binary can be found at `bin/batcry`, move it to `/usr/bin` and then you can start it at startup, for example in niri: `spawn-at-startup "batcry"`.

## Config

The configuration is done in the file `~/.config/batcry/config` which can be changed by modifying the `#define CONFIG_PATH` in main.c, it needs to be the *absolute* path. The config syntax is very very constrained, to ease the parsing. The config contains at least 4 lines, in **this** order:
- The absolute path to the file containing the battery percentage (it must contain **just** that)
- The absolute path to the file containing the charging status (it must contain **just** that)
- The polling time in seconds
- The number of *levels* (so I can just pass that to `malloc`)
- ...the levels

Each level line must start with either `@`, which means execute when the battery has discharged to that amount, or `#`, which means execute when the battery has *charged* to that amount. Then, include the battery percentage at which to fire (0 < n < 100), then a `$`, then the command. Don't put any spaces. Everything after the `$` will be passed to `system()` from `unistd.h`, which will execute that command, which is very safe 🤗.

```
/sys/class/power_supply/BAT0/capacity
/sys/class/power_supply/AC0/online
60
5
#80$notify-send "Charging reached 80%"
@10$echo "I am gonna die" > ~/batdiary
@8$notify-send "hi"
#100$notify-send "Im full!"
@50$notify-send "Am i half-empty or half-full?"
```

The config file above tells batcry to poll every minute, and check for 5 levels.

## Resources

It consumes 1.76 MB (with 100 levels it consumes 1.89MB) of memory, with very little CPU usage (btop shows 0%).
