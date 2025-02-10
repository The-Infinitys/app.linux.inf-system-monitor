from lib import manage
from lib import textual
import os, sys
from textual.app import App, ComposeResult
from textual.color import Gradient
from textual.widgets import Static, ProgressBar, Header, Footer, Button

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
    """

    def compose(self) -> ComposeResult:
        rainbow_gradient = Gradient.from_colors("#ff0000","#ffff00","#00ff00","#00ffff","#0000ff","#ff00ff")
        yield Header("The Infinity's System Monitor")
        yield Static("CPU USAGE")
        yield Static("CPU USAGE", id="cpu-usage")
        yield ProgressBar(total=100,gradient=rainbow_gradient,id="cpu-usage-bar")
        yield Static("MEMORY USAGE")
        yield Static("MEMORY USAGE", id="mem-usage")
        yield Footer()
    def update(self) -> None:
        cpu_usage = manage.get_cpu_usage()
        mem_usage = manage.get_mem_usage()
        self.query_one("#cpu-usage").update(f"{cpu_usage}%")
        self.query_one("#mem-usage").update(f"{mem_usage}%")
        self.query_one("#cpu-usage-bar").update(progress=int(manage.get_cpu_usage()))
    def on_mount(self) -> None:
        self.set_interval(1, self.update)



if __name__ == "__main__":  
    app = InfinitySystemMonitor()
    app.run()