from os.path import expanduser
Import("env")
env.AddCustomTarget(
    name="envdump",
    dependencies=None,
    actions=[
        "pio run -t envdump",
    ],
    title="Show Build Options [envdump]",
    description="Show Build Options "
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
cmd1 = "python.exe " + expanduser("~") + "\.platformio\packages\\tool-esptoolpy\espefuse.py --port $UPLOAD_PORT summary"
env.AddCustomTarget(
    name="fuses-summary",
    dependencies=None,
    actions=[
        cmd1
    ],
    title="Fuses Summary",
    description="Fuses Summary"
)

