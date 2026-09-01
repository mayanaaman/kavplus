import subprocess
import tkinter as tk
from pathlib import Path
from tkinter import messagebox, ttk

from bus_module import (
    make_bus_line,
    sort_by_name,
    sort_by_distance,
    sort_by_duration,
    sort_by_frequency,
)


PROJECT_DIR = Path(__file__).resolve().parent


class BusSortingGui:
    def __init__(self, root):
        self.root = root
        self.root.title("Bus Sorting Lab")
        self.root.configure(bg="#f4f7f6")
        self.lines = []
        self.status_text = tk.StringVar(value="Ready. Load demo bus lines to begin.")

        self.configure_style()
        self.build_layout()
        self.write_status("Welcome to Bus Sorting Lab. Load demo bus lines to begin.")

    def configure_style(self):
        self.style = ttk.Style(self.root)
        self.style.theme_use("clam")

        self.style.configure("App.TFrame", background="#f4f7f6")
        self.style.configure("Card.TFrame", background="#ffffff", relief="flat")
        self.style.configure("Title.TLabel", background="#f4f7f6", foreground="#243b3b")
        self.style.configure("Subtitle.TLabel", background="#f4f7f6", foreground="#5f6f6f")
        self.style.configure("Section.TLabelframe", background="#f4f7f6", foreground="#243b3b")
        self.style.configure("Section.TLabelframe.Label", foreground="#243b3b")
        self.style.configure("Status.TLabel", background="#f4f7f6", foreground="#31525b")
        self.style.configure("TButton", padding=(10, 6))
        self.style.configure(
            "Treeview",
            rowheight=30,
            background="#ffffff",
            fieldbackground="#ffffff",
            foreground="#223030",
        )
        self.style.configure(
            "Treeview.Heading",
            background="#dbe8e6",
            foreground="#223030",
            font=("Segoe UI", 10, "bold"),
            padding=(8, 6),
        )
        self.style.map("Treeview", background=[("selected", "#b8d8d2")])

    def build_layout(self):
        main = ttk.Frame(self.root, style="App.TFrame", padding=18)
        main.pack(fill="both", expand=True)

        header = ttk.Frame(main, style="App.TFrame")
        header.pack(fill="x", pady=(0, 14))

        title = ttk.Label(
            header,
            text="Bus Sorting Lab",
            style="Title.TLabel",
            font=("Segoe UI", 24, "bold"),
        )
        title.pack(anchor="w")

        subtitle = ttk.Label(
            header,
            text="Explore the C-backed bus sorting functions, pointer visualizations, and tests.",
            style="Subtitle.TLabel",
            font=("Segoe UI", 10),
        )
        subtitle.pack(anchor="w", pady=(4, 0))

        table_frame = ttk.Frame(main, style="Card.TFrame", padding=10)
        table_frame.pack(fill="x", expand=False)

        self.table = ttk.Treeview(
            table_frame,
            columns=("name", "distance", "duration", "frequency"),
            show="headings",
            height=6,
        )
        self.table.heading("name", text="Name")
        self.table.heading("distance", text="Distance")
        self.table.heading("duration", text="Duration")
        self.table.heading("frequency", text="Frequency")

        self.table.column("name", width=220, minwidth=160, anchor="w")
        self.table.column("distance", width=140, minwidth=110, anchor="center")
        self.table.column("duration", width=140, minwidth=110, anchor="center")
        self.table.column("frequency", width=140, minwidth=110, anchor="center")

        table_scroll = ttk.Scrollbar(table_frame, orient="vertical", command=self.table.yview)
        self.table.configure(yscrollcommand=table_scroll.set)
        self.table.grid(row=0, column=0, sticky="nsew")
        table_scroll.grid(row=0, column=1, sticky="ns")
        table_frame.columnconfigure(0, weight=1)
        table_frame.rowconfigure(0, weight=1)

        controls = ttk.Frame(main, style="App.TFrame")
        controls.pack(fill="x", pady=14)

        demo_section = ttk.LabelFrame(controls, text="Demo", style="Section.TLabelframe", padding=10)
        sorting_section = ttk.LabelFrame(controls, text="Sorting", style="Section.TLabelframe", padding=10)
        tools_section = ttk.LabelFrame(controls, text="Tools", style="Section.TLabelframe", padding=10)

        demo_section.grid(row=0, column=0, padx=(0, 8), sticky="nsew")
        sorting_section.grid(row=0, column=1, padx=8, sticky="nsew")
        tools_section.grid(row=0, column=2, padx=(8, 0), sticky="nsew")

        controls.columnconfigure(0, weight=1)
        controls.columnconfigure(1, weight=2)
        controls.columnconfigure(2, weight=2)

        ttk.Button(
            demo_section,
            text="Load Demo Bus Lines",
            command=self.load_demo_lines,
        ).pack(fill="x", pady=3)

        sorting_buttons = [
            ("Sort by Name", self.sort_by_name),
            ("Sort by Distance", self.sort_by_distance),
            ("Sort by Duration", self.sort_by_duration),
            ("Sort by Frequency", self.sort_by_frequency),
        ]
        for index, (text, command) in enumerate(sorting_buttons):
            row = index // 2
            column = index % 2
            ttk.Button(sorting_section, text=text, command=command).grid(
                row=row,
                column=column,
                padx=3,
                pady=3,
                sticky="ew",
            )
        sorting_section.columnconfigure(0, weight=1)
        sorting_section.columnconfigure(1, weight=1)

        tools_buttons = [
            ("Bubble Visualization", self.run_bubble_visualization),
            ("Quick Visualization", self.run_quick_visualization),
            ("Run Pytest Tests", self.run_pytest_tests),
            ("Run Full Demo", self.run_full_demo),
            ("About This Project", self.show_about),
            ("Clear Output", self.clear_output),
        ]
        for index, (text, command) in enumerate(tools_buttons):
            row = index // 2
            column = index % 2
            ttk.Button(tools_section, text=text, command=command).grid(
                row=row,
                column=column,
                padx=3,
                pady=3,
                sticky="ew",
            )
        tools_section.columnconfigure(0, weight=1)
        tools_section.columnconfigure(1, weight=1)

        output_header = ttk.Frame(main, style="App.TFrame")
        output_header.pack(fill="x", pady=(0, 6))

        ttk.Label(
            output_header,
            text="Status / Output",
            style="Title.TLabel",
            font=("Segoe UI", 11, "bold"),
        ).pack(side="left")

        self.status_label = ttk.Label(
            output_header,
            textvariable=self.status_text,
            style="Status.TLabel",
            font=("Segoe UI", 9),
        )
        self.status_label.pack(side="right")

        output_frame = ttk.Frame(main, style="Card.TFrame", padding=8)
        output_frame.pack(fill="both", expand=True)

        self.output = tk.Text(
            output_frame,
            height=22,
            wrap="word",
            bg="#fbfcfc",
            fg="#1f2d2d",
            insertbackground="#1f2d2d",
            relief="flat",
            padx=8,
            pady=8,
        )
        output_scroll = ttk.Scrollbar(output_frame, orient="vertical", command=self.output.yview)
        self.output.configure(yscrollcommand=output_scroll.set)
        self.output.grid(row=0, column=0, sticky="nsew")
        output_scroll.grid(row=0, column=1, sticky="ns")
        output_frame.columnconfigure(0, weight=1)
        output_frame.rowconfigure(0, weight=1)

    def write_status(self, message):
        self.status_text.set(message.splitlines()[-1] if message else "Ready.")
        self.output.insert("end", message + "\n")
        self.output.see("end")
        self.root.update_idletasks()

    def clear_output(self):
        self.output.delete("1.0", "end")
        self.status_text.set("Output cleared.")

    def clear_table(self):
        for item in self.table.get_children():
            self.table.delete(item)

    def refresh_table(self):
        self.clear_table()
        for line in self.lines:
            self.table.insert(
                "",
                "end",
                values=(
                    self.decode_name(line),
                    line.distance,
                    line.duration,
                    line.frequency,
                ),
            )

    def load_demo_lines(self):
        self.lines = self.create_demo_lines()
        self.refresh_table()
        self.write_status("Loaded demo bus lines.")

    def create_demo_lines(self):
        return [
            make_bus_line("Dan", 40, 30, 5),
            make_bus_line("Alice", 10, 50, 3),
            make_bus_line("Charlie", 30, 20, 9),
            make_bus_line("Bob", 20, 40, 1),
        ]

    def describe_order(self):
        return ", ".join(
            f"{self.decode_name(line)} ({line.distance})" for line in self.lines
        )

    def run_full_demo(self):
        self.clear_output()
        self.write_status("Starting full demo...")

        self.lines = self.create_demo_lines()
        self.refresh_table()
        self.write_status("Loaded built-in demo bus lines.")
        self.write_status(f"Original order by distance: {self.describe_order()}")

        self.lines = sort_by_distance(self.lines)
        self.refresh_table()
        self.write_status(f"Sorted order by distance: {self.describe_order()}")
        self.write_status(
            "The sorting step used the C-backed Python binding."
        )
        self.write_status("Running pytest tests...")
        self.run_pytest_tests()
        self.write_status("Full demo completed.")

    def show_about(self):
        messagebox.showinfo(
            "About This Project",
            "Part A: Python ctypes binding to C.\n\n"
            "Part B: pointer and memory visualizations.\n\n"
            "Part C: interactive terminal lab and GUI.\n\n"
            "The original C project files were not modified.\n\n"
            "Sorting is performed by the original C logic through the Python binding.",
        )

    def sort_lines(self, sort_function, description):
        if not self.lines:
            self.write_status("No bus lines loaded. Click 'Load Demo Bus Lines' first.")
            return

        self.lines = sort_function(self.lines)
        self.refresh_table()
        self.write_status(description)

    def sort_by_name(self):
        self.sort_lines(sort_by_name, "Sorted by name using the C bubble sort binding.")

    def sort_by_distance(self):
        self.sort_lines(
            sort_by_distance,
            "Sorted by distance using the C quick sort binding.",
        )

    def sort_by_duration(self):
        self.sort_lines(
            sort_by_duration,
            "Sorted by duration using the C quick sort binding.",
        )

    def sort_by_frequency(self):
        self.sort_lines(
            sort_by_frequency,
            "Sorted by frequency using the C quick sort binding.",
        )

    def run_bubble_visualization(self):
        self.run_visualization(
            "memory_visualization",
            "gcc -Wall -Wextra -Wvla -std=c99 memory_visualization.c -o memory_visualization",
        )

    def run_quick_visualization(self):
        self.run_visualization(
            "quick_sort_visualization",
            "gcc -Wall -Wextra -Wvla -std=c99 quick_sort_visualization.c -o quick_sort_visualization",
        )

    def run_visualization(self, name, compile_command):
        executable = self.find_executable(name)
        if executable is None:
            self.write_status(f"Missing visualization executable: {name}")
            self.write_status("Compile it first with:")
            self.write_status(compile_command)
            return

        self.run_command([executable])

    def run_pytest_tests(self):
        self.run_command([
            "python3",
            "-m",
            "pytest",
            "fib_demo/test_fib.py",
            "test_bus_module.py",
        ])

    def run_command(self, command):
        self.write_status(f"Running: {' '.join(command)}")
        try:
            completed = subprocess.run(
                command,
                cwd=PROJECT_DIR,
                check=False,
                capture_output=True,
                text=True,
            )
        except FileNotFoundError:
            self.write_status(f"Could not run: {' '.join(command)}")
            self.write_status("The command was not found on this system.")
            return

        if completed.stdout:
            self.write_status(completed.stdout.rstrip())
        if completed.stderr:
            self.write_status(completed.stderr.rstrip())
        self.write_status(f"Command finished with exit code {completed.returncode}.")

    def find_executable(self, name):
        plain_path = PROJECT_DIR / name
        exe_path = PROJECT_DIR / f"{name}.exe"

        if plain_path.exists():
            return f"./{name}"
        if exe_path.exists():
            return f"./{name}.exe"
        return None

    @staticmethod
    def decode_name(line):
        return line.name.decode().rstrip("\x00")


def main():
    root = tk.Tk()
    root.geometry("960x820")
    root.minsize(860, 720)
    BusSortingGui(root)
    root.mainloop()


if __name__ == "__main__":
    main()
