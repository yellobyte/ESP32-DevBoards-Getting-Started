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
env.AddCustomTarget(
    name="fuses-summary",
    dependencies=None,
    actions=[
        "python.exe c:/Users/tj/.platformio/packages/tool-esptoolpy/espefuse.py --port COM4 summary",
    ],
    title="Fuses Summary",
    description="Fuses Summary"
)
