import serial
import serial.tools.list_ports as list_port
import time as timeLib
from pandas import DataFrame
import openpyxl

from dash import Dash, dcc, html, Input, Output
import plotly.graph_objects as go
import plotly.express as px
import plotly.subplots as make_subplots


fileLocation = "C:\\Users\\imraa\\OneDrive\\Documents\\GitHub\\ArduinoSmarthome\\PythonScripts\\178PythonScript\\dataOut.xlsx"

class IndivBoard:

    def __init__(self, boardNum):
        self.boardNum = boardNum
        self.sensors = []
        for i in range(1,5):
            a = IndivSensor(str(i))
            self.sensors.append(a)

    def addData(self, senType, data):
        for sensor in self.sensors:
            if senType == sensor.senType:
                sensor.updateSelf(data)

class IndivSensor:

    def __init__(self, senType):
        self.data=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
        x = timeLib.time()
        self.time = [x,x,x,x,x,x,x,x,x,x]
        self.senType = senType
        self.updated = False
        self.df = DataFrame(data=self.data, index=self.time)

    def updateSelf(self, data):
        self.data.pop(0)
        self.data.append(data)
        self.time.pop(0)
        # time = str(datetime.datetime.now().strftime('%H:%M:%S.%f'))
        self.time.append(timeLib.time())
        self.updated = True
        self.df = DataFrame(data=self.data, index=self.time)
        # self.printData()

    def printData(self):
        print("---  " + self.senType + "   ---   " + str(self.data[-1]) + " - " + str(self.time[-1]))
        print(self.df)


# --------------------------------------------------------------------------------------------------------


serialInst = serial.Serial()
serialInst.baudrate = 9600
serialInst.port = 'COM4'
serialInst.open()



# dataOut = openpyxl.load_workbook(fileLocation)

# first index is the board #
# second index is the sensor type
boards = []
while not serialInst.in_waiting:
    timeLib.sleep(0.001)

package = serialInst.readline()

graphs = make_subplots.make_subplots(rows=4, cols=1)


app = Dash(__name__, update_title=None)

app.layout = html.Div(children=[
    dcc.Graph(
        id='graph1',
        figure=graphs
    ),
    dcc.Interval(
        id="sensor_update",
        interval=1000,
        n_intervals=0
    )
])


@app.callback(Output('graph1', 'figure'),
              Input('sensor_update', 'n_intervals'))
def sensorLoop(n):
    if serialInst.in_waiting:

        good = True
        package = serialInst.readline()
        line = package.decode('utf')
        print(line)

        try:
            numData = int(line[4:])
        except:
            numData = 0
            good = False
            print("Data is bad")

        try:
            ID = line[:2]
            sensor = line[2]
        except:
            ID = '00'
            sensor = '0'
            good = False
            print("Data is bad")

        if good:
            found = False
            for board in boards:
                if ID == board.boardNum:
                    board.addData(sensor, numData)
                    found = True

            if not found:
                boards.append(IndivBoard(ID))
                boards[-1].addData(sensor, numData)

        graphs = make_subplots.make_subplots(rows=4, cols=1)

        for board in boards:
            for j in range(1,5):
                if board.sensors[j-1].updated:
                    name = ''
                    if j == 1:
                        name += 'Photo'
                    elif j == 2:
                        name += 'Pot'
                    elif j == 3:
                        name += 'Temp'
                    elif j == 4:
                        name += 'Humid'
                    name += board.boardNum
                    graphs.add_trace(row=j,col=1,
                                     trace=go.Scatter(x=board.sensors[j-1].time,
                                                      y=board.sensors[j-1].data,
                                                      name=name ))
        # graphs.show()
        graphs.update_layout(height=1200, width=1800)

        return graphs


app.run_server(debug=False)
