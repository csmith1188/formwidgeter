import tkinter as tk
from tkinter import ttk, scrolledtext
import serial.tools.list_ports
import serial
import threading
import time

class SerialApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial Widget Controller")
        
        self.serial_port = None
        self.is_connected = False
        
        # Create main frame
        self.main_frame = ttk.Frame(root, padding="10")
        self.main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Port selection frame
        self.port_frame = ttk.LabelFrame(self.main_frame, text="Serial Port Settings", padding="5")
        self.port_frame.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        # Port dropdown
        ttk.Label(self.port_frame, text="Port:").grid(row=0, column=0, padx=5)
        self.port_var = tk.StringVar()
        self.port_combo = ttk.Combobox(self.port_frame, textvariable=self.port_var)
        self.port_combo.grid(row=0, column=1, padx=5)
        
        # Baud rate dropdown
        ttk.Label(self.port_frame, text="Baud Rate:").grid(row=0, column=2, padx=5)
        self.baud_var = tk.StringVar(value="115200")
        self.baud_combo = ttk.Combobox(self.port_frame, textvariable=self.baud_var)
        self.baud_combo['values'] = ('9600', '19200', '38400', '57600', '115200')
        self.baud_combo.grid(row=0, column=3, padx=5)
        
        # Connect button
        self.connect_btn = ttk.Button(self.port_frame, text="Connect", command=self.toggle_connection)
        self.connect_btn.grid(row=0, column=4, padx=5)
        
        # Refresh button
        self.refresh_btn = ttk.Button(self.port_frame, text="Refresh", command=self.refresh_ports)
        self.refresh_btn.grid(row=0, column=5, padx=5)
        
        # Command frame
        self.cmd_frame = ttk.LabelFrame(self.main_frame, text="Commands", padding="5")
        self.cmd_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        # Command buttons
        commands = [
            ("Set SSID", "setssid"),
            ("Set Password", "setpass"),
            ("Set Server", "setserver"),
            ("Set API Key", "setapikey"),
            ("Set Room Code", "setroomcode"),
            ("Read Values", "read"),
            ("Help", "help")
        ]
        
        for i, (label, cmd) in enumerate(commands):
            btn = ttk.Button(self.cmd_frame, text=label, 
                           command=lambda c=cmd: self.send_command(c))
            btn.grid(row=i//3, column=i%3, padx=5, pady=2)
        
        # Value entry
        self.value_frame = ttk.Frame(self.main_frame)
        self.value_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=5)
        
        ttk.Label(self.value_frame, text="Value:").grid(row=0, column=0, padx=5)
        self.value_var = tk.StringVar()
        self.value_entry = ttk.Entry(self.value_frame, textvariable=self.value_var)
        self.value_entry.grid(row=0, column=1, sticky=(tk.W, tk.E), padx=5)
        
        # Console output
        self.console = scrolledtext.ScrolledText(self.main_frame, height=15)
        self.console.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=5)
        
        # Configure grid weights
        self.main_frame.columnconfigure(0, weight=1)
        self.main_frame.rowconfigure(3, weight=1)
        
        # Initial port refresh
        self.refresh_ports()
        
        # Start reading thread
        self.read_thread = None
        self.stop_thread = False
        
        # Fit window to contents
        self.root.update_idletasks()
        width = self.main_frame.winfo_reqwidth()
        height = self.main_frame.winfo_reqheight()
        self.root.geometry(f"{width}x{height}")
        
        # Disable resizing
        self.root.resizable(False, False)

    def refresh_ports(self):
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_combo['values'] = ports
        if ports:
            self.port_combo.set(ports[0])

    def toggle_connection(self):
        if not self.is_connected:
            try:
                self.serial_port = serial.Serial(
                    port=self.port_var.get(),
                    baudrate=int(self.baud_var.get()),
                    timeout=1
                )
                self.is_connected = True
                self.connect_btn.configure(text="Disconnect")
                self.console.insert(tk.END, "Connected to " + self.port_var.get() + "\n")
                
                # Start reading thread
                self.stop_thread = False
                self.read_thread = threading.Thread(target=self.read_serial)
                self.read_thread.daemon = True
                self.read_thread.start()
                
            except Exception as e:
                self.console.insert(tk.END, f"Error connecting: {str(e)}\n")
        else:
            self.stop_thread = True
            if self.serial_port:
                self.serial_port.close()
            self.is_connected = False
            self.connect_btn.configure(text="Connect")
            self.console.insert(tk.END, "Disconnected\n")

    def read_serial(self):
        while not self.stop_thread:
            if self.serial_port and self.serial_port.is_open:
                try:
                    if self.serial_port.in_waiting:
                        data = self.serial_port.readline().decode('utf-8').strip()
                        self.console.insert(tk.END, data + "\n")
                        self.console.see(tk.END)
                except Exception as e:
                    self.console.insert(tk.END, f"Error reading: {str(e)}\n")
            time.sleep(0.1)

    def send_command(self, command):
        if not self.is_connected:
            self.console.insert(tk.END, "Not connected to any device\n")
            return
            
        if command in ['read', 'help']:
            cmd = command
        else:
            value = self.value_var.get()
            if not value:
                self.console.insert(tk.END, "Please enter a value\n")
                return
            cmd = f"{command} {value}"
            
        try:
            self.serial_port.write((cmd + '\n').encode('utf-8'))
            self.value_var.set('')  # Clear the value entry
        except Exception as e:
            self.console.insert(tk.END, f"Error sending command: {str(e)}\n")

if __name__ == "__main__":
    root = tk.Tk()
    app = SerialApp(root)
    root.mainloop()
