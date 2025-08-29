#!/usr/bin/env python3
import os
import sys
import subprocess
import shlex
import json
from pathlib import Path

class AdvancedBuildScript:
    def __init__(self, config_file=None):
        self.default_config = {
            "compilers": {
                ".c": {
                    "name": "gcc",
                    "command": ["gcc", "-o", "{output}", "{input}", "-Wall", "-Wextra"],
                    "output_pattern": "{basename}.exe"
                },
                ".go": {
                    "name": "go",
                    "command": ["go", "build", "-o", "{output}", "{input}"],
                    "output_pattern": "{basename}"
                },
                ".rs": {
                    "name": "rustc",
                    "command": ["rustc", "-o", "{output}", "{input}"],
                    "output_pattern": "{basename}.exe"
                },
                ".cpp": {
                    "name": "g++",
                    "command": ["g++", "-o", "{output}", "{input}", "-Wall", "-Wextra"],
                    "output_pattern": "{basename}.exe"
                },
                ".py": {
                    "name": "python",
                    "command": ["python", "-m", "py_compile", "{input}"],
                    "output_pattern": "__pycache__/{basename}.cpython-*.pyc"
                },
                ".java": {
                    "name": "javac",
                    "command": ["javac", "{input}"],
                    "output_pattern": "{basename}.class"
                },
                ".js": {
                    "name": "node",
                    "command": ["node", "--check", "{input}"],
                    "output_pattern": ""
                }
            },
            "options": {
                "show_command": True,
                "color_output": True,
                "stop_on_error": True
            }
        }
        
        self.config = self.default_config
        self.load_config(config_file)
        
        # ANSI colors for pretty output
        self.COLORS = {
            'RED': '\033[91m',
            'GREEN': '\033[92m',
            'YELLOW': '\033[93m',
            'BLUE': '\033[94m',
            'MAGENTA': '\033[95m',
            'CYAN': '\033[96m',
            'WHITE': '\033[97m',
            'RESET': '\033[0m',
            'BOLD': '\033[1m'
        }

    def load_config(self, config_file):
        if config_file and os.path.exists(config_file):
            try:
                with open(config_file, 'r') as f:
                    user_config = json.load(f)
                    # Merge with default config
                    for key, value in user_config.items():
                        if key in self.config:
                            if isinstance(value, dict) and isinstance(self.config[key], dict):
                                self.config[key].update(value)
                            else:
                                self.config[key] = value
            except json.JSONDecodeError:
                print(f"Warning: Invalid config file {config_file}, using defaults")
            except Exception as e:
                print(f"Warning: Could not load config: {e}")

    def color_print(self, text, color=None):
        if self.config['options']['color_output'] and color:
            print(f"{self.COLORS.get(color, '')}{text}{self.COLORS['RESET']}")
        else:
            print(text)

    def expand_command(self, command_template, input_file, output_file):
        """Replace placeholders in command template"""
        expanded = []
        for part in command_template:
            part = part.replace('{input}', input_file)
            part = part.replace('{output}', output_file)
            part = part.replace('{basename}', os.path.splitext(input_file)[0])
            part = part.replace('{dirname}', os.path.dirname(input_file))
            expanded.append(part)
        return expanded

    def build_file(self, file_path):
        file_path = os.path.abspath(file_path)
        
        if not os.path.exists(file_path):
            self.color_print(f"Error: File '{file_path}' not found!", 'RED')
            return 1

        file_extension = os.path.splitext(file_path)[1].lower()
        basename = os.path.splitext(file_path)[0]

        if file_extension not in self.config['compilers']:
            self.color_print(f"Error: Unsupported file type '{file_extension}'", 'RED')
            supported = ', '.join(self.config['compilers'].keys())
            self.color_print(f"Supported extensions: {supported}", 'YELLOW')
            return 1

        compiler_config = self.config['compilers'][file_extension]
        compiler_name = compiler_config['name']
        
        # Determine output file
        output_pattern = compiler_config.get('output_pattern', '')
        output_file = output_pattern.format(
            basename=basename,
            input=file_path
        ) if output_pattern else ""

        # Expand command template
        command = self.expand_command(
            compiler_config['command'],
            file_path,
            output_file
        )

        self.color_print(f"Building: {file_path}", 'CYAN')
        self.color_print(f"File type: {file_extension}", 'BLUE')
        self.color_print(f"Using compiler: {compiler_name}", 'MAGENTA')
        
        if self.config['options']['show_command']:
            self.color_print(f"Command: {' '.join(shlex.quote(part) for part in command)}", 'YELLOW')
        print()

        try:
            result = subprocess.run(
                command,
                capture_output=True,
                text=True,
                check=False
            )

            if result.returncode == 0:
                self.color_print("✓ Compilation successful!", 'GREEN')
                if output_file and os.path.exists(output_file):
                    self.color_print(f"Output: {output_file}", 'GREEN')
                elif output_pattern and '*' in output_pattern:
                    # Handle patterns like __pycache__/*.pyc
                    pattern_dir = os.path.dirname(output_pattern)
                    if pattern_dir and os.path.exists(pattern_dir):
                        self.color_print(f"Output directory: {pattern_dir}", 'GREEN')
                return 0
            else:
                self.color_print("✗ Compilation failed!", 'RED')
                self.color_print(f"Error code: {result.returncode}", 'RED')
                print()
                self.color_print("=== ERROR OUTPUT ===", 'RED')
                
                if result.stderr:
                    print(result.stderr)
                elif result.stdout:
                    print(result.stdout)
                else:
                    self.color_print("(No error output)", 'YELLOW')
                    
                return result.returncode

        except FileNotFoundError:
            self.color_print(f"Error: Compiler '{compiler_name}' not found or not in PATH", 'RED')
            self.color_print("Please make sure the compiler is installed and available", 'YELLOW')
            return 1
        except Exception as e:
            self.color_print(f"Unexpected error: {e}", 'RED')
            return 1

    def run(self):
        if len(sys.argv) < 2:
            self.color_print("Error: No file provided", 'RED')
            self.color_print("Usage: python build.py <filename>", 'YELLOW')
            supported = ', '.join(self.config['compilers'].keys())
            self.color_print(f"Supported extensions: {supported}", 'YELLOW')
            sys.exit(1)

        file_path = sys.argv[1]
        exit_code = self.build_file(file_path)
        sys.exit(exit_code)

if __name__ == "__main__":
    # Look for config file in current directory or home directory
    config_files = ['.buildconfig.json', os.path.expanduser('~/.buildconfig.json')]
    config_file = None
    
    for cf in config_files:
        if os.path.exists(cf):
            config_file = cf
            break

    script = AdvancedBuildScript(config_file)
    script.run()