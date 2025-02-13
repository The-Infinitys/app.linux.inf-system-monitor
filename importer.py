import os,sys,shutil
import_dir=os.path.abspath(os.path.join(os.path.dirname(__file__), './lib'))
print(import_dir)
sys.path.append(import_dir)
import manage, textual, textual_canvas
from textual_canvas import Canvas
from textual.binding import Binding
from textual.app import App, ComposeResult
from textual.color import Gradient, Color
from textual.widgets import Static, Header, Footer, ProgressBar
from textual.theme import Theme
