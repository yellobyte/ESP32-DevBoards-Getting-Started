from os.path import expanduser
Import("env")
env.AddCustomTarget(
    name="uploadAndMonitorV3",
    dependencies=None,
    actions=[
        "pio run --target upload --environment v3", "pio run --target monitor --environment v3"
    ],
    title="Upload and Monitor V3",
    description="Upload and Monitor V3"
)
env.AddCustomTarget(
    name="envdump",
    dependencies=None,
    actions=[
        "pio run -t envdump",
    ],
    title="Show Build Options [envdump]",
    description="Show Build Options"
)
env.AddCustomTarget(
    name="prune",
    dependencies=None,
    actions=[
        "pio system prune -f",
    ],
    title="Prune System",
    description="Prune System"
)
cmd1 = "python.exe " + expanduser("~") + "/.platformio/packages/tool-esptoolpy/espefuse.py --port $UPLOAD_PORT summary"
env.AddCustomTarget(
    name="fuses-summary",
    dependencies=None,
    actions=[
        cmd1
    ],
    title="Fuses Summary",
    description="Fuses Summary"
)
cmd2 = "C:/Users/tj/.platformio/packages/toolchain-xtensa-esp32/bin/xtensa-esp32-elf-addr2line.exe -pfiaC -e"
env.AddCustomTarget(
    name="default",
    dependencies=None,
    actions=[
        cmd2 + " .pio/build/debug/firmware.elf 0x400556d2"
    ],
    title="dbg: addr2line",
    description="dbg: addr2line"
)
