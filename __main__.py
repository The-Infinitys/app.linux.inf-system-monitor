from lib import manage
from lib import textual
import os, sys
from textual.app import App, ComposeResult
from textual.widgets import Static, Header, Footer, Button

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
    Static {
    border: heavy white;
    }
    """
    def quit_sysmonitor(self) -> None:
        sys.exit(0)

    def compose(self) -> ComposeResult:
        # "yield Widgetのクラス"で、ターミナルにWidgetを表示する
        yield Header("The Infinity's System Monitor")
        yield Static("CPU USAGE", id="cpu-usage")
        yield Static("MEMORY USAGE", id="mem-usage")
        yield Footer()

if __name__ == "__main__":  
    app = InfinitySystemMonitor()
    app.run()