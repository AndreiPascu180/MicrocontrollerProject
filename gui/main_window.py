from PySide6.QtWidgets import QMainWindow, QHBoxLayout, QVBoxLayout, QWidget, QGroupBox, QLabel, QPushButton, QLineEdit, QTextEdit
from PySide6.QtGui import QIcon, QPalette, QColor, QFont
from PySide6.QtCore import Qt,QTimer
import pyqtgraph as pg
import serial
import numpy as np

class MainWindow(QMainWindow):
    promotie: str = "2023-2024"
    team: list[str] = [
        "ISTRATE STEFAN",
        "PASCU ANDREI",
    ]
   

    def __init__(self):
        super().__init__()
        self.setWindowTitle(f"Proiect Microprocesoare {self.promotie}")
        self.setWindowIcon(QIcon("./icon.png"))
        self.low_medium = 0.2
        self.medium_high = 0.4
        self.ser = serial.Serial()
        self.ser.baudrate = 38400
        self.sens=0
        primary_layout = QVBoxLayout()
        secondary_layout = QHBoxLayout()
        tertiary_layout = QVBoxLayout()

        team_box = QGroupBox("Membrii echipei")
        bold_font = QFont()
        bold_font.setBold(True)
        team_box.setFont(bold_font)

        first_member = QLabel(f"Membru 1: {self.team[0]}")
        second_member = QLabel(f"Membru 2: {self.team[1]}")
        team_box_layout = QVBoxLayout()
        team_box_layout.addWidget(first_member,1)
        team_box_layout.addWidget(second_member,1)
        team_box.setLayout(team_box_layout)

        control_panel_box = QGroupBox("Control Panel")
        control_panel_box.setFont(bold_font)

        button1 = QPushButton("A-M-G-N")
        button2 = QPushButton("N-G-M-A")
        button3 = QPushButton("Send")
        button1.clicked.connect(lambda: self.update_sens(0))
        button2.clicked.connect(lambda: self.update_sens(1))
        button3.clicked.connect(self.send_input)

        self.line_edit = QLineEdit()
        self.line_edit.setAlignment(Qt.AlignmentFlag.AlignBottom)
        line_edit_label = QLabel("Port COM#:", parent=self.line_edit)
        control_panel_box_layout = QVBoxLayout()
        control_panel_box_layout.setSpacing(5)
        control_panel_box_layout.addWidget(button1,1)
        control_panel_box_layout.addWidget(button2,1)

        control_panel_box_layout.addStretch()
        control_panel_box_layout.addWidget(line_edit_label)
        control_panel_box_layout.addWidget(self.line_edit, 1)
        control_panel_box_layout.addWidget(button3,1)

        control_panel_box.setLayout(control_panel_box_layout)

        tertiary_layout.addWidget(team_box, 1)
        tertiary_layout.addWidget(control_panel_box,5)

        self.plot_widget = pg.PlotWidget()
        self.time = [0,0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25]
        self.voltage=[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9]
        self.plot_widget.getPlotItem().setRange(yRange=[0, 1])
        self.plot_widget.setBackground("w")
        self.plot_widget.setLabel('left','Voltage')
        self.plot_widget.setLabel('bottom','Time')

        
        self.color_gradient = pg.ColorMap(pos=np.array([0, self.low_medium, self.medium_high]), color=[(0, 255, 0), (255, 255, 0), (255, 0, 0)])
        self.colors = self.color_gradient.map(self.voltage)

        self.bar_item = pg.BarGraphItem(x=self.time, height=self.voltage, width=0.2, brushes=[QColor(*c) for c in self.colors])
        self.plot_widget.addItem(self.bar_item)

        secondary_layout.addWidget(self.plot_widget, 3)
        secondary_layout.addLayout(tertiary_layout, 1)

        primary_layout.addLayout(secondary_layout, 4)
        self.text_edit = QTextEdit()
        self.text_edit.setReadOnly(True)

        debug_box = QGroupBox("Debug")
        debug_box_layout = QVBoxLayout()
        debug_box_layout.addWidget(self.text_edit, 1)
        debug_box.setLayout(debug_box_layout)

        primary_layout.addWidget(debug_box, 1)

        widget = QWidget()
        widget.setLayout(primary_layout)
        
        self.setCentralWidget(widget)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_value)
        self.timer.start(100)

    def update_sens(self, new_sens):
        if self.ser.is_open:
            self.sens = new_sens
            self.text_edit.insertPlainText(f"Sense updated to: {new_sens}\n")
            self.ser.write(bytes([new_sens]))


    def update_value(self):
        if self.ser.is_open:
            received_data = self.ser.read(4)
            self.ser.reset_input_buffer()
            decoded_data = received_data.decode('utf-8')
            sensor_value=float(decoded_data)
            if(sensor_value > 1):
                return
            self.text_edit.insertPlainText(f"Received value : {sensor_value}\n")
            self.time=self.time[1:]
            self.time.append(self.time[-1]+0.25)

            self.voltage=self.voltage[1:]
            self.voltage.append(sensor_value)

            self.colors = self.color_gradient.map(self.voltage)
            if hasattr(self, 'bar_item'):
                self.plot_widget.removeItem(self.bar_item)
            self.bar_item = pg.BarGraphItem(x=self.time, height=self.voltage, width=0.2, brushes=[QColor(*c) for c in self.colors])
            self.plot_widget.addItem(self.bar_item)

    def send_input(self):
        com_port = self.line_edit.text().strip()
        self.ser.port = 'COM' + com_port
        if self.ser.is_open:
            self.ser.close()
        self.ser.open()
        self.line_edit.clear()
        self.text_edit.insertPlainText(f"INPUT: COM{com_port}\n")
