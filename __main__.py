from lib import manage
from lib import textual
import os, sys, shutil
from textual.binding import Binding
from textual.app import App, ComposeResult
# from textual.color import Gradient
from textual.widgets import Static, Header, Footer, ProgressBar
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
    ENABLE_COMMAND_PALETTE=False
    BINDINGS = [
      ("q", "quit_app()", "Quit the application")
      ]
    def compose(self) -> ComposeResult:
        yield Header("The Infinity's System Monitor")
        yield Static("CPU USAGE", classes="item-title")
        yield Static("CPU USAGE", id="cpu-usage")
        for i in range(manage.CPU_CORES_COUNT):
            yield Static(f"CORE {i}", id=f"cpu-{i}-usage" , classes="core-usage")
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
        for i in range(manage.CPU_CORES_COUNT):
            cpu_core_usage = int(10 ** round_level * core_usage[i]) / 10 ** round_level
            rounded_cpu_core_usage = int((term_width - 10) * cpu_core_usage / 100)
            usage_bar = f"[{'#' * rounded_cpu_core_usage}{'-' * (term_width - 10 - rounded_cpu_core_usage)}]"
            os.system(f"echo {usage_bar} >> log.txt")
            self.query_one(f"#cpu-{i}-usage").update(usage_bar)
    def on_mount(self) -> None:
        self.set_interval(1, self.update)
    def action_quit_app(self) -> None:
        self.exit(0)
    def action_nothing(self) -> None:
        pass



if __name__ == "__main__":  
    app = InfinitySystemMonitor()
    app.run()