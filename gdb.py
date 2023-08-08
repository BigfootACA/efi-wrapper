import gdb
from pathlib import Path

class GdbRunBreakpoint(gdb.Breakpoint):
	def __init__(self):
		gdb.Breakpoint.__init__(self, "call_debugger_load_symbol", internal=1)
		self.silent = True

	def get_debug(self, path):
		if path.name == "vmlinuz-linux":
			file = Path("/usr/src/linux/vmlinux")
			if p.exists(): return file
		if path.suffix == ".efi":
			p = path.with_suffix(".debug")
			if p.exists(): return p
			p = path.with_suffix(".so")
			if p.exists(): return p
		return None

	def stop(self):
		path = Path(gdb.parse_and_eval("path").string())
		addr = gdb.parse_and_eval("addr")
		file = self.get_debug(path)
		if file is None:
			print("Skip loading symbol table")
			return False
		print(f"Add symbol table {file} for {addr}")
		gdb.execute(f"add-symbol-file {file} -o {addr}")
		return False


GdbRunBreakpoint()
