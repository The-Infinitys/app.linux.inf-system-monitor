from lib import manage
from lib import textual
from lib import textual_canvas
from textual_canvas import Canvas
import os, sys, shutil
from textual.binding import Binding
from textual.app import App, ComposeResult
from textual.color import Gradient, Color
from textual.widgets import Static, Header, Footer, ProgressBar
from textual.theme import Theme

def Color_HSV(h,s,v):
    h=h/360
    s=s/100
    v=v/100
    if s == 0.0: v*=255; return (v, v, v)
    i = int(h*6.) # XXX assume int() truncates!
    f = (h*6.)-i; p,q,t = int(255*(v*(1.-s))), int(255*(v*(1.-s*f))), int(255*(v*(1.-s*(1.-f)))); v*=255; i%=6
    if i == 0: return Color(v, t, p)
    if i == 1: return Color(q, v, p)
    if i == 2: return Color(p, v, t)
    if i == 3: return Color(p, q, v)
    if i == 4: return Color(t, p, v)
    if i == 5: return Color(v, p, q)


class InfinitySystemMonitor(App):
    CSS = """
    Screen {
      align: center top;
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
    """
    CSS+="""
    #cpu-core-usage {
      padding-left: 1;
      padding-right: 1;
      width: 1fr;
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
        yield Footer()
    def update(self) -> None:
        round_level = 0
        cpu_usage = int(10 ** round_level * manage.get_cpu_usage()) / 10 ** round_level
        mem_usage = int(10 ** round_level *manage.get_mem_usage()) / 10 ** round_level
        self.query_one("#cpu-usage").update(f"{cpu_usage}%")
        self.query_one("#mem-usage").update(f"{mem_usage}%")
        term_width = shutil.get_terminal_size().columns
        core_usage = manage.get_cpu_core_usages()
        core_usage_canvas=self.query_one("#cpu-core-usage")
        core_usage_canvas._width = term_width - 4
        core_usage_canvas.clear()
        for i in range(manage.CPU_CORES_COUNT):
            cpu_core_usage = int(10 ** round_level * core_usage[i]) / 10 ** round_level
            resized_usage=int(core_usage_canvas._width * cpu_core_usage/100)
            core_usage_canvas.draw_line(0,i*2,resized_usage,i*2,Color_HSV(360 * i / manage.CPU_CORES_COUNT,100,100))
    def on_mount(self) -> None:
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
        self.register_theme(infinite_theme)
        self.theme = "infinite"
        self.set_interval(1, self.update)
        
    def action_quit_app(self) -> None:
        self.exit(0)
    def action_nothing(self) -> None:
        pass



if __name__ == "__main__":  
    app = InfinitySystemMonitor()
    app.run()