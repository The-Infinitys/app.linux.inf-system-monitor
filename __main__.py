from importer import *

def Color_HSV(h,s,v):
    h=h/360
    s=s/100
    v=v/100
    if s == 0.0: v*=255; return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = int(255*(v*(1.-s))), int(255*(v*(1.-s*f))), int(255*(v*(1.-s*(1.-f)))); v*=255; i%=6
    p=int(p)
    q=int(q)
    v=int(v)
    t=int(t)
    if i == 0: return Color(v, t, p)
    if i == 1: return Color(q, v, p)
    if i == 2: return Color(p, v, t)
    if i == 3: return Color(p, q, v)
    if i == 4: return Color(t, p, v)
    if i == 5: return Color(v, p, q)

mem_usage_data = []
swap_usage_data = []

class InfinitySystemMonitor(App):
    CSS = """
    Screen {
      align: center top;
    }
    #cpu-usage, #mem-usage{
      text-align: right;
    }
    #cpu-usage {
      color: red;
    }
    #mem-usage{
      color: yellow;
    }
    Static.item-title {
      border: heavy white;
    }
    ProgressBar.core-usage {
      padding-left: 1;
    }
    #mem-swap-usage {
      padding-left: 1;
      padding-right: 1;
      overflow: hidden;
    }
    """
    CSS+="""
    #cpu-core-usage {
      padding-left: 1;
      padding-right: 1;
      overflow: hidden;
      height: """+str(manage.CPU_CORES_COUNT + 1)+""";  
    }"""
    # ENABLE_COMMAND_PALETTE=False
    BINDINGS = [
      ("q", "quit_app()", "Quit the application")
      ]
    def compose(self) -> ComposeResult:
        yield Header("The Infinity's System Monitor")
        yield Static("CPU USAGE", classes="item-title")
        yield Static("CPU USAGE", id="cpu-usage")
        yield Canvas(shutil.get_terminal_size().columns - 4, 2 * manage.CPU_CORES_COUNT, Color(0, 0, 0),id="cpu-core-usage")
        yield Static("MEMORY USAGE",classes="item-title")
        yield Static("MEMORY USAGE", id="mem-usage")
        yield Canvas(shutil.get_terminal_size().columns - 4, 20, Color(0, 0, 0),id="mem-swap-usage")
        yield Footer()
    def update(self) -> None:
        # Get the machine info
        machine_info = manage.info()
        # Calculate the CPU and memory usage
        term_width = shutil.get_terminal_size().columns
        round_level = 0
        cpu_usage = int(10 ** round_level * (100 - machine_info.cpu.idle)) / 10 ** round_level
        mem_used = int(10 ** round_level * 100 * (machine_info.memory.used/machine_info.memory.total)) / 10 ** round_level
        mem_buffcache = int(10 ** round_level * 100 * (machine_info.memory.buff_cache/machine_info.memory.total)) / 10 ** round_level
        # Calculate the swap usage
        swap_used=0
        swap_buffcache=0
        if machine_info.swap.total != 0:
          swap_used = int(10 ** round_level * 100 * (machine_info.swap.used/machine_info.swap.total)) / 10 ** round_level
          swap_buffcache = int(10 ** round_level * 100 * (machine_info.swap.buff_cache/machine_info.swap.total)) / 10 ** round_level
        # Update the UI
        self.query_one("#cpu-usage").update(f"{cpu_usage}%")
        self.query_one("#mem-usage").update(f"{mem_used}% (buff/cache: {mem_buffcache}%)")
        # Update the CPU core usage canvas
        core_usage = manage.get_cpu_core_usages()
        core_usage_canvas=self.query_one("#cpu-core-usage")
        core_usage_canvas._width = term_width - 4
        core_usage_canvas.clear()
        for i in range(manage.CPU_CORES_COUNT):
            cpu_core_usage = int(10 ** round_level * core_usage[i]) / 10 ** round_level
            resized_usage=int(core_usage_canvas._width * cpu_core_usage/100)
            core_usage_canvas.draw_line(0,i*2,resized_usage,i*2,Color_HSV(360 * i / manage.CPU_CORES_COUNT,100,100))
        # Update the memory usage canvas
        mem_usage_canvas=self.query_one("#mem-swap-usage")
        mem_usage_canvas._width = term_width - 4
        mem_usage_canvas.clear()
        # Update the memory usage data
        mem_usage_data.insert(0,[mem_used,mem_buffcache])
        swap_usage_data.insert(0,[swap_used,swap_buffcache])
        # Remove the old data
        if len(mem_usage_data) > mem_usage_canvas._width:
          for i in range(len(mem_usage_data) - mem_usage_canvas._width):
            mem_usage_data.pop()
            swap_usage_data.pop()
        for i in range(min(mem_usage_canvas._width, len(mem_usage_data))):
            # Draw the memory usage data
            canv_width = mem_usage_canvas._width
            canv_height = mem_usage_canvas._height
            draw_x = canv_width - i
            draw_y_start = canv_height
            draw_y_end = int(canv_height * (100 - mem_usage_data[i][0]) / 100)
            mem_usage_canvas.draw_line(draw_x, draw_y_start, draw_x, draw_y_end, Color_HSV(60,100,100))
            draw_y_start = draw_y_end
            draw_y_end -= int(canv_height * (mem_usage_data[i][1]) / 100)
            mem_usage_canvas.draw_line(draw_x, draw_y_start, draw_x, draw_y_end, Color_HSV(60,50,100))
            if machine_info.swap.total != 0:
                draw_y_start = canv_height
                draw_y_end = int(canv_height * (100 - swap_usage_data[i][0]) / 100)
                mem_usage_canvas.draw_line(draw_x, draw_y_start, draw_x, draw_y_end, Color_HSV(0,100,100))
                draw_y_start = draw_y_end
                draw_y_end -= int(canv_height * (swap_usage_data[i][1]) / 100)
                mem_usage_canvas.draw_line(draw_x, draw_y_start, draw_x, draw_y_end, Color_HSV(0,50,100))
    def on_mount(self) -> None:
        # Initialize the theme
        infinite_theme = Theme(
          name="infinite",
          primary="#999999",
          secondary="#3333FF",
          accent="#11FFFF",
          foreground="#EEEEEE",
          background="#111111",
          success="#00FFFF",
          warning="#FFFF00",
          error="#FF0000",
          surface="#000000",
          panel="#333333",
          dark=True,
          variables={
              "block-cursor-text-style": "none",
              "footer-key-foreground": "#00ffff",
          },
        )
        # Register the theme
        self.register_theme(infinite_theme)
        self.theme = "infinite"
        self.set_interval(0.25, self.update)
        
    def action_quit_app(self) -> None:
        self.exit(0)
    def action_nothing(self) -> None:
        pass


# Run the application
if __name__ == "__main__":  
    app = InfinitySystemMonitor()
    app.run()